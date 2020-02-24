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
#include "rtwtypes.h"
#include "MEKFstep_types.h"

/* Function Declarations */
extern void predict(const double x_k[7], const double P_k[36], const double w_k
                    [3], double dt, const double Q[36], double x_pred[7], double
                    P_pred[36]);

#endif

/*
 * File trailer for predict.h
 *
 * [EOF]
 */
