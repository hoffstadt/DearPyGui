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
	a->cols[0][0] = m00;
	a->cols[0][1] = m10;
	a->cols[0][2] = m20;
	a->cols[0][3] = m30;
	a->cols[1][0] = m01;
	a->cols[1][1] = m11;
	a->cols[1][2] = m21;
	a->cols[1][3] = m31;
	a->cols[2][0] = m02;
	a->cols[2][1] = m12;
	a->cols[2][2] = m22;
	a->cols[2][3] = m32;
	a->cols[3][0] = m03;
	a->cols[3][1] = m13;
	a->cols[3][2] = m23;
	a->cols[3][3] = m33;
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

	static char km00[] = "m00";
	static char km01[] = "m01";
	static char km02[] = "m02";
	static char km03[] = "m03";
	static char km10[] = "m10";
	static char km11[] = "m11";
	static char km12[] = "m12";
	static char km13[] = "m13";
	static char km20[] = "m20";
	static char km21[] = "m21";
	static char km22[] = "m22";
	static char km23[] = "m23";
	static char km30[] = "m30";
	static char km31[] = "m31";
	static char km32[] = "m32";
	static char km33[] = "m33";

	static char* kwlist[] = { 
	km00, km01, km02, km03,
	km10, km11, km12, km13, 
	km20, km21, km22, km23,
	km30, km31, km32, km33, NULL };
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
			result += std::to_string(self->m.cols[j][i]) + "\t";
		
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
	
	mv_local_persist Py_ssize_t arrayLength = 16;

	auto self = (PymvMat4*)obj;
	view->obj = (PyObject*)self;
	view->buf = (void*)self->m.cols;
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
	return 16;
}

PyObject* 
PymvMat4_getItem(PyObject* obj, Py_ssize_t index)
{
	PymvMat4* self = (PymvMat4*)obj;
	if(index > 15)
		return Py_BuildValue("f", self->m.cols[3][3]);

    i32 column = index/4;
    i32 row = index % 4;
	return Py_BuildValue("f", self->m.cols[column][row]);
}

int 
PymvMat4_setItem(PyObject* obj, Py_ssize_t index, PyObject* value)
{
	PymvMat4* self = (PymvMat4*)obj;
	if (index > 15)
		return 0;

    i32 column = index/4;
    i32 row = index % 4;
	self->m.cols[column][row] = (float)PyFloat_AsDouble(value);
	return 0;
}

PyObject* 
PymvMat4_add(PyObject* left, PyObject* right)
{
	PyObject* newbuffer = nullptr;
	PymvMat4* newbufferview = nullptr;
	newbufferview = PyObject_New(PymvMat4, &PymvMat4Type);
	newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvMat4Type);

	PymvMat4* leftVect = (PymvMat4*)left;
	PymvMat4* rightVect = (PymvMat4*)right;
    
    newbufferview->m = leftVect->m + rightVect->m;

	return newbuffer;
}

PyObject*
PymvMat4_subtract(PyObject* left, PyObject* right)
{
	PyObject* newbuffer = nullptr;
	PymvMat4* newbufferview = nullptr;
	newbufferview = PyObject_New(PymvMat4, &PymvMat4Type);
	newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvMat4Type);

	PymvMat4* leftVect = (PymvMat4*)left;
	PymvMat4* rightVect = (PymvMat4*)right;

    newbufferview->m = leftVect->m - rightVect->m;

	return newbuffer;
}

PyObject*
PymvMat4_multiply(PyObject* left, PyObject* right)
{
	PyObject* newbuffer = nullptr;
	PymvMat4* newbufferview = nullptr;
	newbufferview = PyObject_New(PymvMat4, &PymvMat4Type);
	newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvMat4Type);

	PymvMat4* leftVect = (PymvMat4*)left;
	PymvMat4* rightVect = (PymvMat4*)right;

    newbufferview->m = leftVect->m * rightVect->m;

	return newbuffer;
}
