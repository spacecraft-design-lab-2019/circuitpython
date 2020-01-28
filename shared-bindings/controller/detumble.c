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

uint8_t controller_detumble_test(uint8_t p) {
 // prints a number to test
	return p;
}