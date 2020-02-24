/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: innovation.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 23-Feb-2020 17:14:39
 */

/* Include Files */
#include <string.h>
#include "MEKFstep.h"
#include "innovation.h"
#include "MEKFstep_data.h"

/* Function Definitions */

/*
 * ------------------- Innovation step for state -------------------
 *  Get rotation matrix based on quaternion giving rotation from body to
 *  inertial reference frame
 * Arguments    : const double x_k[7]
 *                const double P_k[36]
 *                const double r_sun_body[3]
 *                const double r_B_body[3]
 *                const double r_sun_inert[3]
 *                const double r_B_inert[3]
 *                const double R[36]
 *                double z[6]
 *                double S[36]
 *                double C[36]
 * Return Type  : void
 */
void innovation(const double x_k[7], const double P_k[36], const double
                r_sun_body[3], const double r_B_body[3], const double
                r_sun_inert[3], const double r_B_inert[3], const double R[36],
                double z[6], double S[36], double C[36])
{
  double L[16];
  double R_N2B[9];
  int i3;
  int L_tmp;
  double b_R[16];
  int b_L_tmp;
  int i4;
  double b_L[16];
  double d1;
  double b_R_N2B[36];
  double b_r_sun_inert[6];
  double y[3];
  double b_y[3];
  double d2;
  double d3;

  /*  returns the rotation matrix from body to inertial frame if a body to */
  /*  inertial quaternion is given */
  /*  for left quaternion multiplication q1*q2 = L(q1)q2 */
  memset(&L[0], 0, sizeof(double) << 4);
  L[4] = -x_k[1];
  L[8] = -x_k[2];
  L[12] = -x_k[3];
  L[0] = x_k[0];
  L[1] = x_k[1];
  L[2] = x_k[2];
  L[3] = x_k[3];

  /*  Returns skew symmetric - cross product matrix of a vector */
  R_N2B[0] = 0.0;
  R_N2B[3] = -x_k[3];
  R_N2B[6] = x_k[2];
  R_N2B[1] = x_k[3];
  R_N2B[4] = 0.0;
  R_N2B[7] = -x_k[1];
  R_N2B[2] = -x_k[2];
  R_N2B[5] = x_k[1];
  R_N2B[8] = 0.0;
  for (i3 = 0; i3 < 3; i3++) {
    L_tmp = (1 + i3) << 2;
    L[L_tmp + 1] = x_k[0] * (double)iv0[3 * i3] + R_N2B[3 * i3];
    b_L_tmp = 1 + 3 * i3;
    L[L_tmp + 2] = x_k[0] * (double)iv0[b_L_tmp] + R_N2B[b_L_tmp];
    b_L_tmp = 2 + 3 * i3;
    L[L_tmp + 3] = x_k[0] * (double)iv0[b_L_tmp] + R_N2B[b_L_tmp];
  }

  /*  for right quaternion multiplication q1*q2 = R(q2)*q1 */
  memset(&b_R[0], 0, sizeof(double) << 4);
  b_R[4] = -x_k[1];
  b_R[8] = -x_k[2];
  b_R[12] = -x_k[3];
  b_R[0] = x_k[0];
  b_R[1] = x_k[1];
  b_R[2] = x_k[2];
  b_R[3] = x_k[3];

  /*  Returns skew symmetric - cross product matrix of a vector */
  R_N2B[0] = 0.0;
  R_N2B[3] = -x_k[3];
  R_N2B[6] = x_k[2];
  R_N2B[1] = x_k[3];
  R_N2B[4] = 0.0;
  R_N2B[7] = -x_k[1];
  R_N2B[2] = -x_k[2];
  R_N2B[5] = x_k[1];
  R_N2B[8] = 0.0;
  for (i3 = 0; i3 < 3; i3++) {
    b_L_tmp = (1 + i3) << 2;
    b_R[b_L_tmp + 1] = x_k[0] * (double)iv0[3 * i3] - R_N2B[3 * i3];
    L_tmp = 1 + 3 * i3;
    b_R[b_L_tmp + 2] = x_k[0] * (double)iv0[L_tmp] - R_N2B[L_tmp];
    L_tmp = 2 + 3 * i3;
    b_R[b_L_tmp + 3] = x_k[0] * (double)iv0[L_tmp] - R_N2B[L_tmp];
  }

  /*  Transpose that to get the matrix we actually need for our prefit */
  /*  residuals */
  for (i3 = 0; i3 < 4; i3++) {
    for (i4 = 0; i4 < 4; i4++) {
      L_tmp = i3 + (i4 << 2);
      b_L[L_tmp] = 0.0;
      b_L[L_tmp] = ((L[i4] * b_R[i3] + L[i4 + 4] * b_R[i3 + 4]) + L[i4 + 8] *
                    b_R[i3 + 8]) + L[i4 + 12] * b_R[i3 + 12];
    }
  }

  /*  prefit residuals of sun sensor and magnetometer measurements */
  for (i3 = 0; i3 < 3; i3++) {
    i4 = (1 + i3) << 2;
    d1 = b_L[i4 + 1];
    R_N2B[3 * i3] = d1;
    b_R_N2B[6 * i3] = d1;
    b_L_tmp = 6 * (i3 + 3);
    b_R_N2B[b_L_tmp] = 0.0;
    b_R_N2B[6 * i3 + 3] = 0.0;
    b_R_N2B[b_L_tmp + 3] = d1;
    d1 = b_L[i4 + 2];
    R_N2B[1 + 3 * i3] = d1;
    b_R_N2B[1 + 6 * i3] = d1;
    b_R_N2B[1 + b_L_tmp] = 0.0;
    b_R_N2B[6 * i3 + 4] = 0.0;
    b_R_N2B[b_L_tmp + 4] = d1;
    d1 = b_L[i4 + 3];
    R_N2B[2 + 3 * i3] = d1;
    b_R_N2B[2 + 6 * i3] = d1;
    b_R_N2B[2 + b_L_tmp] = 0.0;
    b_R_N2B[6 * i3 + 5] = 0.0;
    b_R_N2B[b_L_tmp + 5] = d1;
    b_r_sun_inert[i3] = r_sun_inert[i3];
    b_r_sun_inert[i3 + 3] = r_B_inert[i3];
    z[i3] = r_sun_body[i3];
    z[i3 + 3] = r_B_body[i3];
  }

  for (i3 = 0; i3 < 6; i3++) {
    d1 = 0.0;
    for (i4 = 0; i4 < 6; i4++) {
      d1 += b_R_N2B[i3 + 6 * i4] * b_r_sun_inert[i4];
    }

    z[i3] -= d1;
  }

  /*  --------------------- Innovation/pre-fit covariance ------------- */
  /*  Returns skew symmetric - cross product matrix of a vector */
  /*  Returns skew symmetric - cross product matrix of a vector */
  C[0] = 0.0;
  for (i3 = 0; i3 < 3; i3++) {
    y[i3] = 0.0;
    b_y[i3] = 0.0;
    b_L_tmp = 6 * (i3 + 3);
    C[b_L_tmp] = 0.0;
    C[b_L_tmp + 3] = 0.0;
    d1 = R_N2B[i3 + 3];
    d2 = R_N2B[i3] * r_sun_inert[0] + d1 * r_sun_inert[1];
    d3 = R_N2B[i3] * r_B_inert[0] + d1 * r_B_inert[1];
    C[1 + b_L_tmp] = 0.0;
    C[b_L_tmp + 4] = 0.0;
    d1 = R_N2B[i3 + 6];
    d2 += d1 * r_sun_inert[2];
    d3 += d1 * r_B_inert[2];
    C[2 + b_L_tmp] = 0.0;
    C[b_L_tmp + 5] = 0.0;
    b_y[i3] = d3;
    y[i3] = d2;
  }

  C[6] = -y[2];
  C[12] = y[1];
  C[1] = y[2];
  C[7] = 0.0;
  C[13] = -y[0];
  C[2] = -y[1];
  C[8] = y[0];
  C[14] = 0.0;
  C[3] = 0.0;
  C[9] = -b_y[2];
  C[15] = b_y[1];
  C[4] = b_y[2];
  C[10] = 0.0;
  C[16] = -b_y[0];
  C[5] = -b_y[1];
  C[11] = b_y[0];
  C[17] = 0.0;
  for (i3 = 0; i3 < 6; i3++) {
    for (i4 = 0; i4 < 6; i4++) {
      b_L_tmp = i3 + 6 * i4;
      b_R_N2B[b_L_tmp] = 0.0;
      d1 = 0.0;
      for (L_tmp = 0; L_tmp < 6; L_tmp++) {
        d1 += C[i3 + 6 * L_tmp] * P_k[L_tmp + 6 * i4];
      }

      b_R_N2B[b_L_tmp] = d1;
    }

    for (i4 = 0; i4 < 6; i4++) {
      d1 = 0.0;
      for (L_tmp = 0; L_tmp < 6; L_tmp++) {
        d1 += b_R_N2B[i3 + 6 * L_tmp] * C[i4 + 6 * L_tmp];
      }

      b_L_tmp = i3 + 6 * i4;
      S[b_L_tmp] = d1 + R[b_L_tmp];
    }
  }
}

/*
 * File trailer for innovation.c
 *
 * [EOF]
 */
