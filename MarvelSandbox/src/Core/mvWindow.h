#pragma once

#include "mvApp.h"

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
			m_app->render();
			postrender();
		}

	}

protected:

	bool m_running = true;
	Marvel::mvApp* m_app = nullptr;


};
