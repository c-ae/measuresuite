#include "test_helper.h"
#include <measuresuite.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  // convenience pointer
  void (*err)(measuresuite_t, const char *) =
      error_handling_helper_template_str;

  //
  char fa[] = {"mov rax, [rsi]; *i0 \n"
               "add rax, [rdx]; *i0 + *i1\n"
               "mov [rdi], rax; *o0 <-\n"

               "mov rax, [rsi + 0x08]; *i0+1\n"
               "add rax, [rdx + 0x08]; *i0+1 + *i1+1\n"
               "mov [rdi + 0x08], rax; *o0 + 1 <-\n"

               "mov rax, [rsi + 0x10 ]; *i0 + 2\n"
               "add rax, [rdx + 0x10 ]; *i0 + 2 + *i1+2\n"
               "mov [rdi + 0x10 ], rax; *o0 + 2 <-\n"

               "mov rax, [rsi + 0x18 ]; *i0 + 3\n"
               "add rax, [rdx + 0x18 ]; *i0 + 3 + *i1+3\n"
               "mov [rdi + 0x18 ], rax; *o0 + 3 <-\n"

               "mov rax, [rsi + 0x20 ]; *i0 + 4\n"
               "add rax, [rdx + 0x20 ]; *i0 + 4 + *i1+4\n"
               "mov [rdi + 0x20 ], rax; *o0 + 4 <-\n"

               "ret\n"};
  int num_batches = 10;
  int batch_size = 20;

  // INIT
  int arg_width = 5;
  int arg_num_in = 2;
  int arg_num_out = 1;
  uint64_t bounds[] = {-1, -1, -1, -1, -1};
  int chunksize = 0;
  measuresuite_t ms;
  if (ms_measure_init(&ms, arg_width, arg_num_in, arg_num_out, chunksize, bounds,
                   "./libtest_lib.so", "array_width_five_add_pair")) {
    err(ms, "Failed to measure_init. Reason: %s.");
    return 1;
  }

  // measure
  if (ms_measure(ms, fa, fa, batch_size, num_batches)) {
    err(ms, "Failed to measure. Reason: %s.");
    return 1;
  }
  const char *output;
  size_t jsonlen;
  ms_getJson(ms, &output, &jsonlen);
  printf("%s\n", output);

  // Require to have true in the string but not false
  if (strstr(output, "true") == NULL && strstr(output, "false") != NULL) {
    fprintf(stderr, "should have been a correct result\n");
    ms_measure_end(ms);
    return 1;
  }
  // END
  if (ms_measure_end(ms)) {
    err(ms, "Failed to measure_end. Reason: %s.");
    return 1;
  }
  return 0;
}
