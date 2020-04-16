/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: get_magnetic_field.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 11-Apr-2020 23:47:25
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
#include "get_magnetic_field.h"

/* Function Definitions */


/*
 * Arguments    : mp_float_t lat
 *                mp_float_t lon
 *                mp_float_t alt
 *                mp_float_t year
 *                mp_float_t order
 * Return Type  : mp_float_t B_NED[3]
 */
void get_magnetic_field(mp_float_t lat, mp_float_t lon, mp_float_t alt, mp_float_t year, mp_float_t order, mp_float_t B_NED[3])
{
  mp_float_t B_r;
  mp_float_t B_lat;
  mp_float_t B_lon;
  mp_float_t x_tmp;
  int loop_ub_tmp;
  int b_loop_ub_tmp;
  mp_float_t P_data[144];
  mp_float_t P_data_tmp;
  int b_P_data_tmp;
  int i0;
  int n;
  int i1;
  mp_float_t Pd_data[144];
  int m;
  mp_float_t prev2;
  mp_float_t c_P_data_tmp;
  mp_float_t d_P_data_tmp;
  int e_P_data_tmp;
  mp_float_t coef_tmp;
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

  mp_float_t b_coef_tmp;
  mp_float_t c_coef_tmp;
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

  /*      /$ */
  /*      lat is geocentric latitude in degrees */
  /*      lon is longitude in degrees */
  /*      alt is altitude in km */
  /*      year is the fractional year (include months/days essentially) */
  /*   */
  /*      outputs B vector in NED */
  /*      $/ */
  // order = fmin(order, 10.0F);
  lat = (90.0F - lat) * 0.0174532924F;
  lon *= 0.0174532924F;

  /*      // radius of earth */
  /*      // year since 2015 for secular variation */
  /*      // magnetic field components */
  B_r = 0.0F;
  B_lat = 0.0F;
  B_lon = 0.0F;

  /*      // IGRF model B field calculation, n/m sets order (5) */
  /*  get coefficients */
  x_tmp = cosf(lat);
  loop_ub_tmp = (int)(order + 2.0F);
  b_loop_ub_tmp = loop_ub_tmp * loop_ub_tmp;
  if (0 <= b_loop_ub_tmp - 1) {
    memset(&P_data[0], 0, (unsigned int)(b_loop_ub_tmp * (int)sizeof(mp_float_t)));
  }

  P_data[0] = 1.0F;
  P_data_tmp = sqrtf(1.0F - x_tmp * x_tmp);
  b_P_data_tmp = 1 + loop_ub_tmp;
  P_data[b_P_data_tmp] = P_data_tmp;
  i0 = (int)(order + 1.0F);
  for (n = 0; n < i0; n++) {
    i1 = (int)((order + 1.0F) + 1.0F);
    for (m = 0; m < i1; m++) {
      if (((int)n != 0) || ((int)m != 1)) {
        prev2 = (1.0F + (mp_float_t)n) - 2.0F;
        if ((1.0F + (mp_float_t)n) - 2.0F < 0.0F) {
          prev2 = 0.0F;
        }

        if ((mp_float_t)m < 1.0 + (mp_float_t)n) {
          c_P_data_tmp = (mp_float_t)m * (mp_float_t)m;
          d_P_data_tmp = (1.0F + (mp_float_t)n) * (1.0F + (mp_float_t)n) - c_P_data_tmp;
          e_P_data_tmp = loop_ub_tmp * ((int)((mp_float_t)m + 1.0F) - 1);
          P_data[((int)(unsigned int)(mp_float_t)n + (int)(order + 2.0F) * (int)
                  (unsigned int)(mp_float_t)m) + 1] = (2.0F * (1.0F + (mp_float_t)n) -
            1.0F) / sqrtf(d_P_data_tmp) * x_tmp * P_data[((int)(1.0F + (mp_float_t)n)
            + e_P_data_tmp) - 1] - sqrtf((((1.0F + (mp_float_t)n) - 1.0F) * ((1.0F +
            (mp_float_t)n) - 1.0F) - c_P_data_tmp) / d_P_data_tmp) * P_data[((int)
            (prev2 + 1.0F) + e_P_data_tmp) - 1];
        } else {
          P_data[((int)(unsigned int)(mp_float_t)n + loop_ub_tmp * (int)(unsigned int)
                  (mp_float_t)m) + 1] = sqrtf(1.0F - 1.0F / (2.0F * (mp_float_t)m)) *
            P_data_tmp * P_data[((int)(1.0F + (mp_float_t)n) + loop_ub_tmp * ((int)
            (mp_float_t)m - 1)) - 1];
        }
      }
    }
  }

  if (0 <= b_loop_ub_tmp - 1) {
    memset(&Pd_data[0], 0, (unsigned int)(b_loop_ub_tmp * (int)sizeof(mp_float_t)));
  }

  Pd_data[b_P_data_tmp] = x_tmp;
  for (n = 0; n < i0; n++) {
    i1 = (int)((order + 1.0F) + 1.0F);
    for (m = 0; m < i1; m++) {
      if (((int)n != 0) || ((int)m != 1)) {
        if ((mp_float_t)m < 1.0 + (mp_float_t)n) {
          prev2 = (1.0F + (mp_float_t)n) - 2.0F;
          if ((1.0F + (mp_float_t)n) - 2.0F < 0.0F) {
            prev2 = 0.0F;
          }

          b_P_data_tmp = loop_ub_tmp * ((int)((mp_float_t)m + 1.0F) - 1);
          b_loop_ub_tmp = ((int)(1.0F + (mp_float_t)n) + b_P_data_tmp) - 1;
          P_data_tmp = (mp_float_t)m * (mp_float_t)m;
          c_P_data_tmp = (1.0F + (mp_float_t)n) * (1.0F + (mp_float_t)n) - P_data_tmp;
          Pd_data[((int)(unsigned int)(mp_float_t)n + (int)(order + 2.0F) * (int)
                   (unsigned int)(mp_float_t)m) + 1] = (2.0F * (1.0F + (mp_float_t)n) -
            1.0F) / sqrtf(c_P_data_tmp) * (x_tmp * Pd_data[b_loop_ub_tmp] -
            sqrtf(1.0F - x_tmp * x_tmp) * P_data[b_loop_ub_tmp]) - sqrtf((((1.0F
            + (mp_float_t)n) - 1.0F) * ((1.0F + (mp_float_t)n) - 1.0F) - P_data_tmp) /
            c_P_data_tmp) * Pd_data[((int)(prev2 + 1.0F) + b_P_data_tmp) - 1];
        } else {
          b_loop_ub_tmp = ((int)(1.0F + (mp_float_t)n) + loop_ub_tmp * ((int)(mp_float_t)m
            - 1)) - 1;
          Pd_data[((int)(unsigned int)(mp_float_t)n + loop_ub_tmp * (int)(unsigned
                    int)(mp_float_t)m) + 1] = sqrtf(1.0F - 1.0F / (2.0F * (mp_float_t)m)) *
            (sqrtf(1.0F - x_tmp * x_tmp) * Pd_data[b_loop_ub_tmp] + x_tmp *
             P_data[b_loop_ub_tmp]);
        }
      }
    }
  }

  for (n = 0; n < i0; n++) {
    i1 = (int)((order + 1.0F) + 1.0F);
    for (m = 0; m < i1; m++) {
      b_loop_ub_tmp = (int)((mp_float_t)m + 1.0F) - 1;
      b_P_data_tmp = (int)((1.0F + (mp_float_t)n) + 1.0F);
      e_P_data_tmp = (b_P_data_tmp + 11 * b_loop_ub_tmp) - 1;
      c_P_data_tmp = (mp_float_t)m * lon;
      d_P_data_tmp = powf(6371.2F / (6371.2F + alt), (1.0F + (mp_float_t)n) + 2.0F);
      coef_tmp = g[e_P_data_tmp] + (year - 2015.0F) * g_sv[e_P_data_tmp];
      b_coef_tmp = sinf(c_P_data_tmp);
      c_coef_tmp = h[e_P_data_tmp] + (year - 2015.0F) * h_sv[e_P_data_tmp];
      c_P_data_tmp = cosf(c_P_data_tmp);
      coef = d_P_data_tmp * (coef_tmp * c_P_data_tmp + c_coef_tmp * b_coef_tmp);

      /*              // Radial component */
      b_loop_ub_tmp = (b_P_data_tmp + loop_ub_tmp * b_loop_ub_tmp) - 1;
      B_r += ((1.0F + (mp_float_t)n) + 1.0F) * coef * P_data[b_loop_ub_tmp];

      /*              // Colatitudinal component */
      B_lat -= coef * Pd_data[b_loop_ub_tmp];

      /*              // Address singularity at colatitude of 0 */
      P_data_tmp = sinf(lat);
      if ((int)P_data_tmp == 0) {
        /*                  // Longitudinal component */
        B_lon += -x_tmp * d_P_data_tmp * (-(g[((int)((1.0F + (mp_float_t)n) + 1.0F) +
          11 * ((int)((mp_float_t)m + 1.0F) - 1)) - 1] + (year - 2015.0F) * g_sv
          [((int)((1.0F + (mp_float_t)n) + 1.0F) + 11 * ((int)((mp_float_t)m + 1.0F) - 1))
          - 1]) * sinf((mp_float_t)m * lon) + (h[((int)((1.0F + (mp_float_t)n) + 1.0F) +
          11 * ((int)((mp_float_t)m + 1.0F) - 1)) - 1] + (year - 2015.0F) * h_sv
          [((int)((1.0F + (mp_float_t)n) + 1.0F) + 11 * ((int)((mp_float_t)m + 1.0F) - 1))
          - 1]) * cosf((mp_float_t)m * lon)) * Pd_data[((int)((1.0F + (mp_float_t)n) +
          1.0F) + (int)(order + 2.0F) * ((int)((mp_float_t)m + 1.0F) - 1)) - 1];
      } else {
        B_lon += -1.0F / P_data_tmp * d_P_data_tmp * (mp_float_t)m * (-coef_tmp *
          b_coef_tmp + c_coef_tmp * c_P_data_tmp) * P_data[((int)((1.0F + (mp_float_t)
          n) + 1.0F) + (int)(order + 2.0F) * ((int)((mp_float_t)m + 1.0F) - 1)) - 1];
      }
    }
  }

  /*      // NED (North, East, Down) coordinate frame */
  B_NED[0] = -B_lat;
  B_NED[1] = B_lon;
  B_NED[2] = -B_r;

}

void ulab_get_magnetic_fieldC(mp_obj_t inputs, mp_obj_t B_NED) {

      // get pointers to input arrays
  ulab_ndarray_obj_t *inputs_obj = MP_OBJ_TO_PTR(inputs);
  ulab_ndarray_obj_t *B_NED_obj = MP_OBJ_TO_PTR(B_NED);


    // extract pointers to underlying C data arrays for input ulab arrays
  mp_float_t *inputs_data = (mp_float_t *)inputs_obj->array->items;
  mp_float_t *B_NED_data = (mp_float_t *)B_NED_obj->array->items;

  // Call the actual autocoded function, pray
  get_magnetic_field(inputs_data[0], inputs_data[1], inputs_data[2], inputs_data[3], inputs_data[4], B_NED_data);
   // Write over data in state/covariance vector/array

}



mp_obj_t ulab_get_magnetic_field(size_t n_args, const mp_obj_t *args) {

  ulab_get_magnetic_fieldC(args[0], args[1]);
  return MP_OBJ_FROM_PTR(mp_const_none);

}
