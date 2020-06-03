#pragma once

#include "mvApp.h"
#include "mvLogger.h"

class mvWindow
{

public:


	virtual void show() = 0;

	virtual void setup() {}
	virtual void prerender() {}
	virtual void postrender() {}
	virtual void cleanup() {}

	void run()
	{

		setup();
		while (m_running)
		{
			prerender();
			if(m_app->isOk())
				m_app->render();
			Marvel::AppLog::getLogger()->Draw("Marvel Sandbox");
			postrender();
		}

	}

protected:

	bool m_running = true;
	Marvel::mvApp* m_app = nullptr;
	unsigned m_width;
	unsigned m_height;


};
