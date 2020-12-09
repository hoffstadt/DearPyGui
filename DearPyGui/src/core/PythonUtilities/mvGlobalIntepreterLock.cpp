#include "mvGlobalIntepreterLock.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvGlobalIntepreterLock::mvGlobalIntepreterLock()
	{
		m_gstate = PyGILState_Ensure();
	}

	mvGlobalIntepreterLock::~mvGlobalIntepreterLock()
	{
		PyGILState_Release(m_gstate);
	}

}
