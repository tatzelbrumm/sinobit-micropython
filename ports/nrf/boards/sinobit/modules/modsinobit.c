#include "py/obj.h"

extern const mp_obj_module_t sinobitdisplay_module;

STATIC const mp_map_elem_t sinobit_module_globals_table[] = {
  { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_sinobit) },
  { MP_OBJ_NEW_QSTR(MP_QSTR_display), (mp_obj_t)&sinobitdisplay_module },
};

STATIC MP_DEFINE_CONST_DICT(sinobit_module_globals, sinobit_module_globals_table);

const mp_obj_module_t sinobit_module = {
  .base = { &mp_type_module },
  .globals = (mp_obj_dict_t*)&sinobit_module_globals,
};
