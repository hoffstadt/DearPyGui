#include <utility>
#include "mvSelectable.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvSelectable::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_LABEL |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_CALLBACK_DATA |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED)
		);

		parser.addArg<mvPyDataType::Bool>("default_value", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("span_columns", mvArgType::KEYWORD_ARG, "False", "span all columns");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvSelectable::mvSelectable(const std::string& name)
		: mvBoolPtrBase(name)
	{
	}

	void mvSelectable::setEnabled(bool value)
	{
		if (value == m_enabled)
			return;

		if (value)
			m_flags &= ~ImGuiSelectableFlags_Disabled;

		else
			m_flags |= ImGuiSelectableFlags_Disabled;

		m_enabled = value;
	}

	void mvSelectable::draw(ImDrawList* drawlist, float x, float y)
	{

		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImGui::Selectable(m_label.c_str(), m_value.get(), m_flags, ImVec2((float)m_width, (float)m_height)))
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callback_data);

	}

	void mvSelectable::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags, bool flip)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("span_columns", ImGuiSelectableFlags_SpanAllColumns, m_flags, false);

	}

	void mvSelectable::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags, bool flip)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("span_columns", ImGuiSelectableFlags_SpanAllColumns, m_flags, false);
	}

}