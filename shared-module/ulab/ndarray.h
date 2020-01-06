#ifndef MICROPY_INCLUDED_ULAB_NDARRAY_H
#define MICROPY_INCLUDED_ULAB_NDARRAY_H

#include "py/obj.h"
#include "py/objarray.h"

typedef struct {
    mp_obj_base_t base;
    mp_float_t value;
} mp_obj_float_t;

typedef struct {
	//bool deinited;
    mp_obj_base_t base;
    size_t m, n;
    size_t len;
    mp_obj_array_t *array;
    size_t bytes;
} ulab_ndarray_obj_t;

typedef struct {
    mp_obj_base_t base;
    mp_fun_1_t iternext;
    mp_obj_t ndarray;
    size_t cur;
} mp_obj_ndarray_it_t;

#endif //MICROPY_INCLUDED_ULAB_NDARRAY_H