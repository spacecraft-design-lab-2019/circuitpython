/*
 * This file is part of the pycubed-mini project, 
 *
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

#include "shared-bindings/controller/__init__.h"
#include "shared-bindings/controller/detumble.h"

MP_DEFINE_CONST_FUN_OBJ_1(controller_detumble_test_obj, controller_detumble_test);

STATIC const mp_rom_map_elem_t controller_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR__name__), MP_OBJ_NEW_QSTR(MP_QSTR_controller) },
    { MP_ROM_QSTR(MP_QSTR_test), (mp_obj_t)&controller_detumble_test_obj },
}

STATIC MP_DEFINE_CONST_DICT (mp_controller_module_globals, controller_globals_table);

const mp_obj_module_t controller_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_controller_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_controller, controller_module, MODULE_CONTROLLER_ENABLED);
