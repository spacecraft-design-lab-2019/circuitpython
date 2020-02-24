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
#include "innovation.h"
#include "predict.h"
#include "MEKFstep_data.h"

void bdot_law(mp_float_t *c, size_t length) {
	// this function takes in an array of the bdot and magnetic moment
	// and modifies the moment in place
	
  float K = 20;

  for(size_t i=0; i < length; i++) {
    c[i] = -K*c[i];
  }

}

// changed input types to mp_float_t and size_t
void cholesky(const mp_float_t A_data[], const size_t A_size[2], mp_float_t R_data[], size_t R_size[2]){
  int jmax;
  int n;
  int info;
  int j;
  // changed to standard bool
  bool exitg1;
  int idxAjj;

  // changed double to mp_float_t
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
    memcpy(&R_data[0], &A_data[0], jmax * sizeof(mp_float_t)); // changed memory allocation to mp_float_t
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
        ssq = MICROPY_FLOAT_C_FUN(sqrt)(ssq);   // changed to custom micropython sqrt
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

void MEKFstep(const mp_float_t x_k[7], const mp_float_t P_k[36], const mp_float_t w_k[3],
  const mp_float_t r_sun_body[3], const mp_float_t r_B_body[3], const mp_float_t
  r_sun_inert[3], const mp_float_t r_B_inert[3], const mp_float_t Q[36],
  const mp_float_t R[36], mp_float_t dt, mp_float_t x_k1[7], mp_float_t P_k1[36])
{ 

  // Pre allocate stuff    
  mp_float_t x_pred[7];
  mp_float_t P_pred[36];
  mp_float_t z[6];
  mp_float_t S[36];
  mp_float_t C[36];
  int i0;
  int j;
  int k;
  int jAcol;
  signed char ipiv[6];
  int ix;
  int jj;
  mp_float_t L[36];
  int jA;
  int jp1j;
  mp_float_t smax;
  int n;
  int iy;
  mp_float_t dx[6];
  mp_float_t s;
  mp_float_t b_L[16];
  mp_float_t dv0[9];
  mp_float_t dv1[4];
  mp_float_t P_k1_tmp[36];

  // Predict, measure, and innovate
  predict(x_k, P_k, w_k, dt, Q, x_pred, P_pred);
  innovation(x_pred, P_pred, r_sun_body, r_B_body, r_sun_inert, r_B_inert, R, z,
   S, C);

  // Get Kalman gain, this is with generic L = P*C'/S in MATLAB
  for (i0 = 0; i0 < 6; i0++) {
    for (k = 0; k < 6; k++) {
      ix = i0 + 6 * k;
      L[ix] = 0.0;
      smax = 0.0;
      for (jp1j = 0; jp1j < 6; jp1j++) {
        smax += P_pred[i0 + 6 * jp1j] * C[k + 6 * jp1j];
      }

      L[ix] = smax;
    }

    ipiv[i0] = (signed char)(1 + i0);
  }

  for (j = 0; j < 5; j++) {
    jAcol = j * 7;
    jj = j * 7;
    jp1j = jAcol + 2;
    n = 6 - j;
    iy = 0;
    ix = jAcol;
    smax = fabs(S[jAcol]);
    for (k = 2; k <= n; k++) {
      ix++;
      s = fabs(S[ix]);
      if (s > smax) {
        iy = k - 1;
        smax = s;
      }
    }

    if (S[jj + iy] != 0.0) {
      if (iy != 0) {
        iy += j;
        ipiv[j] = (signed char)(iy + 1);
        ix = j;
        for (k = 0; k < 6; k++) {
          smax = S[ix];
          S[ix] = S[iy];
          S[iy] = smax;
          ix += 6;
          iy += 6;
        }
      }

      i0 = (jj - j) + 6;
      for (n = jp1j; n <= i0; n++) {
        S[n - 1] /= S[jj];
      }
    }

    n = 4 - j;
    iy = jAcol + 6;
    jA = jj;
    for (jAcol = 0; jAcol <= n; jAcol++) {
      smax = S[iy];
      if (S[iy] != 0.0) {
        ix = jj + 1;
        i0 = jA + 8;
        k = (jA - j) + 12;
        for (jp1j = i0; jp1j <= k; jp1j++) {
          S[jp1j - 1] += S[ix] * -smax;
          ix++;
        }
      }

      iy += 6;
      jA += 6;
    }
  }

  for (j = 0; j < 6; j++) {
    jA = 6 * j - 1;
    jAcol = 6 * j;
    for (k = 0; k < j; k++) {
      jp1j = 6 * k;
      smax = S[k + jAcol];
      if (smax != 0.0) {
        for (n = 0; n < 6; n++) {
          ix = (n + jA) + 1;
          L[ix] -= smax * L[n + jp1j];
        }
      }
    }

    smax = 1.0 / S[j + jAcol];
    for (n = 0; n < 6; n++) {
      ix = (n + jA) + 1;
      L[ix] *= smax;
    }
  }

  for (j = 5; j >= 0; j--) {
    iy = 6 * j - 1;
    i0 = j + 2;
    for (k = i0; k < 7; k++) {
      jp1j = 6 * (k - 1);
      smax = S[k + iy];
      if (smax != 0.0) {
        for (n = 0; n < 6; n++) {
          ix = (n + iy) + 1;
          L[ix] -= smax * L[n + jp1j];
        }
      }
    }
  }

  for (iy = 4; iy >= 0; iy--) {
    if (ipiv[iy] != iy + 1) {
      for (jA = 0; jA < 6; jA++) {
        jAcol = jA + 6 * iy;
        smax = L[jAcol];
        ix = jA + 6 * (ipiv[iy] - 1);
        L[jAcol] = L[ix];
        L[ix] = smax;
      }
    }
  }

  // Update
  /*  unpack state */
  /*  post-fit residuals */
  for (i0 = 0; i0 < 6; i0++) {
    dx[i0] = 0.0;
    smax = 0.0;
    for (k = 0; k < 6; k++) {
      smax += L[i0 + 6 * k] * z[k];
    }

    dx[i0] = smax;
  }

  /*  -------------------- State Update ----------------------- */
  /*  equivalent to quaternion multiplication q1*q2 for scalar first */
  /*  quaternions */
  /*  for left quaternion multiplication q1*q2 = L(q1)q2 */
  memset(&b_L[0], 0, sizeof(mp_float_t) << 4);
  b_L[4] = -x_pred[1];
  b_L[8] = -x_pred[2];
  b_L[12] = -x_pred[3];
  b_L[0] = x_pred[0];
  b_L[1] = x_pred[1];
  b_L[2] = x_pred[2];
  b_L[3] = x_pred[3];

  /*  Returns skew symmetric - cross product matrix of a vector */
  dv0[0] = 0.0;
  dv0[3] = -x_pred[3];
  dv0[6] = x_pred[2];
  dv0[1] = x_pred[3];
  dv0[4] = 0.0;
  dv0[7] = -x_pred[1];
  dv0[2] = -x_pred[2];
  dv0[5] = x_pred[1];
  dv0[8] = 0.0;
  smax = 0.0;
  for (i0 = 0; i0 < 3; i0++) {
    ix = (1 + i0) << 2;
    b_L[ix + 1] = x_pred[0] * (mp_float_t)iv0[3 * i0] + dv0[3 * i0];
    iy = 1 + 3 * i0;
    b_L[ix + 2] = x_pred[0] * (mp_float_t)iv0[iy] + dv0[iy];
    iy = 2 + 3 * i0;
    b_L[ix + 3] = x_pred[0] * (mp_float_t)iv0[iy] + dv0[iy];
    smax += dx[i0] * dx[i0];
  }

  dv1[0] = sqrt(1.0 - smax);
  dv1[1] = dx[0] / 2.0;
  dv1[2] = dx[1] / 2.0;
  dv1[3] = dx[2] / 2.0;
  for (i0 = 0; i0 < 4; i0++) {
    x_k1[i0] = ((b_L[i0] * dv1[0] + b_L[i0 + 4] * dv1[1]) + b_L[i0 + 8] * dv1[2])
    + b_L[i0 + 12] * dv1[3];
  }

  x_k1[4] = x_pred[4] + dx[3];
  x_k1[5] = x_pred[5] + dx[4];
  x_k1[6] = x_pred[6] + dx[5];

  /*  -------------------- Covariance Update ------------------ */
  memset(&S[0], 0, 36U * sizeof(mp_float_t));
  for (k = 0; k < 6; k++) {
    S[k + 6 * k] = 1.0;
  }

  for (i0 = 0; i0 < 6; i0++) {
    for (k = 0; k < 6; k++) {
      smax = 0.0;
      for (jp1j = 0; jp1j < 6; jp1j++) {
        smax += L[i0 + 6 * jp1j] * C[jp1j + 6 * k];
      }

      iy = i0 + 6 * k;
      P_k1_tmp[iy] = S[iy] - smax;
    }
  }

  for (i0 = 0; i0 < 6; i0++) {
    for (k = 0; k < 6; k++) {
      iy = i0 + 6 * k;
      S[iy] = 0.0;
      C[iy] = 0.0;
      smax = 0.0;
      s = 0.0;
      for (jp1j = 0; jp1j < 6; jp1j++) {
        jA = i0 + 6 * jp1j;
        jAcol = jp1j + 6 * k;
        smax += P_k1_tmp[jA] * P_pred[jAcol];
        s += L[jA] * R[jAcol];
      }

      C[iy] = s;
      S[iy] = smax;
    }

    for (k = 0; k < 6; k++) {
      iy = i0 + 6 * k;
      P_k1[iy] = 0.0;
      smax = 0.0;
      for (jp1j = 0; jp1j < 6; jp1j++) {
        smax += S[i0 + 6 * jp1j] * P_k1_tmp[k + 6 * jp1j];
      }

      P_k1[iy] = smax;
    }
  }

  for (i0 = 0; i0 < 6; i0++) {
    for (k = 0; k < 6; k++) {
      iy = i0 + 6 * k;
      S[iy] = 0.0;
      smax = 0.0;
      for (jp1j = 0; jp1j < 6; jp1j++) {
        smax += C[i0 + 6 * jp1j] * L[k + 6 * jp1j];
      }

      S[iy] = smax;
    }
  }

  for (i0 = 0; i0 < 36; i0++) {
    P_k1[i0] += S[i0];
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

void ulab_controller_MEKF_step(mp_obj_t x_k_input, mp_obj_t P_k_input, mp_obj_t w_k_input, mp_obj_t r_sun_body_input, mp_obj_t r_B_body_input,
  mp_obj_t r_sun_inert_input, mp_obj_t r_B_inert_input, mp_obj_t Q_input, mp_obj_t R_input, mp_float_t dt) {

    // Do I take in an x_k+1 and P_k+1? (I think I should just update them in place, unless we want to keep a history)
    // How to handle when we're in eclipse? Multiple MEKFs? (one for dead reckoning, etc.) Take in an eclipse flag?


      // get pointers to input arrays
  ulab_ndarray_obj_t *x_k_obj = MP_OBJ_TO_PTR(x_k_input);
  ulab_ndarray_obj_t *P_k_obj = MP_OBJ_TO_PTR(P_k_input);
  ulab_ndarray_obj_t *w_k_obj = MP_OBJ_TO_PTR(w_k_input); 
  ulab_ndarray_obj_t *r_sun_body_obj = MP_OBJ_TO_PTR(r_sun_body_input);
  ulab_ndarray_obj_t *r_B_body_obj = MP_OBJ_TO_PTR(r_B_body_input);
  ulab_ndarray_obj_t *r_sun_inert_obj = MP_OBJ_TO_PTR(r_sun_inert_input);
  ulab_ndarray_obj_t *r_B_inert_obj = MP_OBJ_TO_PTR(r_B_inert_input);
  ulab_ndarray_obj_t *Q_obj = MP_OBJ_TO_PTR(Q_input);
  ulab_ndarray_obj_t *R_obj = MP_OBJ_TO_PTR(R_input);


    // extract pointers to underlying C data arrays for input ulab arrays
  mp_float_t *x_k_data = (mp_float_t *)x_k_obj->array->items;
  mp_float_t *P_k_data = (mp_float_t *)P_k_obj->array->items;
  mp_float_t *w_k_data = (mp_float_t *)w_k_obj->array->items;
  mp_float_t *r_sun_body_data = (mp_float_t *)r_sun_body_obj->array->items;
  mp_float_t *r_B_body_data = (mp_float_t *)r_B_body_obj->array->items;
  mp_float_t *r_sun_inert_data = (mp_float_t *)r_sun_inert_obj->array->items;
  mp_float_t *r_B_inert_data = (mp_float_t *)r_B_inert_obj->array->items;
  mp_float_t *Q_data = (mp_float_t *)Q_obj->array->items;
  mp_float_t *R_data = (mp_float_t *)R_obj->array->items;

  // Call the actual autocoded function, pray
  void MEKFstep(x_k_data, P_k_data, w_k_data,
  r_sun_body_data, r_B_body_data, r_sun_inert_data, r_B_inert_data, Q_data, R_data, dt,
  x_k_data, P_k_data) // Write over data in state/covariance vector/array

    // Do I return R????
  // return MP_OBJ_FROM_PTR(R_obj); // Switched to a void function

}