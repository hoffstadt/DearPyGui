#include "mvWindow.h"

namespace Marvel {

	mvWindow::mvWindow(unsigned width, unsigned height, bool editor, bool error, bool doc) :
		m_width(width), m_height(height), m_editor(editor), m_error(error), m_doc(doc)
	{
		m_app = mvApp::GetAppStandardWindow();
		m_appEditor = new mvAppEditor();
		m_documentation = mvDocWindow::GetWindow();

		if (m_error)
		{
			mvAppLog::Show();
			mvAppLog::setSize(width, height);
		}

		else if (m_doc)
		{
			m_documentation->setToMainMode();
			m_documentation->setSize(width, height);
		}
	}

	mvWindow::~mvWindow()
	{
		delete m_appEditor;
		m_appEditor = nullptr;
	}

	void mvWindow::run()
	{

		setup();
		while (m_running)
		{
			prerender();
			
			if (m_error)
			{
				mvAppLog::setSize(m_width, m_height);
				mvAppLog::render();
			}

			else if (m_editor)
			{
				m_appEditor->prerender();
				m_appEditor->render(m_editor);
				m_appEditor->postrender();
			}

			else if(m_doc)
			{
				m_documentation->prerender();
				m_documentation->render(m_doc);
				m_documentation->postrender();
			}

			else if (!m_error)
			{
				m_app->prerender();
				m_app->render(m_running);
				m_app->postrender();
			}

			postrender();
		}

	}

}