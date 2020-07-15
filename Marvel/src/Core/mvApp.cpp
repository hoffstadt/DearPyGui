#include "mvApp.h"
#include "mvCore.h"
#include "mvThemeScheme.h"
#include "AppItems/mvAppItems.h"
#include <fstream>
#include <streambuf>
#include "Core/StandardWindows/mvAppLog.h"
#include "Core/StandardWindows/mvDocWindow.h"
#include "Core/StandardWindows/mvAboutWindow.h"
#include "Core/StandardWindows/mvMetricsWindow.h"
#include "Core/StandardWindows/mvSourceWindow.h"
#include "Core/StandardWindows/mvDebugWindow.h"
#include <thread>
#include <future>
#include <chrono>

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

	mvStandardWindow* mvApp::GetAppStandardWindow()
	{
		return static_cast<mvStandardWindow*>(GetApp());
	}

	mvApp::mvApp()
	{
		m_style = getAppDefaultStyle();
		m_windows.push_back(new mvWindowAppitem("", "MainWindow", 1280, 800, 0, 0, true, false));
		m_parents.push(m_windows.back());

		addStandardWindow("documentation", mvDocWindow::GetWindow());
		addStandardWindow("about", new mvAboutWindow());
		addStandardWindow("metrics", new mvMetricsWindow());
		addStandardWindow("source", new mvSourceWindow());
		addStandardWindow("debug", new mvDebugWindow());
		addStandardWindow("logger", mvAppLog::GetLoggerStandardWindow());

	}

	mvApp::~mvApp()
	{
		for (auto window : m_windows)
		{
			delete window;
			window = nullptr;
		}

		m_windows.clear();
	}

	void  mvApp::setFile(const std::string& file) 
	{ 
		m_file = file;

		auto sourcewindow = static_cast<mvSourceWindow*>(m_standardWindows["source"].window);
		sourcewindow->setFile(file);
	}

	void mvApp::setWindowSize(unsigned width, unsigned height) 
	{ 
		setSize(width, height);
		m_windows[0]->setWidth(width);
		m_windows[0]->setHeight(height);
	}

	void mvApp::addRuntimeItem(const std::string& parent, const std::string& before, mvAppItem* item) 
	{ 
		if (std::this_thread::get_id() != m_mainThreadID)
			mvAppLog::getLogger()->LogWarning("This function can't be called outside main thread.");

		m_newItemVec.push_back({ item, before, parent });
	}

	void mvApp::addMTCallback(const std::string& name, PyObject* data, const std::string& returnname) 
	{ 
		m_asyncCallbacks.push_back({ name, data, returnname }); 
	}

	void mvApp::precheck()
	{
		if (m_windows.size() == 1)
			popParent();

		// update for any data storage added at compile time
		for (auto& data : m_dataStorage)
		{
			for (auto window : m_windows)
				window->updateDataSource(data.first);
		}

	}

	void mvApp::prerender()
	{

		if (m_threadTime > m_threadPoolTimeout)
		{
			if (m_tpool != nullptr)
			{
				delete m_tpool;
				m_tpool = nullptr;
				m_threadTime = 0.0;
				m_threadPool = false;
				mvAppLog::getLogger()->Log("Threadpool destroyed");
			}
			
		}

		// update times
		ImGuiIO& io = ImGui::GetIO();
		m_deltaTime = io.DeltaTime;
		m_time = ImGui::GetTime();

		if (!m_asyncReturns.empty())
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			while (!m_asyncReturns.empty())
			{
				auto& asyncreturn = m_asyncReturns.front();
				runAsyncCallbackReturn(asyncreturn.name, asyncreturn.data);
				m_asyncReturns.pop();
			}
		}


		for (auto& entry : m_standardWindows)
		{
			if (entry.second.show)
				entry.second.window->render(entry.second.show);
		}

		// set imgui style to mvstyle
		ImGuiStyle& style = ImGui::GetStyle();
		SetStyle(style, m_style);

		prepareStandardCallbacks();

		for (auto window : m_windows)
			window->resetState();

	}

	void mvApp::render(bool& show)
	{

		if (!m_callback.empty())
			runCallback(m_callback, "Main Application");

		for (auto window : m_windows)
			window->draw();

	}

	void mvApp::postrender()
	{
		// delete items from the delete queue
		while (!m_deleteChildrenQueue.empty())
		{
			std::string& itemname = m_deleteChildrenQueue.front();

			auto item = getItem(itemname);
			if (item)
				item->deleteChildren();


			m_deleteChildrenQueue.pop();
		}

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

			bool childfound = false;
			bool itemDeleted = false;

			for (auto window : m_windows)
			{
				if (window->getName() == itemname)
				{
					childfound = true;
					break;
				}
			}

			if (childfound)
			{
				std::vector<mvAppItem*> oldwindows = m_windows;

				m_windows.clear();

				for (auto window : oldwindows)
				{
					if (window->getName() == itemname)
					{
						delete window;
						window = nullptr;
						deletedItem = true;
						continue;
					}

					m_windows.push_back(window);
				}
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

			if (newItem.item->getType() == mvAppItemType::Window)
			{
				m_windows.push_back(newItem.item);
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

		// async callbacks
		if (!m_asyncCallbacks.empty())
		{
			if (m_tpool == nullptr)
			{
				m_tpool = new mvThreadPool(m_threadPoolHighPerformance ? 0 : m_threads);
				m_poolStart = clock_::now();
				m_threadPool = true;
				mvAppLog::getLogger()->Log("Threadpool created");
			}

		}

		for (auto& callback : m_asyncCallbacks)
			m_tpool->submit(std::bind(&mvApp::runAsyncCallback, this, callback.name, callback.data, callback.returnname));

		m_asyncCallbacks.clear();

		if(m_tpool != nullptr)
			m_threadTime = std::chrono::duration_cast<second_>(clock_::now() - m_poolStart).count();
		
	}

	bool mvApp::isMouseDragging(int button, float threshold) const
	{
		return ImGui::IsMouseDragging(button, threshold);
	}

	bool mvApp::isMouseButtonDown(int button) const
	{
		return ImGui::IsMouseDown(button);
	}

	bool mvApp::isMouseButtonClicked(int button) const
	{
		return ImGui::IsMouseClicked(button);
	}

	bool mvApp::isMouseButtonDoubleClicked(int button) const
	{
		return ImGui::IsMouseDoubleClicked(button);
	}

	bool mvApp::isMouseButtonReleased(int button) const
	{
		return ImGui::IsMouseReleased(button);
	}

	bool mvApp::isKeyPressed(int keycode) const
	{
		return ImGui::IsKeyPressed(keycode);
	}

	bool mvApp::isKeyReleased(int keycode) const
	{
		return ImGui::IsKeyReleased(keycode);
	}

	bool mvApp::isKeyDown(int keycode) const
	{
		return ImGui::IsKeyDown(keycode);
	}

	void mvApp::pushParent(mvAppItem* item)
	{
		m_parents.push(item);
	}

	mvAppItem* mvApp::popParent()
	{
		if (m_parents.empty())
		{
			mvAppLog::getLogger()->LogError("No parent to pop.");
			return nullptr;
		}

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

		if (std::this_thread::get_id() != m_mainThreadID)
		{
			mvAppLog::getLogger()->LogWarning("This function can't be called outside main thread.");
			return nullptr;
		}

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

	void mvApp::runAsyncCallback(std::string name, PyObject* data, std::string returnname)
	{
		if (name.empty())
			return;

		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* pHandler = PyDict_GetItemString(m_pDict, name.c_str()); // borrowed reference

		// if callback doesn't exist
		if (pHandler == NULL)
		{
			std::string message(" Callback doesn't exist");
			mvAppLog::getLogger()->LogWarning(name + message);
			PyGILState_Release(gstate);
			return;
		}

		// check if handler is callable
		if (PyCallable_Check(pHandler))
		{

			PyErr_Clear();

			PyObject* pArgs = PyTuple_New(1);
			Py_XINCREF(data);
			PyTuple_SetItem(pArgs, 0, data);

			PyObject* result = PyObject_CallObject(pHandler, pArgs);

			// check if call succeded
			if (!result)
			{
				std::string message("Callback failed");
				mvAppLog::getLogger()->LogError(name + message);
			}

			if (!returnname.empty())
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				m_asyncReturns.push({ returnname, result });
			}
			else
				Py_XDECREF(result);

			Py_XDECREF(pArgs);
			
			// check if error occurred
			if (PyErr_Occurred())
				PyErr_Print();

		}

		else
		{
			std::string message(" Callback not callable");
			mvAppLog::getLogger()->LogError(name + message);
		}

		PyGILState_Release(gstate);
	}

	void mvApp::runAsyncCallbackReturn(std::string name, PyObject* data)
	{
		if (name.empty())
			return;

		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* pHandler = PyDict_GetItemString(m_pDict, name.c_str()); // borrowed reference

		// if callback doesn't exist
		if (pHandler == NULL)
		{
			std::string message(" Callback doesn't exist");
			mvAppLog::getLogger()->LogWarning(name + message);
			PyGILState_Release(gstate);
			return;
		}

		// check if handler is callable
		if (PyCallable_Check(pHandler))
		{

			PyErr_Clear();

			PyObject* pArgs = PyTuple_New(1);
			Py_XINCREF(data);
			PyTuple_SetItem(pArgs, 0, data);

			PyObject* result = PyObject_CallObject(pHandler, pArgs);

			// check if call succeded
			if (!result)
			{
				std::string message("Callback failed");
				mvAppLog::getLogger()->LogError(name + message);
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
			mvAppLog::getLogger()->LogError(name + message);
		}

		PyGILState_Release(gstate);
	}

	void mvApp::runCallback(const std::string& name, const std::string& sender)
	{
		if (name.empty())
			return;

		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* pHandler = PyDict_GetItemString(m_pDict, name.c_str()); // borrowed reference

		// if callback doesn't exist
		if (pHandler == NULL)
		{
			std::string message(" Callback doesn't exist");
			mvAppLog::getLogger()->LogWarning(name + message);
			PyGILState_Release(gstate);
			return;
		}

		// check if handler is callable
		if (PyCallable_Check(pHandler))
		{

			PyErr_Clear();

			PyObject* pArgs = PyTuple_New(1);
			PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(sender.c_str()));

			PyObject* result = PyObject_CallObject(pHandler, pArgs);
			
			// check if call succeded
			if (!result)
			{
				std::string message("Callback failed");
				mvAppLog::getLogger()->LogError(name + message);
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
			mvAppLog::getLogger()->LogError(name + message);
		}

		PyGILState_Release(gstate);
	}

	void mvApp::runCallbackP(const std::string& name, const std::string& sender, PyObject* data)
	{
		if (name.empty())
			return;

		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* pHandler = PyDict_GetItemString(m_pDict, name.c_str()); // borrowed reference

		// if callback doesn't exist
		if (pHandler == NULL)
		{
			std::string message(" Callback doesn't exist");
			mvAppLog::getLogger()->LogWarning(name + message);
			PyGILState_Release(gstate);
			return;
		}

		// check if handler is callable
		if (PyCallable_Check(pHandler))
		{

			PyErr_Clear();

			PyObject* pArgs = PyTuple_New(2);
			PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(sender.c_str()));
			PyTuple_SetItem(pArgs, 1, data);

			PyObject* result = PyObject_CallObject(pHandler, pArgs);

			// check if call succeded
			if (!result)
			{
				std::string message("Callback failed");
				mvAppLog::getLogger()->LogError(name + message);
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
			mvAppLog::getLogger()->LogError(name + message);
		}

		PyGILState_Release(gstate);
	}

	void mvApp::runCallbackD(const std::string& name, int sender, float data)
	{
		if (name.empty())
			return;

		PyGILState_STATE gstate = PyGILState_Ensure();

		PyErr_Clear();

		PyObject* pHandler = PyDict_GetItemString(m_pDict, name.c_str()); // borrowed reference

		// if callback doesn't exist
		if (pHandler == NULL)
		{
			std::string message(" Callback doesn't exist");
			mvAppLog::getLogger()->LogWarning(name + message);
			PyGILState_Release(gstate);
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
				mvAppLog::getLogger()->LogError(name + message);
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
			mvAppLog::getLogger()->LogError(name + message);
		}

		PyGILState_Release(gstate);

	}

	void mvApp::setAppTheme(const std::string& theme)
	{
		m_theme = theme;

		// TODO: All themes borrowed from online need to be
		//       cleaned up for consistency and completed.

		if (theme == "Dark")
			ImGui::StyleColorsDark();

		else if (theme == "Classic")
			ImGui::StyleColorsClassic();

		else if (theme == "Light")
			ImGui::StyleColorsLight();

		else if (theme == "Dark 2")
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style.FrameRounding = 2.3f;
			style.ScrollbarRounding = 0;

			style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
			style.Colors[ImGuiCol_Separator] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
			style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
			style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
			style.Colors[ImGuiCol_Tab] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
			style.Colors[ImGuiCol_TabHovered] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
			style.Colors[ImGuiCol_TabActive] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
			style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
			style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
			style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
		}

		else if (theme == "Dark Grey")
		{
			ImVec4* colors = ImGui::GetStyle().Colors;
			colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
			colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
			colors[ImGuiCol_Header] = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
			colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		}

		else if (theme == "Cherry")
		{
			auto& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_Text] = ImVec4(0.860f, 0.930f, 0.890f, 0.78f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.860f, 0.930f, 0.890f, 0.28f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
			style.Colors[ImGuiCol_ChildBg] = ImVec4(0.200f, 0.220f, 0.270f, 0.58f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.200f, 0.220f, 0.270f, 0.9f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.200f, 0.220f, 0.270f, 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.78f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.232f, 0.201f, 0.271f, 1.00f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.502f, 0.075f, 0.256f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.200f, 0.220f, 0.270f, 0.75f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.200f, 0.220f, 0.270f, 0.47f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.200f, 0.220f, 0.270f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.78f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.86f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.455f, 0.198f, 0.301f, 0.76f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.86f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.502f, 0.075f, 0.256f, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.78f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.860f, 0.930f, 0.890f, 0.63f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.860f, 0.930f, 0.890f, 0.63f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.455f, 0.198f, 0.301f, 0.43f);
			style.WindowPadding = ImVec2(6, 4);
			style.WindowRounding = 0.0f;
			style.FramePadding = ImVec2(5, 2);
			style.FrameRounding = 3.0f;
			style.ItemSpacing = ImVec2(7, 1);
			style.ItemInnerSpacing = ImVec2(1, 1);
			style.TouchExtraPadding = ImVec2(0, 0);
			style.IndentSpacing = 6.0f;
			style.ScrollbarSize = 12.0f;
			style.ScrollbarRounding = 16.0f;
			style.GrabMinSize = 20.0f;
			style.GrabRounding = 2.0f;
			style.WindowTitleAlign.x = 0.50f;
			style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
			style.FrameBorderSize = 0.0f;
			style.WindowBorderSize = 1.0f;
		}

		else if (theme == "Grey")
		{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		/// 0 = FLAT APPEARENCE
		/// 1 = MORE "3D" LOOK
		int is3D = 0;

		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
		colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
		colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);

		style.PopupRounding = 3;
		style.WindowPadding = ImVec2(4, 4);
		style.FramePadding = ImVec2(6, 4);
		style.ItemSpacing = ImVec2(6, 2);
		style.ScrollbarSize = 18;
		style.TabBorderSize = is3D;
		style.TabRounding = 3;
		style.WindowBorderSize = 1;
		style.ChildBorderSize = 1;
		style.PopupBorderSize = 1;
		style.FrameBorderSize = is3D;
		style.WindowRounding = 3;
		style.ChildRounding = 3;
		style.FrameRounding = 3;
		style.ScrollbarRounding = 2;
		style.GrabRounding = 3;
		}

		else if (theme == "Purple")
		{
		EditorColorScheme::SetColors(0x1F2421FF, 0xDCE1DEFF, 0x725AC1FF, 0x8D86C9FF, 0xECA400FF);
		EditorColorScheme::ApplyTheme();
		}

		else if (theme == "Gold")
		{
			ImGuiStyle* style = &ImGui::GetStyle();
			ImVec4* colors = style->Colors;

			colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
			colors[ImGuiCol_Header] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
			colors[ImGuiCol_Tab] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_TabActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

			style->FramePadding = ImVec2(4, 2);
			style->ItemSpacing = ImVec2(10, 2);
			style->IndentSpacing = 12;
			style->ScrollbarSize = 10;

			style->WindowRounding = 4;
			style->FrameRounding = 4;
			style->PopupRounding = 4;
			style->ScrollbarRounding = 6;
			style->GrabRounding = 4;
			style->TabRounding = 4;

			style->WindowTitleAlign = ImVec2(1.0f, 0.5f);
			style->WindowMenuButtonPosition = ImGuiDir_Right;

			style->DisplaySafeAreaPadding = ImVec2(4, 4);
		}

		else if (theme == "Red")
		{
			auto& style = ImGui::GetStyle();
			style.FrameRounding = 4.0f;
			style.WindowBorderSize = 0.0f;
			style.PopupBorderSize = 0.0f;
			style.GrabRounding = 4.0f;

			ImVec4* colors = ImGui::GetStyle().Colors;
			colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.94f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.40f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.67f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.47f, 0.22f, 0.22f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.34f, 0.16f, 0.16f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.71f, 0.39f, 0.39f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.66f, 0.66f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.47f, 0.22f, 0.22f, 0.65f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.71f, 0.39f, 0.39f, 0.65f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
			colors[ImGuiCol_Header] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.65f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.00f);
			colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
			colors[ImGuiCol_Tab] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
			colors[ImGuiCol_TabActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		}

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
		if (std::this_thread::get_id() != m_mainThreadID)
		{
			mvAppLog::getLogger()->LogWarning("Items can not be added outside main thread.");
			return;
		}

		static int count = 0;
		count++;

		assert(!m_started); // should not be callable during runtime

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
		if (parentitem == nullptr)
			parentitem = m_windows[0];

		item->setParent(parentitem);
		parentitem->addChild(item);
	}

	void mvApp::addWindow(mvAppItem* item)
	{
		if (std::this_thread::get_id() != m_mainThreadID)
		{
			mvAppLog::getLogger()->LogWarning("Items can not be added outside main thread.");
			return;
		}

		m_windows.push_back(item);
	}

	void mvApp::addData(const std::string& name, PyObject* data)
	{
		if (std::this_thread::get_id() != m_mainThreadID)
		{
			mvAppLog::getLogger()->LogWarning("Data can not be modified outside main thread.");
			return;
		}

		if (m_dataStorage.count(name) > 0)
		{
			deleteData(name);
			addData(name, data);
			for (auto window : m_windows)
				window->updateDataSource(name);
			return;
		}

		m_dataStorage.insert({ name, data });
		for (auto window : m_windows)
			window->updateDataSource(name);
	}

	PyObject* mvApp::getData(const std::string& name)
	{
		if (std::this_thread::get_id() != m_mainThreadID)
		{
			mvAppLog::getLogger()->LogWarning("Data can not be modified outside main thread.");
			return nullptr;
		}

		if (m_dataStorage.count(name) == 0)
		{
			mvAppLog::getLogger()->LogWarning(name + " does not exists in data storage.");
			return nullptr;
		}
		Py_XINCREF(m_dataStorage.at(name));
		return m_dataStorage.at(name);
	}

	void mvApp::deleteData(const std::string& name)
	{
		if (std::this_thread::get_id() != m_mainThreadID)
		{
			mvAppLog::getLogger()->LogWarning("Data can not be modified outside main thread.");
			return;
		}

		if (m_dataStorage.count(name) == 0)
		{
			mvAppLog::getLogger()->LogWarning(name + " does not exists in data storage.");
			return;
		}

		Py_XDECREF(m_dataStorage.at(name));
		m_dataStorage.erase(name);
		
	}
}