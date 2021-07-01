#include "mvSpacing.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvSpacing::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds vertical spacing.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Integer>("count", mvArgType::KEYWORD_ARG, "1", "Number of spacings to add the size is dependant on the curret style.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}


	mvSpacing::mvSpacing(mvUUID uuid)
		: mvIntPtrBase(uuid)
	{
	}

	void mvSpacing::draw(ImDrawList* drawlist, float x, float y)
	{
		for (int i = 0; i < *m_value; i++)
			ImGui::Spacing();
	}

	void mvSpacing::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "count"))
		{
			*m_value = ToInt(item);
		}


	}

	void mvSpacing::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "count", ToPyBool(*m_value));
	}

}