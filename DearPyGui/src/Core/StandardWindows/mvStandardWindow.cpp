#include "mvStandardWindow.h"

namespace Marvel {

	bool mvStandardWindow::prerender(bool& show)
	{

		if (m_dirty)
		{
			ImGui::SetNextWindowPos(ImVec2((float)m_xpos, (float)m_ypos));
			ImGui::SetNextWindowSize(ImVec2((float)m_width, (float)m_height));
			m_dirty = false;
		}


		if (!ImGui::Begin(m_name.c_str(), &show, m_flags))
		{
			ImGui::End();
			return false;
		}

		return true;
	}

	void mvStandardWindow::showStandardWindow(const std::string& name)
	{
		m_standardWindows[name].show = true;
	}

	void mvStandardWindow::addStandardWindow(const std::string& name, mvStandardWindow* window)
	{
		m_standardWindows.insert({ name, {window, false} });
	}

	mvStandardWindow* mvStandardWindow::getStandardWindow(const std::string& name)
	{
		return m_standardWindows[name].window;
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