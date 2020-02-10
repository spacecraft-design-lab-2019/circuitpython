#include "py/runtime.h"
#include "MyClass.h"
#include "shared-module/ulab/ndarray.h"
#include "shared-bindings/ulab/ndarray.h"

void get_array(float *C);

void shared_module_mymodule_myclass_construct(mymodule_myclass_obj_t* self, int8_t input, ulab_ndarray_obj_t my_ulab_array) {
  self->deinited = 0;
  self->input = input;
  self->length = 10;
  get_array(self->C);
  // Load in data from ulab array
  // ulab_ndarray_obj_t *my_ulab_array = MP_OBJ_TO_PTR(my_ulab_array);
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

mp_int_t shared_module_mymodule_myclass_get_input(mymodule_myclass_obj_t* self) {
	return self->input;
}

mp_int_t shared_module_mymodule_myclass_get_length(mymodule_myclass_obj_t* self) {
	return self->length;
}

mp_obj_t shared_module_mymodule_myclass_get_C(mymodule_myclass_obj_t* self) {
	return self->C;
} 

void get_array(float *C) {
	size_t n = 10;
	for (size_t i = 0; i <= n-1; i++) {
		C[i] = 0;
	}
}

