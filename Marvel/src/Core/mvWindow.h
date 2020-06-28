#pragma once

#include "mvApp.h"
#include "mvAppEditor.h"
#include "mvAppLog.h"

namespace Marvel {

	class mvWindow
	{

	public:

		mvWindow(unsigned width, unsigned height, bool editor = false, bool error = false) : 
			m_width(width), m_height(height), m_editor(editor), m_error(error)
		{
			m_app = mvApp::GetApp();
			m_appEditor = mvAppEditor::GetAppEditor();
		}

		virtual void show      () = 0;
		virtual void setup     () {}
		virtual void prerender () {}
		virtual void postrender() {}
		virtual void cleanup   () {}

		void run();

	protected:

		bool           m_running = true;
		bool           m_editor = false;
		bool           m_error = false;
		mvApp*         m_app = nullptr;
		mvAppEditor*   m_appEditor = nullptr;
		unsigned       m_width;
		unsigned       m_height;

	};

}
