#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCombo, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvCombo : public mvStringPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCombo, add_combo)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvCombo(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:
		ImGuiComboFlags m_flags = ImGuiComboFlags_None;

		std::vector<std::string> m_items;
		std::string              m_default_value = "";
		bool                     m_popup_align_left = false;
		bool                     m_height_small = false;
		bool                     m_height_regular = false;
		bool                     m_height_large = false;
		bool                     m_height_largest = false;
		bool                     m_no_arrow_button = false;
		bool                     m_no_preview = false;
	};

}
