#pragma once
#include "mvPython.h"

namespace Marvel {

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
		bool isOk() const { return m_ok; }

		operator PyObject* ();

	private:

		PyObject* m_rawObject;
		bool      m_borrowed;
		bool      m_ok;

	};

}