/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: get_magnetic_field_series.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 12-Apr-2020 08:21:46
 */

/* Include Files */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lib/utils/context_manager_helpers.h"
#include "shared-bindings/ulab/linalg.h"
#include "shared-bindings/ulab/ndarray.h"

#include "py/obj.h"
#include "py/runtime.h"
#include "py/misc.h"

#include "get_magnetic_field_series.h"

/* Function Declarations */
static mp_float_t b_norm(const mp_float_t x[3]);
static void fake_IGRF(const mp_float_t r[3], mp_float_t t, mp_float_t B_eci[3]);

/* Function Definitions */
mp_float_t b_norm(const mp_float_t x[3])
{
  mp_float_t y;
  mp_float_t scale;
  mp_float_t absxk;
  mp_float_t t;
  scale = 3.3121686421112381E-30;
  absxk = fabs(x[0]);
  if (absxk > 3.3121686421112381E-30) {
    y = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-30;
    y = t * t;
  }

  absxk = fabs(x[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = 1.0 + y * t * t;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  absxk = fabs(x[2]);
  if (absxk > scale) {
    t = scale / absxk;
    y = 1.0 + y * t * t;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  return scale * MICROPY_FLOAT_C_FUN(sqrt)(y);
}


/*
 * Arguments    : mp_float_t r[3]
 *                mp_float_t t
 *                mp_float_t B_eci[3]
 * Return Type  : void
 */
static void fake_IGRF(const mp_float_t r[3], mp_float_t t, mp_float_t B_eci[3])
{
  mp_float_t theta;
  mp_float_t f0;
  mp_float_t fv0[9];
  mp_float_t P_tmp;
  int b_P_tmp;
  mp_float_t r_ecef[3];
  mp_float_t lon;
  mp_float_t lat_tmp;
  mp_float_t lat;
  mp_float_t out_tmp;
  mp_float_t b_lat;
  mp_float_t b_lon;
  mp_float_t B_r;
  mp_float_t B_lat;
  mp_float_t B_lon;
  mp_float_t x_tmp;
  mp_float_t P[49];
  int n;
  mp_float_t Pd[49];
  int m;
  int prev2;
  mp_float_t coef_tmp;
  mp_float_t b_coef_tmp;
  static const mp_float_t h[121] = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 4797.1F, -2845.6F, -115.3F, 283.3F, 47.3F, -20.8F,
    -54.1F, 10.1F, -21.6F, 3.2F, 0.0F, 0.0F, -641.9F, 244.9F, -188.7F, 197.0F,
    33.2F, -19.5F, -18.3F, 10.8F, -0.4F, 0.0F, 0.0F, 0.0F, -538.4F, 180.9F,
    -119.3F, 58.9F, 5.7F, 13.3F, 11.8F, 4.6F, 0.0F, 0.0F, 0.0F, 0.0F, -329.5F,
    16.0F, -66.7F, 24.4F, -14.6F, -6.8F, 4.4F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    100.2F, 7.3F, 3.4F, 16.2F, -6.9F, -7.9F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    62.6F, -27.4F, 5.7F, 7.8F, -0.6F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    -2.2F, -9.1F, 1.0F, -4.2F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    2.1F, -4.0F, -2.8F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    8.4F, -1.2F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    -8.7F };

  static const mp_float_t h_sv[121] = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, -26.6F, -27.4F, 8.2F, -1.3F, 0.6F, 0.0F, 0.8F,
    -0.3F, 0.0F, 0.0F, 0.0F, 0.0F, -14.1F, -0.4F, 5.3F, 1.7F, -2.1F, 0.4F, 0.3F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.8F, 2.9F, -1.2F, -0.7F, -0.2F, 0.1F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, -5.2F, 3.4F, 0.2F, -0.3F, 0.5F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.9F, -0.6F, -0.2F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.1F, -0.3F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, -0.2F, 0.3F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F };

  mp_float_t coef;
  static const mp_float_t g[121] = { 0.0F, -29442.0F, -2445.1F, 1350.7F, 907.6F,
    -232.6F, 70.0F, 81.6F, 24.2F, 5.4F, -1.9F, 0.0F, -1501.0F, 3012.9F, -2352.3F,
    813.7F, 360.1F, 67.7F, -76.1F, 8.8F, 8.8F, -6.3F, 0.0F, 0.0F, 1676.7F,
    1225.6F, 120.4F, 192.4F, 72.7F, -6.8F, -16.9F, 3.1F, 0.1F, 0.0F, 0.0F, 0.0F,
    582.0F, -334.9F, -140.9F, -129.9F, 51.8F, -3.2F, -3.3F, 0.5F, 0.0F, 0.0F,
    0.0F, 0.0F, 70.4F, -157.5F, -28.9F, 15.0F, -20.6F, 0.7F, -0.5F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 4.1F, 13.2F, 9.4F, 13.4F, -13.3F, 1.8F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, -70.9F, -2.8F, 11.7F, -0.1F, -0.7F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 6.8F, -15.9F, 8.7F, 2.1F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, -2.0F, 9.1F, 2.4F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, -10.5F, -1.8F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, -3.6F };

  static const mp_float_t g_sv[121] = { 0.0F, 10.3F, -8.7F, 3.4F, -0.7F, -0.2F, -0.3F,
    0.3F, 0.2F, 0.0F, 0.0F, 0.0F, 18.1F, -3.3F, -5.5F, 0.2F, 0.5F, -0.1F, -0.2F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 2.1F, -0.7F, -9.1F, -1.3F, -0.7F, -0.5F, -0.6F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, -10.1F, 4.1F, -0.1F, 2.1F, 1.3F, 0.5F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, -4.3F, 1.4F, -1.2F, 0.1F, -0.2F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 3.9F, 0.3F, -0.6F, 0.4F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.6F, -0.8F, 0.1F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.2F, -0.4F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.3F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F };

  int i1;

  /*  Copyright (c) 2020 Robotic Exploration Lab */
  /*   */
  /*  Permission is hereby granted, free of charge, to any person obtaining a copy */
  /*  of this software and associated documentation files (the "Software"), to deal */
  /*  in the Software without restriction, including without limitation the rights */
  /*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell */
  /*  copies of the Software, and to permit persons to whom the Software is */
  /*  furnished to do so, subject to the following conditions: */
  /*   */
  /*  The above copyright notice and this permission notice shall be included in all */
  /*  copies or substantial portions of the Software. */
  /*   */
  /*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
  /*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, */
  /*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE */
  /*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
  /*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, */
  /*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE */
  /*  SOFTWARE. */
  /*  This function takes in a position in ECI (in km) and outputs */
  /*  a B vector in teslas that's in the ECI frame */
  /*  INPUTS: */
  /*    r - position in km */
  /*    t - time in MJD */
  /*  first, translate the position into a lat and long */
  /*  this function converts between position in ECI and time to lat, lon and */
  /*  alt */
  /*  INPUTS: */
  /*    r - position in km */
  /*    t - time in MJD */
  /*  OUTPUTS: */
  /*    lat - latitude in deg */
  /*    lon - longitude in deg */
  /*    alt - altitude in km */
  /*  convert to ecef */
  /*  converts a position from eci to ecef */
  /*  find days since 1/1/2000 at 12h */
  /*  converts between MJD and J2000 (days since 1/1/2000 at 12h) */
  /*  MJD epoch: 0h Nov 17, 1858 */
  /*  J2000 epoch: 12h Jan 1, 2000 */
  theta = (280.460602F + 360.985657F * (t - 51544.5F)) * 3.14159274F / 180.0F;

  /*  creates a z axis rotation matrix (rad) */
  /*  get the rotation matrix */
  f0 = MICROPY_FLOAT_C_FUN(cos)(theta);
  fv0[0] = f0;
  P_tmp = MICROPY_FLOAT_C_FUN(sin)(theta);
  fv0[3] = P_tmp;
  fv0[6] = 0.0F;
  fv0[1] = -P_tmp;
  fv0[4] = f0;
  fv0[7] = 0.0F;
  fv0[2] = 0.0F;
  fv0[5] = 0.0F;
  fv0[8] = 1.0F;
  for (b_P_tmp = 0; b_P_tmp < 3; b_P_tmp++) {
    r_ecef[b_P_tmp] = 0.0F;
    r_ecef[b_P_tmp] = (fv0[b_P_tmp] * r[0] + fv0[b_P_tmp + 3] * r[1]) +
      fv0[b_P_tmp + 6] * r[2];
  }

  /*  get lat and lon */
  lon = MICROPY_FLOAT_C_FUN(atan2)(r_ecef[1], r_ecef[0]) * 180.0F / 3.14159274F;
  lat_tmp = b_norm(r_ecef);
  lat = MICROPY_FLOAT_C_FUN(asin)(r_ecef[2] / lat_tmp) * 180.0F / 3.14159274F;

  /*  find B_NED */
  /*  gets the year out of MJD */
  out_tmp = roundf((t / 365.25F + 1858.0F) + 0.87945205F);

  /*      /$ */
  /*      lat is geocentric latitude in degrees */
  /*      lon is longitude in degrees */
  /*      alt is altitude in km */
  /*      year is the fractional year (include months/days essentially) */
  /*   */
  /*      outputs B vector in NED */
  /*      $/ */
  b_lat = (90.0F - lat) * 0.0174532924F;
  b_lon = lon * 0.0174532924F;

  /*      // radius of earth */
  /*      // year since 2015 for secular variation */
  /*      // magnetic field components */
  B_r = 0.0F;
  B_lat = 0.0F;
  B_lon = 0.0F;

  /*      // IGRF model B field calculation, n/m sets order (5) */
  /*  get coefficients */
  x_tmp = MICROPY_FLOAT_C_FUN(cos)(b_lat);
  memset(&P[0], 0, 49U * sizeof(mp_float_t));
  P[0] = 1.0F;
  theta = MICROPY_FLOAT_C_FUN(sqrt)(1.0F - x_tmp * x_tmp);
  P[8] = theta;
  for (n = 0; n < 6; n++) {
    for (m = 0; m < 7; m++) {
      if ((1 + n != 1) || (m != 1)) {
        prev2 = n - 1;
        if (n - 1 < 0) {
          prev2 = 0;
        }

        if (m < 1 + n) {
          b_P_tmp = m * m;
          P_tmp = (mp_float_t)((1 + n) * (1 + n) - b_P_tmp);
          P[(n + 7 * m) + 1] = (2.0F * (1.0F + (mp_float_t)n) - 1.0F) / MICROPY_FLOAT_C_FUN(sqrt)(P_tmp) *
            x_tmp * P[n + 7 * m] - MICROPY_FLOAT_C_FUN(sqrt)((mp_float_t)(n * n - b_P_tmp) / P_tmp) *
            P[prev2 + 7 * m];
        } else {
          P[(n + 7 * m) + 1] = MICROPY_FLOAT_C_FUN(sqrt)(1.0F - 1.0F / (2.0F * (mp_float_t)m)) * theta *
            P[n + 7 * (m - 1)];
        }
      }
    }
  }

  memset(&Pd[0], 0, 49U * sizeof(mp_float_t));
  Pd[8] = x_tmp;
  for (n = 0; n < 6; n++) {
    for (m = 0; m < 7; m++) {
      if ((1 + n != 1) || (m != 1)) {
        if (m < 1 + n) {
          prev2 = n - 1;
          if (n - 1 < 0) {
            prev2 = 0;
          }

          b_P_tmp = m * m;
          theta = (mp_float_t)((1 + n) * (1 + n) - b_P_tmp);
          Pd[(n + 7 * m) + 1] = (2.0F * (1.0F + (mp_float_t)n) - 1.0F) / MICROPY_FLOAT_C_FUN(sqrt)(theta)
            * (x_tmp * Pd[n + 7 * m] - MICROPY_FLOAT_C_FUN(sqrt)(1.0F - x_tmp * x_tmp) * P[n + 7 * m])
            - MICROPY_FLOAT_C_FUN(sqrt)((mp_float_t)(n * n - b_P_tmp) / theta) * Pd[prev2 + 7 * m];
        } else {
          b_P_tmp = n + 7 * (m - 1);
          Pd[(n + 7 * m) + 1] = MICROPY_FLOAT_C_FUN(sqrt)(1.0F - 1.0F / (2.0F * (mp_float_t)m)) * (MICROPY_FLOAT_C_FUN(sqrt)(1.0F - x_tmp * x_tmp) * Pd[b_P_tmp] + x_tmp * P[b_P_tmp]);
        }
      }

      b_P_tmp = (n + 11 * m) + 1;
      theta = (mp_float_t)m * b_lon;
      coef_tmp = powf(6371.2F / (6371.2F + (lat_tmp - 6378.0F)), (1.0F + (mp_float_t)
        n) + 2.0F);
      P_tmp = MICROPY_FLOAT_C_FUN(sin)(theta);
      b_coef_tmp = h[b_P_tmp] + (out_tmp - 2015.0F) * h_sv[b_P_tmp];
      theta = MICROPY_FLOAT_C_FUN(cos)(theta);
      coef = coef_tmp * ((g[b_P_tmp] + (out_tmp - 2015.0F) * g_sv[b_P_tmp]) *
                         theta + b_coef_tmp * P_tmp);

      /*              // Radial component */
      i1 = (n + 7 * m) + 1;
      B_r += ((1.0F + (mp_float_t)n) + 1.0F) * coef * P[i1];

      /*              // Colatitudinal component */
      B_lat -= coef * Pd[(n + 7 * m) + 1];

      /*              // Address singularity at colatitude of 0 */
      f0 = MICROPY_FLOAT_C_FUN(sin)(b_lat);
      if ((int)f0 == 0) {
        /*                  // Longitudinal component */
        B_lon += -x_tmp * coef_tmp * (-(g[b_P_tmp] + (out_tmp - 2015.0F) *
          g_sv[b_P_tmp]) * MICROPY_FLOAT_C_FUN(sin)((mp_float_t)m * b_lon) + (h[b_P_tmp] + (out_tmp -
          2015.0F) * h_sv[b_P_tmp]) * MICROPY_FLOAT_C_FUN(cos)((mp_float_t)m * b_lon)) * Pd[(n + 7 * m) +
          1];
      } else {
        B_lon += -1.0F / f0 * coef_tmp * (mp_float_t)m * (-(g[b_P_tmp] + (out_tmp -
          2015.0F) * g_sv[b_P_tmp]) * P_tmp + b_coef_tmp * theta) * P[i1];
      }
    }
  }

  /*      // NED (North, East, Down) coordinate frame */
  /*  convert to eci */
  lat = lat * 3.14159274F / 180.0F;
  lon = lon * 3.14159274F / 180.0F;

  /*  this function converts a vector from ned to eci */
  /*  first find ENU */
  f0 = MICROPY_FLOAT_C_FUN(sin)(lon);
  fv0[0] = -f0;
  P_tmp = MICROPY_FLOAT_C_FUN(cos)(lon);
  fv0[1] = P_tmp;
  fv0[2] = 0.0F;
  coef_tmp = MICROPY_FLOAT_C_FUN(sin)(lat);
  fv0[3] = -coef_tmp * P_tmp;
  fv0[4] = -coef_tmp * f0;
  theta = MICROPY_FLOAT_C_FUN(cos)(lat);
  fv0[5] = theta;
  fv0[6] = theta * P_tmp;
  fv0[7] = theta * f0;
  fv0[8] = coef_tmp;
  for (b_P_tmp = 0; b_P_tmp < 3; b_P_tmp++) {
    B_eci[b_P_tmp] = 0.0F;
    B_eci[b_P_tmp] = (fv0[b_P_tmp] * -B_lat + fv0[b_P_tmp + 3] * B_lon) +
      fv0[b_P_tmp + 6] * -B_r;
  }
}

/*
 * Arguments    : const mp_float_tx0[6]
 *                const mp_float_tt[500]
 *                mp_float_tB_eci_vec[1497]
 *                mp_float_tX[3000]
 * Return Type  : void
 */
void get_magnetic_field_series(const mp_float_t x0[6], const mp_float_t t[1000], mp_float_t B_eci_vec[3000], mp_float_t X[6000])
{
  int i0;
  int i;
  mp_float_t dt;
  mp_float_t y;
  mp_float_t k1[6];
  mp_float_t c[6];
  mp_float_t k2[6];
  mp_float_t k3[6];
  mp_float_t b_dt[6];

  int N = 1000;

  /*  Copyright (c) 2020 Robotic Exploration Lab */
  /*   */
  /*  Permission is hereby granted, free of charge, to any person obtaining a copy */
  /*  of this software and associated documentation files (the "Software"), to deal */
  /*  in the Software without restriction, including without limitation the rights */
  /*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell */
  /*  copies of the Software, and to permit persons to whom the Software is */
  /*  furnished to do so, subject to the following conditions: */
  /*   */
  /*  The above copyright notice and this permission notice shall be included in all */
  /*  copies or substantial portions of the Software. */
  /*   */
  /*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
  /*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, */
  /*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE */
  /*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
  /*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, */
  /*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE */
  /*  SOFTWARE. */
  /*  This function takes in a series of positions in ECI (in km) as well as a  */
  /*  time series in MJD and outputs a series of ECI magnetic field vectors */
  /*  INPUT: */
  /*  x0 - [r0;v0;q0;w0]; */
  /*  t - time vector in MJD */
  /*  OUTPUT: */
  /*  B_eci_vec - magnetic field in ECI in Teslas */
  /*  Copyright (c) 2020 Robotic Exploration Lab */
  /*   */
  /*  Permission is hereby granted, free of charge, to any person obtaining a copy */
  /*  of this software and associated documentation files (the "Software"), to deal */
  /*  in the Software without restriction, including without limitation the rights */
  /*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell */
  /*  copies of the Software, and to permit persons to whom the Software is */
  /*  furnished to do so, subject to the following conditions: */
  /*   */
  /*  The above copyright notice and this permission notice shall be included in all */
  /*  copies or substantial portions of the Software. */
  /*   */
  /*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
  /*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, */
  /*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE */
  /*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
  /*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, */
  /*  OUT OF OR IN CONNECTteION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE */
  /*  SOFTWARE. */
  /*  This function takes in an initial position and velocity of a satellite along  */
  /*  with a time series and outputs the positions over the time using a simple */
  /*  point mass two body approximation */
  /*  INPUTS: */
  /*  x0 - initial state */
  /*      r0 - initial position in km */
  /*      v0 - initial velocity in km/s */
  /*      q0 - initial quaternion */
  /*      w0 - initial rotation rate */
  /*  t - time series in seconds */
  /*  OUTPUTS: */
  /*  X - array of states with columns: */
  /*      r - position in Earth radii */
  /*      v - velocity in km/s */
  /*  initialize empty array */
  // memset(&X[0], 0, 3000U * sizeof(mp_float_t));
  for (i0 = 0; i0 < 6; i0++) {
    X[i0] = x0[i0];
  }

  /*  fill it */
  for (i = 0; i < N-1; i++) {
    dt = (t[i + 1] - t[i]) * 86400.0F;
    y = powf(b_norm(*(mp_float_t (*)[3])&(*(mp_float_t (*)[6])&X[6 * i])[0]), 3.0F);
    k1[0] = dt * X[6 * i + 3];
    k1[3] = dt * (-398600.0F * X[6 * i] / y);
    k1[1] = dt * X[6 * i + 4];
    k1[4] = dt * (-398600.0F * X[1 + 6 * i] / y);
    k1[2] = dt * X[6 * i + 5];
    k1[5] = dt * (-398600.0F * X[2 + 6 * i] / y);
    for (i0 = 0; i0 < 6; i0++) {
      c[i0] = X[i0 + 6 * i] + k1[i0] / 2.0F;
    }

    y = powf(b_norm(*(mp_float_t (*)[3])&c[0]), 3.0F);
    k2[0] = dt * c[3];
    k2[3] = dt * (-398600.0F * c[0] / y);
    k2[1] = dt * c[4];
    k2[4] = dt * (-398600.0F * c[1] / y);
    k2[2] = dt * c[5];
    k2[5] = dt * (-398600.0F * c[2] / y);
    for (i0 = 0; i0 < 6; i0++) {
      c[i0] = X[i0 + 6 * i] + k2[i0] / 2.0F;
    }

    y = powf(b_norm(*(mp_float_t (*)[3])&c[0]), 3.0F);
    k3[0] = dt * c[3];
    k3[3] = dt * (-398600.0F * c[0] / y);
    k3[1] = dt * c[4];
    k3[4] = dt * (-398600.0F * c[1] / y);
    k3[2] = dt * c[5];
    k3[5] = dt * (-398600.0F * c[2] / y);
    for (i0 = 0; i0 < 6; i0++) {
      c[i0] = X[i0 + 6 * i] + k3[i0];
    }

    y = powf(b_norm(*(mp_float_t (*)[3])&c[0]), 3.0F);
    b_dt[0] = dt * c[3];
    b_dt[3] = dt * (-398600.0F * c[0] / y);
    b_dt[1] = dt * c[4];
    b_dt[4] = dt * (-398600.0F * c[1] / y);
    b_dt[2] = dt * c[5];
    b_dt[5] = dt * (-398600.0F * c[2] / y);
    for (i0 = 0; i0 < 6; i0++) {
      b_dt[i0] = X[i0 + 6 * i] + 0.166666672F * (((k1[i0] + 2.0F * k2[i0]) +
        2.0F * k3[i0]) + b_dt[i0]);
    }

    for (i0 = 0; i0 < 6; i0++) {
      X[i0 + 6 * (i + 1)] = b_dt[i0];
    }
  }

  for (i = 0; i < N-1; i++) {
    fake_IGRF(*(mp_float_t (*)[3])&X[6 * i], t[i], *(mp_float_t (*)[3])&B_eci_vec[3 * i]);
  }
}

void ulab_get_magnetic_field_seriesC(mp_obj_t x0, mp_obj_t t, mp_obj_t B_eci_vec, mp_obj_t X) {


      // get pointers to input arrays
  ulab_ndarray_obj_t *x0_obj = MP_OBJ_TO_PTR(x0);
  ulab_ndarray_obj_t *t_obj = MP_OBJ_TO_PTR(t);
  ulab_ndarray_obj_t *B_eci_vec_obj = MP_OBJ_TO_PTR(B_eci_vec);
  ulab_ndarray_obj_t *X_obj = MP_OBJ_TO_PTR(X);

    // extract pointers to underlying C data arrays for input ulab arrays
  mp_float_t *x0_data = (mp_float_t *)x0_obj->array->items;
  mp_float_t *t_data = (mp_float_t *)t_obj->array->items;
  mp_float_t *B_eci_vec_data = (mp_float_t *)B_eci_vec_obj->array->items;
  mp_float_t *X_data = (mp_float_t *)X_obj->array->items;
      
  // Call the actual autocoded function, pray
  get_magnetic_field_series(x0_data, t_data, B_eci_vec_data, X_data);
   // Write over data in state/covariance vector/array

}

mp_obj_t ulab_get_magnetic_field_series(size_t n_args, const mp_obj_t *args) {

  ulab_get_magnetic_field_seriesC(args[0], args[1], args[2], args[3]);
  return MP_OBJ_FROM_PTR(mp_const_none);

}


