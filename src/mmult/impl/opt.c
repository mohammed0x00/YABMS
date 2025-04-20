/* opt.c
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

/* Alternative Implementation */
#pragma GCC push_options
#pragma GCC optimize ("O1")
void* impl_scalar_opt(void* args)
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
  register size_t block_size   =                  parsed_args->block_size;

  /* Initialize the result matrix R (dest) to zero */
  for (size_t i = 0; i < rowsA; i += block_size) {
    for (size_t j = 0; j < colsB; j += block_size) {
      for (size_t ii = i; ii < i + block_size && ii < rowsA; ii++) {
        for (size_t jj = j; jj < j + block_size && jj < colsB; jj++) {
          dest[ii * colsB + jj] = 0.0f;
        }
      }
    }
  }

  /* Perform blocked matrix multiplication */
  for (size_t ii = 0; ii < rowsA; ii += block_size) {
    for (size_t jj = 0; jj < colsB; jj += block_size) {
      for (size_t kk = 0; kk < colsA; kk += block_size) {
        for (size_t i = ii; i < ii + block_size && i < rowsA; i++) {
          for (size_t j = jj; j < jj + block_size && j < colsB; j++) {
            float val = dest[i * colsB + j];
            for (size_t k = kk; k < kk + block_size && k < colsA; k++) {
              val += src0[i * colsA + k] * src1[k * colsB + j];
            }
            dest[i * colsB + j] = val;
          }
        }
      }
    }
  }

  /* Done */
  return NULL;
}
#pragma GCC pop_options
