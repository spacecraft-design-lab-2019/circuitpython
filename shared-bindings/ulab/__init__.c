/*
 * This file is part of the micropython-ulab xoject, 
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
#include <string.h>
#include "py/runtime.h"
#include "py/binary.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "lib/utils/context_manager_helpers.h"
#include "py/objproperty.h"
#include "py/runtime.h"
#include "shared-bindings/util.h"

#include "shared-bindings/ulab/__init__.h"
#include "shared-bindings/ulab/ndarray.h"
#include "shared-bindings/ulab/linalg.h"
#include "shared-bindings/ulab/controller.h"

const char *ulab_version = "0.262";


MP_DEFINE_CONST_FUN_OBJ_1(ulab_ndarray_shape_obj, ulab_ndarray_shape);
MP_DEFINE_CONST_FUN_OBJ_1(ulab_ndarray_copy_obj, ulab_ndarray_copy);
MP_DEFINE_CONST_FUN_OBJ_1(ulab_ndarray_rawsize_obj, ulab_ndarray_rawsize);
MP_DEFINE_CONST_FUN_OBJ_KW(ulab_ndarray_flatten_obj, 1, ulab_ndarray_flatten);

MP_DEFINE_CONST_FUN_OBJ_1(ulab_linalg_transpose_obj, ulab_linalg_transpose);
MP_DEFINE_CONST_FUN_OBJ_1(ulab_linalg_zeroed_obj, ulab_linalg_zeroed);
MP_DEFINE_CONST_FUN_OBJ_2(ulab_linalg_reshape_obj, ulab_linalg_reshape);
MP_DEFINE_CONST_FUN_OBJ_KW(ulab_linalg_size_obj, 1, ulab_linalg_size);
MP_DEFINE_CONST_FUN_OBJ_1(ulab_linalg_inv_obj, ulab_linalg_inv);
MP_DEFINE_CONST_FUN_OBJ_2(ulab_linalg_dot_obj, ulab_linalg_dot);
MP_DEFINE_CONST_FUN_OBJ_2(ulab_linalg_cross_obj, ulab_linalg_cross);
MP_DEFINE_CONST_FUN_OBJ_KW(ulab_linalg_zeros_obj, 0, ulab_linalg_zeros);
MP_DEFINE_CONST_FUN_OBJ_KW(ulab_linalg_ones_obj, 0, ulab_linalg_ones);
MP_DEFINE_CONST_FUN_OBJ_KW(ulab_linalg_eye_obj, 0, ulab_linalg_eye);
MP_DEFINE_CONST_FUN_OBJ_1(ulab_linalg_det_obj, ulab_linalg_det);
MP_DEFINE_CONST_FUN_OBJ_1(ulab_linalg_eig_obj, ulab_linalg_eig);

MP_DEFINE_CONST_FUN_OBJ_1(ulab_controller_bdot_obj, ulab_controller_bdot);
//this is if you want to modify an array in place
STATIC const mp_rom_map_elem_t ulab_ndarray_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_shape), MP_ROM_PTR(&ulab_ndarray_shape_obj) },
    { MP_ROM_QSTR(MP_QSTR_copy), MP_ROM_PTR(&ulab_ndarray_copy_obj) },
    { MP_ROM_QSTR(MP_QSTR_rawsize), MP_ROM_PTR(&ulab_ndarray_rawsize_obj) },
    { MP_ROM_QSTR(MP_QSTR_flatten), MP_ROM_PTR(&ulab_ndarray_flatten_obj) },    
    { MP_ROM_QSTR(MP_QSTR_transpose), MP_ROM_PTR(&ulab_linalg_transpose_obj) },
    { MP_ROM_QSTR(MP_QSTR_zeroed), MP_ROM_PTR(&ulab_linalg_zeroed_obj) },
    { MP_ROM_QSTR(MP_QSTR_reshape), MP_ROM_PTR(&ulab_linalg_reshape_obj) },

    { MP_ROM_QSTR(MP_QSTR_bdot), MP_ROM_PTR(&ulab_controller_bdot_obj) },

};

STATIC MP_DEFINE_CONST_DICT(ulab_ndarray_locals_dict, ulab_ndarray_locals_dict_table);

const mp_obj_type_t ulab_ndarray_type = {
    { &mp_type_type },
    .name = MP_QSTR_ndarray,
    .print = ulab_ndarray_print,
    .make_new = ulab_ndarray_make_new,
    .subscr = ulab_ndarray_subscr,
    .getiter = ulab_ndarray_getiter,
    .unary_op = ulab_ndarray_unary_op,
    .binary_op = ulab_ndarray_binary_op,
    .locals_dict = (mp_obj_dict_t*)&ulab_ndarray_locals_dict,
};

// this is if you want to create a new array with these functions or output some other value (like size)
STATIC const mp_rom_map_elem_t ulab_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_ulab) },
    { MP_ROM_QSTR(MP_QSTR___version__), MP_ROM_PTR(&ulab_version) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_array), (mp_obj_t)&ulab_ndarray_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_size), (mp_obj_t)&ulab_linalg_size_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_inv), (mp_obj_t)&ulab_linalg_inv_obj },
    { MP_ROM_QSTR(MP_QSTR_dot), (mp_obj_t)&ulab_linalg_dot_obj },
    { MP_ROM_QSTR(MP_QSTR_cross), (mp_obj_t)&ulab_linalg_cross_obj },
    { MP_ROM_QSTR(MP_QSTR_zeros), (mp_obj_t)&ulab_linalg_zeros_obj },
    { MP_ROM_QSTR(MP_QSTR_ones), (mp_obj_t)&ulab_linalg_ones_obj },
    { MP_ROM_QSTR(MP_QSTR_eye), (mp_obj_t)&ulab_linalg_eye_obj },
    { MP_ROM_QSTR(MP_QSTR_det), (mp_obj_t)&ulab_linalg_det_obj },
    { MP_ROM_QSTR(MP_QSTR_eig), (mp_obj_t)&ulab_linalg_eig_obj },   

    //class constants
    { MP_ROM_QSTR(MP_QSTR_uint8), MP_ROM_INT(NDARRAY_UINT8) },
    { MP_ROM_QSTR(MP_QSTR_int8), MP_ROM_INT(NDARRAY_INT8) },
    { MP_ROM_QSTR(MP_QSTR_uint16), MP_ROM_INT(NDARRAY_UINT16) },
    { MP_ROM_QSTR(MP_QSTR_int16), MP_ROM_INT(NDARRAY_INT16) },
    { MP_ROM_QSTR(MP_QSTR_float), MP_ROM_INT(NDARRAY_FLOAT) },
};

STATIC MP_DEFINE_CONST_DICT (mp_ulab_module_globals, ulab_globals_table);
 
const mp_obj_module_t ulab_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_ulab_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_ulab, ulab_module, MODULE_ULAB_ENABLED);
