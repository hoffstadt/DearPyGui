#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvGroup, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvGroup : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvGroup, add_group)

		MV_CREATE_CONSTANT(mvThemeStyle_Group_ItemSpacingX, ImGuiStyleVar_ItemSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Group_ItemSpacingY, ImGuiStyleVar_ItemSpacing, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_Group_ItemSpacingX, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Group_ItemSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvGroup(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y)               override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		bool  m_horizontal = false;
		float m_hspacing = -1.0f;

	};

}