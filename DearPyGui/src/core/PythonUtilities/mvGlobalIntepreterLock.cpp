#include "mvGlobalIntepreterLock.h"
#include "mvApp.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

	mvGlobalIntepreterLock::mvGlobalIntepreterLock()
	{
		m_gstate = (int)PyGILState_Ensure();
	}

	mvGlobalIntepreterLock::~mvGlobalIntepreterLock()
	{
		PyGILState_Release((PyGILState_STATE)m_gstate);
	}

}
