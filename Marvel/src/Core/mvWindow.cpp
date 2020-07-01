#include "mvWindow.h"

namespace Marvel {

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