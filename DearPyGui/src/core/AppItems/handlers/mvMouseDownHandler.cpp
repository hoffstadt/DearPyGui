#include "mvMouseDownHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvMouseDownHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::Integer, "button", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all mouse buttons" });
		args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });
		
		mvPythonParserSetup setup;
		setup.about = "Adds a mouse down handler.";
		setup.category = { "Events", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		
		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvMouseDownHandler::mvMouseDownHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvMouseDownHandler::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvMouseDownHandler*>(item);
		_button = titem->_button;
	}

	void mvMouseDownHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_button == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
			{
				if (ImGui::GetIO().MouseDown[i])
				{
					mvSubmitCallback([=]()
						{
							if (_alias.empty())
								mvRunCallback(getCallback(false), _uuid, ToPyMPair(i, ImGui::GetIO().MouseDownDuration[i]), _user_data);
							else
								mvRunCallback(getCallback(false), _alias, ToPyMPair(i, ImGui::GetIO().MouseDownDuration[i]), _user_data);
						});
				}
			}
		}

		else if (ImGui::GetIO().MouseDown[_button])
		{
			mvSubmitCallback([=]()
				{
					if (_alias.empty())
						mvRunCallback(getCallback(false), _uuid, ToPyMPair(_button, ImGui::GetIO().MouseDownDuration[_button]), _user_data);
					else
						mvRunCallback(getCallback(false), _alias, ToPyMPair(_button, ImGui::GetIO().MouseDownDuration[_button]), _user_data);
				});
		}
	}

	void mvMouseDownHandler::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(GetParsers()[s_command], dict))
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

	void mvMouseDownHandler::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "button")) _button = ToInt(item);
	}

	void mvMouseDownHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "button", mvPyObject(ToPyInt(_button)));
	}

}