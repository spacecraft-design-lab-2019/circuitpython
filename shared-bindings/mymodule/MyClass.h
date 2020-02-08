#ifndef MICROPY_INCLUDED_SHARED_BINDINGS_MYMODULE_MYCLASS_H
#define MICROPY_INCLUDED_SHARED_BINDINGS_MYMODULE_MYCLASS_H
 
#include "shared-module/mymodule/MyClass.h"
 
extern const mp_obj_type_t mymodule_myclass_type;
 
extern void shared_module_mymodule_myclass_construct(mymodule_myclass_obj_t* self, int8_t input);
extern void shared_module_mymodule_myclass_deinit(mymodule_myclass_obj_t* self);
extern bool shared_module_mymodule_myclass_deinited(mymodule_myclass_obj_t* self);
extern char * shared_module_mymodule_myclass_get_question(mymodule_myclass_obj_t* self);
extern char * shared_module_mymodule_myclass_get_author(mymodule_myclass_obj_t* self);
extern mp_int_t shared_module_mymodule_myclass_get_answer(mymodule_myclass_obj_t* self);
extern mp_int_t shared_module_mymodule_myclass_get_input(mymodule_myclass_obj_t* self);
extern mp_int_t shared_module_mymodule_myclass_get_length(mymodule_myclass_obj_t* self);
extern mp_obj_t shared_module_mymodule_myclass_get_C(mymodule_myclass_obj_t* self);
extern mp_int_t shared_module_mymodule_myclass_get_square(mymodule_myclass_obj_t* self);
 
#endif // MICROPY_INCLUDED_SHARED_BINDINGS_MYMODULE_MYCLASS_H
