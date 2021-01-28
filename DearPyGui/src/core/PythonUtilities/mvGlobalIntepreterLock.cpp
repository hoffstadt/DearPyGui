#include "mvGlobalIntepreterLock.h"
#include "mvApp.h"
#include "mvAppLog.h"

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
