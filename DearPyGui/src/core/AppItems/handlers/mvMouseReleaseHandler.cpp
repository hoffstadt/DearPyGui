#include "mvMouseReleaseHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvMouseReleaseHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a handler which runs a given callback when the specified mouse button is released. Parent must be a handler registry.", { "Events", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::Integer>("button", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all mouse buttons");
		parser.addArg<mvPyDataType::UUID>("parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)");
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvMouseReleaseHandler::mvMouseReleaseHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvMouseReleaseHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_button == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
			{
				if (ImGui::IsMouseReleased(i))
				{
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
						{
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, ToPyInt(i), _user_data);
						});
				}
			}
		}

		else if (ImGui::IsMouseReleased(_button))
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, ToPyInt(_button), _user_data);
				});
		}
	}

	void mvMouseReleaseHandler::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_button = ToInt(item);
				break;

			default:
				break;
			}
		}
	}

	void mvMouseReleaseHandler::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "button")) _button = ToInt(item);
	}

	void mvMouseReleaseHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "button", ToPyInt(_button));
	}

}