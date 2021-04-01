#pragma once

#include <utility>
#include "mvTypeBases.h"
#include "mvApp.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvWindowAppItem);
	class mvWindowAppItem : public mvAppItem
	{

		MV_APPITEM_TYPE(mvAppItemType::mvWindowAppItem, add_window)

		MV_CREATE_CONSTANT(mvThemeCol_Window_TitleText			,  0L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_TitleBg				, 10L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_TitleBgActive		, 11L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_TitleBgCollapsed		, 12L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_MenuBar				, 13L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_Bg					,  2L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_Scrollbar			, 14L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ScrollbarGrab		, 15L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ScrollbarGrabHovered	, 16L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ScrollbarGrabActive	, 17L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ResizeBorder			, 29L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ResizeGrip			, 30L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ResizeGripHovered	, 31L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_ResizeGripActive		, 32L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_Border				,  5L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Window_BorderShadow			,  6L, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Window_BorderSize			,  3L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_Rounding			,  2L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_MinSizeX			,  4L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_MinSizeY			,  4L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_TitleAlignX		,  5L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_TitleAlignY		,  5L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_PaddingX			,  1L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_PaddingY			,  1L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_ItemSpacingX		, 13L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_ItemSpacingY		, 13L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_ScrollbarSize		, 17L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Window_ScrollbarRounding	, 18L, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_TitleText,			mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_TitleBg,				mvColor( 10,  10,  10, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_TitleBgActive,		mvColor( 41,  74, 122, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_TitleBgCollapsed,		mvColor(  0,   0,   0, 130)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_MenuBar,				mvColor( 36,  36,  36, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_Bg,					mvColor( 15,  15,  15, 240)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_Scrollbar,			mvColor(  5,   5,   5, 135)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ScrollbarGrab,		mvColor( 79,  79,  79, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ScrollbarGrabHovered, mvColor(105, 105, 105, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ScrollbarGrabActive,	mvColor(130, 130, 130, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ResizeBorder,			mvColor( 26, 102, 191, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ResizeGrip,			mvColor( 66, 150, 250,  42)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ResizeGripHovered,	mvColor( 66, 150, 250, 171)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ResizeGripActive,		mvColor( 66, 150, 250, 242)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_Border,				mvColor(110, 110, 128, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_BorderShadow,			mvColor(  0,   0,   0,   0)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_Window_BorderSize			, 1,    1),
			MV_ADD_CONSTANT(mvThemeStyle_Window_Rounding			, 0,   12),
			MV_ADD_CONSTANT(mvThemeStyle_Window_MinSizeX			,32,   50),
			MV_ADD_CONSTANT(mvThemeStyle_Window_MinSizeY			,32,   50),
			MV_ADD_CONSTANT(mvThemeStyle_Window_TitleAlignX		, 0,    1),
			MV_ADD_CONSTANT(mvThemeStyle_Window_TitleAlignY		, 0.5,  1),
			MV_ADD_CONSTANT(mvThemeStyle_Window_PaddingX			, 8,   20),
			MV_ADD_CONSTANT(mvThemeStyle_Window_PaddingY			, 8,   20),
			MV_ADD_CONSTANT(mvThemeStyle_Window_ItemSpacingX		, 8,   20),
			MV_ADD_CONSTANT(mvThemeStyle_Window_ItemSpacingY		, 4,   20),
			MV_ADD_CONSTANT(mvThemeStyle_Window_ScrollbarSize		,14,   20),
			MV_ADD_CONSTANT(mvThemeStyle_Window_ScrollbarRounding	, 9,   12),
		MV_END_STYLE_CONSTANTS

		enum class Status{ Normal, Transition, Dirty};

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvWindowAppItem(const std::string& name, bool mainWindow, mvCallable closing_callback);

		void   addMenuBar           () { m_hasMenuBar = true; }
		void   addFlag              (ImGuiWindowFlags flag) { m_windowflags |= flag; }
		void   removeFlag           (ImGuiWindowFlags flag) { m_windowflags &= ~flag; }
		void   setWindowAsMainStatus(bool value);
		void   setWindowPos         (float x, float y);
		void   setLabel             (const std::string& value) override;
		void   setWidth             (int width) override;
		void   setHeight            (int height) override;
		mvVec2 getWindowPos         () const;
		void   draw                 (ImDrawList* drawlist, float x, float y) override;
		void   setResizeCallback    (mvCallable callback);
		bool  getWindowAsMainStatus() const { return m_mainWindow; }

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

		void   setFocusedNextFrame  () { m_focusNextFrame = true; }

		~mvWindowAppItem();

	private:

		ImGuiWindowFlags      m_windowflags = ImGuiWindowFlags_NoSavedSettings;
		ImGuiWindowFlags      m_oldWindowflags = ImGuiWindowFlags_NoSavedSettings;
		int                   m_oldxpos = 200;
		int                   m_oldypos = 200;
		int                   m_oldWidth = 200;
		int                   m_oldHeight = 200;
		bool                  m_mainWindow = false;
		mvCallable            m_resize_callback = nullptr;
		bool                  m_dirty_pos = true;
		bool                  m_dirty_size = true;
		bool                  m_hasMenuBar = false;
		bool                  m_focusNextFrame = false;
		bool                  m_closing = true;
		bool                  m_collapsedDirty = true;

		int        m_xpos = 200;
		int        m_ypos = 200;
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
		mvCallable m_on_close = nullptr;
		mvVec2     m_min_size = { 32.0f, 32.0f };
		mvVec2     m_max_size = { 30000.0f, 30000.0f };
		
	};

}