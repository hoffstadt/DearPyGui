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

			else if (m_app->isOk())
				m_app->render();

			if(mvApp::GetApp()->isLoggerShown())
				Marvel::mvAppLog::getLogger()->Draw("Marvel Sandbox", &mvApp::GetApp()->isLoggerShown());
			postrender();
		}

	}

}