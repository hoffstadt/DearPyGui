#include "mvGlobalIntepreterLock.h"
#include "mvContext.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

mvGlobalIntepreterLock::mvGlobalIntepreterLock()
{
	_gstate = (int)PyGILState_Ensure();
}

mvGlobalIntepreterLock::~mvGlobalIntepreterLock()
{
	PyGILState_Release((PyGILState_STATE)_gstate);
}
