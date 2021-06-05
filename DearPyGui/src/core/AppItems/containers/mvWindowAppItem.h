#pragma once

#include <utility>
#include "mvTypeBases.h"
#include "mvApp.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvWindowAppItem, MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_ROOT, StorageValueTypes::None, 1);
	class mvWindowAppItem : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvWindowAppItem, add_window)

		MV_CREATE_EXTRA_COMMAND(set_x_scroll);
		MV_CREATE_EXTRA_COMMAND(set_y_scroll);
		MV_CREATE_EXTRA_COMMAND(get_x_scroll);
		MV_CREATE_EXTRA_COMMAND(get_y_scroll);
		MV_CREATE_EXTRA_COMMAND(get_x_scroll_max);
		MV_CREATE_EXTRA_COMMAND(get_y_scroll_max);

		MV_CREATE_CONSTANT(mvThemeCol_Window_TitleText, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_TitleBg, ImGuiCol_TitleBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_TitleBgActive, ImGuiCol_TitleBgActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_TitleBgCollapsed, ImGuiCol_TitleBgCollapsed, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_MenuBar, ImGuiCol_MenuBarBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_Bg, ImGuiCol_WindowBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_Scrollbar, ImGuiCol_ScrollbarBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ScrollbarGrab, ImGuiCol_ScrollbarGrab, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ScrollbarGrabHovered, ImGuiCol_ScrollbarGrabHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ScrollbarGrabActive, ImGuiCol_ScrollbarGrabActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ResizeBorder, ImGuiCol_SeparatorActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ResizeGrip, ImGuiCol_ResizeGrip, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ResizeGripHovered, ImGuiCol_ResizeGripHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ResizeGripActive, ImGuiCol_ResizeGripActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_Border, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_BorderShadow, ImGuiCol_BorderShadow, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Window_BorderSize, ImGuiStyleVar_WindowBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_Rounding, ImGuiStyleVar_WindowRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_MinSizeX, ImGuiStyleVar_WindowMinSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_MinSizeY, ImGuiStyleVar_WindowMinSize, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_TitleAlignX, ImGuiStyleVar_WindowTitleAlign, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_TitleAlignY, ImGuiStyleVar_WindowTitleAlign, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_PaddingX, ImGuiStyleVar_WindowPadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_PaddingY, ImGuiStyleVar_WindowPadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_ItemSpacingX, ImGuiStyleVar_ItemSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_ItemSpacingY, ImGuiStyleVar_ItemSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_ScrollbarSize, ImGuiStyleVar_ScrollbarSize , 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_ScrollbarRounding, ImGuiStyleVar_ScrollbarRounding, 0L);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(set_x_scroll);
			MV_ADD_EXTRA_COMMAND(set_y_scroll);
			MV_ADD_EXTRA_COMMAND(get_x_scroll);
			MV_ADD_EXTRA_COMMAND(get_y_scroll);
			MV_ADD_EXTRA_COMMAND(get_x_scroll_max);
			MV_ADD_EXTRA_COMMAND(get_y_scroll_max)
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_TitleText, mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_TitleBg, mvImGuiCol_TitleBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_TitleBgActive, mvImGuiCol_TitleBgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_TitleBgCollapsed, mvImGuiCol_TitleBgCollapsed),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_MenuBar, mvImGuiCol_MenuBarBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_Bg, mvImGuiCol_WindowBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_Scrollbar, mvImGuiCol_ScrollbarBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ScrollbarGrab, mvImGuiCol_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ScrollbarGrabHovered, mvImGuiCol_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ScrollbarGrabActive, mvImGuiCol_ScrollbarGrabActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ResizeBorder, mvImGuiCol_SeparatorActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ResizeGrip, mvImGuiCol_ResizeGrip),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ResizeGripHovered, mvImGuiCol_ResizeGripHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ResizeGripActive, mvImGuiCol_ResizeGripActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_Border, mvImGuiCol_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_BorderShadow, mvImGuiCol_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_Window_BorderSize, 1, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Window_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Window_MinSizeX, 32, 50),
			MV_ADD_CONSTANT_F(mvThemeStyle_Window_MinSizeY, 32, 50),
			MV_ADD_CONSTANT_F(mvThemeStyle_Window_TitleAlignX, 0.5, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Window_TitleAlignY, 0.5, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Window_PaddingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Window_PaddingY, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Window_ItemSpacingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Window_ItemSpacingY, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Window_ScrollbarSize, 16, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Window_ScrollbarRounding, 0, 12),
		MV_END_STYLE_CONSTANTS

		enum class Status{ Normal, Transition, Dirty};

	public:

		mvWindowAppItem(const std::string& name, bool mainWindow = false);

		void   addMenuBar           () { m_hasMenuBar = true; }
		void   addFlag              (ImGuiWindowFlags flag) { m_windowflags |= flag; }
		void   removeFlag           (ImGuiWindowFlags flag) { m_windowflags &= ~flag; }
		void   setWindowAsMainStatus(bool value);
		void   setLabel             (const std::string& value) override;
		void   setWidth             (int width) override;
		void   setHeight            (int height) override;
		void   draw                 (ImDrawList* drawlist, float x, float y) override;
		void   setResizeCallback    (PyObject* callback);
		bool  getWindowAsMainStatus() const { return m_mainWindow; }

		void show() override;
		void onChildAdd(mvRef<mvAppItem> item) override;
		void onChildRemoved(mvRef<mvAppItem> item) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

		~mvWindowAppItem();

	private:

		ImGuiWindowFlags      m_windowflags = ImGuiWindowFlags_NoSavedSettings;
		ImGuiWindowFlags      m_oldWindowflags = ImGuiWindowFlags_NoSavedSettings;
		float                 m_oldxpos = 200;
		float                 m_oldypos = 200;
		int                   m_oldWidth = 200;
		int                   m_oldHeight = 200;
		bool                  m_mainWindow = false;
		PyObject*             m_resize_callback = nullptr;
		bool                  m_dirty_size = true;
		bool                  m_hasMenuBar = false;
		bool                  m_closing = true;
		bool                  m_collapsedDirty = true;
		
		bool                  m_modal = false;
		bool                  m_popup = false;
		bool                  m_popupInit = true;

		bool       m_autosize = false;
		bool       m_no_resize = false;
		bool       m_no_title_bar = false;
		bool       m_no_move = false;
		bool       m_no_scrollbar = false;
		bool       m_no_collapse = false;
		bool       m_horizontal_scrollbar = false;
		bool       m_no_focus_on_appearing = false;
		bool       m_no_bring_to_front_on_focus = false;
		bool       m_menubar = false;
		bool       m_no_close = false;
		bool       m_no_background = false;
		bool       m_collapsed = false;
		PyObject*  m_on_close = nullptr;
		mvVec2     m_min_size = { 32.0f, 32.0f };
		mvVec2     m_max_size = { 30000.0f, 30000.0f };

		// scroll info
		float m_scrollX = 0.0f;
		float m_scrollY = 0.0f;
		float m_scrollMaxX = 0.0f;
		float m_scrollMaxY = 0.0f;
		bool  m_scrollXSet = false;
		bool  m_scrollYSet = false;
		
	};

}