#ifndef MICROPY_INCLUDED_SHARED_BINDINGS_ULAB_LINALG_H
#define MICROPY_INCLUDED_SHARED_BINDINGS_ULAB_LINALG_H

#ifndef _LINALG_
#define _LINALG_
#endif

#include "shared-module/ulab/ndarray.h"

#define SWAP(t, a, b) { t tmp = a; a = b; b = tmp; }

#if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_FLOAT
#define epsilon        1.2e-7
#elif MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_DOUBLE
#define epsilon        2.3e-16
#endif

#define JACOBI_MAX     20

mp_obj_t ulab_linalg_transpose(mp_obj_t );
mp_obj_t ulab_linalg_reshape(mp_obj_t , mp_obj_t );
mp_obj_t ulab_linalg_size(size_t , const mp_obj_t *, mp_map_t *);

bool ulab_linalg_invert_matrix(mp_float_t *, size_t );
mp_obj_t ulab_linalg_inv(mp_obj_t );

#endif // MICROPY_INCLUDED_SHARED_BINDINGS_ULAB_LINALG_H