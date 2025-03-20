/* naive.c
 *
 * Author:
 * Date  :
 *
 *  Description
 */

/* Standard C includes */
#include <stdlib.h>

/* Include common headers */
#include "common/macros.h"
#include "common/types.h"

/* Include application-specific headers */
#include "include/types.h"
#include <stdio.h>

/* Naive Implementation */
#pragma GCC push_options
#pragma GCC optimize ("O1")
__attribute__ ((optimize(1)))
void* impl_scalar_naive(void* args)
{
  /* Get the argument struct */
  args_t* parsed_args = (args_t*)args;
  
  /* Get all the arguments */
  register       float*   dest = (      float*)(parsed_args->output);
  register const float*   src0 = (const float*)(parsed_args->input0);
  register const float*   src1 = (const float*)(parsed_args->input1);
  register size_t rowsA        =                  parsed_args->rowsA;
  register size_t colsA        =                  parsed_args->colsA;
  register size_t rowsB        =                  parsed_args->rowsB;
  register size_t colsB        =                  parsed_args->colsB;

  for (int i = 0; i < rowsA; i++) {
    for (int j = 0; j < colsB; j++) {
        dest[i * colsB + j] = 0.0f;
        for (int k = 0; k < colsA; k++) {
          dest[i * colsB + j] += src0[i * colsA + k] * src1[k * colsB + j];
        }
    }
  }

  /* Done */
  return NULL;
}
#pragma GCC pop_options
