/* this is the class for detumbling
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
#include "shared-bindings/controller/detumble.h"
#include "shared-bindings/util.h"

// Constructor
STATIC mp_obj_t controller_detumble_make_new(const mp_obj_type_t *type, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
  mp_arg_check_num(n_args, kw_args, 1, 1, true);
    controller_detumble_obj_t *self = m_new_obj(controller_detumble_obj_t);
    self->base.type = &controller_detumble_type;
    int8_t input = mp_obj_get_int(pos_args[0]);
    shared_module_controller_detumble_construct(self, input);
    return MP_OBJ_FROM_PTR(self);
}

// Deinitialize
STATIC mp_obj_t controller_detumble_deinit(mp_obj_t self_in) {
  shared_module_controller_detumble_deinit(self_in);
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(controller_detumble_deinit_obj, controller_detumble_deinit);

// Check if deinitialized
STATIC mp_obj_t controller_detumble_obj___exit__(size_t n_args, const mp_obj_t *args) {
  shared_module_controller_detumble_deinit(args[0]);
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(controller_detumble___exit___obj, 4, 4, controller_detumble_obj___exit__);



// uint8_t controller_detumble_test(uint8_t p) {
//  // prints a number to test
// 	return p;
// }