#ifndef MICROPY_INCLUDED_SHARED_BINDINGS_ULAB_NDARRAY_H
#define MICROPY_INCLUDED_SHARED_BINDINGS_ULAB_NDARRAY_H

/*
 * This file is part of the micropython-ulab project, 
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Zoltán Vörös
*/
    
#ifndef _NDARRAY_
#define _NDARRAY_
#endif

#include "shared-module/ulab/ndarray.h"

#include "py/objarray.h"
#include "py/binary.h"
#include "py/objstr.h"
#include "py/objlist.h"
#include "py/bc.h"
#include "py/binary.h"
#include "py/gc.h"
#include "py/obj.h"
#include "py/objfun.h"
#include "py/objint.h"
#include "py/objtype.h"
#include "py/runtime.h"


#define PRINT_MAX  10

#if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_FLOAT
#define FLOAT_TYPECODE 'f'
#elif MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_DOUBLE
#define FLOAT_TYPECODE 'd'
#endif

extern const mp_obj_type_t ulab_ndarray_type;

enum NDARRAY_TYPE {
    NDARRAY_UINT8 = 'B',
    NDARRAY_INT8 = 'b',
    NDARRAY_UINT16 = 'H', 
    NDARRAY_INT16 = 'h',
    NDARRAY_FLOAT = FLOAT_TYPECODE,
};

mp_obj_t ulab_ndarray_shape(mp_obj_t );
void ulab_ndarray_print_row(const mp_print_t *, mp_obj_array_t *, size_t , size_t );
void ulab_ndarray_print(const mp_print_t *, mp_obj_t , mp_print_kind_t );
mp_obj_t ulab_ndarray_copy(mp_obj_t );
mp_obj_t ulab_ndarray_make_new(const mp_obj_type_t *, size_t, const mp_obj_t *, mp_map_t *);

#define CREATE_SINGLE_ITEM(outarray, type, typecode, value) do {\
    ndarray_obj_t *tmp = create_new_ndarray(1, 1, (typecode));\
    type *tmparr = (type *)tmp->array->items;\
    tmparr[0] = (type)(value);\
    (outarray) = MP_OBJ_FROM_PTR(tmp);\
} while(0)

extern void shared_module_ulab_ndarray_deinit(ulab_ndarray_obj_t* self);

#endif // MICROPY_INCLUDED_SHARED_BINDINGS_ULAB_NDARRAY_H