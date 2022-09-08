#include "py/runtime.h"
#include "py/mpstate.h"



// function to be called by a micropython timer to poll the usb
STATIC mp_obj_t gamepad_pull(mp_obj_t t_obj) {
    void(*p)(void) = (void(*)(void)) MP_STATE_VM(PollGamePad);
    p();
    return mp_obj_new_bool(1);
}


// function which read the gamepad report
STATIC mp_obj_t gamepad_get(void) {
    uint8_t * pt = (uint8_t *) MP_STATE_VM(GamePadData);
    return mp_obj_new_bytearray_by_ref(10,pt);
}

// Define a Python reference to the functions above.

STATIC MP_DEFINE_CONST_FUN_OBJ_1(gamepad_pull_obj, gamepad_pull);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gamepad_get_obj, gamepad_get);

// Define all properties of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t gamepad_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_gamepad) },
    { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&gamepad_get_obj) },
    { MP_ROM_QSTR(MP_QSTR_pull), MP_ROM_PTR(&gamepad_pull_obj)}
};

STATIC MP_DEFINE_CONST_DICT(gamepad_module_globals, gamepad_module_globals_table);

// Define module object.
const mp_obj_module_t gamepad_user_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&gamepad_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_gamepad_hw, gamepad_user_module);
MP_REGISTER_ROOT_POINTER(void * PollGamePad);
MP_REGISTER_ROOT_POINTER(uint8_t GamePadData[10]);
