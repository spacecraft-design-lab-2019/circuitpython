#ifndef GET_MAGNETIC_FIELD_SERIES_H
#define GET_MAGNETIC_FIELD_SERIES_H

#include "shared-module/ulab/ndarray.h"

#if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_FLOAT
#define epsilon        1.2e-7
#elif MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_DOUBLE
#define epsilon        2.3e-16
#endif

#define JACOBI_MAX     20

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
// #include "rtwtypes.h"
// #include "get_magnetic_field_series_types.h"

/* Function Declarations */
mp_obj_t ulab_get_magnetic_field_series(size_t, const mp_obj_t *);

#endif

/*
 * File trailer for get_magnetic_field_series.h
 *
 * [EOF]
 */
