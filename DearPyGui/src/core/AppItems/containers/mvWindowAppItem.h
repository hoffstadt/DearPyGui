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

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_TitleText				,  0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_TitleBg				, 10L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_TitleBgActive			, 11L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_TitleBgCollapsed		, 12L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_MenuBar				, 13L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_Bg					,  2L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_Scrollbar				, 14L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_ScrollbarGrab			, 15L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_ScrollbarGrabHovered	, 16L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_ScrollbarGrabActive	, 17L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_ResizeBorder			, 29L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_ResizeGrip			, 30L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_ResizeGripHovered		, 31L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_ResizeGripActive		, 32L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_Border				,  5L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Window_BorderShadow			,  6L);

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