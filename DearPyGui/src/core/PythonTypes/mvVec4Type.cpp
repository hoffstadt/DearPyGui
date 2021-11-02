#include "mvVec4Type.h"
#include <string>
#include "mvTypes.h"
#include "mvPythonTranslator.h"
#include "mvPythonExceptions.h"

mv_internal void 
intialize_mvVec4(mvVec4* a, float x, float y, float z, float w)
{
	a->data[0] = x;
	a->data[1] = y;
	a->data[2] = z;
	a->data[3] = w;
}

int 
PymvVec4_init(PymvVec4* self, PyObject* args, PyObject* kwds)
{

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
	char kw_x[] = "x";
	char kw_y[] = "y";
	char kw_z[] = "z";
	char kw_w[] = "w";
	static char* kwlist[] = { kw_x, kw_y, kw_z, kw_w, nullptr };
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|ffff", kwlist, &x, &y, &z, &w))
		return -1;

	intialize_mvVec4(&self->vec4, x, y, z, w);
	return 0;
}

void 
PymvVec4_dealloc(PymvVec4* self)
{
	Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* 
PymvVec4_str(PymvVec4* self)
{
	std::string result = "[ ";

	for (int i = 0; i < 4; i++)
	{
		result += std::to_string(self->vec4.data[i]) + " ";
	}

	result +=  "]";

	PyObject* ret = PyUnicode_FromString(result.c_str());

	return ret;
}

int 
PymvVec4_getbuffer(PyObject* obj, Py_buffer* view, int flags)
{
	if (view == nullptr) {
		PyErr_SetString(PyExc_ValueError, "NULL view in getbuffer");
		return -1;
	}
	
	mv_local_persist int arrayLength = 4;

	auto self = (PymvVec4*)obj;
	view->obj = (PyObject*)self;
	view->buf = (void*)self->vec4.data;
	view->len = 4 * sizeof(float);
	view->readonly = 0;
	view->itemsize = sizeof(float);
	//view->format = (char*)(PyBUF_CONTIG_RO | PyBUF_FORMAT);  // float
	static char format[] = "f";
	view->format = format;  // float
	view->ndim = 1;
	view->shape = (Py_ssize_t*)&arrayLength;  // length-1 sequence of dimensions
	view->strides = &view->itemsize;  // for the simple case we can do this
	view->suboffsets = nullptr;
	view->internal = nullptr;

	Py_INCREF(self);  // need to increase the reference count
	return 0;
}

Py_ssize_t 
PymvVec4_getLength(PyObject* obj)
{
	auto self = (PymvVec4*)obj;
	return 4;
}

PyObject* 
PymvVec4_getItem(PyObject* obj, Py_ssize_t index)
{
	PymvVec4* self = (PymvVec4*)obj;
	if(index > 3)
		return Py_BuildValue("f", self->vec4.data[3]);
	return Py_BuildValue("f", self->vec4.data[index]);
}

int 
PymvVec4_setItem(PyObject* obj, Py_ssize_t index, PyObject* value)
{
	PymvVec4* self = (PymvVec4*)obj;
	if (index > 3)
		return 0;
	self->vec4.data[index] = (float)PyFloat_AsDouble(value);
	return 0;
}

PyObject* 
PymvVec4_add(PyObject* left, PyObject* right)
{
	PyObject* newbuffer = nullptr;
	PymvVec4* newbufferview = nullptr;
	newbufferview = PyObject_New(PymvVec4, &PymvVec4Type);
	newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvVec4Type);

	PymvVec4* leftVect = (PymvVec4*)left;
	PymvVec4* rightVect = (PymvVec4*)right;

	newbufferview->vec4.data[0] = leftVect->vec4.data[0] + rightVect->vec4.data[0];
	newbufferview->vec4.data[1] = leftVect->vec4.data[1] + rightVect->vec4.data[1];
	newbufferview->vec4.data[2] = leftVect->vec4.data[2] + rightVect->vec4.data[2];
	newbufferview->vec4.data[3] = leftVect->vec4.data[3] + rightVect->vec4.data[3];

	return newbuffer;
}

PyObject*
PymvVec4_subtract(PyObject* left, PyObject* right)
{
	PyObject* newbuffer = nullptr;
	PymvVec4* newbufferview = nullptr;
	newbufferview = PyObject_New(PymvVec4, &PymvVec4Type);
	newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvVec4Type);

	PymvVec4* leftVect = (PymvVec4*)left;
	PymvVec4* rightVect = (PymvVec4*)right;

	newbufferview->vec4.data[0] = leftVect->vec4.data[0] - rightVect->vec4.data[0];
	newbufferview->vec4.data[1] = leftVect->vec4.data[1] - rightVect->vec4.data[1];
	newbufferview->vec4.data[2] = leftVect->vec4.data[2] - rightVect->vec4.data[2];
	newbufferview->vec4.data[3] = leftVect->vec4.data[3] - rightVect->vec4.data[3];

	return newbuffer;
}
