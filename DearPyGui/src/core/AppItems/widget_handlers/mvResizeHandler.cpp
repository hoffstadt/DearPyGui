#include "mvResizeHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "containers/mvWindowAppItem.h"

namespace Marvel {

	void mvResizeHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_CALLBACK)
		);

		mvPythonParserSetup setup;
		setup.about = "Adds a resize handler.";
		setup.category = { "Widgets", "Events" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvResizeHandler::mvResizeHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvResizeHandler::customAction(void* data)
	{

		if (_parentPtr)
		{
			auto parentPtr = static_cast<mvWindowAppItem*>(_parentPtr);
			if (parentPtr->_resized && _callback)
			{
				if (_alias.empty())
					GContext->callbackRegistry->submitCallback([=]() {
						PyObject* dimensions = PyTuple_New(2);
						PyTuple_SetItem(dimensions, 0, PyLong_FromLong(_parentPtr->_width));
						PyTuple_SetItem(dimensions, 1, PyLong_FromLong(_parentPtr->_height));
						GContext->callbackRegistry->addCallback(_callback, _uuid, dimensions, _user_data);
						});
				else
					GContext->callbackRegistry->submitCallback([=]() {
					PyObject* dimensions = PyTuple_New(2);
					PyTuple_SetItem(dimensions, 0, PyLong_FromLong(_parentPtr->_width));
					PyTuple_SetItem(dimensions, 1, PyLong_FromLong(_parentPtr->_height));
					GContext->callbackRegistry->addCallback(_callback, _alias, dimensions, _user_data);
						});
			}

			parentPtr->_resized = false;
		}
	}
}