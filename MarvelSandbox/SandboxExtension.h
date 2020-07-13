#include "marvel.h"
#include <iostream>

namespace Marvel {

	static std::map<std::string, mvPythonParser> Translators = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbExtension");

	std::map<std::string, mvPythonParser>& BuildExtensionsInterface() {

		std::map<std::string, mvPythonParser>* translators = new std::map<std::string, mvPythonParser>{

			{"testme", mvPythonParser({
			}, "Needs documentation")},

		};

		return *translators;
	}

	PyObject* testme(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!Translators["testme"].parse(args, kwargs, __FUNCTION__))
			Py_RETURN_NONE;

		std::cout << "hell yes" << std::endl;

		Py_RETURN_NONE;
	}

	mvPythonModule* CreateExtensionInterface()
	{

		auto pyModule = new mvPythonModule("sbExtension", {});
		pyModule->addMethodD(testme);
		return pyModule;
	}

	class mvSandboxInitializer : public mvModuleInitializer
	{

	public:

		virtual void initializeUserModules(int startindex) override
		{
			initModule<MV_MODULE_START>("sbExtension", CreateExtensionInterface, BuildExtensionsInterface);
		}

	};

	mvModuleInitializer* mvModuleInitializer::getInitializer()
	{
		return new mvSandboxInitializer();
	}
}