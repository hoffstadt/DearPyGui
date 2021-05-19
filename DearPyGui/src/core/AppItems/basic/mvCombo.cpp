#include <utility>
#include "mvCombo.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvCombo::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
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

		parser.addArg<mvPyDataType::StringList>("items", mvArgType::POSITIONAL_ARG, "()");

		parser.addArg<mvPyDataType::String>("default_value", mvArgType::KEYWORD_ARG, "''");
		parser.addArg<mvPyDataType::Bool>("popup_align_left", mvArgType::KEYWORD_ARG, "False", "Align the popup toward the left by default");
		parser.addArg<mvPyDataType::Bool>("height_small", mvArgType::KEYWORD_ARG, "False", "Max ~4 items visible");
		parser.addArg<mvPyDataType::Bool>("height_regular", mvArgType::KEYWORD_ARG, "False", "Max ~8 items visible");
		parser.addArg<mvPyDataType::Bool>("height_large", mvArgType::KEYWORD_ARG, "False", "Max ~20 items visible");
		parser.addArg<mvPyDataType::Bool>("height_largest", mvArgType::KEYWORD_ARG, "False", "As many items visible as possible");
		parser.addArg<mvPyDataType::Bool>("no_arrow_button", mvArgType::KEYWORD_ARG, "False", "Display on the preview box without the square arrow button");
		parser.addArg<mvPyDataType::Bool>("no_preview", mvArgType::KEYWORD_ARG, "False", "Display only a square arrow button");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvCombo::mvCombo(const std::string& name)
		: 
		mvStringPtrBase(name)
	{
	}

	void mvCombo::draw(ImDrawList* drawlist, float x, float y)
	{

		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		static std::vector<std::string> disabled_items{};

		// The second parameter is the label previewed before opening the combo.
		if (ImGui::BeginCombo(m_label.c_str(), m_value->c_str(), m_flags)) 
		{
			for (const auto& name : m_enabled ? m_items : disabled_items)
			{
				bool is_selected = (*m_value == name);
				if (ImGui::Selectable((name).c_str(), is_selected))
				{
					if (m_enabled) { *m_value = name; }
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callback_data);

				}

				// Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				if (is_selected)
					ImGui::SetItemDefaultFocus();   
			}

			ImGui::EndCombo();
		}

	}

	void mvCombo::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyPositionalArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_items = ToStringVect(item);
				break;

			default:
				break;
			}
		}
	}

	void mvCombo::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "items")) m_items = ToStringVect(item);

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		auto conflictingflagop = [dict](const char* const keywords[4], const int flags[4], int& mflags)
		{

			for (size_t i = 0; i < 4; i++)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keywords[i]))
				{
					//turning all conflicting flags false
					for (int i = 0; i < 4; i++) mflags &= ~flags[i];
					//writing only the first conflicting flag
					ToBool(item) ? mflags |= flags[i] : mflags &= ~flags[i];
					break;
				}
			}

		};

		flagop("popup_align_left", ImGuiComboFlags_PopupAlignLeft, m_flags);
		flagop("no_arrow_button", ImGuiComboFlags_NoArrowButton, m_flags);
		flagop("no_preview", ImGuiComboFlags_NoPreview, m_flags);

		constexpr static const char* HeightKeywords[4]{
			"height_small",
			"height_regular",
			"height_large",
			"height_largest" };

		constexpr static const int HeightFlags[4]{
			ImGuiComboFlags_HeightSmall,
			ImGuiComboFlags_HeightRegular,
			ImGuiComboFlags_HeightLarge,
			ImGuiComboFlags_HeightLargest };

		conflictingflagop(HeightKeywords, HeightFlags, m_flags);

	}

	void mvCombo::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "items", ToPyList(m_items));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};
		checkbitset("popup_align_left", ImGuiComboFlags_PopupAlignLeft, m_flags);
		checkbitset("height_small", ImGuiComboFlags_HeightSmall, m_flags);
		checkbitset("height_regular", ImGuiComboFlags_HeightRegular, m_flags);
		checkbitset("height_large", ImGuiComboFlags_HeightLarge, m_flags);
		checkbitset("height_largest", ImGuiComboFlags_HeightLargest, m_flags);
		checkbitset("no_arrow_button", ImGuiComboFlags_NoArrowButton, m_flags);
		checkbitset("no_preview", ImGuiComboFlags_NoPreview, m_flags);
	}

}