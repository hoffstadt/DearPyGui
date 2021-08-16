#include "mvClickedHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvClickedHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a handler which runs a given callback when the specified item is clicked.", { "Events", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_CALLBACK)
		);

		parser.addArg<mvPyDataType::UUID>("parent");

		parser.addArg<mvPyDataType::Integer>("button", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all mouse buttons");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvClickedHandler::mvClickedHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvClickedHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_button == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
			{
				if (ImGui::IsItemClicked(i))
				{
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
						{
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, ToPyInt(i), _user_data);
						});
				}
			}
		}

		else if(ImGui::IsItemClicked(_button))
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, ToPyInt(_button), _user_data);
				});
		}
	}

	void mvClickedHandler::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_parent = mvAppItem::GetIDFromPyObject(item);
				break;
			
			case 1:
				_button = ToInt(item);
				break;

			default:
				break;
			}
		}
	}

	void mvClickedHandler::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "button")) _button = ToInt(item);
	}

	void mvClickedHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "button", ToPyInt(_button));
	}

	void mvClickedHandler::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvClickedHandler*>(item);
		_button = titem->_button;
	}
}