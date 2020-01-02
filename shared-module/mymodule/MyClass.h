#ifndef MICROPY_INCLUDED_MYMODULE_MYCLASS_H
#define MICROPY_INCLUDED_MYMODULE_MYCLASS_H
 
#include "py/obj.h"
 
typedef struct {
  mp_obj_base_t base;
  bool deinited;
  int8_t input;
} mymodule_myclass_obj_t;
 
 
#endif // MICROPY_INCLUDED_MYMODULE_MYCLASS_H