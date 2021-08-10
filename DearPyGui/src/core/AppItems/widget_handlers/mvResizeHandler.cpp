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
			MV_PARSER_ARG_CALLBACK)
		);

		parser.addArg<mvPyDataType::UUID>("parent");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvResizeHandler::mvResizeHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvResizeHandler::draw(ImDrawList* drawlist, float x, float y)
	{

		if (_parentPtr)
		{
			auto parentPtr = static_cast<mvWindowAppItem*>(_parentPtr);
			if (parentPtr->_resized && _callback)
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
					PyObject* dimensions = PyTuple_New(2);
					PyTuple_SetItem(dimensions, 0, PyLong_FromLong(_parentPtr->getWidth()));
					PyTuple_SetItem(dimensions, 1, PyLong_FromLong(_parentPtr->getHeight()));
					mvApp::GetApp()->getCallbackRegistry().addCallback(_callback, _uuid, dimensions, _user_data);
					});
			}

			parentPtr->_resized = false;
		}
	}

	void mvResizeHandler::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_parent = ToUUID(item);
				break;

			default:
				break;
			}
		}
	}
}