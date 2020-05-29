#define PY_SSIZE_T_CLEAN
#include <Python.h>

class Entity
{

public:

	static PyObject* pInstance;
	static PyObject* pModule;
	std::string m_callback;
	std::string m_name = "Unset";
	static Entity* s_instance;


public:

	static Entity* GetEntity()
	{
		if (s_instance)
			return s_instance;

		s_instance = new Entity();

		return s_instance;
	}

	void setName(const std::string& name)
	{
		m_name = name;
	}

	void setCallback(const std::string& name)
	{
		m_callback = name;
	}

	PyObject* run()
	{

		//PyObject* result = PyObject_CallMethod(pInstance, m_callback.c_str(), nullptr);
		PyObject* result = PyObject_CallFunction(pInstance, nullptr);

		if (result == NULL)
			return NULL;

		return result;
	}

	const std::string& getName()
	{
		return m_name;
	}



};

Entity* Entity::s_instance = nullptr;
PyObject* Entity::pModule = nullptr;
PyObject* Entity::pInstance = nullptr;


PyObject* setName(PyObject* self, PyObject* args)
{
	const char* name = Entity::GetEntity()->getName().c_str();

	PyArg_ParseTuple(args, "s", &name);

	Entity::GetEntity()->setName(std::string(name));

	return Py_None;
}

PyObject* setCallback(PyObject* self, PyObject* args)
{
	const char* name = Entity::GetEntity()->getName().c_str();

	PyArg_ParseTuple(args, "s", &name);

	Entity::GetEntity()->setCallback(std::string(name));

	return Py_None;
}

PyObject* runLoop(PyObject* self, PyObject* args)
{

	auto entity = Entity::GetEntity();
	PyObject* result = entity->run();

	return result;
}

PyObject* getName(PyObject* self, PyObject* args)
{
	PyObject* name = PyUnicode_FromString(Entity::GetEntity()->getName().c_str());

	return name;
}

PyMethodDef EntityMethods[] = {
	{"setName", setName, METH_VARARGS, "documentation"},
	{"getName", getName, METH_VARARGS, "documentation"},
	{"runLoop", runLoop, METH_VARARGS, "documentation"},
	{"setCallback", setCallback, METH_VARARGS, "documentation"},
	{NULL, NULL, 0, NULL}
};

PyModuleDef EntityModule = {
	PyModuleDef_HEAD_INIT, "entity", NULL, -1, EntityMethods,
	NULL, NULL, NULL, NULL
};

PyObject* PyInit_emb(void)
{
	return PyModule_Create(&EntityModule);
}

