#ifndef MEASURE_H
#define MEASURE_H

#include <stddef.h> // size_t
#include <stdint.h> // uint64_t

typedef struct measuresuite *measuresuite_t;
typedef struct ERROR_NUMBER error_t;

// will save a reference to the creted measuresuite at *ms.
int ms_measure_init(measuresuite_t *ms, int arg_width, int num_arg_in,
                    int num_arg_out, int chunk_size, const uint64_t *bounds,
                    const char *lib_check_functions_filename,
                    const char *check_function_symbolname);

int ms_measure(measuresuite_t ms, char *functionA, char *functionB,
               int batch_size, int num_batches);

int ms_measure_end(measuresuite_t ms);

void ms_getJson(measuresuite_t ms, const char **json, size_t *json_len);

/**
 * prints the error to stdout
 */
void ms_p_error(measuresuite_t ms);

/**
 * returns a pointer to the string corresponding to the last error stored in
 * @param ms
 */
const char *ms_str_error(measuresuite_t ms);

/**
 * prints most @param len chars from the error code to *dest. Also adds
 * ms->additional_info to it.
 */
void ms_str_full_error(measuresuite_t ms, char *dest, int len);
#endif
