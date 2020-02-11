/*
 * This file is part of the micropython-ulab project, 
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Zoltán Vörös
*/
    
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lib/utils/context_manager_helpers.h"

#include "py/obj.h"
#include "py/runtime.h"
#include "py/misc.h"
#include "shared-bindings/ulab/linalg.h"
#include "shared-bindings/ulab/ndarray.h"

/*void bdot_law(uint8_t c[], uint8_t length) {
	// this function takes in an array of the bdot and magnetic moment
	// and modifies the moment in place
	
	uint8_t* tmp = calloc(length, sizeof(uint8_t));

	for(size_t i = 0; i < 6; i++) {
		*(tmp+i) = 5;
    }
    free(c);
    *c = *tmp;

}*/


mp_obj_t ulab_controller_bdot(mp_obj_t self_in) {
    ulab_ndarray_obj_t *self = MP_OBJ_TO_PTR(self_in); 
    // this passes in the array
    // the size of a single item in the array
    uint8_t _sizeof = mp_binary_get_size('@', self->array->typecode, NULL);
    
    // NOTE: 
    // we assume that the array passed in is of the form [u[1:3],Bdot[1:3]]
    uint8_t *c = (uint8_t *)self->array->items;

    if((self->m != 1) && (self->n != 6)) {
    	mp_raise_ValueError(translate("array is wrong size"));
    }
	else {
	    // self->bytes is the size of the bytearray, irrespective of the typecode
	    uint8_t length = 6;

	    //agnotstic to function
        uint8_t *tmp = m_new(uint8_t, self->bytes);
	    for(size_t n=0; n < length; n++) {
	    	tmp[n] = 1;
	    }
	    
        for(size_t n=0; n < self->n; n++) {
            memcpy(c+_sizeof*(n + 1), &tmp[n], _sizeof);
        }

	    // copy back to circuitpython
	    memcpy(self->array->items, c, self->bytes);
	    m_del(uint8_t, c, self->bytes);
	}

    return mp_const_none;
}

