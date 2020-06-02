#include "mvPythonInterface.h"

namespace Marvel {

	PyMethodDef SandboxMethods[] = {
		{"setItemCallback", setItemCallback, METH_VARARGS, "documentation"},
		{"addInputText", (PyCFunction)addInputText, METH_VARARGS | METH_KEYWORDS, "documentation"},
		{"addTabBar", addTabBar, METH_VARARGS, "documentation"},
		{"addTab", addTab, METH_VARARGS, "documentation"},
		{"endTab", endTab, METH_VARARGS, "documentation"},
		{"endTabBar", endTabBar, METH_VARARGS, "documentation"},
		{"getValue", getValue, METH_VARARGS, "documentation"},
		{NULL, NULL, 0, NULL}
	};

	PyModuleDef SandboxModule = {
		PyModuleDef_HEAD_INIT, "sandbox", NULL, -1, SandboxMethods,
		NULL, NULL, NULL, NULL
	};

	PyObject* PyInit_emb(void)
	{
		return PyModule_Create(&SandboxModule);
	}

	PyObject* getValue(PyObject* self, PyObject* args)
	{
		const char* name;

		PyArg_ParseTuple(args, "s", &name);

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if(item == nullptr)
			return Py_None;

		return item->getPyValue();
	}

	PyObject* setItemCallback(PyObject* self, PyObject* args)
	{
		const char* callback, * item;

		PyArg_ParseTuple(args, "ss", &callback, &item);

		mvApp::GetApp()->setItemCallback(std::string(callback), std::string(item));

		return Py_None;
	}

	PyObject* addInputText(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent, *name;
		const char* hint = "";
		static const char* keywords[] = { "parent", "name", "hint", NULL };

		if(!PyArg_ParseTupleAndKeywords(args, kwargs, "ss|$s", const_cast<char**>(keywords), &parent, &name, &hint))
			__debugbreak();

		mvApp::GetApp()->addInputText(std::string(parent), std::string(name), std::string(hint));
		
		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addTabBar(PyObject* self, PyObject* args)
	{
		const char* parent, * name;

		PyArg_ParseTuple(args, "ss", &parent, &name);

		mvApp::GetApp()->addTabBar(std::string(parent), std::string(name));

		return Py_None;
	}

	PyObject* addTab(PyObject* self, PyObject* args)
	{
		const char* parent, * name;

		PyArg_ParseTuple(args, "ss", &parent, &name);

		mvApp::GetApp()->addTab(std::string(parent), std::string(name));

		return Py_None;
	}

	PyObject* endTab(PyObject* self, PyObject* args)
	{
		const char* parent;

		PyArg_ParseTuple(args, "s", &parent);

		mvApp::GetApp()->endTab(std::string(parent));

		return Py_None;
	}

	PyObject* endTabBar(PyObject* self, PyObject* args)
	{
		const char* parent;

		PyArg_ParseTuple(args, "s", &parent);

		mvApp::GetApp()->endTabBar(std::string(parent));

		return Py_None;
	}
}