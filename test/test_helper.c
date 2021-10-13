#include "test_helper.h"
#include <stdio.h>
#include <stdlib.h>

void error_handling_helper(measuresuite_t ms) {
  error_handling_helper_template_str(ms, "Failed. Reason: %s\n");
}

void error_handling_helper_template_str(measuresuite_t ms,
                                        const char *tpl_str) {
  int len = 1000;
  char *s = calloc(1, len * sizeof(char));
  ms_str_full_error(ms, s, len);
  fprintf(stderr, tpl_str, s);
  free(s);
}
