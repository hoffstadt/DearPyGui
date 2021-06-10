#include "mvResizeHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "containers/mvWindowAppItem.h"

namespace Marvel {

	void mvResizeHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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

	mvResizeHandler::mvResizeHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	bool mvResizeHandler::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvWindowAppItem) return true;
		MV_ITEM_REGISTRY_ERROR("mvResizeHandler item parent must be a Window.");
		assert(false);
		return false;
	}

	void mvResizeHandler::draw(ImDrawList* drawlist, float x, float y)
	{

		if (m_parentPtr)
		{
			auto parentPtr = static_cast<mvWindowAppItem*>(m_parentPtr);
			if (parentPtr->isResized())
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
					{
						mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_uuid, GetPyNone(), m_user_data);
					});
			}
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
				m_parent = ToUUID(item);
				break;

			default:
				break;
			}
		}
	}
}