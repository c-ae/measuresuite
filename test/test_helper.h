#include <measuresuite.h>
/**
 * this function prints ms's errors to stderror.
 */
void error_handling_helper(measuresuite_t ms);

/*
 * same as error_handling_helper but uses given @param tpl_str instead of the
 * default.
 */
void error_handling_helper_template_str(measuresuite_t ms, const char *tpl_str);
