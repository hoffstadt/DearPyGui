#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvChild, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvChild : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvChild, add_child)

		MV_CREATE_CONSTANT(mvThemeCol_Child_Bg, ImGuiCol_ChildBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Child_MenuBar, ImGuiCol_MenuBarBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Child_Border, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Child_Scrollbar, ImGuiCol_ScrollbarBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Child_ScrollbarGrab, ImGuiCol_ScrollbarGrab, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Child_ScrollbarGrabHovered, ImGuiCol_ScrollbarGrabHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Child_ScrollbarGrabActive, ImGuiCol_ScrollbarGrabActive, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Child_BorderSize, ImGuiStyleVar_ChildBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Child_Rounding, ImGuiStyleVar_ChildRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Child_PaddingX, ImGuiStyleVar_WindowPadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Child_PaddingY, ImGuiStyleVar_WindowPadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Child_ItemSpacingX, ImGuiStyleVar_ItemSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Child_ItemSpacingY, ImGuiStyleVar_ItemSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Child_ScrollbarSize, ImGuiStyleVar_ScrollbarSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Child_ScrollbarRounding, ImGuiStyleVar_ScrollbarRounding, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_Bg, mvImGuiCol_ChildBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_MenuBar, mvImGuiCol_MenuBarBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_Border, mvImGuiCol_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_Scrollbar, mvImGuiCol_ScrollbarBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_ScrollbarGrab, mvImGuiCol_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_ScrollbarGrabHovered, mvImGuiCol_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Child_ScrollbarGrabActive, mvImGuiCol_ScrollbarGrabActive),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_Child_BorderSize			, 1,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Child_Rounding				, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Child_PaddingX				, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Child_PaddingY				, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Child_ItemSpacingX			, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Child_ItemSpacingY			, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Child_ScrollbarSize		,14, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Child_ScrollbarRounding	, 0, 12),
		MV_END_STYLE_CONSTANTS

	public:

		mvChild(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y)               override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

		void addFlag           (ImGuiWindowFlags flag);
		void removeFlag        (ImGuiWindowFlags flag);

		void setScrollX(float value) { m_scrollX = value; m_scrollXSet = true; }
		void setScrollY(float value) { m_scrollY = value; m_scrollYSet = true; }
		float getScrollX() const { return m_scrollX; }
		float getScrollY() const { return m_scrollY; }
		float getScrollXMax() const { return m_scrollMaxX; }
		float getScrollYMax() const { return m_scrollMaxY; }

	private:

		bool             m_border     = true;
		bool             m_autosize_x = false;
		bool             m_autosize_y = false;
		ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoSavedSettings;

		// scroll info
		float m_scrollX = 0.0f;
		float m_scrollY = 0.0f;
		float m_scrollMaxX = 0.0f;
		float m_scrollMaxY = 0.0f;
		bool  m_scrollXSet = false;
		bool  m_scrollYSet = false;

	};

}