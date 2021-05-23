#include <utility>
#include "mvProgressBar.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvProgressBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_LABEL |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::String>("overlay", mvArgType::KEYWORD_ARG, "''", "overlayed text");
		parser.addArg<mvPyDataType::Float>("default_value", mvArgType::KEYWORD_ARG, "0.0", "value from 0 to 1");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvProgressBar::mvProgressBar(const std::string& name)
		: mvFloatPtrBase(name)
	{
	}

	void mvProgressBar::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		ImGui::ProgressBar(*m_value, ImVec2((float)m_width, (float)m_height), m_overlay.c_str());

	}

	void mvProgressBar::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "overlay")) m_overlay = ToString(item);
	}

	void mvProgressBar::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "overlay", ToPyString(m_overlay));
	}

}