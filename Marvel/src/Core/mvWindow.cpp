#include "mvWindow.h"

namespace Marvel {

	void mvWindow::run()
	{

		setup();
		while (m_running)
		{
			prerender();
			
			if (m_editor)
				m_appEditor->render();

			else if(m_doc)
				m_documentation->GetWindow()->render(m_doc);

			else if (!m_error)
			{
				m_app->render();
				m_app->postRender();

			}

			if(mvApp::GetApp()->isLoggerShown())
				Marvel::mvAppLog::getLogger()->Draw("Marvel Sandbox", &mvApp::GetApp()->isLoggerShown());
			postrender();
		}

	}

}