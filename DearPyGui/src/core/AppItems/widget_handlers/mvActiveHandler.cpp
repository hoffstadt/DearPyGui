#include "mvActiveHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvActiveHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Textures", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_USER_DATA |
			MV_PARSER_ARG_CALLBACK)
		);

		parser.addArg<mvPyDataType::String>("parent");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvActiveHandler::mvActiveHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	bool mvActiveHandler::isParentCompatible(mvAppItemType type)
	{
		return true;

		//mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		//MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		//assert(false);
		//return false;
	}

	void mvActiveHandler::draw(ImDrawList* drawlist, float x, float y)
	{

		if (ImGui::IsItemActive())
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_uuid, GetPyNone(), m_user_data);
				});
		}
	}

	void mvActiveHandler::handleSpecificRequiredArgs(PyObject* dict)
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

			default:
				break;
			}
		}
	}
}