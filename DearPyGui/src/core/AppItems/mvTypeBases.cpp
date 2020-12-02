#include "mvTypeBases.h"
#include "PythonUtilities/mvPythonTranslator.h"
#include "Registries/mvDataStorage.h"
#include "Registries/mvValueStorage.h"
#include "core/mvUtilities.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include <utility>

namespace Marvel
{

	mvBaseWindowAppitem::mvBaseWindowAppitem(const std::string& name)
		: mvAppItem(name), mvEventHandler()
	{
		m_description.root = true;
	}

	void mvBaseWindowAppitem::addFlag(ImGuiWindowFlags flag)
	{
		m_windowflags |= flag;
	}

	void mvBaseWindowAppitem::removeFlag(ImGuiWindowFlags flag)
	{
		m_windowflags &= ~flag;
	}

	void mvBaseWindowAppitem::setWindowPos(float x, float y)
	{
		m_xpos = (int)x;
		m_ypos = (int)y;
		m_dirty_pos = true;
	}

	void mvBaseWindowAppitem::setWidth(int width)
	{
		m_width = width;
		m_dirty_size = true;
	}

	void mvBaseWindowAppitem::setHeight(int height)
	{
		m_height = height;
		m_dirty_size = true;
	}

	mvVec2 mvBaseWindowAppitem::getWindowPos() const
	{
		return { (float)m_xpos, (float)m_ypos };
	}

	bool mvBaseWindowAppitem::prerender()
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

		if (!ImGui::Begin(m_label.c_str(), &m_show, m_windowflags))
		{
			ImGui::End();
			return false;
		}

		return true;
	}

	void mvBaseWindowAppitem::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "x_pos"))
			setWindowPos((float)ToInt(item), (float)m_ypos);
		if (PyObject* item = PyDict_GetItemString(dict, "y_pos"))
			setWindowPos((float)m_xpos, (float)ToInt(item));

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
		  if (PyObject* item = PyDict_GetItemString(dict, keyword))
			  ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("autosize", ImGuiWindowFlags_AlwaysAutoResize, m_windowflags);
		flagop("no_move", ImGuiWindowFlags_NoMove, m_windowflags);
		flagop("no_resize", ImGuiWindowFlags_NoResize, m_windowflags);
		flagop("no_title_bar", ImGuiWindowFlags_NoTitleBar, m_windowflags);
		flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
		flagop("no_collapse", ImGuiWindowFlags_NoCollapse, m_windowflags);
		flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar,
			m_windowflags);
		flagop("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing,
			m_windowflags);
		flagop("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus,
			m_windowflags);
		flagop("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
		flagop("no_background", ImGuiWindowFlags_NoBackground, m_windowflags);
	}

	void mvBaseWindowAppitem::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "x_pos", ToPyInt(m_xpos));
		PyDict_SetItemString(dict, "y_pos", ToPyInt(m_ypos));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
		  PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("autosize", ImGuiWindowFlags_AlwaysAutoResize, m_windowflags);
		checkbitset("no_resize", ImGuiWindowFlags_NoResize, m_windowflags);
		checkbitset("no_title_bar", ImGuiWindowFlags_NoTitleBar, m_windowflags);
		checkbitset("no_move", ImGuiWindowFlags_NoMove, m_windowflags);
		checkbitset("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
		checkbitset("no_collapse", ImGuiWindowFlags_NoCollapse, m_windowflags);
		checkbitset("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar,
			m_windowflags);
		checkbitset("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing,
			m_windowflags);
		checkbitset("no_bring_to_front_on_focus",
			ImGuiWindowFlags_NoBringToFrontOnFocus, m_windowflags);
		checkbitset("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
		checkbitset("no_background", ImGuiWindowFlags_NoBackground, m_windowflags);
	}

} // namespace Marvel