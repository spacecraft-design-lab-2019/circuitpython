#include "py/runtime.h"
#include "ndarray.h"

bool shared_module_ulab_ndarray_deinited(ulab_ndarray_obj_t* self) {
  return self->deinited;
}
 
void shared_module_ulab_ndarray_deinit(ulab_ndarray_obj_t* self) {
  self->deinited = 1;
}
