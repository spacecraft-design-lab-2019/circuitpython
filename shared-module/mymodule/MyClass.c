#include "py/runtime.h"
#include "MyClass.h"
 
void shared_module_mymodule_myclass_construct(mymodule_myclass_obj_t* self, int8_t input) {
  self->deinited = 0;
  self->input = input;
}

bool shared_module_mymodule_myclass_deinited(mymodule_myclass_obj_t* self) {
  return self->deinited;
}
 
void shared_module_mymodule_myclass_deinit(mymodule_myclass_obj_t* self) {
  self->deinited = 1;
}

const char * shared_module_mymodule_myclass_get_question(mymodule_myclass_obj_t* self) {
  return "Tricky...";
}
 
mp_int_t shared_module_mymodule_myclass_get_answer(mymodule_myclass_obj_t* self) {
  return 42;
}

const char * shared_module_mymodule_myclass_get_author(mymodule_myclass_obj_t* self) {
	return "Douglas Adams";
}

mp_int_t shared_module_mymodule_myclass_get_square(mymodule_myclass_obj_t* self) {
	return self->input*self->input;
}