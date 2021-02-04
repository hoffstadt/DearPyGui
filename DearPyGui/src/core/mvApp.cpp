#include "mvApp.h"
#include "mvMarvel.h"
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
#include "mvTheme.h"
#include "mvCallbackRegistry.h"

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
                        ThrowPythonException("Window does not exists.", false);
                    });
		}

        m_future = std::async(std::launch::async, [&]() {return m_callbackRegistry->runCallbacks(); });

		m_viewport->run();

        GetApp()->getCallbackRegistry().stop();
#ifdef MV_CPP
        GetApp()->getCallbackRegistry().addCallback([]() {}, "null", nullptr);
#else
        GetApp()->getCallbackRegistry().addCallback(nullptr, "null", nullptr);
#endif // !MV_CPP

        
        m_future.get();
		delete m_viewport;
		s_started = false;
	}

	mvApp::mvApp()
	{

		mvEventBus::Subscribe(this, 0, mvEVT_CATEGORY_VIEWPORT);

		m_parsers = BuildDearPyGuiInterface();

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
		m_valueStorage = CreateOwnedPtr<mvValueStorage>();
		m_themeManager = CreateOwnedPtr<mvTheme>();
        m_callbackRegistry = CreateOwnedPtr<mvCallbackRegistry>();

	}

    mvCallbackRegistry& mvApp::getCallbackRegistry()
    { 
        return *m_callbackRegistry; 
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
#ifndef MV_CPP
        mvDataStorage::DeleteAllData();
#endif // !MV_CPP
	
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

}
