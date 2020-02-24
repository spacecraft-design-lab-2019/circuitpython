/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: innovation.h
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 23-Feb-2020 17:14:39
 */

#ifndef INNOVATION_H
#define INNOVATION_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "py/obj.h"
#include "py/runtime.h"
#include "py/misc.h"

/* Function Declarations */
extern void innovation(const mp_float_t x_k[7], const mp_float_t P_k[36], const mp_float_t
  r_sun_body[3], const mp_float_t r_B_body[3], const mp_float_t r_sun_inert[3], const
  mp_float_t r_B_inert[3], const mp_float_t R[36], mp_float_t z[6], mp_float_t S[36], mp_float_t C
  [36]);

#endif

/*
 * File trailer for innovation.h
 *
 * [EOF]
 */
