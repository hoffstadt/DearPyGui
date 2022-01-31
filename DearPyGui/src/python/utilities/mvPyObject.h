#pragma once

// forward declare PyObject
// as suggested on the python mailing list
// http://mail.python.org/pipermail/python-dev/2003-August/037601.html
#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

class mvPyObject
{

public:

	mvPyObject(PyObject* rawObject, bool borrowed=false);
	mvPyObject(mvPyObject&& other);
	mvPyObject& operator=(mvPyObject&& other);

	mvPyObject(const mvPyObject& other) = delete;
	mvPyObject& operator=(mvPyObject& other) = delete;

	~mvPyObject();

	void addRef();
	void delRef();
	bool isOk() const { return m_ok; }

	operator PyObject* ();

private:

	PyObject* m_rawObject;
	bool      m_borrowed;
	bool      m_ok;
	bool      m_del = false;

};
