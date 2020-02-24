/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: norm.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 23-Feb-2020 17:14:39
 */

/* Include Files */
#include <math.h>
#include "MEKFstep.h"
#include "norm.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/misc.h"

/* Function Definitions */

/*
 * Arguments    : const mp_float_t x[3]
 * Return Type  : mp_float_t
 */
mp_float_t b_norm(const mp_float_t x[3])
{
  mp_float_t y;
  mp_float_t scale;
  mp_float_t absxk;
  mp_float_t t;
  scale = 3.3121686421112381E-170;
  absxk = fabs(x[0]);
  if (absxk > 3.3121686421112381E-170) {
    y = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
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

  return scale * sqrt(y);
}

/*
 * File trailer for norm.c
 *
 * [EOF]
 */
