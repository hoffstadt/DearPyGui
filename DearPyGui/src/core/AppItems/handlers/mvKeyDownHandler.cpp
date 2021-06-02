#include "mvKeyDownHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvKeyDownHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Textures", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_PARENT)
		);

		parser.addArg<mvPyDataType::Integer>("key");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvKeyDownHandler::mvKeyDownHandler(const std::string& name)
		:
		mvAppItem(name)
	{

	}

	bool mvKeyDownHandler::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvHandlerRegistry) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvKeyDownHandler::draw(ImDrawList* drawlist, float x, float y)
	{

		if (m_key == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
			{
				if (ImGui::GetIO().KeysDown[i])
				{
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
						{
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_name, ToPyMPair(i, ImGui::GetIO().KeysDownDuration[i]));
						});
				}
			}
		}

		else if (ImGui::GetIO().KeysDown[m_key])
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_name, ToPyFloat((ImGui::GetIO().KeysDownDuration[m_key])));
				});
		}
	}

	void mvKeyDownHandler::handleSpecificRequiredArgs(PyObject* dict)
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

	void mvKeyDownHandler::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "key")) m_key = ToInt(item);
	}

	void mvKeyDownHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}