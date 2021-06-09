#include "mvKeyReleaseHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvKeyReleaseHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Textures", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_USER_DATA |
			MV_PARSER_ARG_PARENT)
		);

		parser.addArg<mvPyDataType::Integer>("key", mvArgType::POSITIONAL_ARG, "-1");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvKeyReleaseHandler::mvKeyReleaseHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	bool mvKeyReleaseHandler::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvHandlerRegistry) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvKeyReleaseHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		if (m_key == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
			{
				if (ImGui::GetIO().KeysDownDurationPrev[i] >= 0.0f && !ImGui::GetIO().KeysDown[i])
				{
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
						{
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_uuid, ToPyInt(i), m_user_data);
						});
				}
			}
		}

		else if (ImGui::IsKeyReleased(m_key))
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_uuid, ToPyInt(m_key), m_user_data);
				});
		}
	}

	void mvKeyReleaseHandler::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_key = ToInt(item);
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

		if (PyObject* item = PyDict_GetItemString(dict, "key")) m_key = ToInt(item);
	}

	void mvKeyReleaseHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "key", ToPyInt(m_key));
	}

}