#include "mvDataStorage.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include <thread>

namespace Marvel {

	std::unordered_map<std::string, PyObject*> mvDataStorage::s_dataStorage;
	std::mutex mvDataStorage::s_mutex;

	void mvDataStorage::DeleteAllData()
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		for (auto& data : s_dataStorage)
			Py_XDECREF(data.second);

		s_dataStorage.clear();
	}

	void mvDataStorage::AddData(const std::string& name, PyObject* data)
	{

		std::lock_guard<std::mutex> lock(s_mutex);

		// data doesn't exist, create it for the first time
		if (s_dataStorage.count(name) == 0)
			s_dataStorage.insert({ name, data });

		else
		{
			if (s_dataStorage.at(name) != data)
			{

				if (s_dataStorage.at(name) == Py_None)
					Py_XDECREF(s_dataStorage.at(name));
				else if (PyNumber_Check(s_dataStorage.at(name)))
					Py_XDECREF(s_dataStorage.at(name));
				else if (PyUnicode_Check(s_dataStorage.at(name)))
					Py_XDECREF(s_dataStorage.at(name));
				else
				{
					while (s_dataStorage.at(name)->ob_refcnt > 0)
						Py_XDECREF(s_dataStorage.at(name));
				}

				s_dataStorage.erase(name);
				s_dataStorage[name] = data;
			}
		}

	}

	void mvDataStorage::DeleteData(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (s_dataStorage.count(name) == 0)
		{
			ThrowPythonException(name + " does not exists in data storage.");
			return;
		}

		 
		Py_XDECREF(s_dataStorage.at(name));
		s_dataStorage.erase(name);
	}

	PyObject* mvDataStorage::GetDataIncRef(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (s_dataStorage.count(name) == 0)
		{
			ThrowPythonException(name + " does not exists in data storage.");
			return nullptr;
		}

		 
		Py_XINCREF(s_dataStorage.at(name));
		return s_dataStorage.at(name);
	}

	PyObject* mvDataStorage::GetData(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (s_dataStorage.count(name) == 0)
		{
			ThrowPythonException(name + " does not exists in data storage.");
			return nullptr;
		}
		return s_dataStorage.at(name);
	}

	unsigned  mvDataStorage::GetDataCount()
	{
		std::lock_guard<std::mutex> lock(s_mutex);
		return (unsigned)s_dataStorage.size();
	}

}
