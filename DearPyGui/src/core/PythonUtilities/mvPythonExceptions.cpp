#include "mvPythonExceptions.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <frameobject.h>
#include <string>
#include "mvContext.h"
#include "mvAppItem.h"
#include "mvAppItemCommons.h"
#include "mvViewport.h"
#include "mvCallbackRegistry.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel
{

	void 
	mvThrowPythonError(mvErrorCode code, const std::string& message)
	{
		std::string fullMessage = "Error: [%d] Message: \t" + message;
		PyErr_Format(PyExc_Exception, fullMessage.c_str(), (int)code);
	}

	void 
	mvThrowPythonError(mvErrorCode code, const std::string& command, const std::string& message, mvAppItem* item)
	{
		if (item)
		{
			std::string fullMessage = "\nError:     [%d]\nCommand:   %s\nItem:      %d \nLabel:     %s\nItem Type: %s\nMessage:   %s";
			PyErr_Format(PyExc_Exception,
				fullMessage.c_str(),
				(int)code,
				command.c_str(),
				item->_uuid,
				item->_specifiedLabel.c_str(),
				GetEntityTypeString(item->getType()),
				message.c_str());
		}
		else
		{
			std::string fullMessage = "\nError:     [%d]\nCommand:   %s\nItem:      %d \nLabel:     %s\nItem Type: %s\nMessage:   %s";
			PyErr_Format(PyExc_Exception,
				fullMessage.c_str(),
				(int)code,
				command.c_str(),
				0,
				"Not found",
				"Unknown",
				message.c_str());
		}
	}

}
