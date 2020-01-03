/*
 * This file is part of the micropython-ulab project, 
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Zoltán Vörös
*/
    
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lib/utils/context_manager_helpers.h"

#include "py/objproperty.h"
#include "py/runtime0.h"
#include "py/runtime.h"
#include "py/binary.h"
#include "py/obj.h"
#include "py/objtuple.h"
#include "shared-bindings/ulab/ndarray.h"
#include "shared-bindings/util.h"
//| .. currentmodule:: ulab
//|
//| :class:`ndarray` -- the primary way to play with arrays.
//| ====================================================================================
//|


//|
//| .. method:: array_new
//|
//|   helps to create a new array.

extern const mp_obj_type_t ulab_ndarray_type;

STATIC mp_obj_array_t *array_new(char typecode, size_t n) {
    int typecode_size = mp_binary_get_size('@', typecode, NULL);
    mp_obj_array_t *o = m_new_obj(mp_obj_array_t);
    // this step could probably be skipped: we are never going to store a bytearray per se
    #if MICROPY_PY_BUILTINS_BYTEARRAY && MICROPY_PY_ARRAY
    o->base.type = (typecode == BYTEARRAY_TYPECODE) ? &mp_type_bytearray : &mp_type_array;
    #elif MICROPY_PY_BUILTINS_BYTEARRAY
    o->base.type = &mp_type_bytearray;
    #else
    o->base.type = &mp_type_array;
    #endif
    o->typecode = typecode;
    o->free = 0;
    o->len = n;
    o->items = m_new(byte, typecode_size * o->len);
    return o;
}

//|
//| .. method:: ndarray_get_float_value
//|
//|   returns the mp_float_t type given data.


mp_float_t ulab_ndarray_get_float_value(void *data, uint8_t typecode, size_t index) {
    if(typecode == NDARRAY_UINT8) {
        return (mp_float_t)((uint8_t *)data)[index];
    } else if(typecode == NDARRAY_INT8) {
        return (mp_float_t)((int8_t *)data)[index];
    } else if(typecode == NDARRAY_UINT16) {
        return (mp_float_t)((uint16_t *)data)[index];
    } else if(typecode == NDARRAY_INT16) {
        return (mp_float_t)((int16_t *)data)[index];
    } else {
        return (mp_float_t)((mp_float_t *)data)[index];
    }
}

//|
//| .. method:: fill_array_iterable
//|
//|   fills the array after you initialize it.

void fill_array_iterable(mp_float_t *array, mp_obj_t iterable) {
    mp_obj_iter_buf_t x_buf;
    mp_obj_t x_item, x_iterable = mp_getiter(iterable, &x_buf);
    size_t i=0;
    while ((x_item = mp_iternext(x_iterable)) != MP_OBJ_STOP_ITERATION) {
        array[i] = (mp_float_t)mp_obj_get_float(x_item);
        i++;
    }
}

//|
//| .. method:: ndarray_print_row
//|
//|   prints a row, you need this to print any array.

void ulab_ndarray_print_row(const mp_print_t *print, mp_obj_array_t *data, size_t n0, size_t n) {
    mp_print_str(print, "[");
    if(n < PRINT_MAX) { // if the array is short, print everything
        mp_obj_print_helper(print, mp_binary_get_val_array(data->typecode, data->items, n0), PRINT_REPR);
        for(size_t i=1; i<n; i++) {
            mp_print_str(print, ", ");
            mp_obj_print_helper(print, mp_binary_get_val_array(data->typecode, data->items, n0+i), PRINT_REPR);
        }
    } else {
        mp_obj_print_helper(print, mp_binary_get_val_array(data->typecode, data->items, n0), PRINT_REPR);
        for(size_t i=1; i<3; i++) {
            mp_print_str(print, ", ");
            mp_obj_print_helper(print, mp_binary_get_val_array(data->typecode, data->items, n0+i), PRINT_REPR);
        }
        mp_printf(print, ", ..., ");
        mp_obj_print_helper(print, mp_binary_get_val_array(data->typecode, data->items, n0+n-3), PRINT_REPR);
        for(size_t i=1; i<3; i++) {
            mp_print_str(print, ", ");
            mp_obj_print_helper(print, mp_binary_get_val_array(data->typecode, data->items, n0+n-3+i), PRINT_REPR);
        }
    }
    mp_print_str(print, "]");
}

//|
//| .. method:: ndarray_print
//|
//|   prints an entire ndarray type.

void ulab_ndarray_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    ulab_ndarray_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_print_str(print, "array(");
    
    if(self->array->len == 0) {
        mp_print_str(print, "[]");
    } else {
        if((self->m == 1) || (self->n == 1)) {
            ulab_ndarray_print_row(print, self->array, 0, self->array->len);
        } else {
            // TODO: add vertical ellipses for the case, when self->m > PRINT_MAX
            mp_print_str(print, "[");
            ulab_ndarray_print_row(print, self->array, 0, self->n);
            for(size_t i=1; i < self->m; i++) {
                mp_print_str(print, ",\n\t ");
                ulab_ndarray_print_row(print, self->array, i*self->n, self->n);
            }
            mp_print_str(print, "]");
        }
    }
    if(self->array->typecode == NDARRAY_UINT8) {
        mp_print_str(print, ", dtype=uint8)");
    } else if(self->array->typecode == NDARRAY_INT8) {
        mp_print_str(print, ", dtype=int8)");
    } else if(self->array->typecode == NDARRAY_UINT16) {
        mp_print_str(print, ", dtype=uint16)");
    } else if(self->array->typecode == NDARRAY_INT16) {
        mp_print_str(print, ", dtype=int16)");
    } else if(self->array->typecode == NDARRAY_FLOAT) {
        mp_print_str(print, ", dtype=float)");
    }
}

//STATIC MP_DEFINE_CONST_FUN_OBJ_1(ulab_ndarray_print_obj, ulab_ndarray_print);

/*const mp_obj_property_t ulab_ndarray_printed_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&ulab_ndarray_print_obj,
              (mp_obj_t)&mp_const_none_obj},
};*/


//|
//| .. method:: init_helper
//|
//|   helps with initialization.


STATIC uint8_t ulab_ndarray_init_helper(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)} },
        { MP_QSTR_dtype, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = NDARRAY_FLOAT } },
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(1, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    
    uint8_t dtype = args[1].u_int;
    return dtype;
}

//|
//| .. method:: ndarray_assign_elements
//|
//|   assigns elements.

void ulab_ndarray_assign_elements(mp_obj_array_t *data, mp_obj_t iterable, uint8_t typecode, size_t *idx) {
    // assigns a single row in the matrix
    mp_obj_t item;
    while ((item = mp_iternext(iterable)) != MP_OBJ_STOP_ITERATION) {
        mp_binary_set_val_array(typecode, data->items, (*idx)++, item);
    }
}


ulab_ndarray_obj_t *create_new_ndarray(size_t m, size_t n, uint8_t typecode) {
    // Creates the base ndarray with shape (m, n), and initialises the values to straight 0s
    ulab_ndarray_obj_t *ndarray = m_new_obj(ulab_ndarray_obj_t);
    ndarray->base.type = &ulab_ndarray_type;
    ndarray->m = m;
    ndarray->n = n;
    mp_obj_array_t *array = array_new(typecode, m*n);
    ndarray->bytes = m * n * mp_binary_get_size('@', typecode, NULL);
    // this should set all elements to 0, irrespective of the of the typecode (all bits are zero)
    // we could, perhaps, leave this step out, and initialise the array only, when needed
    memset(array->items, 0, ndarray->bytes); 
    ndarray->array = array;
    return ndarray;
}

//|
//| .. method:: ndarray_copy
//|
//|   returns a verbatim copy of self_in.

mp_obj_t ulab_ndarray_copy(mp_obj_t self_in) {
    // returns a verbatim (shape and typecode) copy of self_in
    ulab_ndarray_obj_t *self = MP_OBJ_TO_PTR(self_in);
    ulab_ndarray_obj_t *out = create_new_ndarray(self->m, self->n, self->array->typecode);
    memcpy(out->array->items, self->array->items, self->bytes);
    return MP_OBJ_FROM_PTR(out);
}

//|
//| .. method:: make_new
//|
//|   Create an object.

mp_obj_t ulab_ndarray_make_new(const mp_obj_type_t *type, size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    mp_arg_check_num(n_args, kw_args, 1, 2, true);
    size_t n_kw = 0;
    if (kw_args != 0) {
        n_kw = kw_args->used;
    }
    mp_map_init_fixed_table(kw_args, n_kw, args + n_args);
    uint8_t dtype = ulab_ndarray_init_helper(n_args, args, kw_args);

    size_t len1, len2=0, i=0;
    mp_obj_t len_in = mp_obj_len_maybe(args[0]);
    if (len_in == MP_OBJ_NULL) {
        mp_raise_ValueError(translate("first argument must be an iterable"));
    } else {
        // len1 is either the number of rows (for matrices), or the number of elements (row vectors)
        len1 = MP_OBJ_SMALL_INT_VALUE(len_in);
    }

    // We have to figure out, whether the first element of the iterable is an iterable itself
    // Perhaps, there is a more elegant way of handling this
    mp_obj_iter_buf_t iter_buf1;
    mp_obj_t item1, iterable1 = mp_getiter(args[0], &iter_buf1);
    while ((item1 = mp_iternext(iterable1)) != MP_OBJ_STOP_ITERATION) {
        len_in = mp_obj_len_maybe(item1);
        if(len_in != MP_OBJ_NULL) { // indeed, this seems to be an iterable
            // Next, we have to check, whether all elements in the outer loop have the same length
            if(i > 0) {
                size_t temp = abs(MP_OBJ_SMALL_INT_VALUE(len_in));
                if(len2 != temp) {
                    mp_raise_ValueError(translate("iterables are not of the same length"));
                }
            }
            len2 = MP_OBJ_SMALL_INT_VALUE(len_in);
            i++;
        }
    }
    // By this time, it should be established, what the shape is, so we can now create the array
    ulab_ndarray_obj_t *self = create_new_ndarray((len2 == 0) ? 1 : len1, (len2 == 0) ? len1 : len2, dtype);
    iterable1 = mp_getiter(args[0], &iter_buf1);
    i = 0;
    if(len2 == 0) { // the first argument is a single iterable
        ulab_ndarray_assign_elements(self->array, iterable1, dtype, &i);
    } else {
        mp_obj_iter_buf_t iter_buf2;
        mp_obj_t iterable2; 

        while ((item1 = mp_iternext(iterable1)) != MP_OBJ_STOP_ITERATION) {
            iterable2 = mp_getiter(item1, &iter_buf2);
            ulab_ndarray_assign_elements(self->array, iterable2, dtype, &i);
        }
    }
    return MP_OBJ_FROM_PTR(self);
}

//|   .. method:: slice_length()
//|
//|       returns the length of the slice.
//|

size_t slice_length(mp_bound_slice_t slice) {
    // TODO: check, whether this is true!
    if(slice.step < 0) {
        slice.step = -slice.step;
        return (slice.start - slice.stop) / slice.step;
    } else {
        return (slice.stop - slice.start) / slice.step;        
    }
}

//|   .. method:: true_length()
//|
//|       returns the length of the bool (or a list of them).
//|

size_t true_length(mp_obj_t bool_list) {
    // returns the number of Trues in a Boolean list
    // I wonder, wouldn't this be faster, if we looped through bool_list->items instead?
    mp_obj_iter_buf_t iter_buf;
    mp_obj_t item, iterable = mp_getiter(bool_list, &iter_buf);
    size_t trues = 0;
    while((item = mp_iternext(iterable)) != MP_OBJ_STOP_ITERATION) {
        if(!MP_OBJ_IS_TYPE(item, &mp_type_bool)) {
            // numpy seems to be a little bit inconsistent in when an index is considered
            // to be True/False. Bail out immediately, if the items are not True/False
            return 0;
        }
        if(mp_obj_is_true(item)) {
            trues++;
        }
    }
    return trues;
}

//|   .. method:: generate_slice()
//|
//|       creates a slice for a given index.
//|


mp_bound_slice_t generate_slice(mp_uint_t n, mp_obj_t index) {
    // micropython seems to have difficulties with negative steps
    mp_bound_slice_t slice;
    if(MP_OBJ_IS_TYPE(index, &mp_type_slice)) {
        mp_seq_get_fast_slice_indexes(n, index, &slice);
    } else if(MP_OBJ_IS_INT(index)) {
        uint32_t _index = mp_obj_get_int(index);
        if(_index < 0) {
            _index += n;
        } 
        if((_index >= n) || (_index < 0)) {
            mp_raise_msg(&mp_type_IndexError, translate("index is out of bounds"));
        }
        slice.start = _index;
        slice.stop = _index + 1;
        slice.step = 1;
    } else {
        mp_raise_msg(&mp_type_IndexError, translate("indices must be integers, slices, or Boolean lists"));
    }
    return slice;
}

//|   .. method:: simple_slice()
//|
//|      generates a simple slice using a start, stop, and step. 

mp_bound_slice_t simple_slice(int16_t start, int16_t stop, int16_t step) {
    mp_bound_slice_t slice;
    slice.start = start;
    slice.stop = stop;
    slice.step = step;
    return slice;
}

//|   .. method:: insert_binary_value()
//|
//|

void insert_binary_value(ulab_ndarray_obj_t *ndarray, size_t nd_index, ulab_ndarray_obj_t *values, size_t value_index) {
    // there is probably a more elegant implementation...
    mp_obj_t tmp = mp_binary_get_val_array(values->array->typecode, values->array->items, value_index);
    if((values->array->typecode == NDARRAY_FLOAT) && (ndarray->array->typecode != NDARRAY_FLOAT)) {
        // workaround: rounding seems not to work in the arm compiler
        int32_t x = (int32_t)floorf(mp_obj_get_float(tmp)+0.5);
        tmp = mp_obj_new_int(x);
    }
    mp_binary_set_val_array(ndarray->array->typecode, ndarray->array->items, nd_index, tmp); 
}

//|   .. method:: insert_slice_list()
//|
//|     columns and rows are built through slices
//|


mp_obj_t insert_slice_list(ulab_ndarray_obj_t *ndarray, size_t m, size_t n, 
                            mp_bound_slice_t row, mp_bound_slice_t column, 
                            mp_obj_t row_list, mp_obj_t column_list, 
                            ulab_ndarray_obj_t *values) {
    if((m != values->m) && (n != values->n)) {
        if((values->array->len != 1)) { // not a single item
            mp_raise_ValueError(translate("could not broadast input array from shape"));
        }
    }
    size_t cindex, rindex;
    // M, and N are used to manipulate how the source index is incremented in the loop
    uint8_t M = 1, N = 1;
    if(values->m == 1) {
        M = 0;
    }
    if(values->n == 1) {
        N = 0;
    }
    
    if(row_list == mp_const_none) { // rows are indexed by a slice
        rindex = row.start;
        if(column_list == mp_const_none) { // columns are indexed by a slice
            for(size_t i=0; i < m; i++) {
                cindex = column.start;
                for(size_t j=0; j < n; j++) {
                    insert_binary_value(ndarray, rindex*ndarray->n+cindex, values, i*M*n+j*N);
                    cindex += column.step;
                }
                rindex += row.step;
            }
        } else { // columns are indexed by a Boolean list
            mp_obj_iter_buf_t column_iter_buf;
            mp_obj_t column_item, column_iterable;
            for(size_t i=0; i < m; i++) {
                column_iterable = mp_getiter(column_list, &column_iter_buf);
                size_t j = 0;
                cindex = 0;
                while((column_item = mp_iternext(column_iterable)) != MP_OBJ_STOP_ITERATION) {
                    if(mp_obj_is_true(column_item)) {
                        insert_binary_value(ndarray, rindex*ndarray->n+cindex, values, i*M*n+j*N);
                        j++;
                    }
                    cindex++;
                }
                rindex += row.step;
            }
        }
    } else { // rows are indexed by a Boolean list
        mp_obj_iter_buf_t row_iter_buf;
        mp_obj_t row_item, row_iterable;
        row_iterable = mp_getiter(row_list, &row_iter_buf);
        size_t i = 0;
        rindex = 0;
        if(column_list == mp_const_none) { // columns are indexed by a slice
            while((row_item = mp_iternext(row_iterable)) != MP_OBJ_STOP_ITERATION) {
                if(mp_obj_is_true(row_item)) {
                    cindex = column.start;
                    for(size_t j=0; j < n; j++) {
                        insert_binary_value(ndarray, rindex*ndarray->n+cindex, values, i*M*n+j*N);
                        cindex += column.step;
                    }
                    i++;
                }
                rindex++;
            } 
        } else { // columns are indexed by a list
            mp_obj_iter_buf_t column_iter_buf;
            mp_obj_t column_item, column_iterable;
            size_t j = 0; 
            cindex = 0;
            while((row_item = mp_iternext(row_iterable)) != MP_OBJ_STOP_ITERATION) {
                if(mp_obj_is_true(row_item)) {
                    column_iterable = mp_getiter(column_list, &column_iter_buf);                   
                    while((column_item = mp_iternext(column_iterable)) != MP_OBJ_STOP_ITERATION) {
                        if(mp_obj_is_true(column_item)) {
                            insert_binary_value(ndarray, rindex*ndarray->n+cindex, values, i*M*n+j*N);
                            j++;
                        }
                        cindex++;
                    }
                    i++;
                }
                rindex++;
            }
        }
    }
    return mp_const_none;
}

//|   .. method:: iterate_slice_list()
//|
//|      iterates over an array to get the list of slices. 
//|


mp_obj_t iterate_slice_list(ulab_ndarray_obj_t *ndarray, size_t m, size_t n, 
                            mp_bound_slice_t row, mp_bound_slice_t column, 
                            mp_obj_t row_list, mp_obj_t column_list, 
                            ulab_ndarray_obj_t *values) {
    if((m == 0) || (n == 0)) {
        mp_raise_msg(&mp_type_IndexError, translate("empty index range"));
    }

    if(values != NULL) {
        return insert_slice_list(ndarray, m, n, row, column, row_list, column_list, values);
    }
    uint8_t _sizeof = mp_binary_get_size('@', ndarray->array->typecode, NULL);
    ulab_ndarray_obj_t *out = create_new_ndarray(m, n, ndarray->array->typecode);
    uint8_t *target = (uint8_t *)out->array->items;
    uint8_t *source = (uint8_t *)ndarray->array->items;
    size_t cindex, rindex;    
    if(row_list == mp_const_none) { // rows are indexed by a slice
        rindex = row.start;
        if(column_list == mp_const_none) { // columns are indexed by a slice
            for(size_t i=0; i < m; i++) {
                cindex = column.start;
                for(size_t j=0; j < n; j++) {
                    memcpy(target+(i*n+j)*_sizeof, source+(rindex*ndarray->n+cindex)*_sizeof, _sizeof);
                    cindex += column.step;
                }
                rindex += row.step;
            }
        } else { // columns are indexed by a Boolean list
            // TODO: the list must be exactly as long as the axis
            mp_obj_iter_buf_t column_iter_buf;
            mp_obj_t column_item, column_iterable;
            for(size_t i=0; i < m; i++) {
                column_iterable = mp_getiter(column_list, &column_iter_buf);
                size_t j = 0;
                cindex = 0;
                while((column_item = mp_iternext(column_iterable)) != MP_OBJ_STOP_ITERATION) {
                    if(mp_obj_is_true(column_item)) {
                        memcpy(target+(i*n+j)*_sizeof, source+(rindex*ndarray->n+cindex)*_sizeof, _sizeof);
                        j++;
                    }
                    cindex++;
                }
                rindex += row.step;
            }
        }
    } else { // rows are indexed by a Boolean list
        mp_obj_iter_buf_t row_iter_buf;
        mp_obj_t row_item, row_iterable;
        row_iterable = mp_getiter(row_list, &row_iter_buf);
        size_t i = 0;
        rindex = 0;
        if(column_list == mp_const_none) { // columns are indexed by a slice
            while((row_item = mp_iternext(row_iterable)) != MP_OBJ_STOP_ITERATION) {
                if(mp_obj_is_true(row_item)) {
                    cindex = column.start;
                    for(size_t j=0; j < n; j++) {
                        memcpy(target+(i*n+j)*_sizeof, source+(rindex*ndarray->n+cindex)*_sizeof, _sizeof);
                        cindex += column.step;
                    }
                    i++;
                }
                rindex++;
            } 
        } else { // columns are indexed by a list
            mp_obj_iter_buf_t column_iter_buf;
            mp_obj_t column_item, column_iterable;
            size_t j = 0;
            cindex = 0;
            while((row_item = mp_iternext(row_iterable)) != MP_OBJ_STOP_ITERATION) {
                if(mp_obj_is_true(row_item)) {
                    column_iterable = mp_getiter(column_list, &column_iter_buf);                   
                    while((column_item = mp_iternext(column_iterable)) != MP_OBJ_STOP_ITERATION) {
                        if(mp_obj_is_true(column_item)) {
                            memcpy(target+(i*n+j)*_sizeof, source+(rindex*ndarray->n+cindex)*_sizeof, _sizeof);
                            j++;
                        }
                        cindex++;
                    }
                    i++;
                }
                rindex++;
            }
        }
    }
    return MP_OBJ_FROM_PTR(out);
}

//|   .. method:: ulab_ndarray_getiter()
//|
//|      Deinitializes the array and releases any hardware resources for reuse.
//|

mp_obj_t ulab_ndarray_iternext(mp_obj_t self_in) {
    mp_obj_ndarray_it_t *self = MP_OBJ_TO_PTR(self_in);
    ulab_ndarray_obj_t *ndarray = MP_OBJ_TO_PTR(self->ndarray);
    // TODO: in numpy, ndarrays are iterated with respect to the first axis. 
    size_t iter_end = 0;
    if(ndarray->m == 1) {
        iter_end = ndarray->array->len;
    } else {
        iter_end = ndarray->m;
    }
    if(self->cur < iter_end) {
        if(ndarray->n == ndarray->array->len) { // we have a linear array
            // read the current value
            mp_obj_t value;
            value = mp_binary_get_val_array(ndarray->array->typecode, ndarray->array->items, self->cur);
            self->cur++;
            return value;
        } else { // we have a matrix, return the number of rows
            ulab_ndarray_obj_t *value = create_new_ndarray(1, ndarray->n, ndarray->array->typecode);
            // copy the memory content here
            uint8_t *tmp = (uint8_t *)ndarray->array->items;
            size_t strip_size = ndarray->n * mp_binary_get_size('@', ndarray->array->typecode, NULL);
            memcpy(value->array->items, &tmp[self->cur*strip_size], strip_size);
            self->cur++;
            return value;
        }
    } else {
        return MP_OBJ_STOP_ITERATION;
    }
}

mp_obj_t mp_obj_new_ndarray_iterator(mp_obj_t ndarray, size_t cur, mp_obj_iter_buf_t *iter_buf) {
    assert(sizeof(mp_obj_ndarray_it_t) <= sizeof(mp_obj_iter_buf_t));
    mp_obj_ndarray_it_t *o = (mp_obj_ndarray_it_t*)iter_buf;
    o->base.type = &mp_type_polymorph_iter;
    o->iternext = ulab_ndarray_iternext;
    o->ndarray = ndarray;
    o->cur = cur;
    return MP_OBJ_FROM_PTR(o);
}

mp_obj_t ulab_ndarray_shape(mp_obj_t self_in) {
    ulab_ndarray_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_obj_t tuple[2] = {
        mp_obj_new_int(self->m),
        mp_obj_new_int(self->n)
    };
    return mp_obj_new_tuple(2, tuple);
}

mp_obj_t ndarray_getiter(mp_obj_t o_in, mp_obj_iter_buf_t *iter_buf) {
    return mp_obj_new_ndarray_iterator(o_in, 0, iter_buf);
}

/*
//|   .. method:: deinit()
//|
//|      Deinitializes the array and releases any hardware resources for reuse.
//|
STATIC mp_obj_t ulab_ndarray_deinit(mp_obj_t self_in) {
  shared_module_ulab_ndarray_deinit(self_in);
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(ulab_ndarray_deinit_obj, ulab_ndarray_deinit);


//|   .. method:: __enter__()
//|
//|      No-op used by Context Managers.
//|
//  Provided by context manager helper.
 
//|   .. method:: __exit__()
//|
//|      Automatically deinitializes the hardware when exiting a context. See
//|      :ref:`lifetime-and-contextmanagers` for more info.
//|
STATIC mp_obj_t ulab_ndarray_obj___exit__(size_t n_args, const mp_obj_t *args) {
  shared_module_ulab_ndarray_deinit(args[0]);
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(ulab_ndarray___exit___obj, 4, 4, ulab_ndarray_obj___exit__);
*/
/*
STATIC const mp_rom_map_elem_t ulab_ndarray_locals_dict_table[] = {
    // Methods
    { MP_ROM_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&ulab_ndarray_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR___enter__), MP_ROM_PTR(&default___enter___obj) },
    { MP_ROM_QSTR(MP_QSTR___exit__), MP_ROM_PTR(&ulab_ndarray___exit___obj) },
    { MP_ROM_QSTR(MP_QSTR_copy), MP_ROM_PTR(&ulab_ndarray_copy_obj) },
    { MP_ROM_QSTR(MP_QSTR_shape), MP_ROM_PTR(&ulab_ndarray_shape_obj) },
};
STATIC MP_DEFINE_CONST_DICT(ulab_ndarray_locals_dict, ulab_ndarray_locals_dict_table);

const mp_obj_type_t ulab_ndarray_type = {
    { &mp_type_type },
    .name = MP_QSTR_Ndarray,
    .make_new = ulab_ndarray_make_new,
    .print = ulab_ndarray_print, 
    .getiter = ndarray_getiter,
    .locals_dict = (mp_obj_dict_t*)&ulab_ndarray_locals_dict,
};*/
