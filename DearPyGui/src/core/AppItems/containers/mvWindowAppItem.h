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
		PyObject* on_close = nullptr;
		
		mvWindowAppItemConfig()
		{
			width = 200;
			height = 200;
		}

	};

	PyObject* add_window(PyObject* self, PyObject* args, PyObject* kwargs);
	void mv_add_window(const char* name, const mvWindowAppItemConfig& config = {});

	class mvWindowAppItem : public mvAppItem
	{

		enum class Status{ Normal, Transition, Dirty};

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::Window, "add_window")

		mvWindowAppItem(const std::string& name, bool mainWindow, PyObject* closing_callback);
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
		void   setResizeCallback    (PyObject* callback);
		void   setExtraConfigDict   (PyObject* dict) override;
		void   getExtraConfigDict   (PyObject* dict) override;
		void   setFocusedNextFrame  () { m_focusNextFrame = true; }
		mvRef<mvDrawList> getDrawList     () { return m_drawList; }

		~mvWindowAppItem();

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvWindowAppItemConfig getConfig() const;

	private:

		ImGuiWindowFlags      m_windowflags = ImGuiWindowFlags_NoSavedSettings;
		ImGuiWindowFlags      m_oldWindowflags = ImGuiWindowFlags_NoSavedSettings;
		mvWindowAppItemConfig m_config;
		int                   m_oldxpos = 200;
		int                   m_oldypos = 200;
		int                   m_oldWidth = 200;
		int                   m_oldHeight = 200;
		bool                  m_mainWindow = false;
		PyObject*             m_resize_callback = nullptr;
		bool                  m_dirty_pos = true;
		bool                  m_dirty_size = true;
		bool                  m_hasMenuBar = false;
		bool                  m_focusNextFrame = false;
		bool                  m_closing = true;
		mvRef<mvDrawList>     m_drawList;
		
	};

}