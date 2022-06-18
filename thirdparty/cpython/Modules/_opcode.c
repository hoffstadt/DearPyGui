#include "Python.h"
#include "opcode.h"

/*[clinic input]
module _opcode
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=117442e66eb376e6]*/

#include "clinic/_opcode.c.h"

/*[clinic input]

_opcode.stack_effect -> int

  opcode: int
  oparg: object = None
  /
  *
  jump: object = None

Compute the stack effect of the opcode.
[clinic start generated code]*/

static int
_opcode_stack_effect_impl(PyObject *module, int opcode, PyObject *oparg,
                          PyObject *jump)
/*[clinic end generated code: output=64a18f2ead954dbb input=461c9d4a44851898]*/
{
    int effect;
    int oparg_int = 0;
    int jump_int;
    if (HAS_ARG(opcode)) {
        if (oparg == Py_None) {
            PyErr_SetString(PyExc_ValueError,
                    "stack_effect: opcode requires oparg but oparg was not specified");
            return -1;
        }
        oparg_int = (int)PyLong_AsLong(oparg);
        if ((oparg_int == -1) && PyErr_Occurred())
            return -1;
    }
    else if (oparg != Py_None) {
        PyErr_SetString(PyExc_ValueError,
                "stack_effect: opcode does not permit oparg but oparg was specified");
        return -1;
    }
    if (jump == Py_None) {
        jump_int = -1;
    }
    else if (jump == Py_True) {
        jump_int = 1;
    }
    else if (jump == Py_False) {
        jump_int = 0;
    }
    else {
        PyErr_SetString(PyExc_ValueError,
                "stack_effect: jump must be False, True or None");
        return -1;
    }
    effect = PyCompile_OpcodeStackEffectWithJump(opcode, oparg_int, jump_int);
    if (effect == PY_INVALID_STACK_EFFECT) {
            PyErr_SetString(PyExc_ValueError,
                    "invalid opcode or oparg");
            return -1;
    }
    return effect;
}




static PyMethodDef
opcode_functions[] =  {
    _OPCODE_STACK_EFFECT_METHODDEF
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef opcodemodule = {
    PyModuleDef_HEAD_INIT,
    "_opcode",
    "Opcode support module.",
    -1,
    opcode_functions,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC
PyInit__opcode(void)
{
    return PyModule_Create(&opcodemodule);
}
