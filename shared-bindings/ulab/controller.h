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


#endif // MICROPY_INCLUDED_SHARED_BINDINGS_ULAB_CONTROLLER_H