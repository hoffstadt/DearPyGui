#pragma once
#include "mvWindowAppItem.h"
#include "Core/mvInput.h"

namespace Marvel {

		mvWindowAppitem::Status mvWindowAppitem::s_status = mvWindowAppitem::Status::Normal;

		mvWindowAppitem::mvWindowAppitem(const std::string& name, bool mainWindow, PyObject* closing_callback)
			: mvAppItem(name), mvEventHandler(), m_mainWindow(mainWindow), m_closing_callback(SanitizeCallback(closing_callback))
		{
			m_container = true;

			m_width = 500;
			m_height = 500;

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
				m_windowflags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings
					| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar;
				s_status = Status::Dirty;
			}
			else
			{
				s_status = Status::Dirty;
				removeFlag(ImGuiWindowFlags_NoBringToFrontOnFocus);
				removeFlag(ImGuiWindowFlags_NoResize);
				removeFlag(ImGuiWindowFlags_NoCollapse);
				removeFlag(ImGuiWindowFlags_NoTitleBar);
				m_xpos = 200;
				m_ypos = 200;
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
				setHovered(false);
				setFocused(false);
				setActivated(false);
				setVisible(false);
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
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // to prevent main window corners from showing
				ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
				ImGui::SetNextWindowSize(ImVec2((float)mvApp::GetApp()->getActualWidth(), (float)mvApp::GetApp()->getActualHeight()));

				if (s_status == Status::Transition)
					s_status = Status::Normal;

				// let other windows bring themselves into focus
				if (s_status == Status::Dirty)
					s_status = Status::Transition;

			}

			else if (m_dirty_pos)
			{
				ImGui::SetNextWindowPos(ImVec2((float)m_xpos, (float)m_ypos));
				m_dirty_pos = false;
			}

			else if (s_status == Status::Transition)
				ImGui::SetNextWindowFocus();

			if (m_dirty_size)
			{
				ImGui::SetNextWindowSize(ImVec2((float)m_width, (float)m_height));
				m_dirty_size = false;
			}


			pushColorStyles();
			ImGui::PushID(this);

			if (!ImGui::Begin(m_label.c_str(), m_noclose ? nullptr : &m_show, m_windowflags))
			{
				if (m_mainWindow)
					ImGui::PopStyleVar();

				ImGui::End();
				ImGui::PopID();
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

				item->pushColorStyles();
				item->draw();
				item->popColorStyles();

				item->setHovered(ImGui::IsItemHovered());
				item->setActive(ImGui::IsItemActive());
				item->setFocused(ImGui::IsItemFocused());
				item->setClicked(ImGui::IsItemClicked());
				item->setVisible(ImGui::IsItemVisible());
				item->setEdited(ImGui::IsItemEdited());
				item->setActivated(ImGui::IsItemActivated());
				item->setDeactivated(ImGui::IsItemDeactivated());
				item->setDeactivatedAfterEdit(ImGui::IsItemDeactivatedAfterEdit());
				item->setToggledOpen(ImGui::IsItemToggledOpen());
				item->setRectMin({ ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y });
				item->setRectMax({ ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y });
				item->setRectSize({ ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y });

			}

			setVisible(true);
			setHovered(ImGui::IsWindowHovered());
			setFocused(ImGui::IsWindowFocused());
			setRectSize({ ImGui::GetWindowSize().x, ImGui::GetWindowSize().y });
			setActivated(ImGui::IsWindowCollapsed());

			if (ImGui::GetWindowWidth() != (float)m_width || ImGui::GetWindowHeight() != (float)m_height)
				mvApp::GetApp()->runCallback(getResizeCallback(), m_name);

			m_width = (int)ImGui::GetWindowWidth();
			m_height = (int)ImGui::GetWindowHeight();

			if (m_focused)
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


			ImGui::End();
			ImGui::PopID();
			popColorStyles();
		}

		void mvWindowAppitem::setExtraConfigDict(PyObject* dict)
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "x_pos")) setWindowPos(ToInt(item), m_ypos);
			if (PyObject* item = PyDict_GetItemString(dict, "y_pos")) setWindowPos(m_xpos, ToInt(item));
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