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
		m_windows.push_back(new mvWindowAppitem("", "MainWindow", 1280, 800, 0, 0, true));
		m_parents.push(m_windows.back());

	}

	static void prepareStandardCallbacks()
	{
		ImGuiIO& io = ImGui::GetIO();

		mvApp* app = mvApp::GetApp();

		if (io.MouseWheel != 0.0f)
			app->runCallbackD(app->getMouseWheelCallback(), 0, io.MouseWheel);


		for (int i = 0; i < 3; i++)
		{
			if (ImGui::IsMouseDragging(i, app->getMouseDragThreshold()))
			{
				app->setMouseDragging(true);
				app->setMouseDragDelta({ ImGui::GetMouseDragDelta().x, ImGui::GetMouseDragDelta().y });
				app->runCallbackD(app->getMouseDragCallback(), i, i);
				ImGui::ResetMouseDragDelta(i);
				break;
			}
			app->setMouseDragging(false);
			app->setMouseDragDelta({ 0.0f, 0.0f });
			
		}

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

	void mvApp::setWindowSize(unsigned width, unsigned height) 
	{ 
		m_windows[0]->setWidth(width);
		m_windows[0]->setHeight(height);
	}

	void mvApp::preRender()
	{

		if (m_windows.size() == 1)
			popParent();

	}

	void mvApp::render()
	{

		// set imgui style to mvstyle
		ImGuiStyle& style = ImGui::GetStyle();
		SetStyle(style, m_style);

		// update mouse
		//ImVec2 mousePos = ImGui::GetMousePos();
		////m_mousePos.x = mousePos.x;
		////m_mousePos.y = mousePos.y;
		//m_mousePos.x = 0.0f;
		//m_mousePos.y = 0.0f;

		prepareStandardCallbacks();

		if (!m_callback.empty())
			runMainCallback(m_callback, "Main Application");

		// standard windows
		if(m_showMetrics)
			mvMetricsWindow::GetWindow()->render(m_showMetrics);
		if(m_showAbout)
			mvAboutWindow::GetWindow()->render(m_showAbout);
		if (m_showSource)
			mvSourceWindow::GetWindow()->render(m_showSource);
		if (m_showDoc)
			mvDocWindow::GetWindow()->render(m_showDoc);

		for (auto window : m_windows)
			window->draw();

	}

	void mvApp::postRender()
	{
		// delete items from the delete queue
		while (!m_deleteQueue.empty())
		{
			std::string& itemname = m_deleteQueue.front();

			bool deletedItem = false;

			for (auto window : m_windows)
			{
				deletedItem = window->deleteChild(itemname);
				if (deletedItem)
					break;
			}

			if (!deletedItem)
				mvAppLog::getLogger()->LogWarning(itemname + " not deleted because it was not found");

			m_deleteQueue.pop();
		}

		// add runtime items
		for (auto& newItem : m_newItemVec)
		{

			bool addedItem = false;

			if (auto otheritem = getItem(newItem.item->getName()))
			{
				std::string message = newItem.item->getName();
				mvAppLog::getLogger()->LogWarning(message + ": Items of this type must have unique names");
				delete newItem.item;
				newItem.item = nullptr;
				continue;
			}

			for (auto window : m_windows)
			{
				addedItem = window->addRuntimeChild(newItem.parent, newItem.before, newItem.item);
				if (addedItem)
					break;
			}

			if (!addedItem)
			{
				mvAppLog::getLogger()->LogWarning(newItem.item->getName() + " not added because its parent was not found");
				delete newItem.item;
				newItem.item = nullptr;
			}

		}

		m_newItemVec.clear();

		// move items up
		while (!m_upQueue.empty())
		{
			std::string& itemname = m_upQueue.front();

			bool movedItem = false;

			for (auto window : m_windows)
			{
				movedItem = window->moveChildUp(itemname);
				if (movedItem)
					break;
			}

			if (!movedItem)
				mvAppLog::getLogger()->LogWarning(itemname + " not moved because it was not found");

			m_upQueue.pop();
		}

		// move items down
		while (!m_downQueue.empty())
		{
			std::string& itemname = m_downQueue.front();

			bool movedItem = false;

			for (auto window : m_windows)
			{
				movedItem = window->moveChildDown(itemname);
				if (movedItem)
					break;
			}

			if (!movedItem)
				mvAppLog::getLogger()->LogWarning(itemname + " not moved because it was not found");

			m_downQueue.pop();
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
		for (auto window : m_windows)
		{
			if (window->getName() == name)
				return window;

			auto child = window->getChild(name);
			if (child)
				return child;
		}

		return nullptr;
	}

	void mvApp::runMainCallback(const std::string& name, const std::string& sender)
	{
		if (name.empty())
			return;

		if (PyGILState_Check()) // if holding gil, don't run
		{
			mvAppLog::getLogger()->LogWarning("Waiting for another thread to finish.");
			return;
		}

		std::atomic<bool> check(false);
		triggerCallback(&check, &name, &sender);
	}

	void mvApp::runCallback(const std::string& name, const std::string& sender)
	{
		if (name.empty())
			return;

		std::atomic<bool> check(false);

		if (m_threadPool)
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
			while (elapsedTime < m_threadPoolThreshold)
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
			if (m_threadPoolAuto)
			{
				m_threadPool = true;
				mvAppLog::getLogger()->LogWarning("Thread pool will be activated automatically.");
			}

		}
			
	}

	void mvApp::runCallbackD(const std::string& name, int sender, float data)
	{
		if (name.empty())
			return;

		std::atomic<bool> check(false);

		if (m_threadPool)
		{
			auto threadpool = mvThreadPool::GetThreadPool();
			threadpool->submit(std::bind(&mvApp::triggerCallbackD, this, &check, &name, sender, data));
		}

		else
		{

			auto beg_ = clock_::now();

			std::thread thread1(&mvApp::triggerCallbackD, this, &check, &name, sender, data);

			double elapsedTime = 0.0;
			while (elapsedTime < m_threadPoolThreshold)
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
			if (m_threadPoolAuto)
			{
				m_threadPool = true;
				mvAppLog::getLogger()->LogWarning("Thread pool will be activated automatically.");
			}

		}

	}

	void mvApp::triggerCallback(std::atomic<bool>* p, const std::string* name, const std::string* sender)
	{

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
		m_theme = theme;

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

	std::pair<float, float> mvApp::getStyleItem(long item)
	{
		if (m_style.count(item) > 0)
		{
			return std::make_pair(m_style[item].x, m_style[item].y);
		}

		return std::make_pair(-1.0f, -1.0f);

	}

	mvColor mvApp::getThemeItem(long item)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		mvColor color = {(int)style->Colors[item].x * 255, (int)style->Colors[item].y * 255 ,
			(int)style->Colors[item].z * 255 , (int)style->Colors[item].w * 255 };
		return color;
	}

	void mvApp::addItem(mvAppItem* item)
	{
		static int count = 0;
		count++;

		if (m_started)
		{
			mvAppLog::getLogger()->Log("Runtime item adding.");
			return;
		}

		if (!item->areDuplicatesAllowed())
		{
			if (auto otheritem = getItem(item->getName()))
			{
				std::string message = item->getName() + " " + std::to_string(count);
				mvAppLog::getLogger()->LogWarning(message + ": Items of this type must have unique names");
				return;
			}
		}

		mvAppItem* parentitem = topParent();	
		item->setParent(parentitem);
		parentitem->addChild(item);
	}

	void mvApp::addWindow(mvAppItem* item)
	{
		m_windows.push_back(item);
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