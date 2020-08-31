#include "mvDataStorage.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include <thread>
#include "Core/mvPythonExceptions.h"

namespace Marvel {

	std::map<std::string, PyObject*> mvDataStorage::s_dataStorage;

	void mvDataStorage::DeleteAllData()
	{
		for (auto& data : s_dataStorage)
			Py_XDECREF(data.second);
		s_dataStorage.clear();
	}

	void mvDataStorage::UpdateData()
	{
		for (auto& data : s_dataStorage)
		{
			for (auto window : mvApp::GetApp()->getWindows())
				window->updateDataSource(data.first);
		}
	}

	void mvDataStorage::AddData(const std::string& name, PyObject* data)
	{

		if (!mvApp::GetApp()->checkIfMainThread())
			return;

		// data doesn't exist, create it for the first time
		if (s_dataStorage.count(name) == 0)
			s_dataStorage.insert({ name, data });
		else
		{
			Py_XINCREF(s_dataStorage.at(name));
			s_dataStorage[name] = data;
		}

		for (auto window : mvApp::GetApp()->getWindows())
			window->updateDataSource(name);
	}

	void mvDataStorage::DeleteData(const std::string& name)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return;

		if (s_dataStorage.count(name) == 0)
		{
			ThrowPythonException(name + " does not exists in data storage.");
			return;
		}

		Py_XDECREF(s_dataStorage.at(name));
		s_dataStorage.erase(name);
	}

	bool mvDataStorage::HasData(const std::string& name)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return false;

		if (s_dataStorage.count(name) == 0)
			return false;

		return true;
	}

	PyObject* mvDataStorage::GetData(const std::string& name)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return nullptr;

		if (s_dataStorage.count(name) == 0)
		{
			ThrowPythonException(name + " does not exists in data storage.");
			return nullptr;
		}
		Py_XINCREF(s_dataStorage.at(name));
		return s_dataStorage.at(name);
	}

	unsigned  mvDataStorage::GetDataCount()
	{
		return s_dataStorage.size();
	}

}
