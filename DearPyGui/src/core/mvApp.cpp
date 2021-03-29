#include "mvApp.h"
#include "mvModule_Core.h"
#include "mvWindow.h"
#include "mvCallbackRegistry.h"
#include "mvInput.h"
#include <thread>
#include <future>
#include <chrono>
#include "mvThreadPool.h"
#include "mvAppItems.h"
#include "mvProfiler.h"
#include <implot.h>
#include "mvEventListener.h"
#include "mvImGuiThemeScope.h"
#include "mvCallbackRegistry.h"
#include "mvTextureStorage.h"
#include "mvItemRegistry.h"
#include "mvLog.h"

namespace Marvel {

	mvApp* mvApp::s_instance = nullptr;
	std::atomic_bool mvApp::s_started = false;


	// utility structure for realtime plot
	struct ScrollingBuffer {
		int MaxSize;
		int Offset;
		ImVector<ImVec2> Data;
		ScrollingBuffer() {
			MaxSize = 2000;
			Offset = 0;
			Data.reserve(MaxSize);
		}
		void AddPoint(float x, float y) {
			if (Data.size() < MaxSize)
				Data.push_back(ImVec2(x, y));
			else {
				Data[Offset] = ImVec2(x, y);
				Offset = (Offset + 1) % MaxSize;
			}
		}
		void Erase() {
			if (!Data.empty())
			{
				Data.shrink(0);
				Offset = 0;
			}
		}
	};

	mvApp* mvApp::GetApp()
	{
		mvLog::Init();

		if (s_instance)
			return s_instance;

		s_instance = new mvApp();
		return s_instance;
	}

	void mvApp::DeleteApp()
	{
		if (s_instance)
		{
			delete s_instance;
			s_instance = nullptr;
		}

		s_started = false;
	}

	void mvApp::SetAppStarted() 
	{
		s_started = true; 
        GetApp()->m_future = std::async(std::launch::async, []() {return GetApp()->m_callbackRegistry->runCallbacks(); });
	}

	void mvApp::SetAppStopped() 
	{ 

        GetApp()->getCallbackRegistry().stop();
        GetApp()->getCallbackRegistry().addCallback(nullptr, "null", nullptr);
        GetApp()->m_future.get();
		s_started = false; 
		auto viewport = s_instance->getViewport();
		if (viewport)
			viewport->stop();
	}

	void mvApp::start(const std::string& primaryWindow)
	{
		s_started = true;

		MV_CORE_INFO("application starting");

		// create window
		m_viewport = mvWindow::CreatemvWindow(m_actualWidth, m_actualHeight, false);
		m_viewport->show();

		if (!std::string(primaryWindow).empty())
		{
			// reset other windows
			for (auto window : m_itemRegistry->getFrontWindows())
			{
				if (window->m_core_config.name != primaryWindow)
                    dynamic_cast<mvWindowAppItem*>(window.get())->setWindowAsMainStatus(false);
			}

			mvWindowAppItem* window = m_itemRegistry->getWindow(primaryWindow);

			if (window)
				window->setWindowAsMainStatus(true);
			else
                mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
                    {
                        ThrowPythonException("Window does not exists.");
                    });
		}
	}

	void mvApp::cleanup()
	{
		getCallbackRegistry().stop();
		#ifdef MV_CPP
		        getCallbackRegistry().addCallback([]() {}, "null", nullptr);
		#else
		        getCallbackRegistry().addCallback(nullptr, "null", nullptr);
		#endif // !MV_CPP
		
		        
		m_future.get();
		delete m_viewport;
		s_started = false;
	}

	mvApp::mvApp()
	{
		mvEventBus::Subscribe(this, 0, mvEVT_CATEGORY_VIEWPORT);

		// info
        mvAppLog::Clear();
		mvAppLog::AddLog("[DearPyGui Version] %0s\n", mvApp::GetVersion());
		mvAppLog::AddLog("[DearImGui Version] %0s\n", IMGUI_VERSION);

#ifndef MV_CPP
        mvAppLog::AddLog("[Python Version] %0s\n", PY_VERSION);
#endif // !MV_CPP


		m_mainThreadID = std::this_thread::get_id();

#if defined(MV_DEBUG)
		new mvEventListener();
#endif // MV_PROFILE

		// create managers
		m_itemRegistry = CreateOwnedPtr<mvItemRegistry>();
		m_textureStorage = CreateOwnedPtr<mvTextureStorage>();
		m_themeManager = CreateOwnedPtr<mvThemeManager>();
        m_callbackRegistry = CreateOwnedPtr<mvCallbackRegistry>();

	}

    mvCallbackRegistry& mvApp::getCallbackRegistry()
    { 
        return *m_callbackRegistry; 
    }

	mvItemRegistry& mvApp::getItemRegistry() 
	{ 
		return *m_itemRegistry; 
	}

	mvTextureStorage& mvApp::getTextureStorage() 
	{ 
		return *m_textureStorage; 
	}

	mvThemeManager& mvApp::getThemeManager()
	{
		return *m_themeManager;
	}

	bool mvApp::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);

		dispatcher.dispatch(BIND_EVENT_METH(onViewPortResize), mvEVT_VIEWPORT_RESIZE);

		return event.handled;
	}

	bool mvApp::onViewPortResize(mvEvent& event)
	{
		m_actualWidth  = GetEInt(event, "actual_width");
		m_actualHeight = GetEInt(event, "actual_height");
		m_clientWidth  = GetEInt(event, "client_width");
		m_clientHeight = GetEInt(event, "client_height");

		m_callbackRegistry->addCallback(
			m_callbackRegistry->getResizeCallback(), 
			"Main Application", nullptr);

		return true;
	}

	mvApp::~mvApp()
	{
		m_itemRegistry->clearRegistry();

		mvApp::GetApp()->getTextureStorage().deleteAllTextures();

		mvLog::Cleanup();
	
	}

	void mvApp::turnOnDocking(bool shiftOnly, bool dockSpace)
	{ 
		m_docking = true; 
		m_dockingShiftOnly = shiftOnly; 
		m_dockingViewport = dockSpace;
	}

	void mvApp::render()
	{

		// update timing
		m_deltaTime = ImGui::GetIO().DeltaTime;
		m_time = ImGui::GetTime();
		ImGui::GetIO().FontGlobalScale = m_globalFontScale;

		if (m_dockingViewport)
			ImGui::DockSpaceOverViewport();

		mvEventBus::Publish(mvEVT_CATEGORY_APP, mvEVT_FRAME, {CreateEventArgument("FRAME", ImGui::GetFrameCount() )});

		mvAppLog::render();

		// route input callbacks
		mvInput::CheckInputs();

		m_textureStorage->show_debugger();

        std::lock_guard<std::mutex> lk(m_mutex);
		mvEventBus::Publish(mvEVT_CATEGORY_APP, mvEVT_PRE_RENDER);
		mvEventBus::Publish(mvEVT_CATEGORY_APP, mvEVT_PRE_RENDER_RESET);
		mvEventBus::Publish(mvEVT_CATEGORY_APP, mvEVT_RENDER);
		mvEventBus::Publish(mvEVT_CATEGORY_APP, mvEVT_END_FRAME);

#if defined(MV_PROFILE) && defined(MV_DEBUG)
        //mvEventBus::ShowDebug();
		postProfile();
#endif // MV_PROFILE
	}

	void mvApp::setMainPos(int x, int y)
	{

		m_mainXPos = x;
		m_mainYPos = y;
	}

	void mvApp::setGlobalFontScale(float scale)
	{
		m_globalFontScale = scale;
	}

	void mvApp::setFont(const std::string& file, float size, const std::string& glyphRange,
		std::vector<std::array<ImWchar, 3>> customRanges, std::vector<ImWchar> chars)
	{
		m_fontFile = file;
		m_fontGlyphRange = glyphRange;
		m_fontSize = size;
		m_fontGlyphRangeCustom = std::move(customRanges);
		m_fontGlyphChars = std::move(chars);
	}

	bool mvApp::checkIfMainThread() const
	{
		if (std::this_thread::get_id() != m_mainThreadID)
		{
			int line = PyFrame_GetLineNumber(PyEval_GetFrame());
			PyErr_Format(PyExc_Exception,
				"DearPyGui command on line %d can not be called asynchronously", line);
			PyErr_Print();
			return false;
		}
		return true;
	}

	void mvApp::postProfile()
	{
		static std::map<std::string, ScrollingBuffer> buffers;
		static float t = 0;
		t += ImGui::GetIO().DeltaTime;

		const auto& results = mvInstrumentor::Get().getResults();

		for (const auto& item : results)
			buffers[item.first].AddPoint(t, item.second.count());

		//ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Profiling", nullptr))
		{
			ImGui::End();
			return;
		}

		static float history = 10.0f;
		ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

		static ImPlotAxisFlags rt_axis = ImPlotAxisFlags_NoTickLabels;
		ImPlot::SetNextPlotLimitsX(t - history, t, ImGuiCond_Always);
		if (ImPlot::BeginPlot("##Scrolling", nullptr, nullptr, ImVec2(-1, -1), 0, rt_axis, 0 | ImPlotAxisFlags_LockMin)) {

			for (const auto& item : results)
				ImPlot::PlotLine(item.first.c_str(), &buffers[item.first].Data[0].x, &buffers[item.first].Data[0].y, buffers[item.first].Data.size(), buffers[item.first].Offset, 2 * sizeof(float));
			ImPlot::EndPlot();
		}

		ImGui::End();

	}

#ifdef MV_CPP
#else

	void AddAppCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_additional_font", mvPythonParser({
			{mvPythonDataType::String, "file", "ttf or otf file"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "size", "", "13.0"},
			{mvPythonDataType::String, "glyph_ranges", "options: korean, japanese, chinese_full, chinese_simplified_common, cryillic, thai, vietnamese", "''"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "custom_glyph_chars", "", "()"},
			{mvPythonDataType::Object, "custom_glyph_ranges", "list of ranges", "List[List[int]]"},
		}, "Adds additional font.", "None", "Themes and Styles") });

		parsers->insert({ "set_theme_color", mvPythonParser({
			{mvPythonDataType::Integer, "constant", "mvThemeCol_* constants"},
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "item", "", "''"}
		}, "Sets a color of a theme item for when the item is enabled.", "None", "Themes and Styles") });

		parsers->insert({ "set_theme_color_disabled", mvPythonParser({
			{mvPythonDataType::Integer, "constant", "mvThemeCol_* constants"},
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "item", "", "''"}
		}, "Sets a color of a theme item for when the item is disabled.", "None", "Themes and Styles") });

		parsers->insert({ "set_theme_style", mvPythonParser({
			{mvPythonDataType::Integer, "constant", "mvThemeStyle_* constants"},
			{mvPythonDataType::Float, "style"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "item", "", "''"}
		}, "Sets a style of a theme item.", "None", "Themes and Styles") });

		parsers->insert({ "end", mvPythonParser({
		}, "Ends a container.", "None", "Containers") });

		parsers->insert({ "enable_docking", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "shift_only", "press shift for docking", "True"},
			{mvPythonDataType::Bool, "dock_space", "add explicit dockspace over viewport", "False"},
		}, "Decrements a texture.") });

		parsers->insert({ "set_start_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback"},
		}, "Callback to run when starting main window.") });

		parsers->insert({ "set_exit_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback"},
		}, "Callback to run when exiting main window.") });

		parsers->insert({ "set_accelerator_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback"},
		}, "Callback similar to keypress but used for accelerator keys.") });

		parsers->insert({ "set_vsync", mvPythonParser({
			{mvPythonDataType::Bool, "value"},
		}, "Sets vsync on or off.") });

		parsers->insert({ "is_dearpygui_running", mvPythonParser({
		}, "Checks if dearpygui is still running", "bool") });

		parsers->insert({ "set_main_window_title", mvPythonParser({
			{mvPythonDataType::String, "title"}
		}, "Sets the title of the main window.") });

		parsers->insert({ "set_main_window_resizable", mvPythonParser({
			{mvPythonDataType::Bool, "resizable"}
		}, "Sets the main window to be resizable.") });

		parsers->insert({ "set_main_window_pos", mvPythonParser({
			{mvPythonDataType::Integer, "x"},
			{mvPythonDataType::Integer, "y"},
		}, "Sets the main window position.") });

		parsers->insert({ "add_character_remap", mvPythonParser({
			{mvPythonDataType::Integer, "destination"},
			{mvPythonDataType::Integer, "source"},
		}, "Remaps characters.") });

		parsers->insert({ "setup_dearpygui", mvPythonParser({
		}, "Sets up DearPyGui for user controlled rendering. Only call once and you must call cleanup_deapygui when finished.") });

		parsers->insert({ "render_dearpygui_frame", mvPythonParser({
		}, "Renders a DearPyGui frame. Should be called within a user's event loop. Must first call setup_dearpygui outside of event loop.") });

		parsers->insert({ "cleanup_dearpygui", mvPythonParser({
		}, "Cleans up DearPyGui after calling setup_dearpygui.") });

		parsers->insert({ "start_dearpygui", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "primary_window", "Window that will expand into viewport.", "''"},
		}, "Starts DearPyGui.") });

		parsers->insert({ "stop_dearpygui", mvPythonParser({
		}, "Stops DearPyGui.") });

		parsers->insert({ "set_global_font_scale", mvPythonParser({
			{mvPythonDataType::Float, "scale", "default is 1.0"}
		}, "Changes the global font scale.") });

		parsers->insert({ "get_global_font_scale", mvPythonParser({
		}, "Returns the global font scale.", "float") });

		parsers->insert({ "get_total_time", mvPythonParser({
		}, "Returns total time since app started.", "float") });

		parsers->insert({ "get_delta_time", mvPythonParser({
		}, "Returns time since last frame.", "float") });

		parsers->insert({ "get_main_window_size", mvPythonParser({
		}, "Returns the size of the main window.", "[int, int]") });

		parsers->insert({ "get_active_window", mvPythonParser({
		}, "Returns the active window name.", "str") });

		parsers->insert({ "get_dearpygui_version", mvPythonParser({
		}, "Returns the current version of Dear PyGui.", "str") });

		parsers->insert({ "set_main_window_size", mvPythonParser({
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"}
		}, "Sets the main window size.") });

		parsers->insert({ "set_primary_window", mvPythonParser({
			{mvPythonDataType::String, "window"},
			{mvPythonDataType::Bool, "value"},
		}, "Sets the primary window to fill the viewport.") });
	}

	PyObject* end(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->getItemRegistry().popParent();
		return GetPyNone();
	}

	PyObject* enable_docking(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int shift_only = true;
		int dockspace = false;

		if (!(mvApp::GetApp()->getParsers())["enable_docking"].parse(args, kwargs, __FUNCTION__,
			&shift_only, &dockspace))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->turnOnDocking(shift_only, dockspace);
			});

		return GetPyNone();
	}

	PyObject* is_dearpygui_running(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyBool(mvApp::IsAppStarted());
	}

	PyObject* set_main_window_title(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* title;

		if (!(mvApp::GetApp()->getParsers())["set_main_window_title"].parse(args, kwargs, __FUNCTION__,
			&title))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->setTitle(title);

				if (mvApp::IsAppStarted())
					mvApp::GetApp()->getViewport()->setWindowText(title);
			});

		return GetPyNone();
	}

	PyObject* set_main_window_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int x;
		int y;

		if (!(mvApp::GetApp()->getParsers())["set_main_window_pos"].parse(args, kwargs, __FUNCTION__,
			&x, &y))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->setMainPos(x, y);

		return GetPyNone();
	}

	PyObject* add_character_remap(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int destination;
		int source;

		if (!(mvApp::GetApp()->getParsers())["add_character_remap"].parse(args, kwargs, __FUNCTION__,
			&destination, &source))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->addRemapChar(destination, source);
			});

		return GetPyNone();
	}

	PyObject* set_main_window_resizable(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int resizable = true;

		if (!(mvApp::GetApp()->getParsers())["set_main_window_resizable"].parse(args, kwargs, __FUNCTION__,
			&resizable))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->setResizable(resizable);

		return GetPyNone();
	}

	PyObject* set_vsync(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int value;

		if (!(mvApp::GetApp()->getParsers())["set_vsync"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->setVSync(value);
			});

		return GetPyNone();
	}

	PyObject* setup_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		Py_BEGIN_ALLOW_THREADS;
		mvLog::Init();

		if (mvApp::IsAppStarted())
		{
			ThrowPythonException("Cannot call \"start_dearpygui\" while a Dear PyGUI app is already running.");
			return GetPyNone();
		}

		mvApp::SetAppStarted();

		mvApp::GetApp()->start("");
		Py_END_ALLOW_THREADS;

		return GetPyNone();
	}

	PyObject* render_dearpygui_frame(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		Py_BEGIN_ALLOW_THREADS;
		auto window = mvApp::GetApp()->getViewport();
		window->renderFrame();
		Py_END_ALLOW_THREADS;

		return GetPyNone();
	}

	PyObject* cleanup_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		Py_BEGIN_ALLOW_THREADS;
		mvApp::GetApp()->cleanup();
		mvApp::DeleteApp();
		mvEventBus::Reset();
		mvAppLog::Clear();
		Py_END_ALLOW_THREADS;

		return GetPyNone();
	}

	PyObject* set_start_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_start_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setOnStartCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* set_exit_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_exit_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setOnCloseCallback(callback);
			});
		return GetPyNone();
	}

	PyObject* set_accelerator_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_accelerator_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setAcceleratorCallback(callback);
			});
		return GetPyNone();
	}

	PyObject* stop_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::StopApp();
		auto viewport = mvApp::GetApp()->getViewport();
		if (viewport)
			viewport->stop();
		return GetPyNone();
	}

	PyObject* get_total_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		return ToPyFloat((float)mvApp::GetApp()->getTotalTime());
	}

	PyObject* get_delta_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		return ToPyFloat(mvApp::GetApp()->getDeltaTime());

	}

	PyObject* get_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		return ToPyPairII(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight());
	}

	PyObject* get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		return ToPyString(mvApp::GetApp()->getItemRegistry().getActiveWindow());
	}

	PyObject* get_dearpygui_version(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyString(mvApp::GetApp()->GetVersion());
	}

	PyObject* set_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int width;
		int height;

		if (!(mvApp::GetApp()->getParsers())["set_main_window_size"].parse(args, kwargs, __FUNCTION__, &width, &height))
			return GetPyNone();


		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
			CreateEventArgument("actual_width", width),
			CreateEventArgument("actual_height", height),
			CreateEventArgument("client_width", width),
			CreateEventArgument("client_height", height)
			});

		return GetPyNone();
	}

	PyObject* set_primary_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int value;

		if (!(mvApp::GetApp()->getParsers())["set_primary_window"].parse(args, kwargs, __FUNCTION__, &item, &value))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->getItemRegistry().setPrimaryWindow(item, value);

		return GetPyNone();
	}

	PyObject* set_theme_color(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		long constant;
		PyObject* color;
		const char* item = "";

		if (!(mvApp::GetApp()->getParsers())["set_theme_color"].parse(args, kwargs, __FUNCTION__, &constant, &color, &item))
			return GetPyNone();

		Py_XINCREF(color);
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());

		mvEventBus::Publish
		(
			mvEVT_CATEGORY_THEMES,
			SID("color_change"),
			{
				CreateEventArgument("WIDGET", std::string(item)),
				CreateEventArgument("ID", constant),
				CreateEventArgument("COLOR", ToColor(color)),
				CreateEventArgument("ENABLED", true)
			}
		);

		Py_XDECREF(color);


		return GetPyNone();
	}

	PyObject* set_theme_color_disabled(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		long constant;
		PyObject* color;
		const char* item = "";

		if (!(mvApp::GetApp()->getParsers())["set_theme_color_disabled"].parse(args, kwargs, __FUNCTION__, &constant, &color, &item))
			return GetPyNone();

		Py_XINCREF(color);
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvEventBus::Publish
				(
					mvEVT_CATEGORY_THEMES,
					SID("color_change"),
					{
						CreateEventArgument("WIDGET", std::string(item)),
						CreateEventArgument("ID", constant),
						CreateEventArgument("COLOR", ToColor(color)),
						CreateEventArgument("ENABLED", false)
					}
				);

				// to ensure the decrement happens on the python thread
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
					{
						Py_XDECREF(color);
					});

			});

		return GetPyNone();
	}

	PyObject* set_theme_style(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		long constant;
		float style;
		const char* item = "";

		if (!(mvApp::GetApp()->getParsers())["set_theme_style"].parse(args, kwargs, __FUNCTION__, &constant, &style, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvEventBus::Publish
				(
					mvEVT_CATEGORY_THEMES,
					SID("style_change"),
					{
						CreateEventArgument("WIDGET", std::string(item)),
						CreateEventArgument("ID", constant),
						CreateEventArgument("STYLE", style)
					}
				);
			});

		return GetPyNone();
	}

	PyObject* set_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float scale;

		if (!(mvApp::GetApp()->getParsers())["set_global_font_scale"].parse(args, kwargs, __FUNCTION__, &scale))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->setGlobalFontScale(scale);

		return GetPyNone();
	}

	PyObject* get_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyFloat(mvApp::GetApp()->getGlobalFontScale());
	}

	PyObject* add_additional_font(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* file;
		float size = 13.0f;
		const char* glyph_ranges = "";
		PyObject* custom_glyph_ranges = nullptr;
		PyObject* custom_glyph_chars = nullptr;


		if (!(mvApp::GetApp()->getParsers())["add_additional_font"].parse(args, kwargs, __FUNCTION__,
			&file, &size, &glyph_ranges, &custom_glyph_chars, &custom_glyph_ranges))
			return GetPyNone();

		std::vector<int> custom_chars = ToIntVect(custom_glyph_chars);
		std::vector<std::pair<int, int>> custom_ranges = ToVectInt2(custom_glyph_ranges);
		std::vector<std::array<ImWchar, 3>> imgui_custom_ranges;
		std::vector<ImWchar> imgui_custom_chars;

		for (auto& item : custom_ranges)
			imgui_custom_ranges.push_back({ (ImWchar)item.first, (ImWchar)item.second, 0 });
		for (auto& item : custom_chars)
			imgui_custom_chars.push_back((ImWchar)item);

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->setFont(file, size, glyph_ranges, imgui_custom_ranges, imgui_custom_chars);
			});

		return GetPyNone();
	}
#endif

}
