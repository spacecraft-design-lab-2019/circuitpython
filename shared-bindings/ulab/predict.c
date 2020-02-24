/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: predict.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 23-Feb-2020 17:14:39
 */

/* Include Files */
#include <math.h>
#include <string.h>
#include "MEKFstep.h"
#include "predict.h"
#include "norm.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/misc.h"

/* Function Definitions */

/*
 * ------------Predict State-----------------
 *  Unpack state
 * Arguments    : const mp_float_t x_k[7]
 *                const mp_float_t P_k[36]
 *                const mp_float_t w_k[3]
 *                mp_float_t dt
 *                const mp_float_t Q[36]
 *                mp_float_t x_pred[7]
 *                mp_float_t P_pred[36]
 * Return Type  : void
 */
void predict(const mp_float_t x_k[7], const mp_float_t P_k[36], const mp_float_t w_k[3],
             mp_float_t dt, const mp_float_t Q[36], mp_float_t x_pred[7], mp_float_t P_pred[36])
{
  mp_float_t theta_tmp_tmp;
  mp_float_t theta_tmp[3];
  mp_float_t b_theta_tmp_tmp;
  mp_float_t c_theta_tmp_tmp;
  mp_float_t theta;
  mp_float_t b;
  mp_float_t d0;
  mp_float_t s_k[4];
  mp_float_t L[16];
  mp_float_t dv2[9];
  int i1;
  int L_tmp;
  int b_L_tmp;
  mp_float_t R[16];
  mp_float_t A_k[36];
  int i2;
  mp_float_t a[12];
  static const signed char b_a[12] = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  signed char b_I[9];
  mp_float_t c_a[12];
  static const signed char b_b[12] = { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

  mp_float_t b_A_k[36];

  /*  "control input" */
  theta_tmp_tmp = w_k[0] - x_k[4];
  theta_tmp[0] = theta_tmp_tmp;
  b_theta_tmp_tmp = w_k[1] - x_k[5];
  theta_tmp[1] = b_theta_tmp_tmp;
  c_theta_tmp_tmp = w_k[2] - x_k[6];
  theta_tmp[2] = c_theta_tmp_tmp;
  theta = b_norm(theta_tmp) * dt;
  b = sin(theta / 2.0);
  theta_tmp[0] = theta_tmp_tmp;
  theta_tmp[1] = b_theta_tmp_tmp;
  theta_tmp[2] = c_theta_tmp_tmp;
  d0 = b_norm(theta_tmp);
  s_k[0] = cos(theta / 2.0);
  s_k[1] = theta_tmp_tmp / d0 * b;
  s_k[2] = b_theta_tmp_tmp / d0 * b;
  s_k[3] = c_theta_tmp_tmp / d0 * b;

  /*  error quaternion */
  /*  propagate quaternion */
  /*  equivalent to quaternion multiplication q1*q2 for scalar first */
  /*  quaternions */
  /*  for left quaternion multiplication q1*q2 = L(q1)q2 */
  memset(&L[0], 0, sizeof(mp_float_t) << 4);
  L[4] = -x_k[1];
  L[8] = -x_k[2];
  L[12] = -x_k[3];
  L[0] = x_k[0];
  L[1] = x_k[1];
  L[2] = x_k[2];
  L[3] = x_k[3];

  /*  Returns skew symmetric - cross product matrix of a vector */
  dv2[0] = 0.0;
  dv2[3] = -x_k[3];
  dv2[6] = x_k[2];
  dv2[1] = x_k[3];
  dv2[4] = 0.0;
  dv2[7] = -x_k[1];
  dv2[2] = -x_k[2];
  dv2[5] = x_k[1];
  dv2[8] = 0.0;
  for (i1 = 0; i1 < 3; i1++) {
    L_tmp = (1 + i1) << 2;
    L[L_tmp + 1] = x_k[0] * (mp_float_t)iv0[3 * i1] + dv2[3 * i1];
    b_L_tmp = 1 + 3 * i1;
    L[L_tmp + 2] = x_k[0] * (mp_float_t)iv0[b_L_tmp] + dv2[b_L_tmp];
    b_L_tmp = 2 + 3 * i1;
    L[L_tmp + 3] = x_k[0] * (mp_float_t)iv0[b_L_tmp] + dv2[b_L_tmp];
  }

  /*  update bias */
  /*  pack up state */
  for (i1 = 0; i1 < 4; i1++) {
    x_pred[i1] = ((L[i1] * s_k[0] + L[i1 + 4] * s_k[1]) + L[i1 + 8] * s_k[2]) +
      L[i1 + 12] * s_k[3];
  }

  x_pred[4] = x_k[4];
  x_pred[5] = x_k[5];
  x_pred[6] = x_k[6];

  /* --------------------Predict Covariance-------------------- */
  /*  version from 236 notes */
  /*  for left quaternion multiplication q1*q2 = L(q1)q2 */
  memset(&L[0], 0, sizeof(mp_float_t) << 4);
  L[4] = -s_k[1];
  L[8] = -s_k[2];
  L[12] = -s_k[3];
  L[0] = s_k[0];
  L[1] = s_k[1];
  L[2] = s_k[2];
  L[3] = s_k[3];

  /*  Returns skew symmetric - cross product matrix of a vector */
  dv2[0] = 0.0;
  dv2[3] = -s_k[3];
  dv2[6] = s_k[2];
  dv2[1] = s_k[3];
  dv2[4] = 0.0;
  dv2[7] = -s_k[1];
  dv2[2] = -s_k[2];
  dv2[5] = s_k[1];
  dv2[8] = 0.0;
  for (i1 = 0; i1 < 3; i1++) {
    L_tmp = (1 + i1) << 2;
    L[L_tmp + 1] = s_k[0] * (mp_float_t)iv0[3 * i1] + dv2[3 * i1];
    b_L_tmp = 1 + 3 * i1;
    L[L_tmp + 2] = s_k[0] * (mp_float_t)iv0[b_L_tmp] + dv2[b_L_tmp];
    b_L_tmp = 2 + 3 * i1;
    L[L_tmp + 3] = s_k[0] * (mp_float_t)iv0[b_L_tmp] + dv2[b_L_tmp];
  }

  /*  for right quaternion multiplication q1*q2 = R(q2)*q1 */
  memset(&R[0], 0, sizeof(mp_float_t) << 4);
  R[4] = -s_k[1];
  R[8] = -s_k[2];
  R[12] = -s_k[3];
  R[0] = s_k[0];
  R[1] = s_k[1];
  R[2] = s_k[2];
  R[3] = s_k[3];

  /*  Returns skew symmetric - cross product matrix of a vector */
  dv2[0] = 0.0;
  dv2[3] = -s_k[3];
  dv2[6] = s_k[2];
  dv2[1] = s_k[3];
  dv2[4] = 0.0;
  dv2[7] = -s_k[1];
  dv2[2] = -s_k[2];
  dv2[5] = s_k[1];
  dv2[8] = 0.0;
  for (i1 = 0; i1 < 3; i1++) {
    L_tmp = (1 + i1) << 2;
    R[L_tmp + 1] = s_k[0] * (mp_float_t)iv0[3 * i1] - dv2[3 * i1];
    b_L_tmp = 1 + 3 * i1;
    R[L_tmp + 2] = s_k[0] * (mp_float_t)iv0[b_L_tmp] - dv2[b_L_tmp];
    b_L_tmp = 2 + 3 * i1;
    R[L_tmp + 3] = s_k[0] * (mp_float_t)iv0[b_L_tmp] - dv2[b_L_tmp];
  }

  memset(&A_k[0], 0, 36U * sizeof(mp_float_t));
  for (i1 = 0; i1 < 3; i1++) {
    for (i2 = 0; i2 < 4; i2++) {
      L_tmp = i1 + 3 * i2;
      a[L_tmp] = 0.0;
      a[L_tmp] = (((mp_float_t)b_a[i1] * L[i2] + (mp_float_t)b_a[i1 + 3] * L[i2 + 4]) +
                  (mp_float_t)b_a[i1 + 6] * L[i2 + 8]) + (mp_float_t)b_a[i1 + 9] * L[i2
        + 12];
    }

    for (i2 = 0; i2 < 4; i2++) {
      L_tmp = i1 + 3 * i2;
      c_a[L_tmp] = 0.0;
      b_L_tmp = i2 << 2;
      c_a[L_tmp] = ((a[i1] * R[b_L_tmp] + a[i1 + 3] * R[1 + b_L_tmp]) + a[i1 + 6]
                    * R[2 + b_L_tmp]) + a[i1 + 9] * R[3 + b_L_tmp];
    }

    for (i2 = 0; i2 < 3; i2++) {
      L_tmp = i1 + 6 * i2;
      A_k[L_tmp] = 0.0;
      b_L_tmp = i2 << 2;
      A_k[L_tmp] = ((c_a[i1] * (mp_float_t)b_b[b_L_tmp] + c_a[i1 + 3] * (mp_float_t)b_b
                     [1 + b_L_tmp]) + c_a[i1 + 6] * (mp_float_t)b_b[2 + b_L_tmp]) +
        c_a[i1 + 9] * (mp_float_t)b_b[3 + b_L_tmp];
    }
  }

  theta_tmp_tmp = -0.5 * dt;
  for (i1 = 0; i1 < 3; i1++) {
    L_tmp = 6 * (3 + i1);
    A_k[L_tmp] = theta_tmp_tmp * (mp_float_t)iv0[3 * i1];
    A_k[1 + L_tmp] = theta_tmp_tmp * (mp_float_t)iv0[1 + 3 * i1];
    A_k[2 + L_tmp] = theta_tmp_tmp * (mp_float_t)iv0[2 + 3 * i1];
  }

  for (i1 = 0; i1 < 9; i1++) {
    b_I[i1] = 0;
  }

  b_I[0] = 1;
  b_I[4] = 1;
  b_I[8] = 1;
  for (i1 = 0; i1 < 3; i1++) {
    L_tmp = 6 * (3 + i1);
    A_k[L_tmp + 3] = b_I[3 * i1];
    A_k[L_tmp + 4] = b_I[1 + 3 * i1];
    A_k[L_tmp + 5] = b_I[2 + 3 * i1];
  }

  /*  try 279C version */
  for (i1 = 0; i1 < 6; i1++) {
    for (i2 = 0; i2 < 6; i2++) {
      L_tmp = i1 + 6 * i2;
      b_A_k[L_tmp] = 0.0;
      d0 = 0.0;
      for (b_L_tmp = 0; b_L_tmp < 6; b_L_tmp++) {
        d0 += A_k[i1 + 6 * b_L_tmp] * P_k[b_L_tmp + 6 * i2];
      }

      b_A_k[L_tmp] = d0;
    }

    for (i2 = 0; i2 < 6; i2++) {
      d0 = 0.0;
      for (b_L_tmp = 0; b_L_tmp < 6; b_L_tmp++) {
        d0 += b_A_k[i1 + 6 * b_L_tmp] * A_k[i2 + 6 * b_L_tmp];
      }

      L_tmp = i1 + 6 * i2;
      P_pred[L_tmp] = d0 + Q[L_tmp];
    }
  }
}

/*
 * File trailer for predict.c
 *
 * [EOF]
 */
