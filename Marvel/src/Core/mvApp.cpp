#include "mvApp.h"
#include "mvCore.h"
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

	void mvApp::setWindowSize(unsigned width, unsigned height) 
	{ 
		m_windows[0]->setWidth(width);
		m_windows[0]->setHeight(height);
	}

	void mvApp::addRuntimeItem(const std::string& parent, const std::string& before, mvAppItem* item) 
	{ 
		if (std::this_thread::get_id() != m_mainThreadID)
			mvAppLog::getLogger()->LogWarning("This function can't be called outside main thread.");

		m_newItemVec.push_back(NewRuntimeItem(parent, before, item)); 
	}

	void mvApp::precheck()
	{
		if (m_windows.size() == 1)
			popParent();

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
			mvAppLog::getLogger()->LogWarning(name + " already exists in data storage.");
			return;
		}

		m_dataStorage.insert({ name, data });
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