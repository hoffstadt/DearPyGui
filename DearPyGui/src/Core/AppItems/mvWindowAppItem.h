#pragma once

#include <utility>

#include "Core/AppItems/mvTypeBases.h"
#include "mvApp.h"
#include "mvEventHandler.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvWindowAppitem
	//-----------------------------------------------------------------------------
	class mvWindowAppitem : public mvAppItem, public mvEventHandler
	{

		enum class Status{ Normal, Transition, Dirty};

	public:

		MV_APPITEM_TYPE(mvAppItemType::Window, "add_window")

		mvWindowAppitem(const std::string& name, bool mainWindow, PyObject* closing_callback);

		bool   isARoot              () const override       { return true; }
		void   addFlag              (ImGuiWindowFlags flag) { m_windowflags |= flag; }
		void   removeFlag           (ImGuiWindowFlags flag) { m_windowflags &= ~flag; }
		void   setWindowAsMainStatus(bool value);
		void   setWindowPos         (float x, float y);
		void   setWidth             (int width) override;
		void   setHeight            (int height) override;
		mvVec2 getWindowPos         () const;
		void   draw                 () override;
		void   setExtraConfigDict   (PyObject* dict) override;
		void   getExtraConfigDict   (PyObject* dict) override;

		~mvWindowAppitem();

	private:

		static Status s_status;

		ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoSavedSettings;
		int              m_xpos = 200;
		int              m_ypos = 200;
		bool             m_mainWindow = false;
		PyObject*        m_closing_callback = nullptr;
		bool             m_dirty_pos = true;
		bool             m_dirty_size = true;
		bool             m_closing = true;
		bool             m_noclose = false;
		

	};

}