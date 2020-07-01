#include "mvStandardWindow.h"

namespace Marvel {

	void mvStandardWindow::prerender()
	{
		for (auto& entry : m_standardWindows)
		{
			if (entry.second.show)
				entry.second.window->render(entry.second.show);
		}
	}

	void mvStandardWindow::showStandardWindow(const std::string& name)
	{
		m_standardWindows[name].show = true;
	}

	void mvStandardWindow::addStandardWindow(const std::string& name, mvStandardWindow* window)
	{
		m_standardWindows.insert({ name, {window, false} });
	}

	void mvStandardWindow::setToMainMode() 
	{ 
		m_mainMode = true; 
	}

	void mvStandardWindow::setSize(unsigned width, unsigned height) 
	{ 
		m_width = width; 
		m_height = height; 
	}

	unsigned mvStandardWindow::getWindowWidth() const
	{ 
		return m_width; 
	}

	unsigned mvStandardWindow::getWindowHeight() const
	{ 
		return m_height; 
	}


}