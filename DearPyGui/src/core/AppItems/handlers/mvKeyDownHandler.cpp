#include "mvKeyDownHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvKeyDownHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::Integer, "key", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all keys" });
		args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });
		
		mvPythonParserSetup setup;
		setup.about = "Adds a key down handler.";
		setup.category = { "Events", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		
		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvKeyDownHandler::mvKeyDownHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvKeyDownHandler::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvKeyDownHandler*>(item);
		_key = titem->_key;
	}

	void mvKeyDownHandler::draw(ImDrawList* drawlist, float x, float y)
	{

		if (_key == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
			{
				if (ImGui::GetIO().KeysDown[i])
				{
					GContext->callbackRegistry->submitCallback([=]()
						{
							if(_alias.empty())
								GContext->callbackRegistry->runCallback(getCallback(false), _uuid, ToPyMPair(i, ImGui::GetIO().KeysDownDuration[i]), _user_data);
							else
								GContext->callbackRegistry->runCallback(getCallback(false), _alias, ToPyMPair(i, ImGui::GetIO().KeysDownDuration[i]), _user_data);
						});
				}
			}
		}

		else if (ImGui::GetIO().KeysDown[_key])
		{
			GContext->callbackRegistry->submitCallback([=]()
				{
					if (_alias.empty())
						GContext->callbackRegistry->runCallback(getCallback(false), _uuid, ToPyMPair(_key, ImGui::GetIO().KeysDownDuration[_key]), _user_data);
					else
						GContext->callbackRegistry->runCallback(getCallback(false), _alias, ToPyMPair(_key, ImGui::GetIO().KeysDownDuration[_key]), _user_data);
				});
		}
	}

	void mvKeyDownHandler::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_key = ToInt(item);
				break;

			default:
				break;
			}
		}
	}

	void mvKeyDownHandler::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "key")) _key = ToInt(item);
	}

	void mvKeyDownHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "key", mvPyObject(ToPyInt(_key)));
	}

}