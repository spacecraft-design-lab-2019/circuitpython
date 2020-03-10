#ifndef MICROPY_INCLUDED_SHARED_BINDINGS_ULAB_CONTROLLER_H
#define MICROPY_INCLUDED_SHARED_BINDINGS_ULAB_CONTROLLER_H

#include "shared-module/ulab/ndarray.h"

#if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_FLOAT
#define epsilon        1.2e-7
#elif MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_DOUBLE
#define epsilon        2.3e-16
#endif

#define JACOBI_MAX     20

mp_obj_t ulab_controller_bdot(mp_obj_t );
mp_obj_t ulab_controller_cholesky(mp_obj_t A_input, mp_obj_t R_input);
mp_obj_t ulab_controller_MEKFstep(size_t, const mp_obj_t *args);
// void ulab_controller_MEKFstep(mp_obj_t x_k_input, mp_obj_t P_k_input, mp_obj_t w_k_input, mp_obj_t r_sun_body_input, mp_obj_t r_B_body_input,
//   mp_obj_t r_sun_inert_input, mp_obj_t r_B_inert_input, mp_obj_t Q_input, mp_obj_t R_input, mp_float_t dt);

#endif // MICROPY_INCLUDED_SHARED_BINDINGS_ULAB_CONTROLLER_H