#include "mvStandardWindow.h"
#include "mvPythonTranslator.h"

namespace Marvel {

	bool mvStandardWindow::prerender(bool& show)
	{

		if (m_dirty_size)
		{
			ImGui::SetNextWindowSize(ImVec2((float)m_width, (float)m_height));
			m_dirty_size = false;
		}

		if (m_dirty_pos)
		{
			ImGui::SetNextWindowPos(ImVec2((float)m_xpos, (float)m_ypos));
			m_dirty_pos = false;
		}

		if (!m_show)
			return false;

		if (!ImGui::Begin(m_name.c_str(), &show, m_flags))
		{
			ImGui::End();
			return false;
		}

		return true;
	}

	void mvStandardWindow::setConfigDict(PyObject* dict)
	{
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "width")) setWidth(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "height")) setHeight(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "x_pos")) setXPos(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "y_pos")) setYPos(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "show")) m_show = ToBool(item);
	}

	void mvStandardWindow::getConfigDict(PyObject* dict)
	{
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "width", ToPyInt(m_width));
		PyDict_SetItemString(dict, "height", ToPyInt(m_height));
		PyDict_SetItemString(dict, "x_pos", ToPyInt(m_xpos));
		PyDict_SetItemString(dict, "y_pos", ToPyInt(m_ypos));
		PyDict_SetItemString(dict, "show", ToPyBool(m_show));
	}

	void mvStandardWindow::showStandardWindow(const std::string& name)
	{
		m_standardWindows[name].show = true;
		m_standardWindows[name].window->m_show = true;
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
		setWidth(width);
		setHeight(height);
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