#include "mvDataStorage.h"
#include "mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"
#include <thread>

namespace Marvel {

	std::map<std::string, PyObject*> mvDataStorage::s_dataStorage;

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
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Data can not be modified outside main thread.");
			return;
		}

		if (s_dataStorage.count(name) > 0)
		{
			DeleteData(name);
			AddData(name, data);
			for (auto window : mvApp::GetApp()->getWindows())
				window->updateDataSource(name);
			return;
		}

		s_dataStorage.insert({ name, data });

		for (auto window : mvApp::GetApp()->getWindows())
			window->updateDataSource(name);
	}

	void mvDataStorage::DeleteData(const std::string& name)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Data can not be modified outside main thread.");
			return;
		}

		if (s_dataStorage.count(name) == 0)
		{
			mvAppLog::getLogger()->LogWarning(name + " does not exists in data storage.");
			return;
		}

		Py_XDECREF(s_dataStorage.at(name));
		s_dataStorage.erase(name);
	}

	PyObject* mvDataStorage::GetData(const std::string& name)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Data can not be modified outside main thread.");
			return nullptr;
		}

		if (s_dataStorage.count(name) == 0)
		{
			mvAppLog::getLogger()->LogWarning(name + " does not exists in data storage.");
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