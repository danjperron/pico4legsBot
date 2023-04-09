// Include MicroPython API.
//#include <stdio.h>
#include "py/runtime.h"
#include "py/mpstate.h"


#define HatIndex (5)




STATIC mp_obj_t gamepad_init(void) {
    MP_STATE_VM(GamePadData)[HatIndex]=99;
    return mp_const_none;
   }



// This is the function which will be called from Python as cexample.add_ints(a, b).
STATIC mp_obj_t gamepad_hat(void) {

    const static  int hatx[9]={ 0,1,1,1,0,-1,-1,-1,0};
    const static  int haty[9]={ 1,1,0,-1,-1,-1,0,1,0};
    uint8_t idx = MP_STATE_VM(GamePadData)[HatIndex];

    if(idx>8)
       idx = 8;

    mp_obj_t tuple[2]={
           mp_obj_new_int(hatx[idx]),
           mp_obj_new_int(haty[idx]),
          };
   return mp_obj_new_tuple(2, tuple);
}

// This is the function which will be called from Python as cexample.add_ints(a, b).
STATIC mp_obj_t gamepad_get(void) {
    uint8_t * pt = (uint8_t *) MP_STATE_VM(GamePadData);
    return mp_obj_new_bytearray_by_ref(10,pt);
}

// Define a Python reference to the functions above.
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gamepad_init_obj, gamepad_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gamepad_hat_obj, gamepad_hat);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gamepad_get_obj, gamepad_get);

// Define all properties of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t gamepad_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_gamepad) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&gamepad_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&gamepad_get_obj) },
    { MP_ROM_QSTR(MP_QSTR_hat), MP_ROM_PTR(&gamepad_hat_obj) },
};

STATIC MP_DEFINE_CONST_DICT(gamepad_module_globals, gamepad_module_globals_table);

// Define module object.
const mp_obj_module_t gamepad_user_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&gamepad_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_gamepad, gamepad_user_module);
MP_REGISTER_ROOT_POINTER(uint8_t GamePadData[10]);

