/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: MEKFstep.h
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 23-Feb-2020 17:14:39
 */

#ifndef MEKFSTEP_H
#define MEKFSTEP_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "MEKFstep_types.h"

/* Function Declarations */
extern void MEKFstep(const double x_k[7], const double P_k[36], const double
                     w_k[3], const double r_sun_body[3], const double r_B_body[3],
                     const double r_sun_inert[3], const double r_B_inert[3],
                     const double Q[36], const double R[36], double dt, double
                     x_k1[7], double P_k1[36]);

#endif

/*
 * File trailer for MEKFstep.h
 *
 * [EOF]
 */
