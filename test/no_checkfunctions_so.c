#include <measuresuite.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char symbol[] = {"fiat_p521_carry_mul"};
int arg_width = 9;
int arg_num_in = 2;
// those arrays are below

int main() {

  // INIT
  int arg_num_out = 1;
  int chunksize = 0;
  measuresuite_t ms;
  uint64_t bounds[] = {
      0xc00000000000000 - 1, 0xc00000000000000 - 1, 0xc00000000000000 - 1,
      0xc00000000000000 - 1, 0xc00000000000000 - 1, 0xc00000000000000 - 1,
      0xc00000000000000 - 1, 0xc00000000000000 - 1, 0x600000000000000 - 1};
  if (ms_measure_init(&ms, arg_width, arg_num_in, arg_num_out, chunksize,
                      bounds, "./does_not_exist.so", symbol)) {

    int len = 1000;
    char *s = calloc(1, len * sizeof(char));
    ms_str_full_error(ms, s, len);
    const char expected[] =
        "The given lib check file is not readable and executable. "
        "Reason: No such file or directory\n";

    if (strstr(s, expected) == 0) {
      fprintf(stderr, "the error received \n'%s' is not the expected \n'%s'\n",
              s, expected);
      return 99;
    };

    free(s);

    return 1;
  }

  return 0;
}
