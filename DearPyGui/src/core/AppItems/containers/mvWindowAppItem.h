#pragma once

#include <utility>
#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvDrawList.h"

namespace Marvel {

	struct mvWindowAppItemConfig : public mvAppItemConfig
	{
		int xpos = 200;
		int ypos = 200;
		bool autosize = false;
		bool no_resize = false;
		bool no_title_bar = false;
		bool no_move = false;
		bool no_scrollbar = false;
		bool no_collapse = false;
		bool horizontal_scrollbar = false;
		bool no_focus_on_appearing = false;
		bool no_bring_to_front_on_focus = false;
		bool menubar = false;
		bool no_close = false;
		bool no_background = false;
		mvCallable on_close = nullptr;
		
		mvWindowAppItemConfig()
		{
			width = 200;
			height = 200;
		}

	};

#ifdef MV_CPP
	void add_window(const char* name, const mvWindowAppItemConfig& config = {});
#else
	PyObject* add_window(PyObject* self, PyObject* args, PyObject* kwargs);
#endif //

	class mvWindowAppItem : public mvAppItem
	{

		MV_APPITEM_TYPE(mvAppItemType::Window, mvWindowAppItem, "add_window")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_TitleText				,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_TitleBg				, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_TitleBgActive			, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_TitleBgCollapsed		, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_MenuBar				, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_Bg					,  2L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_Scrollbar				, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_ScrollbarGrab			, 15L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_ScrollbarGrabHovered	, 16L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_ScrollbarGrabActive	, 17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_ResizeBorder			, 29L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_ResizeGrip			, 30L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_ResizeGripHovered		, 31L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_ResizeGripActive		, 32L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_Border				,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeCol_Window_BorderShadow			,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeStyle_Window_BorderSize			,  3L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeStyle_Window_Rounding			,  2L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeStyle_Window_MinSizeX			,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeStyle_Window_MinSizeY			,  4L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeStyle_Window_TitleAlignX			,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeStyle_Window_TitleAlignY			,  5L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeStyle_Window_PaddingX			,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeStyle_Window_PaddingY			,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeStyle_Window_ItemSpacingX		, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeStyle_Window_ItemSpacingY		, 13L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeStyle_Window_ScrollbarSize		, 17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Window, mvThemeStyle_Window_ScrollbarRounding	, 18L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_TitleText),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_TitleBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_TitleBgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_TitleBgCollapsed),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_MenuBar),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_Scrollbar),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ScrollbarGrabActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ResizeBorder),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ResizeGrip),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ResizeGripHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_ResizeGripActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Window_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Window_BorderSize			, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Window_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Window_MinSizeX			, 0, 50),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Window_MinSizeY			, 0, 50),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Window_TitleAlignX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Window_TitleAlignY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Window_PaddingX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Window_PaddingY			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Window_ItemSpacingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Window_ItemSpacingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Window_ScrollbarSize		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Window_ScrollbarRounding	, 0, 12),
		MV_END_STYLE_CONSTANTS

		enum class Status{ Normal, Transition, Dirty};

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvWindowAppItem(const std::string& name, bool mainWindow, mvCallable closing_callback);
		mvWindowAppItem(const std::string& name, const mvWindowAppItemConfig& config);

		void   addMenuBar           () { m_hasMenuBar = true; }
		void   addFlag              (ImGuiWindowFlags flag) { m_windowflags |= flag; }
		void   removeFlag           (ImGuiWindowFlags flag) { m_windowflags &= ~flag; }
		void   setWindowAsMainStatus(bool value);
		void   setWindowPos         (float x, float y);
		void   setLabel             (const std::string& value) override;
		void   setWidth             (int width) override;
		void   setHeight            (int height) override;
		mvVec2 getWindowPos         () const;
		void   draw                 () override;
		void   setResizeCallback    (mvCallable callback);
		bool  getWindowAsMainStatus() const { return m_mainWindow; }

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

		void   setFocusedNextFrame  () { m_focusNextFrame = true; }
		mvRef<mvDrawList> getDrawList     () { return m_drawList; }

		~mvWindowAppItem();

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		ImGuiWindowFlags      m_windowflags = ImGuiWindowFlags_NoSavedSettings;
		ImGuiWindowFlags      m_oldWindowflags = ImGuiWindowFlags_NoSavedSettings;
		mvWindowAppItemConfig m_config;
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
		mvRef<mvDrawList>     m_drawList;
		
	};

}