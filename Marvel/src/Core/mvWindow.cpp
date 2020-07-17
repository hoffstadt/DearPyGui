#include "mvWindow.h"

namespace Marvel {

	mvWindow::mvWindow(unsigned width, unsigned height, bool editor, bool error, bool doc) :
		m_width(width), m_height(height), m_editor(editor), m_error(error), m_doc(doc)
	{
		m_app = mvApp::GetAppStandardWindow();
		m_appEditor = mvAppEditor::GetAppEditor();
		m_documentation = mvDocWindow::GetWindow();
		m_logger = mvAppLog::GetLoggerStandardWindow();

		if (m_error)
		{
			m_logger->setToMainMode();
			m_logger->setSize(width, height);
		}

		else if (m_doc)
		{
			m_documentation->setToMainMode();
			m_documentation->setSize(width, height);
		}
	}

	void mvWindow::run()
	{

		setup();
		while (m_running)
		{
			prerender();
			
			if (m_error)
			{
				m_logger->prerender();
				m_logger->render(m_error);
				m_logger->postrender();
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