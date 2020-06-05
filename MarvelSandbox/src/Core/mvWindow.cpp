#include "mvWindow.h"

namespace Marvel {

	void mvWindow::run()
	{

		setup();
		while (m_running)
		{
			prerender();
			if (m_app->isOk())
				m_app->render();
			if (m_app->showLog())
				Marvel::AppLog::getLogger()->Draw("Marvel Sandbox");
			postrender();
		}

	}

}