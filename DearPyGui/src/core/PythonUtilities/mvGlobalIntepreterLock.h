#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <time.h>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvGlobalIntepreterLock
	//     - A convience class to automatically handle aqcuiring and releasing
	//       python's global intpreter lock between python c api calls.
	//-----------------------------------------------------------------------------
	class mvGlobalIntepreterLock
	{

	public:

		mvGlobalIntepreterLock();
		~mvGlobalIntepreterLock();

	private:

		PyGILState_STATE m_gstate;

	};

}
