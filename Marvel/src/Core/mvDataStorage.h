#pragma once

//-----------------------------------------------------------------------------
// mvDataStorage
//
//     - This class acts as a manager for data storage by the users. The 
//       data storage system serves 3 purposes:
//
//         * Allows some widgets to share the same underlying data.
//         * Allows some widgets secondary data be modified (i.e. listbox list items)
//         * Allows the user to store data for any other purpose where
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
#include <map>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvDataStorage
	//-----------------------------------------------------------------------------
	class mvDataStorage
	{

	public:

		static void      AddData      (const std::string& name, PyObject* data);
		static void      DeleteData   (const std::string& name);
		static void      UpdateData   ();
		static PyObject* GetData      (const std::string& name);
		static unsigned  GetDataCount ();
		static void      DeleteAllData();

	private:

		mvDataStorage() = default;

		static std::map<std::string, PyObject*> s_dataStorage;

	};

}