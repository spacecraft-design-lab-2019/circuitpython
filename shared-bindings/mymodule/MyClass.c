#include <stdint.h>
#include <string.h>
#include "lib/utils/context_manager_helpers.h"
#include "py/objproperty.h"
#include "py/runtime.h"
#include "py/runtime0.h"
#include "shared-bindings/mymodule/MyClass.h"
#include "shared-bindings/util.h"
#include "shared-module/ulab/ndarray.h"
#include "shared-bindings/ulab/ndarray.h"

//| .. currentmodule:: mymodule
//|
//| :class:`MyClass` -- The great question (and answer to it) of life, the universe, and everything.
//| ====================================================================================
//|
//| Provides the great question (and answer to it) of life, the universie, and everything.
//|
//| .. class:: MyClass()
//|
//|   Create an object.


STATIC mp_obj_t mymodule_myclass_make_new(const mp_obj_type_t *type, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
  mp_arg_check_num(n_args, kw_args, 0, 2, true);
    mymodule_myclass_obj_t *self = m_new_obj(mymodule_myclass_obj_t);
    self->base.type = &mymodule_myclass_type;
    int8_t input = mp_obj_get_int(pos_args[0]);
    ulab_ndarray_obj_t *my_ulab_array = MP_OBJ_TO_PTR(pos_args[0]);
    shared_module_mymodule_myclass_construct(self, input, *my_ulab_array);
    return MP_OBJ_FROM_PTR(self);
}


//|   .. method:: deinit()
//|
//|      Deinitializes the Meaning and releases any hardware resources for reuse.
//|
STATIC mp_obj_t mymodule_myclass_deinit(mp_obj_t self_in) {
  shared_module_mymodule_myclass_deinit(self_in);
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mymodule_myclass_deinit_obj, mymodule_myclass_deinit);
 
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
STATIC mp_obj_t mymodule_myclass_obj___exit__(size_t n_args, const mp_obj_t *args) {
  shared_module_mymodule_myclass_deinit(args[0]);
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mymodule_myclass___exit___obj, 4, 4, mymodule_myclass_obj___exit__);

//|   .. attribute:: question
//|
//|     The question of life, the universe and everything
//|
STATIC mp_obj_t mymodule_myclass_obj_get_question(mp_obj_t self_in) {
  char *str = shared_module_mymodule_myclass_get_question(self_in);
  return mp_obj_new_str(str, strlen(str));
}

MP_DEFINE_CONST_FUN_OBJ_1(mymodule_myclass_get_question_obj, mymodule_myclass_obj_get_question);

//|   .. attribute:: author
//|
//|     Who wrote this?
//|
STATIC mp_obj_t mymodule_myclass_obj_get_author(mp_obj_t self_in) {
  char *str = shared_module_mymodule_myclass_get_author(self_in);
  return mp_obj_new_str(str, strlen(str));
}

MP_DEFINE_CONST_FUN_OBJ_1(mymodule_myclass_get_author_obj, mymodule_myclass_obj_get_author);
 
//|   .. attribute:: answer
//|
//|
//|     The answer to the question of life, the universe and everything
//|
STATIC mp_obj_t mymodule_myclass_obj_get_answer(mp_obj_t self_in) {
  return mp_obj_new_int(shared_module_mymodule_myclass_get_answer(self_in));
}
MP_DEFINE_CONST_FUN_OBJ_1(mymodule_myclass_get_answer_obj, mymodule_myclass_obj_get_answer);

//|   .. attribute:: return input
//|
//|
//|     returns the input
//|
STATIC mp_obj_t mymodule_myclass_obj_get_input(mp_obj_t self_in) {
  return mp_obj_new_int(shared_module_mymodule_myclass_get_input(self_in));
}
MP_DEFINE_CONST_FUN_OBJ_1(mymodule_myclass_get_input_obj, mymodule_myclass_obj_get_input);

//|   .. attribute:: square
//|
//|
//|     this squares an integer input
//|
STATIC mp_obj_t mymodule_myclass_obj_get_square(mp_obj_t self_in) {
  return mp_obj_new_int(shared_module_mymodule_myclass_get_square(self_in));
}
MP_DEFINE_CONST_FUN_OBJ_1(mymodule_myclass_get_square_obj, mymodule_myclass_obj_get_square);

//|   .. attribute:: return length
//|
//|
//|     returns the length
//|
STATIC mp_obj_t mymodule_myclass_obj_get_length(mp_obj_t self_in) {
  return mp_obj_new_int(shared_module_mymodule_myclass_get_length(self_in));
}
MP_DEFINE_CONST_FUN_OBJ_1(mymodule_myclass_get_length_obj, mymodule_myclass_obj_get_length);

//|   .. attribute:: return C (array)
//|
//|
//|     returns an array
//|
STATIC mp_obj_t mymodule_myclass_obj_get_C(mp_obj_t self_in) {
  size_t l = 10;
  mp_obj_t data = shared_module_mymodule_myclass_get_C;
  return mp_obj_new_list(l, data);
}
MP_DEFINE_CONST_FUN_OBJ_1(mymodule_myclass_get_C_obj, mymodule_myclass_obj_get_C);

const mp_obj_property_t mymodule_myclass_question_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&mymodule_myclass_get_question_obj,
              (mp_obj_t)&mp_const_none_obj},
};
 
const mp_obj_property_t mymodule_myclass_answer_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&mymodule_myclass_get_answer_obj,
              (mp_obj_t)&mp_const_none_obj},
};

const mp_obj_property_t mymodule_myclass_author_obj = {
	.base.type = &mp_type_property,
	.proxy = {(mp_obj_t)&mymodule_myclass_get_author_obj,
			  (mp_obj_t)&mp_const_none_obj},
};

const mp_obj_property_t mymodule_myclass_input_obj = {
  .base.type = &mp_type_property,
  .proxy = {(mp_obj_t)&mymodule_myclass_get_input_obj,
        (mp_obj_t)&mp_const_none_obj},
};

const mp_obj_property_t mymodule_myclass_length_obj = {
  .base.type = &mp_type_property,
  .proxy = {(mp_obj_t)&mymodule_myclass_get_length_obj,
        (mp_obj_t)&mp_const_none_obj},
};

const mp_obj_property_t mymodule_myclass_C_obj = {
  .base.type = &mp_type_property,
  .proxy = {(mp_obj_t)&mymodule_myclass_get_C_obj,
        (mp_obj_t)&mp_const_none_obj},
};

const mp_obj_property_t mymodule_myclass_square_obj = {
	.base.type = &mp_type_property,
	.proxy = {(mp_obj_t)&mymodule_myclass_get_square_obj,
			  (mp_obj_t)&mp_const_none_obj},
};
 
STATIC const mp_rom_map_elem_t mymodule_myclass_locals_dict_table[] = {
    // Methods
    { MP_ROM_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&mymodule_myclass_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR___enter__), MP_ROM_PTR(&default___enter___obj) },
    { MP_ROM_QSTR(MP_QSTR___exit__), MP_ROM_PTR(&mymodule_myclass___exit___obj) },
    { MP_ROM_QSTR(MP_QSTR_question), MP_ROM_PTR(&mymodule_myclass_question_obj) },
    { MP_ROM_QSTR(MP_QSTR_answer), MP_ROM_PTR(&mymodule_myclass_answer_obj) },
    { MP_ROM_QSTR(MP_QSTR_author), MP_ROM_PTR(&mymodule_myclass_author_obj) },
    { MP_ROM_QSTR(MP_QSTR_input), MP_ROM_PTR(&mymodule_myclass_input_obj) },
    { MP_ROM_QSTR(MP_QSTR_length), MP_ROM_PTR(&mymodule_myclass_length_obj) },
    { MP_ROM_QSTR(MP_QSTR_C), MP_ROM_PTR(&mymodule_myclass_C_obj) },
    { MP_ROM_QSTR(MP_QSTR_square), MP_ROM_PTR(&mymodule_myclass_square_obj) },

};
STATIC MP_DEFINE_CONST_DICT(mymodule_myclass_locals_dict, mymodule_myclass_locals_dict_table);
 
const mp_obj_type_t mymodule_myclass_type = {
    { &mp_type_type },
    .name = MP_QSTR_Meaning,
    .make_new = mymodule_myclass_make_new,
    .locals_dict = (mp_obj_dict_t*)&mymodule_myclass_locals_dict,
};
