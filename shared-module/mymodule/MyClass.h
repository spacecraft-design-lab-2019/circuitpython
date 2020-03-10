#ifndef MICROPY_INCLUDED_MYMODULE_MYCLASS_H
#define MICROPY_INCLUDED_MYMODULE_MYCLASS_H
 
#include "py/obj.h"
#include "shared-module/ulab/ndarray.h"
#include "shared-bindings/ulab/ndarray.h"
 
typedef struct {
  mp_obj_base_t base;
  bool deinited;
  int8_t input;
  size_t length;
  float C[10];
  // ulab_ndarray_obj_t *my_ulab_array;
} mymodule_myclass_obj_t;
 
 
#endif // MICROPY_INCLUDED_MYMODULE_MYCLASS_H