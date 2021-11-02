#include "mvMat4Type.h"
#include "mvVec4Type.h"
#include <string>
#include "mvTypes.h"
#include "mvPythonTranslator.h"
#include "mvPythonExceptions.h"

mv_internal void 
intialize_mvMat4(mvMat4* a,
	f32 m00, f32 m01, f32 m02, f32 m03,
	f32 m10, f32 m11, f32 m12, f32 m13,
	f32 m20, f32 m21, f32 m22, f32 m23,
	f32 m30, f32 m31, f32 m32, f32 m33)
{
	a->data[0] = m00;
	a->data[1] = m10;
	a->data[2] = m20;
	a->data[3] = m30;
	a->data[4] = m01;
	a->data[5] = m11;
	a->data[6] = m21;
	a->data[7] = m31;
	a->data[8] = m02;
	a->data[9] = m12;
	a->data[10] = m22;
	a->data[11] = m32;
	a->data[12] = m03;
	a->data[13] = m13;
	a->data[14] = m23;
	a->data[15] = m33;
}

int 
PymvMat4_init(PymvMat4* self, PyObject* args, PyObject* kwds)
{

	f32 m00 = 0.0f;
	f32 m01 = 0.0f;
	f32 m02 = 0.0f;
	f32 m03 = 0.0f;
	f32 m10 = 0.0f;
	f32 m11 = 0.0f;
	f32 m12 = 0.0f;
	f32 m13 = 0.0f;
	f32 m20 = 0.0f;
	f32 m21 = 0.0f;
	f32 m22 = 0.0f;
	f32 m23 = 0.0f;
	f32 m30 = 0.0f;
	f32 m31 = 0.0f;
	f32 m32 = 0.0f;
	f32 m33 = 0.0f;

	char km00[] = "m00";
	char km01[] = "m01";
	char km02[] = "m02";
	char km03[] = "m03";
	char km10[] = "m10";
	char km11[] = "m11";
	char km12[] = "m12";
	char km13[] = "m13";
	char km20[] = "m20";
	char km21[] = "m21";
	char km22[] = "m22";
	char km23[] = "m23";
	char km30[] = "m30";
	char km31[] = "m31";
	char km32[] = "m32";
	char km33[] = "m33";

	static char* kwlist[] = { 
	km00, km01, km02, km03,
	km10, km11, km12, km13, 
	km20, km21, km22, km23,
	km30, km31, km32, km33, nullptr };
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|ffffffffffffffff", kwlist, 
		&m00, &m01, &m02, &m03,
	    &m10, &m11, &m12, &m13,
	    &m20, &m21, &m22, &m23,
	    &m30, &m31, &m32, &m33))
		return -1;

	intialize_mvMat4(&self->m,
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33);

	return 0;
}

void 
PymvMat4_dealloc(PymvMat4* self)
{
	Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* 
PymvMat4_str(PymvMat4* self)
{
	std::string result;

	for (int i = 0; i < 4; i++)
	{
		result += "|";

		for(int j = 0; j  < 4; j++)
			result += std::to_string(self->m.data[4*j+i]) + "\t";
		
		result += "\t|\n";
	}

	PyObject* ret = PyUnicode_FromString(result.c_str());

	return ret;
}

int 
PymvMat4_getbuffer(PyObject* obj, Py_buffer* view, int flags)
{
	if (view == nullptr) {
		PyErr_SetString(PyExc_ValueError, "NULL view in getbuffer");
		return -1;
	}
	
	mv_local_persist int arrayLength = 4;

	auto self = (PymvMat4*)obj;
	view->obj = (PyObject*)self;
	view->buf = (void*)self->m.data;
	view->len = 16 * sizeof(float);
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
PymvMat4_getLength(PyObject* obj)
{
	auto self = (PymvMat4*)obj;
	return 4;
}

PyObject* 
PymvMat4_getItem(PyObject* obj, Py_ssize_t index)
{
	PymvMat4* self = (PymvMat4*)obj;
	if(index > 15)
		return Py_BuildValue("f", self->m.data[15]);
	return Py_BuildValue("f", self->m.data[index]);
}

int 
PymvMat4_setItem(PyObject* obj, Py_ssize_t index, PyObject* value)
{
	PymvMat4* self = (PymvMat4*)obj;
	if (index > 15)
		return 0;
	self->m.data[index] = (float)PyFloat_AsDouble(value);
	return 0;
}

PyObject* 
PymvMat4_add(PyObject* left, PyObject* right)
{
	PyObject* newbuffer = nullptr;
	PymvMat4* newbufferview = nullptr;
	newbufferview = PyObject_New(PymvMat4, &PymvVec4Type);
	newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvVec4Type);

	PymvMat4* leftVect = (PymvMat4*)left;
	PymvMat4* rightVect = (PymvMat4*)right;

	newbufferview->m.data[0] = leftVect->m.data[0] + rightVect->m.data[0];
	newbufferview->m.data[1] = leftVect->m.data[1] + rightVect->m.data[1];
	newbufferview->m.data[2] = leftVect->m.data[2] + rightVect->m.data[2];
	newbufferview->m.data[3] = leftVect->m.data[3] + rightVect->m.data[3];
	newbufferview->m.data[4] = leftVect->m.data[4] + rightVect->m.data[4];
	newbufferview->m.data[5] = leftVect->m.data[5] + rightVect->m.data[5];
	newbufferview->m.data[6] = leftVect->m.data[6] + rightVect->m.data[6];
	newbufferview->m.data[7] = leftVect->m.data[7] + rightVect->m.data[7];
	newbufferview->m.data[8] = leftVect->m.data[8] + rightVect->m.data[8];
	newbufferview->m.data[9] = leftVect->m.data[9] + rightVect->m.data[9];
	newbufferview->m.data[10] = leftVect->m.data[10] + rightVect->m.data[10];
	newbufferview->m.data[11] = leftVect->m.data[11] + rightVect->m.data[11];
	newbufferview->m.data[12] = leftVect->m.data[12] + rightVect->m.data[12];
	newbufferview->m.data[13] = leftVect->m.data[13] + rightVect->m.data[13];
	newbufferview->m.data[14] = leftVect->m.data[14] + rightVect->m.data[14];
	newbufferview->m.data[15] = leftVect->m.data[15] + rightVect->m.data[15];

	return newbuffer;
}

PyObject*
PymvMat4_subtract(PyObject* left, PyObject* right)
{
	PyObject* newbuffer = nullptr;
	PymvMat4* newbufferview = nullptr;
	newbufferview = PyObject_New(PymvMat4, &PymvVec4Type);
	newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvVec4Type);

	PymvMat4* leftVect = (PymvMat4*)left;
	PymvMat4* rightVect = (PymvMat4*)right;

	newbufferview->m.data[0] = leftVect->m.data[0] - rightVect->m.data[0];
	newbufferview->m.data[1] = leftVect->m.data[1] - rightVect->m.data[1];
	newbufferview->m.data[2] = leftVect->m.data[2] - rightVect->m.data[2];
	newbufferview->m.data[3] = leftVect->m.data[3] - rightVect->m.data[3];
	newbufferview->m.data[4] = leftVect->m.data[4] - rightVect->m.data[4];
	newbufferview->m.data[5] = leftVect->m.data[5] - rightVect->m.data[5];
	newbufferview->m.data[6] = leftVect->m.data[6] - rightVect->m.data[6];
	newbufferview->m.data[7] = leftVect->m.data[7] - rightVect->m.data[7];
	newbufferview->m.data[8] = leftVect->m.data[8] - rightVect->m.data[8];
	newbufferview->m.data[9] = leftVect->m.data[9] - rightVect->m.data[9];
	newbufferview->m.data[10] = leftVect->m.data[10] - rightVect->m.data[10];
	newbufferview->m.data[11] = leftVect->m.data[11] - rightVect->m.data[11];
	newbufferview->m.data[12] = leftVect->m.data[12] - rightVect->m.data[12];
	newbufferview->m.data[13] = leftVect->m.data[13] - rightVect->m.data[13];
	newbufferview->m.data[14] = leftVect->m.data[14] - rightVect->m.data[14];
	newbufferview->m.data[15] = leftVect->m.data[15] - rightVect->m.data[15];

	return newbuffer;
}
