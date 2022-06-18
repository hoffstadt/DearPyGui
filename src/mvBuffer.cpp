#include "mvBuffer.h"
#include <string>
#include "mvTypes.h"
#include "mvPythonTranslator.h"
#include "mvPythonExceptions.h"


mv_internal void 
intialize_mvBuffer(mvBuffer* a, long length)
{
	a->length = length;
	a->data = new float[length];
	for (int i = 0; i < length; i++)
		a->data[i] = 1.0f;
}

mv_internal void
deallocate_mvBuffer(mvBuffer* a)
{
	delete[] a->data;
	a->data = nullptr;
}

int 
PymvBuffer_init(PymvBuffer* self, PyObject* args, PyObject* kwds)
{
	// init may have already been called
	if (self->arr.data != nullptr)
		deallocate_mvBuffer(&self->arr);

	int length = 0;
	static char kw[] = "length";
	static char* kwlist[] = { kw, nullptr };
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, &length))
		return -1;

	if (length < 0)
		length = 0;

	intialize_mvBuffer(&self->arr, length);

	return 0;
}

PyObject* 
PymvBuffer_clear_value(PymvBuffer* self, PyObject* args, PyObject* kwds)
{

	float value = 0.0f;
	if (!PyArg_ParseTuple(args, "f", &value))
		Py_RETURN_NONE;

	for (int i = 0; i < self->arr.length; i++)
		self->arr.data[i] = value;

	Py_RETURN_NONE;
}

PyObject*
PymvBuffer_get_width(PymvBuffer* self, PyObject* args, PyObject* kwds)
{
	return ToPyInt(self->arr.width);
}

PyObject* 
PymvBuffer_get_height(PymvBuffer* self, PyObject* args, PyObject* kwds)
{
	return ToPyInt(self->arr.height);
}

void 
PymvBuffer_dealloc(PymvBuffer* self)
{
	deallocate_mvBuffer(&self->arr);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* 
PymvBuffer_str(PymvBuffer* self)
{
	std::string result = "[ ";

	for (int i = 0; i < 10; i++)
	{
		if (self->arr.length == i)
			break;

		result += std::to_string(self->arr.data[i]) + " ";
	}

	result +=  " ... ]";

	PyObject* ret = PyUnicode_FromString(result.c_str());

	return ret;
}

int 
PymvBuffer_getbuffer(PyObject* obj, Py_buffer* view, int flags)
{
	if (view == nullptr) {
		PyErr_SetString(PyExc_ValueError, "NULL view in getbuffer");
		return -1;
	}

	auto self = (PymvBuffer*)obj;
	view->obj = (PyObject*)self;
	view->buf = (void*)self->arr.data;
	view->len = self->arr.length * sizeof(float);
	view->readonly = 0;
	view->itemsize = sizeof(float);
	//view->format = (char*)(PyBUF_CONTIG_RO | PyBUF_FORMAT);  // float
	static char format[] = "f";
	view->format = format;  // float
	view->ndim = 1;
	view->shape = (Py_ssize_t*)&self->arr.length;  // length-1 sequence of dimensions
	view->strides = &view->itemsize;  // for the simple case we can do this
	view->suboffsets = nullptr;
	view->internal = nullptr;

	Py_INCREF(self);  // need to increase the reference count
	return 0;
}

Py_ssize_t 
PymvBuffer_getLength(PyObject* obj)
{
	auto self = (PymvBuffer*)obj;
	return self->arr.length;
}

PyObject* 
PymvBuffer_getItem(PyObject* obj, Py_ssize_t index)
{
	PymvBuffer* self = (PymvBuffer*)obj;
	return Py_BuildValue("f", self->arr.data[index]);
}

int 
PymvBuffer_setItem(PyObject* obj, Py_ssize_t index, PyObject* value)
{
	PymvBuffer* self = (PymvBuffer*)obj;
	self->arr.data[index] = (float)PyFloat_AsDouble(value);
	return 0;
}