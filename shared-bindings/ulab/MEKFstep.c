/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: MEKFstep.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 23-Feb-2020 17:14:39
 */

/* Include Files */
#include <math.h>
#include <string.h>
#include "MEKFstep.h"
#include "innovation.h"
#include "predict.h"
#include "MEKFstep_data.h"

/* Function Definitions */

/*
 * Arguments    : const double x_k[7]
 *                const double P_k[36]
 *                const double w_k[3]
 *                const double r_sun_body[3]
 *                const double r_B_body[3]
 *                const double r_sun_inert[3]
 *                const double r_B_inert[3]
 *                const double Q[36]
 *                const double R[36]
 *                double dt
 *                double x_k1[7]
 *                double P_k1[36]
 * Return Type  : void
 */
void MEKFstep(const double x_k[7], const double P_k[36], const double w_k[3],
              const double r_sun_body[3], const double r_B_body[3], const double
              r_sun_inert[3], const double r_B_inert[3], const double Q[36],
              const double R[36], double dt, double x_k1[7], double P_k1[36])
{
  double x_pred[7];
  double P_pred[36];
  double z[6];
  double S[36];
  double C[36];
  int i0;
  int j;
  int k;
  int jAcol;
  signed char ipiv[6];
  int ix;
  int jj;
  double L[36];
  int jA;
  int jp1j;
  double smax;
  int n;
  int iy;
  double dx[6];
  double s;
  double b_L[16];
  double dv0[9];
  double dv1[4];
  double P_k1_tmp[36];
  predict(x_k, P_k, w_k, dt, Q, x_pred, P_pred);
  innovation(x_pred, P_pred, r_sun_body, r_B_body, r_sun_inert, r_B_inert, R, z,
             S, C);
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
  memset(&b_L[0], 0, sizeof(double) << 4);
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
    b_L[ix + 1] = x_pred[0] * (double)iv0[3 * i0] + dv0[3 * i0];
    iy = 1 + 3 * i0;
    b_L[ix + 2] = x_pred[0] * (double)iv0[iy] + dv0[iy];
    iy = 2 + 3 * i0;
    b_L[ix + 3] = x_pred[0] * (double)iv0[iy] + dv0[iy];
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
  memset(&S[0], 0, 36U * sizeof(double));
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

/*
 * File trailer for MEKFstep.c
 *
 * [EOF]
 */
