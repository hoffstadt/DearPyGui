#include "mvPythonInterface.h"

namespace Marvel {

	PyMethodDef SandboxMethods[] = {
		{"setItemCallback", setItemCallback, METH_VARARGS, "documentation"},
		{"setItemWidth", setItemWidth, METH_VARARGS, "documentation"},
		{"addButton", addButton, METH_VARARGS, "documentation"},
		{"addInputText", (PyCFunction)addInputText, METH_VARARGS | METH_KEYWORDS, "documentation"},
		{"addRadioButton", (PyCFunction)addRadioButton, METH_VARARGS | METH_KEYWORDS, "documentation"},
		{"addGroup", addGroup, METH_VARARGS, "documentation"},
		{"endGroup", endGroup, METH_VARARGS, "documentation"},
		{"addChild", addChild, METH_VARARGS, "documentation"},
		{"endChild", endChild, METH_VARARGS, "documentation"},
		{"addTabBar", addTabBar, METH_VARARGS, "documentation"},
		{"addTab", addTab, METH_VARARGS, "documentation"},
		{"endTab", endTab, METH_VARARGS, "documentation"},
		{"endTabBar", endTabBar, METH_VARARGS, "documentation"},
		{"addMenuBar", addMenuBar, METH_VARARGS, "documentation"},
		{"addMenu", addMenu, METH_VARARGS, "documentation"},
		{"endMenu", endMenu, METH_VARARGS, "documentation"},
		{"endMenuBar", endMenuBar, METH_VARARGS, "documentation"},
		{"addMenuItem", addMenuItem, METH_VARARGS, "documentation"},
		{"addSpacing", (PyCFunction)addSpacing, METH_VARARGS | METH_KEYWORDS, "documentation"},
		{"addSameLine", (PyCFunction)addSameLine, METH_VARARGS | METH_KEYWORDS, "documentation"},
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

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* setItemWidth(PyObject* self, PyObject* args)
	{
		const char* item;
		int width;

		PyArg_ParseTuple(args, "si", &item, &width);

		mvApp::GetApp()->setItemWidth(std::string(item), width);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addButton(PyObject* self, PyObject* args)
	{
		const char* parent, * name;

		PyArg_ParseTuple(args, "ss", &parent, &name);

		mvApp::GetApp()->addButton(std::string(parent), std::string(name));

		Py_INCREF(Py_None);

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

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addTab(PyObject* self, PyObject* args)
	{
		const char* parent, * name;

		PyArg_ParseTuple(args, "ss", &parent, &name);

		mvApp::GetApp()->addTab(std::string(parent), std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endTab(PyObject* self, PyObject* args)
	{
		const char* parent;

		PyArg_ParseTuple(args, "s", &parent);

		mvApp::GetApp()->endTab(std::string(parent));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endTabBar(PyObject* self, PyObject* args)
	{
		const char* parent;

		PyArg_ParseTuple(args, "s", &parent);

		mvApp::GetApp()->endTabBar(std::string(parent));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addMenuBar(PyObject* self, PyObject* args)
	{
		const char* name;

		PyArg_ParseTuple(args, "s", &name);

		mvApp::GetApp()->addMenuBar(std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addMenu(PyObject* self, PyObject* args)
	{
		const char* parent, * name;

		PyArg_ParseTuple(args, "ss", &parent, &name);

		mvApp::GetApp()->addMenu(std::string(parent), std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endMenu(PyObject* self, PyObject* args)
	{
		const char* parent;

		PyArg_ParseTuple(args, "s", &parent);

		mvApp::GetApp()->endMenu(std::string(parent));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endMenuBar(PyObject* self, PyObject* args)
	{
		const char* parent;

		PyArg_ParseTuple(args, "s", &parent);

		mvApp::GetApp()->endMenuBar(std::string(parent));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addMenuItem(PyObject* self, PyObject* args)
	{
		const char* parent, * name;

		PyArg_ParseTuple(args, "ss", &parent, &name);

		mvApp::GetApp()->addMenuItem(std::string(parent), std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addSpacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;
		int count = 1;
		static const char* keywords[] = { "parent", "count", NULL };

		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|i", const_cast<char**>(keywords), &parent, &count))
			__debugbreak();

		mvApp::GetApp()->addSpacing(std::string(parent), count);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addSameLine(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;
		float xoffset = 0.0f;
		float spacing = 0.0f;
		static const char* keywords[] = { "parent", "xoffset", "spacing", NULL };

		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|ff", const_cast<char**>(keywords), &parent, &xoffset, &spacing))
			__debugbreak();

		mvApp::GetApp()->addSameLine(std::string(parent), xoffset, spacing);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addRadioButton(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* parent;
		const char* name;
		PyObject* items;
		int default_value = 0;
		static const char* keywords[] = { "parent", "name", "items", "default_value", NULL };

		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ssO|i", const_cast<char**>(keywords), &parent, &name, &items, &default_value))
			__debugbreak();

		std::vector<std::string> sitems;

		for (int i = 0; i < PyTuple_Size(items); i++)
			sitems.emplace_back(PyUnicode_AsUTF8(PyTuple_GetItem(items, i)));

		mvApp::GetApp()->addRadioButtons(std::string(parent), std::string(name), sitems, default_value);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addGroup(PyObject* self, PyObject* args)
	{
		const char* parent, * name;

		PyArg_ParseTuple(args, "ss", &parent, &name);

		mvApp::GetApp()->addGroup(std::string(parent), std::string(name));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endGroup(PyObject* self, PyObject* args)
	{
		const char* parent;

		PyArg_ParseTuple(args, "s", &parent);

		mvApp::GetApp()->endGroup(std::string(parent));

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* addChild(PyObject* self, PyObject* args)
	{
		const char* parent, * name;
		int width, height;

		PyArg_ParseTuple(args, "ssii", &parent, &name, &width, &height);

		mvApp::GetApp()->addChild(std::string(parent), std::string(name), width, height);

		Py_INCREF(Py_None);

		return Py_None;
	}

	PyObject* endChild(PyObject* self, PyObject* args)
	{
		const char* parent;

		PyArg_ParseTuple(args, "s", &parent);

		mvApp::GetApp()->endChild(std::string(parent));

		Py_INCREF(Py_None);

		return Py_None;
	}
}