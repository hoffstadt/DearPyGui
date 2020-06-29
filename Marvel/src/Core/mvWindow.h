#pragma once

#include "mvApp.h"
#include "mvAppEditor.h"
#include "mvAppLog.h"
#include "StandardWindows/mvDocWindow.h"

namespace Marvel {

	class mvWindow
	{

	public:

		mvWindow(unsigned width, unsigned height, bool editor = false, bool error = false, bool doc = false) : 
			m_width(width), m_height(height), m_editor(editor), m_error(error), m_doc(doc)
		{
			m_app = mvApp::GetApp();
			m_appEditor = mvAppEditor::GetAppEditor();
			m_documentation = mvDocWindow::GetWindow();

			if (m_doc)
			{
				m_documentation->setToMainMode();
				m_documentation->setSize(width, height);
			}
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
		bool           m_doc = false;
		mvApp*         m_app = nullptr;
		mvAppEditor*   m_appEditor = nullptr;
		mvDocWindow*   m_documentation = nullptr;
		unsigned       m_width;
		unsigned       m_height;

	};

}
