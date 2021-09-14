#include "mvKeyReleaseHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvKeyReleaseHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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
		setup.about = "Adds a key release handler.";
		setup.category = { "Events", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		
		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvKeyReleaseHandler::mvKeyReleaseHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvKeyReleaseHandler::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvKeyReleaseHandler*>(item);
		_key = titem->_key;
	}

	void mvKeyReleaseHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_key == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
			{
				if (ImGui::GetIO().KeysDownDurationPrev[i] >= 0.0f && !ImGui::GetIO().KeysDown[i])
				{
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
						{
							if (_alias.empty())
								mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, ToPyInt(i), _user_data);
							else
								mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _alias, ToPyInt(i), _user_data);
						});
				}
			}
		}

		else if (ImGui::IsKeyReleased(_key))
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					if (_alias.empty())
						mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, ToPyInt(_key), _user_data);
					else
						mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _alias, ToPyInt(_key), _user_data);
				});
		}
	}

	void mvKeyReleaseHandler::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(mvApp::GetApp()->getParsers()[s_command], dict))
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

	void mvKeyReleaseHandler::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "key")) _key = ToInt(item);
	}

	void mvKeyReleaseHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "key", mvPyObject(ToPyInt(_key)));
	}

}