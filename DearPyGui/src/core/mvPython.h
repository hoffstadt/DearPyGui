#pragma once
#include "mvCore.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <frameobject.h>
#include "mvPyObject.h"
#include "mvPythonExceptions.h"
#include "mvPythonParser.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"
