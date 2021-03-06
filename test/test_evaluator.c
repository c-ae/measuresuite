// unit-test
#include "../src/evaluator.h"
#include <assemblyline.h>
#include <measuresuite.h>
#include <stdio.h>

static const int ARG_WIDTH = 3;

static void mock_check(uint64_t *out1, uint64_t *arg1) {
  for (int i = 0; i < ARG_WIDTH; i++) {
    *(out1 + i) = *(arg1 + i);
  }
}

/*
 * shouldnt segfault and return 0
 */
static int should_not_segfault(measuresuite_t *ms) {
  return run_measurement(*ms);
}

static void init(measuresuite_t *_ms) { measuresuite_t ms = *_ms; };

int main() {
  measuresuite_t ms;

  // INIT

  int arg_num_in = 1;
  int arg_num_out = 1;
  int chunksize = 0;
  uint64_t bounds[] = {-1, -1, -1};

  /*
   * dont matter, function check will be overwritten.
   * the file and symbol only need to exist
   */
  const char symbol[] = {"add_two_numbers"};
  const char lib[] = {"./libtest_lib.so"};
  ms_measure_init(&ms, ARG_WIDTH, arg_num_in, arg_num_out, chunksize, bounds, lib,
               symbol);
  ms->function_check = &mock_check;
  ms->batch_size = 2;
  ms->num_batches = 2;

  ms->al_A = asm_create_instance(NULL, 0);
  ms->al_B = asm_create_instance(NULL, 0);

  char *s = calloc(ARG_WIDTH * 50, 1);
  for (int i = 0; i < ARG_WIDTH; i++) {
    sprintf(s, "mov rax, [rsi + %d]\n mov [rdi + %d],rax\n", i * 8, i * 8);
    asm_assemble_str(ms->al_A, s);
    asm_assemble_str(ms->al_B, s);
  }
  asm_assemble_str(ms->al_A, "ret");
  asm_assemble_str(ms->al_B, "ret");
  free(s);

  // TEST
  int res = 0;
  res |= should_not_segfault(&ms);
  return res;
}
