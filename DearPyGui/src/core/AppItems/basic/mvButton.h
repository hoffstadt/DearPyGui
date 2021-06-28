#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvButton, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvButton : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvButton, add_button)

		MV_CREATE_CONSTANT(mvTool_About, MV_TOOL_ABOUT_UUID);
		MV_CREATE_CONSTANT(mvTool_Debug, MV_TOOL_DEBUG_UUID);
		MV_CREATE_CONSTANT(mvTool_Doc, MV_TOOL_DOC_UUID);
		MV_CREATE_CONSTANT(mvTool_ItemRegistry, MV_TOOL_ITEM_REGISTRY_UUID);
		MV_CREATE_CONSTANT(mvTool_Metrics, MV_TOOL_METRICS_UUID);
		MV_CREATE_CONSTANT(mvTool_Style, MV_TOOL_STYLE_UUID);
		MV_CREATE_CONSTANT(mvTool_Font, MV_TOOL_FONT_UUID);

		MV_CREATE_CONSTANT(mvFontAtlas, MV_ATLAS_UUID);
		MV_CREATE_CONSTANT(mvAppUUID, MV_APP_UUID);
		MV_CREATE_CONSTANT(mvInvalidUUID, MV_INVALID_UUID);

		MV_CREATE_CONSTANT(mvDir_None, ImGuiDir_None);
		MV_CREATE_CONSTANT(mvDir_Left, ImGuiDir_Left);
		MV_CREATE_CONSTANT(mvDir_Right, ImGuiDir_Right);
		MV_CREATE_CONSTANT(mvDir_Up, ImGuiDir_Up);
		MV_CREATE_CONSTANT(mvDir_Down, ImGuiDir_Down);

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvTool_About),
			MV_ADD_CONSTANT(mvTool_Debug),
			MV_ADD_CONSTANT(mvTool_Doc),
			MV_ADD_CONSTANT(mvTool_ItemRegistry),
			MV_ADD_CONSTANT(mvTool_Metrics),
			MV_ADD_CONSTANT(mvTool_Style),
			MV_ADD_CONSTANT(mvTool_Font),
			MV_ADD_CONSTANT(mvFontAtlas),
			MV_ADD_CONSTANT(mvAppUUID),
			MV_ADD_CONSTANT(mvInvalidUUID),
			MV_ADD_CONSTANT(mvDir_None),
			MV_ADD_CONSTANT(mvDir_Left),
			MV_ADD_CONSTANT(mvDir_Right),
			MV_ADD_CONSTANT(mvDir_Up),
			MV_ADD_CONSTANT(mvDir_Down),
		MV_END_CONSTANTS

	public:

		mvButton(mvUUID uuid);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override{}

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiDir m_direction = ImGuiDir_Up;
		bool     m_small_button = false;
		bool     m_arrow = false;

	};

}
