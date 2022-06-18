/*[clinic input]
preserve
[clinic start generated code]*/

static PyObject *
mappingproxy_new_impl(PyTypeObject *type, PyObject *mapping);

static PyObject *
mappingproxy_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"mapping", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "mappingproxy", 0};
    PyObject *argsbuf[1];
    PyObject * const *fastargs;
    Py_ssize_t nargs = PyTuple_GET_SIZE(args);
    PyObject *mapping;

    fastargs = _PyArg_UnpackKeywords(_PyTuple_CAST(args)->ob_item, nargs, kwargs, NULL, &_parser, 1, 1, 0, argsbuf);
    if (!fastargs) {
        goto exit;
    }
    mapping = fastargs[0];
    return_value = mappingproxy_new_impl(type, mapping);

exit:
    return return_value;
}

PyDoc_STRVAR(property_init__doc__,
"property(fget=None, fset=None, fdel=None, doc=None)\n"
"--\n"
"\n"
"Property attribute.\n"
"\n"
"  fget\n"
"    function to be used for getting an attribute value\n"
"  fset\n"
"    function to be used for setting an attribute value\n"
"  fdel\n"
"    function to be used for del\'ing an attribute\n"
"  doc\n"
"    docstring\n"
"\n"
"Typical use is to define a managed attribute x:\n"
"\n"
"class C(object):\n"
"    def getx(self): return self._x\n"
"    def setx(self, value): self._x = value\n"
"    def delx(self): del self._x\n"
"    x = property(getx, setx, delx, \"I\'m the \'x\' property.\")\n"
"\n"
"Decorators make defining new properties or modifying existing ones easy:\n"
"\n"
"class C(object):\n"
"    @property\n"
"    def x(self):\n"
"        \"I am the \'x\' property.\"\n"
"        return self._x\n"
"    @x.setter\n"
"    def x(self, value):\n"
"        self._x = value\n"
"    @x.deleter\n"
"    def x(self):\n"
"        del self._x");

static int
property_init_impl(propertyobject *self, PyObject *fget, PyObject *fset,
                   PyObject *fdel, PyObject *doc);

static int
property_init(PyObject *self, PyObject *args, PyObject *kwargs)
{
    int return_value = -1;
    static const char * const _keywords[] = {"fget", "fset", "fdel", "doc", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "property", 0};
    PyObject *argsbuf[4];
    PyObject * const *fastargs;
    Py_ssize_t nargs = PyTuple_GET_SIZE(args);
    Py_ssize_t noptargs = nargs + (kwargs ? PyDict_GET_SIZE(kwargs) : 0) - 0;
    PyObject *fget = NULL;
    PyObject *fset = NULL;
    PyObject *fdel = NULL;
    PyObject *doc = NULL;

    fastargs = _PyArg_UnpackKeywords(_PyTuple_CAST(args)->ob_item, nargs, kwargs, NULL, &_parser, 0, 4, 0, argsbuf);
    if (!fastargs) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (fastargs[0]) {
        fget = fastargs[0];
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (fastargs[1]) {
        fset = fastargs[1];
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (fastargs[2]) {
        fdel = fastargs[2];
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    doc = fastargs[3];
skip_optional_pos:
    return_value = property_init_impl((propertyobject *)self, fget, fset, fdel, doc);

exit:
    return return_value;
}
/*[clinic end generated code: output=916624e717862abc input=a9049054013a1b77]*/
