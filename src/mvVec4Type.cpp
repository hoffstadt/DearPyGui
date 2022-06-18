#include "mvVec4Type.h"
#include <string>
#include "mvTypes.h"
#include "mvPythonTranslator.h"
#include "mvPythonTypeChecker.h"
#include "mvPythonExceptions.h"

mv_internal void 
intialize_mvVec4(mvVec4* a, float x, float y, float z, float w)
{
	a->x = x;
	a->y = y;
	a->z = z;
	a->w = w;
}

int 
PymvVec4_init(PymvVec4* self, PyObject* args, PyObject* kwds)
{

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
	static char kw_x[] = "x";
	static char kw_y[] = "y";
	static char kw_z[] = "z";
	static char kw_w[] = "w";
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
    result += std::to_string(self->vec4.x) + " ";
    result += std::to_string(self->vec4.y) + " ";
    result += std::to_string(self->vec4.z) + " ";
    result += std::to_string(self->vec4.w) + " ";
	result +=  "]";

	PyObject* ret = PyUnicode_FromString(result.c_str());

	return ret;
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
    switch(index)
    {
        case 0: return Py_BuildValue("f", self->vec4.x);            
        case 1: return Py_BuildValue("f", self->vec4.y);            
        case 2: return Py_BuildValue("f", self->vec4.z);            
        case 3: return Py_BuildValue("f", self->vec4.w);
        default: return Py_BuildValue("f", self->vec4.w);
    }
}

int 
PymvVec4_setItem(PyObject* obj, Py_ssize_t index, PyObject* value)
{
	PymvVec4* self = (PymvVec4*)obj;
	if (index > 3)
		return 0;
    f32 val  = (float)PyFloat_AsDouble(value);
        switch(index)
    {
        case 0:
            self->vec4.x = val;
            break;
        case 1:
            self->vec4.y = val;
            break;
        case 2:
            self->vec4.z = val;
            break;
        case 3:
            self->vec4.w = val;
            break;
        default: break;
    }
        
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

	newbufferview->vec4.x = leftVect->vec4.x + rightVect->vec4.x;
	newbufferview->vec4.y = leftVect->vec4.y + rightVect->vec4.y;
	newbufferview->vec4.z = leftVect->vec4.z + rightVect->vec4.z;
	newbufferview->vec4.w = leftVect->vec4.w + rightVect->vec4.w;

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

	newbufferview->vec4.x = leftVect->vec4.x - rightVect->vec4.x;
	newbufferview->vec4.y = leftVect->vec4.y - rightVect->vec4.y;
	newbufferview->vec4.z = leftVect->vec4.z - rightVect->vec4.z;
	newbufferview->vec4.w = leftVect->vec4.w - rightVect->vec4.w;

	return newbuffer;
}

PyObject* PymvVec4_multiply(PyObject* left, PyObject* right)
{
	PyObject* newbuffer = nullptr;
	PymvVec4* newbufferview = nullptr;
	newbufferview = PyObject_New(PymvVec4, &PymvVec4Type);
	newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvVec4Type);

	//PymvVec4* leftVect = (PymvVec4*)left;
	//PymvVec4* rightVect = (PymvVec4*)right;

	if (isPyObject_Float(left))
	{
		float value = ToFloat(left);
		PymvVec4* rightVect = (PymvVec4*)right;

		newbufferview->vec4.x = value * rightVect->vec4.x;
		newbufferview->vec4.y = value * rightVect->vec4.y;
		newbufferview->vec4.z = value * rightVect->vec4.z;
		newbufferview->vec4.w = value * rightVect->vec4.w;
	}

	else if (isPyObject_Float(right))
	{
		float value = ToFloat(right);
		PymvVec4* leftVect = (PymvVec4*)left;

		newbufferview->vec4.x = value * leftVect->vec4.x;
		newbufferview->vec4.y = value * leftVect->vec4.y;
		newbufferview->vec4.z = value * leftVect->vec4.z;
		newbufferview->vec4.w = value * leftVect->vec4.w;
	}

	else
	{
		PymvVec4* leftVect = (PymvVec4*)left;
		PymvVec4* rightVect = (PymvVec4*)right;
		newbufferview->vec4.x = leftVect->vec4.x * rightVect->vec4.x;
		newbufferview->vec4.y = leftVect->vec4.y * rightVect->vec4.y;
		newbufferview->vec4.z = leftVect->vec4.z * rightVect->vec4.z;
		newbufferview->vec4.w = leftVect->vec4.w * rightVect->vec4.w;
	}

	return newbuffer;
}