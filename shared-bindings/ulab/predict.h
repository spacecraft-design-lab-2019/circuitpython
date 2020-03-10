/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: predict.h
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 23-Feb-2020 17:14:39
 */

#ifndef PREDICT_H
#define PREDICT_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "py/obj.h"
#include "py/runtime.h"
#include "py/misc.h"

/* Function Declarations */
extern void predict(const mp_float_t x_k[7], const mp_float_t P_k[36], const mp_float_t w_k[3],
             mp_float_t dt, const mp_float_t Q[36], mp_float_t x_pred[7], mp_float_t P_pred[36]);

#endif

/*
 * File trailer for predict.h
 *
 * [EOF]
 */
