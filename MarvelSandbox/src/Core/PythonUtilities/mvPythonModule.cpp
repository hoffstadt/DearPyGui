#include "mvPythonModule.h"
#include "Core/mvApp.h"

namespace Marvel {

	mvPythonModule::mvPythonModule(const char* name, const std::vector<std::pair<std::string, long>>& constants)
		: m_name(name), m_intconstants(constants)
	{
	}

	PyModuleDef* mvPythonModule::getModuleDefinition()
	{
		m_methods.push_back({ NULL, NULL, 0, NULL });

		auto pyModule = new PyModuleDef{
			PyModuleDef_HEAD_INIT, m_name, NULL, -1, m_methods.data(),
			NULL, NULL, NULL, NULL };

		m_moduledef.reset(pyModule);

		return pyModule;
	}

	void mvPythonModule::addMethod_(const char* name, PyCFunction function, const char* documentation)
	{
		m_methods.push_back({ name, function, METH_VARARGS, documentation });
		mvApp::GetApp()->addKeyword(name, documentation);
	}

	void mvPythonModule::addMethod_(const char* name, PyCFunctionWithKeywords function, const char* documentation)
	{
		m_methods.push_back({ name, (PyCFunction)function, METH_VARARGS | METH_KEYWORDS, documentation });
		mvApp::GetApp()->addKeyword(name, documentation);
	}

}