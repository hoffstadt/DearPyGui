#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCombo, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvCombo : public mvStringPtrBase
	{

		enum class ComboHeightMode {
			mvComboHeight_Small = 0L,
			mvComboHeight_Regular,
			mvComboHeight_Large,
			mvComboHeight_Largest
		};

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCombo, add_combo)

		MV_CREATE_CONSTANT(mvComboHeight_Small, 0L);
		MV_CREATE_CONSTANT(mvComboHeight_Regular, 1L);
		MV_CREATE_CONSTANT(mvComboHeight_Large, 2L);
		MV_CREATE_CONSTANT(mvComboHeight_Largest, 3L);

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvComboHeight_Small),
			MV_ADD_CONSTANT(mvComboHeight_Regular),
			MV_ADD_CONSTANT(mvComboHeight_Large),
			MV_ADD_CONSTANT(mvComboHeight_Largest)
		MV_END_CONSTANTS

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
		bool                     m_no_arrow_button = false;
		bool                     m_no_preview = false;
	};

}
