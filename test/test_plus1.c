#include "test_helper.h"
#include <measuresuite.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const char lib[] = {"./libtest_lib.so"};
const char symbol[] = {"inc"};

int main(int argc, char **argv) {
  // convenience pointer
  void (*err)(measuresuite_t, const char *) =
      error_handling_helper_template_str;

  char fa[] = {"mov rax, [rsi]\n"
               "add rax, 1\n"
               "mov [rdi], rax\n"
               "ret\n"};
  int num_batches = 2;
  int batch_size = 2;

  // INIT
  int arg_width = 1;
  int arg_num_in = 1;
  int arg_num_out = 1;
  int chunksize = 0;
  uint64_t bounds[] = {-1};

  measuresuite_t ms;
  if (ms_measure_init(&ms, arg_width, arg_num_in, arg_num_out, chunksize, bounds,
                   lib, symbol)) {
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

  // END
  if (ms_measure_end(ms)) {
    err(ms, "Failed to measure_end. Reason: %s.");
    return 1;
  }

  return 0;
}
