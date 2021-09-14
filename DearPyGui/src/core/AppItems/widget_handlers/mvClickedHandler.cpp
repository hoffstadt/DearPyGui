#include "mvClickedHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvClickedHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_CALLBACK)
		);

		args.push_back({ mvPyDataType::Integer, "button", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all mouse buttons" });

		mvPythonParserSetup setup;
		setup.about = "Adds a clicked handler.";
		setup.category = { "Widgets", "Events" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvClickedHandler::mvClickedHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvClickedHandler::customAction(void* data)
	{

		if(_button == -1 || _button == 0)
			if (static_cast<mvAppItemState*>(data)->leftclicked)
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
					{
						if (_alias.empty())
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, ToPyInt(0), _user_data);
						else
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _alias, ToPyInt(0), _user_data);
					});
			}

		if (_button == -1 || _button == 1)
			if (static_cast<mvAppItemState*>(data)->rightclicked)
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
					{
						if (_alias.empty())
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, ToPyInt(1), _user_data);
						else
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _alias, ToPyInt(1), _user_data);
					});
			}

		if (_button == -1 || _button == 2)
			if (static_cast<mvAppItemState*>(data)->middleclicked)
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
					{
						if (_alias.empty())
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, ToPyInt(2), _user_data);
						else
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _alias, ToPyInt(2), _user_data);
					});
			}

	}

	void mvClickedHandler::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(mvApp::GetApp()->getParsers()[s_command], dict))
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