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
			MV_PARSER_ARG_USER_DATA |
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

	bool mvClickedHandler::isParentCompatible(mvAppItemType type)
	{
		return true;

		//mvThrowPythonError(mvErrorCode::mvN1one, "Drawing item parent must be a drawing.");
		//MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		//assert(false);
		//return false;
	}

	void mvClickedHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		if (m_button == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
			{
				if (ImGui::IsItemClicked(i))
				{
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
						{
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_uuid, ToPyInt(i), m_user_data);
						});
				}
			}
		}

		else if(ImGui::IsItemClicked(m_button))
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_uuid, ToPyInt(m_button), m_user_data);
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
				m_parent = ToUUID(item);
				break;
			
			case 1:
				m_button = ToInt(item);
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

		if (PyObject* item = PyDict_GetItemString(dict, "button")) m_button = ToInt(item);
	}

	void mvClickedHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "button", ToPyInt(m_button));
	}
}