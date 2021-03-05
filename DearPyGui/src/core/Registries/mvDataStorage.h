#pragma once

//-----------------------------------------------------------------------------
// mvDataStorage
//
//     *  Allows the user to store data for any purpose where
//           the data can be retrieved at a later time.
//
//     - AddData is also used to update existing data. We probably need to add
//       an UpdataData method to make this explicit and come up with another
//       name for the current UpdateData method. The current UpdateData just
//       updates all items in mvApp (which should not be this class's
//       responsibility).
//     
//-----------------------------------------------------------------------------

#include <string>
#include <unordered_map>
#include <mutex>
#include <map>
#include "mvPython.h"

namespace Marvel {

	// forward declarations
	class mvPythonParser;

#ifdef MV_CPP
#else
	void AddDataStorageCommands(std::map<std::string, mvPythonParser>* parsers);

	PyObject* add_data   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_data   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_data(PyObject* self, PyObject* args, PyObject* kwargs);
#endif

	//-----------------------------------------------------------------------------
	// mvDataStorage
	//-----------------------------------------------------------------------------
	class mvDataStorage
	{

	public:

		static void      AddData      (const std::string& name, PyObject* data);
		static void      DeleteData   (const std::string& name);
		static PyObject* GetData      (const std::string& name); // does not add to ref count
		static PyObject* GetDataIncRef(const std::string& name); // adds to ref count
		static unsigned  GetDataCount ();
		static void      DeleteAllData();

	private:

		mvDataStorage() = default;

		static std::unordered_map<std::string, PyObject*> s_dataStorage;
		static std::mutex                                 s_mutex;

	};

}