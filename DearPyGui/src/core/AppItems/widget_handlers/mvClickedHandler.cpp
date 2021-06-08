#include "mvClickedHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvClickedHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Textures", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_USER_DATA |
			MV_PARSER_ARG_CALLBACK)
		);

		parser.addArg<mvPyDataType::String>("parent");
		parser.addArg<mvPyDataType::Integer>("button");

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

		//mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
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
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_name, ToPyInt(i), m_user_data);
						});
				}
			}
		}

		else if(ImGui::IsItemClicked(m_button))
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_name, ToPyInt(m_button), m_user_data);
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
				m_parent = ToString(item);
				break;
			
			case 1:
				m_button = ToInt(item);
				break;

			default:
				break;
			}
		}
	}
}