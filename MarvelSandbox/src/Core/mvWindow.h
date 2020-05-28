#pragma once

class mvWindow
{

public:


	virtual void show() = 0;

	virtual void setup() {}
	virtual void prerender() {}
	virtual void render() = 0;
	virtual void postrender() {}
	virtual void cleanup() {}

	void run()
	{

		setup();
		while (m_running)
		{
			prerender();
			render();
			postrender();
		}

	}

protected:

	bool m_running = true;


};
