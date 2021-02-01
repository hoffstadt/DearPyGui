#pragma once

#include <utility>
#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvDrawList.h"

namespace Marvel {

	PyObject* add_window(PyObject* self, PyObject* args, PyObject* kwargs);
	PyContextManager addc_window(const char* name, const PyObject& kwargs);

	class mvWindowAppItem : public mvAppItem
	{

		enum class Status{ Normal, Transition, Dirty};

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::Window, "add_window")

		mvWindowAppItem(const std::string& name, bool mainWindow, PyObject* closing_callback);

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

	private:

		ImGuiWindowFlags  m_windowflags = ImGuiWindowFlags_NoSavedSettings;
		ImGuiWindowFlags  m_oldWindowflags = ImGuiWindowFlags_NoSavedSettings;
		int               m_xpos = 200;
		int               m_oldxpos = 200;
		int               m_ypos = 200;
		int               m_oldypos = 200;
		int               m_oldWidth = 200;
		int               m_oldHeight = 200;
		bool              m_mainWindow = false;
		PyObject*         m_closing_callback = nullptr;
		PyObject*         m_resize_callback = nullptr;
		bool              m_dirty_pos = true;
		bool              m_dirty_size = true;
		bool              m_closing = true;
		bool              m_noclose = false;
		bool              m_hasMenuBar = false;
		bool              m_focusNextFrame = false;
		mvRef<mvDrawList> m_drawList;
		
	};

}