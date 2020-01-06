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
mp_obj_t ulab_ndarray_rawsize(mp_obj_t );
mp_obj_t ulab_ndarray_flatten(size_t , const mp_obj_t *, mp_map_t *);
mp_obj_t ulab_ndarray_asbytearray(mp_obj_t );

mp_obj_t ulab_ndarray_copy(mp_obj_t );
mp_obj_t ulab_ndarray_make_new(const mp_obj_type_t *, size_t, const mp_obj_t *, mp_map_t *);
mp_obj_t ulab_ndarray_subscr(mp_obj_t , mp_obj_t , mp_obj_t );
mp_obj_t ulab_ndarray_getiter(mp_obj_t , mp_obj_iter_buf_t *);
mp_obj_t ulab_ndarray_binary_op(mp_binary_op_t , mp_obj_t , mp_obj_t );
mp_obj_t ulab_ndarray_unary_op(mp_unary_op_t , mp_obj_t );

void ulab_ndarray_print_row(const mp_print_t *, mp_obj_array_t *, size_t , size_t );
void ulab_ndarray_print(const mp_print_t *, mp_obj_t , mp_print_kind_t );
ulab_ndarray_obj_t *create_new_ndarray(size_t , size_t , uint8_t );

#define CREATE_SINGLE_ITEM(outarray, type, typecode, value) do {\
    ulab_ndarray_obj_t *tmp = create_new_ndarray(1, 1, (typecode));\
    type *tmparr = (type *)tmp->array->items;\
    tmparr[0] = (type)(value);\
    (outarray) = MP_OBJ_FROM_PTR(tmp);\
} while(0)

#define RUN_BINARY_LOOP(typecode, type_out, type_left, type_right, ol, or, op) do {\
    type_left *left = (type_left *)(ol)->array->items;\
    type_right *right = (type_right *)(or)->array->items;\
    uint8_t inc = 0;\
    if((or)->array->len > 1) inc = 1;\
    if(((op) == MP_BINARY_OP_ADD) || ((op) == MP_BINARY_OP_SUBTRACT) || ((op) == MP_BINARY_OP_MULTIPLY)) {\
        ulab_ndarray_obj_t *out = create_new_ndarray(ol->m, ol->n, typecode);\
        type_out *(odata) = (type_out *)out->array->items;\
        if((op) == MP_BINARY_OP_ADD) { for(size_t i=0, j=0; i < (ol)->array->len; i++, j+=inc) odata[i] = left[i] + right[j];}\
        if((op) == MP_BINARY_OP_SUBTRACT) { for(size_t i=0, j=0; i < (ol)->array->len; i++, j+=inc) odata[i] = left[i] - right[j];}\
        if((op) == MP_BINARY_OP_MULTIPLY) { for(size_t i=0, j=0; i < (ol)->array->len; i++, j+=inc) odata[i] = left[i] * right[j];}\
        return MP_OBJ_FROM_PTR(out);\
    } else if((op) == MP_BINARY_OP_TRUE_DIVIDE) {\
        ulab_ndarray_obj_t *out = create_new_ndarray(ol->m, ol->n, NDARRAY_FLOAT);\
        mp_float_t *odata = (mp_float_t *)out->array->items;\
        for(size_t i=0, j=0; i < (ol)->array->len; i++, j+=inc) odata[i] = (mp_float_t)left[i]/(mp_float_t)right[j];\
        return MP_OBJ_FROM_PTR(out);\
    } else if(((op) == MP_BINARY_OP_LESS) || ((op) == MP_BINARY_OP_LESS_EQUAL) ||  \
             ((op) == MP_BINARY_OP_MORE) || ((op) == MP_BINARY_OP_MORE_EQUAL)) {\
        mp_obj_t out_list = mp_obj_new_list(0, NULL);\
        size_t m = (ol)->m, n = (ol)->n;\
        for(size_t i=0, r=0; i < m; i++, r+=inc) {\
            mp_obj_t row = mp_obj_new_list(n, NULL);\
            mp_obj_list_t *row_ptr = MP_OBJ_TO_PTR(row);\
            for(size_t j=0, s=0; j < n; j++, s+=inc) {\
                row_ptr->items[j] = mp_const_false;\
                if((op) == MP_BINARY_OP_LESS) {\
                    if(left[i*n+j] < right[r*n+s]) row_ptr->items[j] = mp_const_true;\
                } else if((op) == MP_BINARY_OP_LESS_EQUAL) {\
                    if(left[i*n+j] <= right[r*n+s]) row_ptr->items[j] = mp_const_true;\
                } else if((op) == MP_BINARY_OP_MORE) {\
                    if(left[i*n+j] > right[r*n+s]) row_ptr->items[j] = mp_const_true;\
                } else if((op) == MP_BINARY_OP_MORE_EQUAL) {\
                    if(left[i*n+j] >= right[r*n+s]) row_ptr->items[j] = mp_const_true;\
                }\
            }\
            if(m == 1) return row;\
            mp_obj_list_append(out_list, row);\
        }\
        return out_list;\
    }\
} while(0)

#endif // MICROPY_INCLUDED_SHARED_BINDINGS_ULAB_NDARRAY_H