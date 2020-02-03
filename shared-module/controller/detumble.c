#include "py/runtime.h"
#include "detumble.h"
#include "detumble_algorithms.h"


void shared_module_controller_detumble_construct(controller_detumble_obj_t* self, mp_float_t input) {
  self->deinited = 0;
  self->input = input;
}

bool shared_module_controller_detumble_deinited(controller_detumble_obj_t* self) {
  return self->deinited;
}
 
void shared_module_controller_detumble_deinit(controller_detumble_obj_t* self) {
  self->deinited = 1;
}

// types must be native C types (char*, double*, etc.) or mp types (mp_int_t, etc.)


// detumble_B_dot_C(double* B_dot, double* max_dipoles, double* commanded_dipole);

// double* shared_module_controller_detumble_detumbleBdot(controller_detumble_obj_t* self, double* B_dot, double* max_dipoles, double* commanded_dipole) {
//   detumble_B_dot_C(B_dot, max_dipoles, commanded_dipole);
//   return commanded_dipole;
// }