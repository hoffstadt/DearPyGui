#include "mvResizeHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "containers/mvWindowAppItem.h"

namespace Marvel {

	void mvResizeHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a handler which runs a given callback when the specified item is resized.", { "Events", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_CALLBACK)
		);

		parser.finalize();

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
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
						PyObject* dimensions = PyTuple_New(2);
						PyTuple_SetItem(dimensions, 0, PyLong_FromLong(_parentPtr->_width));
						PyTuple_SetItem(dimensions, 1, PyLong_FromLong(_parentPtr->_height));
						mvApp::GetApp()->getCallbackRegistry().addCallback(_callback, _uuid, dimensions, _user_data);
						});
				else
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
					PyObject* dimensions = PyTuple_New(2);
					PyTuple_SetItem(dimensions, 0, PyLong_FromLong(_parentPtr->_width));
					PyTuple_SetItem(dimensions, 1, PyLong_FromLong(_parentPtr->_height));
					mvApp::GetApp()->getCallbackRegistry().addCallback(_callback, _alias, dimensions, _user_data);
						});
			}

			parentPtr->_resized = false;
		}
	}
}