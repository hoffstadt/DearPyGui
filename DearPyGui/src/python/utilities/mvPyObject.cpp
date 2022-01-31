#include "mvPyObject.h"
#include <utility>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

mvPyObject::mvPyObject(PyObject* rawObject, bool borrowed) 
	:
	m_rawObject(rawObject),
	m_borrowed(borrowed),
	m_ok(rawObject != nullptr)
{

}

mvPyObject::mvPyObject(mvPyObject&& other)
	:
	m_rawObject(nullptr),
	m_borrowed(false),
	m_ok(false)
{
	std::swap(m_rawObject, other.m_rawObject);
	std::swap(m_borrowed, other.m_borrowed);
	std::swap(m_ok, other.m_ok);
}

mvPyObject& mvPyObject::operator=(mvPyObject&& other)
{
	if (this != &other)
	{
		if (m_rawObject != nullptr && !m_borrowed)
			Py_XDECREF(m_rawObject);

		std::swap(other.m_rawObject, m_rawObject);
		std::swap(other.m_borrowed, m_borrowed);
		std::swap(other.m_ok, m_ok);
	}

	return *this;
}

mvPyObject::~mvPyObject()
{
	if(!m_borrowed && !m_del)
		Py_XDECREF(m_rawObject);
}

mvPyObject::operator PyObject*()
{
	return m_rawObject;
}

void mvPyObject::addRef()
{
	Py_XINCREF(m_rawObject);
}

void mvPyObject::delRef()
{
	Py_XDECREF(m_rawObject);
	m_del = true;
}