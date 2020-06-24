#include "mvApp.h"
#include "mvCore.h"
#include "mvAppLog.h"
#include "AppItems/mvAppItems.h"
#include <fstream>
#include <streambuf>
#include "Core/StandardWindows/mvDocWindow.h"
#include "Core/StandardWindows/mvAboutWindow.h"
#include "Core/StandardWindows/mvMetricsWindow.h"
#include "Core/StandardWindows/mvSourceWindow.h"
#include <thread>
#include <future>
#include <chrono>

typedef std::chrono::high_resolution_clock clock_;
typedef std::chrono::duration<double, std::ratio<1> > second_;

namespace Marvel {

	mvApp* mvApp::s_instance = nullptr;

	static void SetStyle(ImGuiStyle& style, mvStyle& mvstyle)
	{
		style.Alpha = mvstyle[ImGuiStyleVar_Alpha].x;
		style.WindowPadding = { mvstyle[ImGuiStyleVar_WindowPadding].x, mvstyle[ImGuiStyleVar_WindowPadding].y };
		style.WindowRounding = mvstyle[ImGuiStyleVar_WindowRounding].x;
		style.WindowBorderSize = mvstyle[ImGuiStyleVar_WindowBorderSize].x;
		style.WindowTitleAlign = { mvstyle[ImGuiStyleVar_WindowTitleAlign].x, mvstyle[ImGuiStyleVar_WindowTitleAlign].y };
		style.ChildRounding = mvstyle[ImGuiStyleVar_ChildRounding].x;
		style.ChildBorderSize = mvstyle[ImGuiStyleVar_ChildBorderSize].x;
		style.PopupRounding = mvstyle[ImGuiStyleVar_PopupRounding].x;
		style.PopupBorderSize = mvstyle[ImGuiStyleVar_PopupBorderSize].x;
		style.FramePadding = { mvstyle[ImGuiStyleVar_FramePadding].x, mvstyle[ImGuiStyleVar_FramePadding].y };
		style.FrameRounding = mvstyle[ImGuiStyleVar_FrameRounding].x;
		style.FrameBorderSize = mvstyle[ImGuiStyleVar_FrameBorderSize].x;
		style.ItemSpacing = { mvstyle[ImGuiStyleVar_ItemSpacing].x, mvstyle[ImGuiStyleVar_ItemSpacing].y };
		style.ItemInnerSpacing = { mvstyle[ImGuiStyleVar_ItemInnerSpacing].x, mvstyle[ImGuiStyleVar_ItemInnerSpacing].y };
		style.TouchExtraPadding = { mvstyle[23].x, mvstyle[23].y };
		style.IndentSpacing = mvstyle[ImGuiStyleVar_IndentSpacing].x;
		style.ScrollbarSize = mvstyle[ImGuiStyleVar_ScrollbarSize].x;
		style.ScrollbarRounding = mvstyle[ImGuiStyleVar_ScrollbarRounding].x;
		style.GrabMinSize = mvstyle[ImGuiStyleVar_GrabMinSize].x;
		style.GrabRounding = mvstyle[ImGuiStyleVar_GrabRounding].x;
		style.TabRounding = mvstyle[ImGuiStyleVar_TabRounding].x;
		style.TabBorderSize = mvstyle[24].x;
		style.ButtonTextAlign = { mvstyle[ImGuiStyleVar_ButtonTextAlign].x, mvstyle[ImGuiStyleVar_ButtonTextAlign].y };
		style.SelectableTextAlign = { mvstyle[ImGuiStyleVar_SelectableTextAlign].x, mvstyle[ImGuiStyleVar_SelectableTextAlign].y };
		//style.DisplayWindowPadding = { mvstyle[25].x, mvstyle[25].y };
		style.DisplaySafeAreaPadding = { mvstyle[25].x, mvstyle[25].y };
		style.AntiAliasedLines = mvstyle[26].x > 0.5f;
		style.AntiAliasedFill = mvstyle[27].x > 0.5f;
		style.CurveTessellationTol = mvstyle[28].x;
		style.CircleSegmentMaxError = mvstyle[29].x;
	}

	mvApp::mvApp()
	{
		m_style = getAppDefaultStyle();
		m_windowflags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

	}

	void mvApp::setFile(const std::string& file)
	{
		m_file = file;
		m_editor.SetLanguageDefinition(mvTextEditor::LanguageDefinition::Python());
		{
			std::ifstream t(m_file.c_str());
			if (t.good())
			{
				std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
				m_editor.SetText(str);
			}
		}

		m_editor.SetReadOnly(true);
		m_editor.SetShowWhitespaces(false);
	}

	static void prepareStandardCallbacks()
	{
		ImGuiIO& io = ImGui::GetIO();

		mvApp* app = mvApp::GetApp();

		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
		{

			if (ImGui::IsMouseClicked(i))
				app->runCallbackD(app->getMouseClickCallback(), i);

			if (io.MouseDownDuration[i] >= 0.0f)
				app->runCallbackD(app->getMouseDownCallback(), i, io.MouseDownDuration[i]);

			if (ImGui::IsMouseDoubleClicked(i))
				app->runCallbackD(app->getMouseDoubleClickCallback(), i);

			if (ImGui::IsMouseReleased(i))
				app->runCallbackD(app->getMouseReleaseCallback(), i);

		}

		for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
		{
			if (ImGui::IsKeyPressed(i))
				app->runCallbackD(app->getKeyPressCallback(), i);

			if (io.KeysDownDuration[i] >= 0.0f)
				app->runCallbackD(app->getKeyDownCallback(), i, io.KeysDownDuration[i]);

			if (ImGui::IsKeyReleased(i))
				app->runCallbackD(app->getKeyReleaseCallback(), i);

		}
	}

	static bool canItemTypeHaveDuplicates(mvAppItem* item)
	{

		// ugly and quick
		switch (item->getType())
		{
		case mvAppItemType::Spacing: return true;
		case mvAppItemType::SameLine: return true;
		case mvAppItemType::Separator: return true;
		case mvAppItemType::Indent: return true;
		case mvAppItemType::Unindent: return true;
		default: return false;
		}
	}

	static int getPopupButton(const std::string& name)
	{
		if (mvAppItem* item = mvApp::GetApp()->getItem(name))
			return static_cast<mvPopup*>(item)->getButton();
		return -1;
	}

	mvApp* mvApp::GetApp()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvApp();
		return s_instance;
	}

	void mvApp::preRender()
	{

		bool endWindowFound = false;

		for (mvAppItem* item : m_items)
		{
			if (item->getType() == mvAppItemType::EndWindow)
				endWindowFound = true;
		}

		if (!endWindowFound)
		{
			mvAppItem* item = new mvEndWindowAppitem("");
			mvApp::GetApp()->addItem(item);
		}
	}

	void mvApp::render()
	{

		// set imgui style to mvstyle
		ImGuiStyle& style = ImGui::GetStyle();
		SetStyle(style, m_style);

		// update mouse
		ImVec2 mousePos = ImGui::GetMousePos();
		m_mousePos.x = mousePos.x;
		m_mousePos.y = mousePos.y;

		prepareStandardCallbacks();
			
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height));
		ImGui::Begin("Main Window", (bool*)0, m_windowflags);

		m_parents.push(nullptr);

		if(!m_callback.empty())
			runCallback(m_callback, "Main Application");

		// standard windows
		if(m_showMetrics)
			mvMetricsWindow::GetWindow()->render(m_showMetrics);
		if(m_showAbout)
			mvAboutWindow::GetWindow()->render(m_showAbout);
		if (m_showSource)
			mvSourceWindow::GetWindow()->render(m_showSource);
		if (m_showDoc)
			mvDocWindow::GetWindow()->render(m_showDoc);

		for (mvAppItem* item : m_items)
		{

			// skip item if it's not shown
			if (!item->isShown())
				continue;

			// set item width
			if(item->getWidth() > 0)
				ImGui::SetNextItemWidth((float)item->getWidth());

			item->pushColorStyles();
			item->draw();
			item->popColorStyles();

			// Context Menu
			if (item->getPopup() != "")
				ImGui::OpenPopupOnItemClick(item->getPopup().c_str(), getPopupButton(item->getPopup()));

			// Regular Tooltip (simple)
			if (item->getTip() != "" && ImGui::IsItemHovered())
				ImGui::SetTooltip(item->getTip().c_str());

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
			item->setRectMin({ ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y});
			item->setRectMax({ ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y });
			item->setRectSize({ ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y });

		}

	}

	bool mvApp::isMouseButtonPressed(int button) const
	{
		ImGuiIO& io = ImGui::GetIO();
		return io.MouseDown[button];
	}

	bool mvApp::isKeyPressed(int keycode) const
	{
		ImGuiIO& io = ImGui::GetIO();
		return io.KeysDown[keycode];
	}

	void mvApp::pushParent(mvAppItem* item)
	{
		m_parents.push(item);
	}

	mvAppItem* mvApp::popParent()
	{
		mvAppItem* item = m_parents.top();
		m_parents.pop();
		return item;
	}

	mvAppItem* mvApp::topParent()
	{
		if(m_parents.size() != 0)
			return m_parents.top();
		return nullptr;
	}

	void mvApp::addItemColorStyle(const std::string& name, ImGuiCol item, mvColor color)
	{
		auto aitem = getItem(name);
		if (aitem)
			aitem->addColorStyle(item, color);
	}

	mvAppItem* mvApp::getItem(const std::string& name)
	{
		for (mvAppItem* item : m_items)
		{
			if (item->getName() == name)
				return item;

			if (item->isContainer())
			{
				auto child = item->getChild(name);
				if (child)
					return child;
			}
			
		}

		return nullptr;
	}

	void mvApp::runCallback(const std::string& name, const std::string& sender)
	{
		std::atomic<bool> check(false);

		if (m_multithread)
		{
			auto threadpool = mvThreadPool::GetThreadPool();
			threadpool->submit(std::bind(&mvApp::triggerCallback, this, &check, &name, &sender));
			return;
		}

		else
		{

			auto beg_ = clock_::now();

			std::thread thread1(&mvApp::triggerCallback, this, &check, &name, &sender);

			double elapsedTime = 0.0;
			while (elapsedTime < 1.0)
			{
				auto now = std::chrono::high_resolution_clock::now();

				elapsedTime = std::chrono::duration_cast<second_>(clock_::now() - beg_).count();

				if (check)
				{
					thread1.join();
					return;
				}

			}

			thread1.detach();
			m_multithread = true;
			mvAppLog::getLogger()->LogWarning("Multithreading activated.");

		}
			
	}

	void mvApp::runCallbackD(const std::string& name, int sender, float data)
	{
		std::atomic<bool> check(false);

		if (m_multithread)
		{
			auto threadpool = mvThreadPool::GetThreadPool();
			threadpool->submit(std::bind(&mvApp::triggerCallbackD, this, &check, &name, sender, data));
		}

		else
		{

			auto beg_ = clock_::now();

			std::thread thread1(&mvApp::triggerCallbackD, this, &check, &name, sender, data);

			double elapsedTime = 0.0;
			while (elapsedTime < 1.0)
			{
				auto now = std::chrono::high_resolution_clock::now();

				elapsedTime = std::chrono::duration_cast<second_>(clock_::now() - beg_).count();

				if (check)
				{
					thread1.join();
					return;
				}

			}

			thread1.detach();
			m_multithread = true;
			mvAppLog::getLogger()->LogWarning("Multithreading activated.");

		}

	}

	void mvApp::triggerCallback(std::atomic<bool>* p, const std::string* name, const std::string* sender)
	{
		
		if (name->empty())
		{
			if(p)
			 *p = true;
			return;
		}

		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* pHandler = PyDict_GetItemString(m_pDict, name->c_str()); // borrowed reference

		// if callback doesn't exist
		if (pHandler == NULL)
		{
			std::string message(" Callback doesn't exist");
			mvAppLog::getLogger()->LogWarning((*name) + message);
			PyGILState_Release(gstate);
			if (p)
				*p = true;
			return;
		}

		// check if handler is callable
		if (PyCallable_Check(pHandler))
		{

			PyErr_Clear();

			PyObject* pArgs = PyTuple_New(1);
			PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(sender->c_str()));

			PyObject* result = PyObject_CallObject(pHandler, pArgs);
			
			// check if call succeded
			if (!result)
			{
				std::string message("Callback failed");
				mvAppLog::getLogger()->LogError((*name) + message);
			}

			Py_XDECREF(pArgs);
			Py_XDECREF(result);

			// check if error occurred
			if(PyErr_Occurred())
				PyErr_Print();

		}

		else
		{
			std::string message(" Callback not callable");
			mvAppLog::getLogger()->LogError((*name) + message);
		}

		PyGILState_Release(gstate);
		if (p)
			*p = true;
	}

	void mvApp::triggerCallbackD(std::atomic<bool>* p, const std::string* name, int sender, float data)
	{
		if (name->empty())
		{
			if (p)
				*p = true;
			return;
		}

		PyGILState_STATE gstate = PyGILState_Ensure();

		PyErr_Clear();

		PyObject* pHandler = PyDict_GetItemString(m_pDict, name->c_str()); // borrowed reference

		// if callback doesn't exist
		if (pHandler == NULL)
		{
			std::string message(" Callback doesn't exist");
			mvAppLog::getLogger()->LogWarning((*name) + message);
			PyGILState_Release(gstate);
			if (p)
				*p = true;
			return;
		}

		// check if handler is callable
		if (PyCallable_Check(pHandler))
		{
			PyObject* pArgs = PyTuple_New(2);
			PyTuple_SetItem(pArgs, 0, PyLong_FromLong(sender));
			PyTuple_SetItem(pArgs, 1, PyFloat_FromDouble(data));

			PyObject* result = PyObject_CallObject(pHandler, pArgs);

			// check if call succeded
			if (!result)
			{
				std::string message("Callback failed");
				mvAppLog::getLogger()->LogError((*name) + message);
			}

			Py_XDECREF(pArgs);
			Py_XDECREF(result);

			// check if error occurred
			if (PyErr_Occurred())
				PyErr_Print();
		}

		else
		{
			std::string message(" Callback not callable");
			mvAppLog::getLogger()->LogError((*name) + message);
		}

		PyGILState_Release(gstate);
		if (p)
			*p = true;

	}

	void mvApp::setAppTheme(const std::string& theme)
	{
		if (theme == "dark")
			ImGui::StyleColorsDark();
		else if (theme == "classic")
			ImGui::StyleColorsClassic();
		else if (theme == "light")
			ImGui::StyleColorsLight();
		else
			ImGui::StyleColorsDark();
	}

	void mvApp::changeThemeItem(long item, mvColor color)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[item] = ImVec4(color.r, color.g, color.b, color.a);
	}

	void mvApp::changeStyleItem(long item, float x, float y)
	{

		if (m_style.count(item) > 0)
		{
			m_style.at(item).x = x;
			m_style.at(item).y = y;
		}

	}

	void mvApp::addItemManual(mvAppItem* item)
	{
		if (m_started)
		{
			mvAppLog::getLogger()->LogWarning("Items can't be added during runtime.");
			return;
		}

		if (!canItemTypeHaveDuplicates(item))
		{
			if (auto otheritem = getItem(item->getName()))
			{
				std::string message = item->getName() + " " + std::to_string(m_items.size());
				mvAppLog::getLogger()->LogWarning(message + ": Items of this type must have unique names");
				return;
			}
		}

		m_items.push_back(item);
	}

	void mvApp::addItem(mvAppItem* item)
	{
		if (m_started)
		{
			mvAppLog::getLogger()->LogWarning("Items can't be added during runtime.");
			return;
		}

		if (!canItemTypeHaveDuplicates(item))
		{
			if (auto otheritem = getItem(item->getName()))
			{
				std::string message = item->getName() + " " + std::to_string(m_items.size());
				mvAppLog::getLogger()->LogWarning(message + ": Items of this type must have unique names");
				return;
			}
		}

		mvAppItem* parentitem = topParent();	
		item->setParent(parentitem);
		if(parentitem)
			parentitem->addChild(item);
		else
			m_items.push_back(item);
	}

	void mvApp::closePopup()
	{
		if (!m_started)
		{
			mvAppLog::getLogger()->LogWarning("Popups can't be closed until runtime");
			return;
		}

		ImGui::CloseCurrentPopup();
	}

}