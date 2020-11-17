#pragma once
#include "mvWindowAppItem.h"
#include "core/mvInput.h"
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {

		mvWindowAppitem::mvWindowAppitem(const std::string& name, bool mainWindow, PyObject* closing_callback)
			: mvAppItem(name), mvEventHandler(), m_mainWindow(mainWindow), m_closing_callback(SanitizeCallback(closing_callback))
		{
			m_description.root = true;
			m_description.container = true;

			m_width = 500;
			m_height = 500;

			m_oldWindowflags = ImGuiWindowFlags_NoSavedSettings;

			if (mainWindow)
			{
				m_windowflags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings
					| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
			}
		}

		void mvWindowAppitem::setWindowAsMainStatus(bool value)
		{
			m_mainWindow = value;
			if (value)
			{
				m_oldWindowflags = m_windowflags;
				m_windowflags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings
					| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

				if (m_hasMenuBar)
					m_windowflags |= ImGuiWindowFlags_MenuBar;
				m_oldxpos = m_xpos;
				m_oldypos = m_ypos;
				m_oldWidth = m_width;
				m_oldHeight = m_height;
			}
			else
			{
				m_focusNextFrame = true;
				m_windowflags = m_oldWindowflags;
				if (m_hasMenuBar)
					m_windowflags |= ImGuiWindowFlags_MenuBar;
				m_xpos = m_oldxpos;
				m_ypos = m_oldypos;
				m_width = m_oldWidth;
				m_height = m_oldHeight;
				m_dirty_pos = true;
				m_dirty_size = true;
			}


		}

		void mvWindowAppitem::setWindowPos(float x, float y)
		{
			m_xpos = (int)x;
			m_ypos = (int)y;
			m_dirty_pos = true;
		}

		void mvWindowAppitem::setWidth(int width) 
		{ 
			m_width = width;
			m_dirty_size = true; 
		}

		void mvWindowAppitem::setHeight(int height) 
		{ 
			m_height = height; 
			m_dirty_size = true; 
		}

		mvVec2 mvWindowAppitem::getWindowPos() const
		{
			return { (float)m_xpos, (float)m_ypos };
		}

		void mvWindowAppitem::draw()
		{
			// shouldn't have to do this but do. Fix later
			if (!m_show)
			{
				m_state.setHovered(false);
				m_state.setFocused(false);
				m_state.setActivated(false);
				m_state.setVisible(false);
				if (!m_closing)
				{
					m_closing = true;
					mvApp::GetApp()->runCallback(m_closing_callback, m_name);

				}
				return;
			}
			m_closing = false;

			if (m_mainWindow)
			{
				ImGui::SetNextWindowBgAlpha(1.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // to prevent main window corners from showing
				ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
				ImGui::SetNextWindowSize(ImVec2((float)mvApp::GetApp()->getClientWidth(), (float)mvApp::GetApp()->getClientHeight()));
			}

			else if (m_dirty_pos)
			{
				ImGui::SetNextWindowPos(ImVec2((float)m_xpos, (float)m_ypos));
				m_dirty_pos = false;
			}

			else if (m_focusNextFrame)
			{
				ImGui::SetNextWindowFocus();
				m_focusNextFrame = false;
			}

			if (m_dirty_size)
			{
				ImGui::SetNextWindowSize(ImVec2((float)m_width, (float)m_height));
				m_dirty_size = false;
			}


			auto styleManager = m_styleManager.getScopedStyleManager();
			ScopedID id;

			if (!ImGui::Begin(m_label.c_str(), m_noclose ? nullptr : &m_show, m_windowflags))
			{
				if (m_mainWindow)
					ImGui::PopStyleVar();

				ImGui::End();
				return;
			}

			if (m_mainWindow)
				ImGui::PopStyleVar();

			for (mvAppItem* item : m_children)
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				if (item->getWidth() != 0)
					ImGui::SetNextItemWidth((float)item->getWidth());

				item->draw();

				// Regular Tooltip (simple)
				if (!item->getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", item->getTip().c_str());

				item->getState().update();

			}

			m_state.setVisible(true);
			m_state.setHovered(ImGui::IsWindowHovered());
			m_state.setFocused(ImGui::IsWindowFocused());
			m_state.setRectSize({ ImGui::GetWindowSize().x, ImGui::GetWindowSize().y });
			m_state.setActivated(ImGui::IsWindowCollapsed());

			if (ImGui::GetWindowWidth() != (float)m_width || ImGui::GetWindowHeight() != (float)m_height)
				mvApp::GetApp()->runCallback(getResizeCallback(), m_name);

			m_width = (int)ImGui::GetWindowWidth();
			m_height = (int)ImGui::GetWindowHeight();

			if (m_state.isItemFocused())
			{

				float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

				// update mouse
				mvVec2 oldMousePos = mvInput::getGlobalMousePosition();
				ImVec2 mousePos = ImGui::GetMousePos();
				mvInput::setGlobalMousePosition(mousePos.x, mousePos.y);
				float x = mousePos.x - ImGui::GetWindowPos().x;
				float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
				mvInput::setMousePosition(x, y);
				mvApp::GetApp()->setActiveWindow(m_name);

				// mouse move callback
				if (oldMousePos.x != mousePos.x || oldMousePos.y != mousePos.y)
				{
					mvApp::GetApp()->runCallback(mvApp::GetApp()->getMouseMoveCallback(), m_name,
						ToPyPair(x, y));
				}

			}

			m_xpos = (int)ImGui::GetWindowPos().x;
			m_ypos = (int)ImGui::GetWindowPos().y;

			m_drawList.draw(ImGui::GetWindowDrawList(), m_xpos, m_ypos);

			ImGui::End();
		}

		void mvWindowAppitem::setExtraConfigDict(PyObject* dict)
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "x_pos")) setWindowPos((float)ToInt(item), (float)m_ypos);
			if (PyObject* item = PyDict_GetItemString(dict, "y_pos")) setWindowPos((float)m_xpos, (float)ToInt(item));
			if (PyObject* item = PyDict_GetItemString(dict, "no_close")) m_noclose = ToBool(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// window flags
			flagop("autosize", ImGuiWindowFlags_AlwaysAutoResize, m_windowflags);
			flagop("no_move", ImGuiWindowFlags_NoMove, m_windowflags);
			flagop("no_resize", ImGuiWindowFlags_NoResize, m_windowflags);
			flagop("no_title_bar", ImGuiWindowFlags_NoTitleBar, m_windowflags);
			flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
			flagop("no_collapse", ImGuiWindowFlags_NoCollapse, m_windowflags);
			flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
			flagop("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, m_windowflags);
			flagop("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, m_windowflags);
			flagop("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
			flagop("no_background", ImGuiWindowFlags_NoBackground, m_windowflags);

			m_oldxpos = m_xpos;
			m_oldypos = m_ypos;
			m_oldWidth = m_width;
			m_oldHeight = m_height;
			m_oldWindowflags = m_windowflags;

		}

		void mvWindowAppitem::getExtraConfigDict(PyObject* dict)
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "x_pos", ToPyInt(m_xpos));
			PyDict_SetItemString(dict, "y_pos", ToPyInt(m_ypos));
			PyDict_SetItemString(dict, "no_close", ToPyBool(m_closing));

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
			checkbitset("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
			checkbitset("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, m_windowflags);
			checkbitset("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, m_windowflags);
			checkbitset("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
			checkbitset("no_background", ImGuiWindowFlags_NoBackground, m_windowflags);
		}

		mvWindowAppitem::~mvWindowAppitem()
		{
			mvGlobalIntepreterLock gil;
			if (m_closing_callback)
				Py_XDECREF(m_closing_callback);
		}

}