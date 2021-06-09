#include <utility>
#include "mvSelectable.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvSelectable::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Widgets" });
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
			MV_PARSER_ARG_USER_DATA |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Bool>("default_value", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("span_columns", mvArgType::KEYWORD_ARG, "False", "span all columns");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvSelectable::mvSelectable(mvUUID uuid)
		: mvBoolPtrBase(uuid)
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

		ScopedID id(m_uuid);
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImGui::Selectable(m_label.c_str(), m_value.get(), m_flags, ImVec2((float)m_width, (float)m_height)))
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_uuid, nullptr, m_user_data);

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