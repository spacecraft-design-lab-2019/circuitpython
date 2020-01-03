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

#include "shared-bindings/ulab/__init__.h"
#include "shared-bindings/ulab/ndarray.h"

#define ULAB_VERSION 0.262

mp_obj_float_t ulab_version = {{&mp_type_float}, ULAB_VERSION};

MP_DEFINE_CONST_FUN_OBJ_1(ulab_ndarray_shape_obj, ulab_ndarray_shape);

STATIC const mp_rom_map_elem_t ulab_ndarray_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_shape), MP_ROM_PTR(&ulab_ndarray_shape_obj) },
    //{ MP_ROM_QSTR(MP_QSTR_rawsize), MP_ROM_PTR(&ndarray_rawsize_obj) },
    //{ MP_ROM_QSTR(MP_QSTR_flatten), MP_ROM_PTR(&ndarray_flatten_obj) },    
    //{ MP_ROM_QSTR(MP_QSTR_asbytearray), MP_ROM_PTR(&ndarray_asbytearray_obj) },
};

STATIC MP_DEFINE_CONST_DICT(ulab_ndarray_locals_dict, ulab_ndarray_locals_dict_table);

const mp_obj_type_t ulab_ndarray_type = {
    { &mp_type_type },
    .name = MP_QSTR_ndarray,
    .print = ulab_ndarray_print,
    .make_new = ulab_ndarray_make_new,
    .locals_dict = (mp_obj_dict_t*)&ulab_ndarray_locals_dict,
};

STATIC const mp_rom_map_elem_t ulab_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_ulab) },
    { MP_ROM_QSTR(MP_QSTR___version__), MP_ROM_PTR(&ulab_version) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_array), (mp_obj_t)&ulab_ndarray_type },
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
