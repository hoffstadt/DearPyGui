#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDatePicker, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Time, 1);
	class mvDatePicker : public mvTimePtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDatePicker, add_date_picker)

		MV_CREATE_CONSTANT(mvThemeCol_DatePicker_Date, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DatePicker_DateBg, ImGuiCol_Button, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DatePicker_DateDisabled, ImGuiCol_TextDisabled, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DatePicker_DateHovered, ImGuiCol_ButtonHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DatePicker_DateActive, ImGuiCol_ButtonActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DatePicker_Border, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DatePicker_BorderShadow, ImGuiCol_BorderShadow, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_DatePicker_Rounding, ImGuiStyleVar_FrameRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DatePicker_BorderSize, ImGuiStyleVar_FrameBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DatePicker_PaddingX, ImGuiStyleVar_FramePadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DatePicker_PaddingY, ImGuiStyleVar_FramePadding, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_Date, mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_DateBg, mvImGuiCol_FrameBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_DateDisabled, mvImGuiCol_TextDisabled),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_DateHovered, mvImGuiCol_ButtonHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_DateActive, mvImGuiCol_ButtonActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_Border, mvImGuiCol_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DatePicker_BorderShadow, mvImGuiCol_BorderShadow),
		MV_END_COLOR_CONSTANTS

			MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_DatePicker_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_DatePicker_BorderSize, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_DatePicker_PaddingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_DatePicker_PaddingY, 3, 20),
			MV_END_STYLE_CONSTANTS

	public:

		mvDatePicker(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		int m_level = 0;

	};

}