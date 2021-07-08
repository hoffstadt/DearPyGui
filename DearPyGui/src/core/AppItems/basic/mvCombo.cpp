#include <utility>
#include "mvCombo.h"
#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvCombo::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a combo dropdown that allows a user to select a single option from a drop down window.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::StringList>("items", mvArgType::POSITIONAL_ARG, "()", "A tuple of items to be shown in the drop down window. Can consist of any combination of types.");

		parser.addArg<mvPyDataType::String>("default_value", mvArgType::KEYWORD_ARG, "''");
		parser.addArg<mvPyDataType::Bool>("popup_align_left", mvArgType::KEYWORD_ARG, "False", "Align the popup toward the left.");
		parser.addArg<mvPyDataType::Bool>("no_arrow_button", mvArgType::KEYWORD_ARG, "False", "Display the preview box without the square arrow button.");
		parser.addArg<mvPyDataType::Bool>("no_preview", mvArgType::KEYWORD_ARG, "False", "Display only the square arrow button.");

		parser.addArg<mvPyDataType::Long>("height_mode", mvArgType::KEYWORD_ARG, "1", "mvComboHeight_Small, _Regular, _Large, _Largest");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvCombo::mvCombo(mvUUID uuid)
		: 
		mvStringPtrBase(uuid)
	{
	}

	void mvCombo::draw(ImDrawList* drawlist, float x, float y)
	{

		ScopedID id(m_uuid);

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
	
					auto value = *m_value;
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
						mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, ToPyString(value), m_user_data);
						});


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

		if (PyObject* item = PyDict_GetItemString(dict, "height_mode"))
		{
			long height_mode = ToUUID(item);

			if (height_mode == (long)mvCombo::ComboHeightMode::mvComboHeight_Small)
				m_flags = ImGuiComboFlags_HeightSmall;
			else if (height_mode == (long)mvCombo::ComboHeightMode::mvComboHeight_Regular)
				m_flags = ImGuiComboFlags_HeightRegular;
			else if (height_mode == (long)mvCombo::ComboHeightMode::mvComboHeight_Large)
				m_flags = ImGuiComboFlags_HeightLarge;
			else
				m_flags = ImGuiComboFlags_HeightLargest;
		}

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		flagop("popup_align_left", ImGuiComboFlags_PopupAlignLeft, m_flags);
		flagop("no_arrow_button", ImGuiComboFlags_NoArrowButton, m_flags);
		flagop("no_preview", ImGuiComboFlags_NoPreview, m_flags);

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
		checkbitset("no_arrow_button", ImGuiComboFlags_NoArrowButton, m_flags);
		checkbitset("no_preview", ImGuiComboFlags_NoPreview, m_flags);

		if (m_flags & ImGuiComboFlags_HeightSmall)
			PyDict_SetItemString(dict, "height_mode", ToPyUUID((long)mvCombo::ComboHeightMode::mvComboHeight_Small));
		else if (m_flags & ImGuiComboFlags_HeightRegular)
			PyDict_SetItemString(dict, "height_mode", ToPyUUID((long)mvCombo::ComboHeightMode::mvComboHeight_Regular));
		else if (m_flags & ImGuiComboFlags_HeightLarge)
			PyDict_SetItemString(dict, "height_mode", ToPyUUID((long)mvCombo::ComboHeightMode::mvComboHeight_Large));
		else
			PyDict_SetItemString(dict, "height_mode", ToPyUUID((long)mvCombo::ComboHeightMode::mvComboHeight_Largest));

	}

}