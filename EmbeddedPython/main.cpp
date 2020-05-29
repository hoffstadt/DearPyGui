#include <iostream>
#include "Entity.h"

int main()
{

	Py_SetPath(L"python38.zip;../../../../EmbeddedPython");
	PyImport_AppendInittab("entity", &PyInit_emb);

	// start the intpreter
	Py_Initialize();

	// import the module (python file) looking in the path
	Entity::pModule = PyImport_ImportModule("Entity"); // new reference

	// returns the dictionary object representing the module namespace
	PyObject* pDict = PyModule_GetDict(Entity::pModule); // borrowed reference

	// getting the python class defintion
	PyObject* pEntity = PyDict_GetItemString(pDict, "Handler"); // borrowed reference

	// calling the python Entity class constructor and storing the
	// created instance
	//Entity::pInstance = PyObject_CallObject(pEntity, nullptr); // new reference
	Entity::pInstance = pEntity; // new reference

	Entity::GetEntity()->run();
	Entity::GetEntity()->run();
	Entity::GetEntity()->run();
	Entity::GetEntity()->run();

	// shutdown the intpreter
	Py_Finalize();

}