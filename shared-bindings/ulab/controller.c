/*
 * This file is part of the micropython-ulab project, 
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Zoltán Vörös
*/
    
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lib/utils/context_manager_helpers.h"

#include "py/obj.h"
#include "py/runtime.h"
#include "py/misc.h"
#include "shared-bindings/ulab/linalg.h"
#include "shared-bindings/ulab/ndarray.h"

void bdot_law(mp_float_t *c, size_t length) {
	// this function takes in an array of the bdot and magnetic moment
	// and modifies the moment in place
	
    float K = 20;

    for(size_t i=0; i < length; i++) {
        c[i] = -K*c[i];
    }

}

void cholesky(const mp_float_t A_data[], const size_t A_size[2], mp_float_t R_data[], size_t R_size[2])
{
  int jmax;
  int n;
  int info;
  int j;
  bool exitg1;
  int idxAjj;
  mp_float_t ssq;
  int ix;
  int iy;
  int i;
  int idxAjp1j;
  int b_i;
  int iac;
  mp_float_t c;
  int i1;
  int ia;
  R_size[0] = A_size[0];
  R_size[1] = A_size[1];
  jmax = A_size[0] * A_size[1];
  if (0 <= jmax - 1) {
    memcpy(&R_data[0], &A_data[0], jmax * sizeof(mp_float_t));
  }

  n = A_size[1];
  if (A_size[1] != 0) {
    info = 0;
    j = 0;
    exitg1 = false;
    while ((!exitg1) && (j <= n - 1)) {
      idxAjj = j + j * n;
      ssq = 0.0;
      if (j >= 1) {
        ix = j;
        iy = j;
        for (jmax = 0; jmax < j; jmax++) {
          ssq += R_data[ix] * R_data[iy];
          ix += n;
          iy += n;
        }
      }

      ssq = R_data[idxAjj] - ssq;
      if (ssq > 0.0) {
        ssq = MICROPY_FLOAT_C_FUN(sqrt)(ssq);
        R_data[idxAjj] = ssq;
        if (j + 1 < n) {
          jmax = (n - j) - 1;
          i = j + 2;
          idxAjp1j = idxAjj + 2;
          if ((jmax != 0) && (j != 0)) {
            ix = j;
            b_i = (j + n * (j - 1)) + 2;
            for (iac = i; n < 0 ? iac >= b_i : iac <= b_i; iac += n) {
              c = -R_data[ix];
              iy = idxAjj + 1;
              i1 = (iac + jmax) - 1;
              for (ia = iac; ia <= i1; ia++) {
                R_data[iy] += R_data[ia - 1] * c;
                iy++;
              }

              ix += n;
            }
          }

          ssq = 1.0 / ssq;
          b_i = idxAjj + jmax;
          for (jmax = idxAjp1j; jmax <= b_i + 1; jmax++) {
            R_data[jmax - 1] *= ssq;
          }
        }

        j++;
      } else {
        R_data[idxAjj] = ssq;
        info = j + 1;
        exitg1 = true;
      }
    }

    if (info == 0) {
      jmax = A_size[1];
    } else {
      jmax = info - 1;
    }

    for (j = 2; j <= jmax; j++) {
      for (i = 0; i <= j - 2; i++) {
        R_data[i + R_size[0] * (j - 1)] = 0.0;
      }
    }
  }
}

mp_obj_t ulab_controller_bdot(mp_obj_t self_in) {
    ulab_ndarray_obj_t *self = MP_OBJ_TO_PTR(self_in); 
    // this passes in the array
    // the size of a single item in the array
    //uint8_t _sizeof = mp_binary_get_size('@', self->array->typecode, NULL);
    
    // NOTE: 
    // we assume that the array passed in is of the form [u[1:3],Bdot[1:3]]
    mp_float_t *c = (mp_float_t *)self->array->items;

    size_t a = 6;
    bdot_law(c, a);       

    return MP_OBJ_FROM_PTR(self);

}

mp_obj_t ulab_controller_cholesky(mp_obj_t A_input, mp_obj_t R_input) {

    // get pointers to input arrays
    ulab_ndarray_obj_t *A_obj = MP_OBJ_TO_PTR(A_input);
    ulab_ndarray_obj_t *R_obj = MP_OBJ_TO_PTR(R_input);

    // extract pointers to underlying C data arrays for input ulab arrays
    mp_float_t *A_data = (mp_float_t *)A_obj->array->items;
    mp_float_t *R_data = (mp_float_t *)R_obj->array->items;


    // Is it bad to create these local size_t arrays?
    size_t A_size[2] = {A_obj->m, A_obj->n};
    size_t R_size[2] = {R_obj->m, R_obj->n};

    // Call Cholesky
    cholesky(A_data, A_size, R_data, R_size);


    // Do I return R????
    return MP_OBJ_FROM_PTR(R_obj);

}