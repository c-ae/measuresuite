#include <assemblyline.h> // asm_get_code
#include <errno.h>        // errno
#include <stdio.h>        // snprintf
#include <stdlib.h>       // alloc / size_t
#include <string.h>       // memset / strerror

#include "checker.h" // check
#include "evaluator.h"
#include "randomizer.h" // randomize / get_random_byte
#include "struct_measuresuite.h"
#include "timer.h" // ms_start/ms_stop_timer / ms_current_timestamp

static void run_batch(struct measuresuite *ms, uint64_t *cycl_count_dest,
                      uint64_t out[], void (*func)(uint64_t *o, ...));

static int generate_json_from_measurement_results(
    struct measuresuite *ms, uint64_t start_time, int check_result, int count_a,
    int count_c, uint64_t results_a[], uint64_t results_c[]);

static int realloc_or_fail(struct measuresuite *ms, void **dest,
                           size_t new_len);

int run_measurement(struct measuresuite *ms) {

  // getting pointers
  void *function_A = asm_get_code(ms->al_A);
  void *function_B = asm_get_code(ms->al_B);

  // init arith_result mem; setting for each measurement to 0
  // size:|arg_width*so(u64)*num_arg_out     arg_width*so(u64*num_arg_out)
  //      +--------------------------------+------------------------------+
  // ref: |    arith_res_test              |        arith_res_chk         |
  //      +--------------------------------+------------------------------+
  memset(ms->arithmetic_results, 0,
         ms->arithmetic_results_size_u64 * sizeof(uint64_t));
  uint64_t *arith_res_tst = ms->arithmetic_results;
  uint64_t *arith_res_chk =
      ms->arithmetic_results + ms->arg_width * ms->num_arg_out;

  // init cyclecount mem
  memset(ms->cycle_results, 0, ms->cycle_results_size_u64 * sizeof(uint64_t));
  size_t count_a = 0, count_c = 0;
  // allocate results arrays for a and b - runs, each maxruns long
  // assume, runs == 2;  max_runs == 4; will allocate 16 elements.
  // 4 for A, 4 for B, 8 for checking each of those
  // [ABBABABB    | CCCCC  ]
  // a start the start
  uint64_t *cycl_res_tst = ms->cycle_results;
  // second half for c
  uint64_t *cycl_res_chk = ms->cycle_results + ms->cycle_results_size_u64 / 2;

  // init run_order mem (see in measure_helper, basically since we dont save 'c'
  // we only need half as much + NUL)
  memset(ms->run_order, 0, (ms->cycle_results_size_u64 / 2 + 1) * sizeof(char));

  // init result indicator
  int check_result = 0;

  // START MEASUREMENT
  uint64_t start_time = ms_current_timestamp();

  do {
    if (randomize(ms) != 0) {
      return 1;
    }

    uint8_t rand_byte;

    if (get_random_byte(ms, &rand_byte)) {
      return 1;
    }

    unsigned char run_a = rand_byte & 1;

    ms->run_order[count_c] = 'b' - run_a;
    // because count_c is incremented everytime, we can use it as an index for
    // runorder as well. and since run_order was \0ed, it is always a perfect
    // valid string.

    void(*f) = run_a ? function_A : function_B;

    run_batch(ms, cycl_res_tst + count_c, arith_res_tst, f);
    count_a += run_a;

    run_batch(ms, cycl_res_chk + count_c, arith_res_chk, ms->function_check);
    count_c++;

    // check
    check_result =
        check(ms->arg_width * ms->num_arg_out, arith_res_chk, arith_res_tst);

    // if it failed
    if (check_result) {
      break;
    }

    int repeat_measurement = 0;
    // if we fall out of space
    if (count_c >= ms->cycle_results_size_u64 / 2) {
      // new size, *2 backoff
      ms->cycle_results_size_u64 *= 2;

      if (realloc_or_fail(ms, (void **)&(ms->cycle_results),
                          ms->cycle_results_size_u64 * sizeof(uint64_t))) {
        return 1;
      }
      repeat_measurement = 1;
    }

    if (count_c >= ms->run_order_size_bytes / 2) {

      // new size, *2 backoff
      ms->run_order_size_bytes *= 2;

      if (realloc_or_fail(ms, (void **)&(ms->run_order),
                          (ms->run_order_size_bytes + 1) * sizeof(char)))
        return 1;
      repeat_measurement = 1;
    }

    if (repeat_measurement) {
      // memset'ing is done as init in recursive step
      return run_measurement(ms);
    }

    // as long as not both functions has been run for arg-runs amounts
  } while (count_a < ms->num_batches || count_c - count_a < ms->num_batches);

  int json_generation_failed = generate_json_from_measurement_results(
      ms, start_time, check_result, count_a, count_c, cycl_res_tst,
      cycl_res_chk);

  if (json_generation_failed) {
    return 1;
  }

  return 0;
}

static int realloc_or_fail(struct measuresuite *ms, void **dest,
                           size_t new_len) {
  *dest = realloc(*dest, new_len);
  if (*dest == NULL) {
    ms->errorno = E_INTERNAL_MEASURE__AI__ALLOC;
    ms->additional_info = strerror(errno);
    return 1;
  }
  return 0;
}

static int generate_json_from_measurement_results(
    struct measuresuite *ms, uint64_t start_time, int check_result, int count_a,
    int count_c, uint64_t cycl_res_test[], uint64_t cycl_res_chk[]) {
  char *json = ms->json;
  char *json_end = ms->json + ms->json_len;
  json += snprintf(json, ms->json_len,
                   "{\"stats\":"
                   "{\"countA\":%d,"
                   "\"countB\":%d,"
                   "\"chunksA\":%d,"
                   "\"chunksB\":%d,"
                   "\"batchSize\":%lu,"
                   "\"numBatches\":%d,"
                   "\"runtime\":%" PRIu64 ","
                   "\"runOrder\":\"%s\","
                   "\"checkResult\":%s},"
                   "\"times\":[",
                   count_a, count_c - count_a, ms->chunks_A, ms->chunks_B,
                   ms->batch_size, ms->num_batches,
                   ms_current_timestamp() - start_time, ms->run_order,
                   check_result ? "false" : "true");
  int max_print_len = 48;
  // each iteration we append "[-1,LU,LU],\0", which has a maxlen of strlen(LU)
  // -= 20; strlen("[-1,LU,LU],") == 7+2*20==47 + NUL-char
  for (int idx = 0; idx < count_c; idx++) {
    if (max_print_len + json > json_end) {
      ms->json_len *= 2;                    // double-backoff
      int old_len_actual = json - ms->json; // like:strlen

      if (realloc_or_fail(ms, (void **)&(ms->json), ms->json_len))
        return 1;

      // update local pointers
      json = ms->json + old_len_actual;
      json_end = ms->json + ms->json_len;
      memset(json, '\0', json_end - json); // and zero the rest
    }
    char c = ms->run_order[idx];
    // print out the results in json-format, using -1 as the 'no
    // measurement'-placeholder
    const char *tpl = c == 'a' ? "[%" PRIu64 ",-1,%" PRIu64 "],"
                               : "[-1,%" PRIu64 ",%" PRIu64 "],";

    json += snprintf(json, max_print_len, tpl, cycl_res_test[idx],
                     cycl_res_chk[idx]);
  }
  // finalise json, overwriting the last comma
  snprintf(json - 1, 3, "]}");
  return 0;
}

static void run_batch(struct measuresuite *ms, uint64_t *count, uint64_t *out,
                      void (*func)(uint64_t *o, ...)) {

  // we always call the function with three in-arguments. It itself will then
  // take which ever it needs. However, the positon of the in-args is dependent
  // on the num out args, thus the switch
  int bs = ms->batch_size;
  int w = ms->arg_width;

  uint64_t *arg1 = ms->random_data;
  uint64_t *arg2 = ms->random_data + w;
  uint64_t *arg3 = ms->random_data + 2 * w;

  uint64_t start_time = 0;

  switch (ms->num_arg_out) {
  case 1:
    ms_start_timer(&start_time);
    while (bs--)
      func(out, arg1, arg2, arg3);
    break;
  case 2:
    ms_start_timer(&start_time);
    while (bs--)
      func(out, out + w, arg1, arg2, arg3);
    break;
  case 3:
    ms_start_timer(&start_time);
    while (bs--)
      func(out, out + w, out + w + w, arg1, arg2, arg3);
    break;
  }

  *count = ms_stop_timer(start_time);
}
