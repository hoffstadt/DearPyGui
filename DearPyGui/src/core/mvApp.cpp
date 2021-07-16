#include "mvApp.h"
#include "mvModule_DearPyGui.h"
#include "mvViewport.h"
#include "mvCallbackRegistry.h"
#include "mvInput.h"
#include <thread>
#include <future>
#include <chrono>
#include "mvProfiler.h"
#include <implot.h>
#include "mvItemRegistry.h"
#include "mvFontManager.h"
#include "mvCallbackRegistry.h"
#include "mvPythonTranslator.h"
#include "mvPythonExceptions.h"
#include "mvGlobalIntepreterLock.h"
#include <frameobject.h>
#include "mvModule_DearPyGui.h"
#include "mvLog.h"
#include "mvEventMacros.h"
#include "mvToolManager.h"
#include <imnodes.h>
#include <thread>
#include <stb_image.h>
#include "mvBuffer.h"

namespace Marvel {

	mvApp* mvApp::s_instance = nullptr;
	std::atomic_bool mvApp::s_started = false;
	std::atomic_bool mvApp::s_manualMutexControl = false;
	std::atomic_bool mvApp::s_waitOneFrame = false;
	float mvApp::s_deltaTime = 0.0f;
	int mvApp::s_frame = 0;
	double mvApp::s_time = 0.0;
	std::mutex mvApp::s_mutex = {};
	mvUUID mvApp::s_id = MV_START_UUID;

	mvApp* mvApp::GetApp()
	{
		
		if (s_instance)
			return s_instance;

		mvLog::Init();
		s_instance = new mvApp();
		s_id = MV_START_UUID;
		return s_instance;
	}

	mvUUID mvApp::GenerateUUID()
	{
		return ++s_id;
	}

	void mvApp::SetDefaultTheme()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text] = MV_BASE_COL_textColor;
		colors[ImGuiCol_TextDisabled] = MV_BASE_COL_textDisabledColor;
		colors[ImGuiCol_WindowBg] = mvImGuiCol_WindowBg;
		colors[ImGuiCol_ChildBg] = mvImGuiCol_ChildBg;
		colors[ImGuiCol_PopupBg] = mvImGuiCol_PopupBg;
		colors[ImGuiCol_Border] = mvImGuiCol_Border;
		colors[ImGuiCol_BorderShadow] = mvImGuiCol_BorderShadow;
		colors[ImGuiCol_FrameBg] = mvImGuiCol_FrameBg;
		colors[ImGuiCol_FrameBgHovered] = mvImGuiCol_FrameBgHovered;
		colors[ImGuiCol_FrameBgActive] = mvImGuiCol_FrameBgActive;
		colors[ImGuiCol_TitleBg] = mvImGuiCol_TitleBg;
		colors[ImGuiCol_TitleBgActive] = mvImGuiCol_TitleBgActive;
		colors[ImGuiCol_TitleBgCollapsed] = mvImGuiCol_TitleBgCollapsed;
		colors[ImGuiCol_MenuBarBg] = mvImGuiCol_MenuBarBg;
		colors[ImGuiCol_ScrollbarBg] = mvImGuiCol_ScrollbarBg;
		colors[ImGuiCol_ScrollbarGrab] = mvImGuiCol_ScrollbarGrab;
		colors[ImGuiCol_ScrollbarGrabHovered] = mvImGuiCol_ScrollbarGrabHovered;
		colors[ImGuiCol_ScrollbarGrabActive] = mvImGuiCol_ScrollbarGrabActive;
		colors[ImGuiCol_CheckMark] = mvImGuiCol_CheckMark;
		colors[ImGuiCol_SliderGrab] = mvImGuiCol_SliderGrab;
		colors[ImGuiCol_SliderGrabActive] = mvImGuiCol_SliderGrabActive;
		colors[ImGuiCol_Button] = mvImGuiCol_Button;
		colors[ImGuiCol_ButtonHovered] = mvImGuiCol_ButtonHovered;
		colors[ImGuiCol_ButtonActive] = mvImGuiCol_ButtonActive;
		colors[ImGuiCol_Header] = mvImGuiCol_Header;
		colors[ImGuiCol_HeaderHovered] = mvImGuiCol_HeaderHovered;
		colors[ImGuiCol_HeaderActive] = mvImGuiCol_HeaderActive;
		colors[ImGuiCol_Separator] = mvImGuiCol_Separator;
		colors[ImGuiCol_SeparatorHovered] = mvImGuiCol_SeparatorHovered;
		colors[ImGuiCol_SeparatorActive] = mvImGuiCol_SeparatorActive;
		colors[ImGuiCol_ResizeGrip] = mvImGuiCol_ResizeGrip;
		colors[ImGuiCol_ResizeGripHovered] = mvImGuiCol_ResizeGripHovered;
		colors[ImGuiCol_ResizeGripActive] = mvImGuiCol_ResizeGripHovered;
		colors[ImGuiCol_Tab] = mvImGuiCol_Tab;
		colors[ImGuiCol_TabHovered] = mvImGuiCol_TabHovered;
		colors[ImGuiCol_TabActive] = mvImGuiCol_TabActive;
		colors[ImGuiCol_TabUnfocused] = mvImGuiCol_TabUnfocused;
		colors[ImGuiCol_TabUnfocusedActive] = mvImGuiCol_TabUnfocusedActive;
		colors[ImGuiCol_DockingPreview] = mvImGuiCol_DockingPreview;
		colors[ImGuiCol_DockingEmptyBg] = mvImGuiCol_DockingEmptyBg;
		colors[ImGuiCol_PlotLines] = mvImGuiCol_PlotLines;
		colors[ImGuiCol_PlotLinesHovered] = mvImGuiCol_PlotLinesHovered;
		colors[ImGuiCol_PlotHistogram] = mvImGuiCol_PlotHistogram;
		colors[ImGuiCol_PlotHistogramHovered] = mvImGuiCol_PlotHistogramHovered;
		colors[ImGuiCol_TableHeaderBg] = mvImGuiCol_TableHeaderBg;
		colors[ImGuiCol_TableBorderStrong] = mvImGuiCol_TableBorderStrong;   // Prefer using Alpha=1.0 here
		colors[ImGuiCol_TableBorderLight] = mvImGuiCol_TableBorderLight;   // Prefer using Alpha=1.0 here
		colors[ImGuiCol_TableRowBg] = mvImGuiCol_TableRowBg;
		colors[ImGuiCol_TableRowBgAlt] = mvImGuiCol_TableRowBgAlt;
		colors[ImGuiCol_TextSelectedBg] = mvImGuiCol_TextSelectedBg;
		colors[ImGuiCol_DragDropTarget] = mvImGuiCol_DragDropTarget;
		colors[ImGuiCol_NavHighlight] = mvImGuiCol_NavHighlight;
		colors[ImGuiCol_NavWindowingHighlight] = mvImGuiCol_NavWindowingHighlight;
		colors[ImGuiCol_NavWindowingDimBg] = mvImGuiCol_NavWindowingDimBg;
		colors[ImGuiCol_ModalWindowDimBg] = mvImGuiCol_ModalWindowDimBg;

		imnodes::GetStyle().colors[imnodes::ColorStyle_NodeBackground] = mvColor::ConvertToUnsignedInt(mvColor(62, 62, 62, 255));
		imnodes::GetStyle().colors[imnodes::ColorStyle_NodeBackgroundHovered] = mvColor::ConvertToUnsignedInt(mvColor(75, 75, 75, 255));
		imnodes::GetStyle().colors[imnodes::ColorStyle_NodeBackgroundSelected] = mvColor::ConvertToUnsignedInt(mvColor(75, 75, 75, 255));
		imnodes::GetStyle().colors[imnodes::ColorStyle_NodeOutline] = mvColor::ConvertToUnsignedInt(mvColor(100, 100, 100, 255));
		imnodes::GetStyle().colors[imnodes::ColorStyle_TitleBar] = mvColor::ConvertToUnsignedInt(mvImGuiCol_TitleBg);
		imnodes::GetStyle().colors[imnodes::ColorStyle_TitleBarHovered] = mvColor::ConvertToUnsignedInt(mvImGuiCol_TitleBgActive);
		imnodes::GetStyle().colors[imnodes::ColorStyle_TitleBarSelected] = mvColor::ConvertToUnsignedInt(mvImGuiCol_TitleBgCollapsed);
		imnodes::GetStyle().colors[imnodes::ColorStyle_Link] = mvColor::ConvertToUnsignedInt(mvColor(255, 255, 255, 200));
		imnodes::GetStyle().colors[imnodes::ColorStyle_LinkHovered] = mvColor::ConvertToUnsignedInt(mvColor(66, 150, 250, 255));
		imnodes::GetStyle().colors[imnodes::ColorStyle_LinkSelected] = mvColor::ConvertToUnsignedInt(mvColor(66, 150, 250, 255));
		imnodes::GetStyle().colors[imnodes::ColorStyle_Pin] = mvColor::ConvertToUnsignedInt(mvColor(199, 199, 41, 255));
		imnodes::GetStyle().colors[imnodes::ColorStyle_PinHovered] = mvColor::ConvertToUnsignedInt(mvColor(255, 255, 50, 255));
		imnodes::GetStyle().colors[imnodes::ColorStyle_BoxSelector] = mvColor::ConvertToUnsignedInt(mvColor(61, 133, 224, 30));
		imnodes::GetStyle().colors[imnodes::ColorStyle_BoxSelectorOutline] = mvColor::ConvertToUnsignedInt(mvColor(61, 133, 224, 150));
		imnodes::GetStyle().colors[imnodes::ColorStyle_GridBackground] = mvColor::ConvertToUnsignedInt(mvColor(35, 35, 35, 255));
		imnodes::GetStyle().colors[imnodes::ColorStyle_GridLine] = mvColor::ConvertToUnsignedInt(mvColor(0, 0, 0, 255));

	}

	void mvApp::DeleteApp()
	{
		if (s_instance)
		{
			delete s_instance;
			s_instance = nullptr;
		}

		s_started = false;
		s_id = MV_START_UUID;
	}

	void mvApp::cleanup()
	{
		getCallbackRegistry().submitCallback([=]() {
			mvApp::GetApp()->getCallbackRegistry().stop();
			});
		m_future.get();
		if(m_viewport)
			delete m_viewport;
		s_started = false;
	}

	mvApp::mvApp()
	{
		// create managers
		m_itemRegistry = CreateOwnedPtr<mvItemRegistry>();
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

	mvApp::~mvApp()
	{
		m_itemRegistry->clearRegistry();

		mvLog::Cleanup();
	
	}

	void mvApp::turnOnDocking(bool dockSpace)
	{ 
		m_docking = true; 
		m_dockingViewport = dockSpace;
	}

	void mvApp::render()
	{

		// update timing
		s_deltaTime = ImGui::GetIO().DeltaTime;
		s_time = ImGui::GetTime();
		s_frame = ImGui::GetFrameCount();

		ImGui::GetIO().FontGlobalScale = mvToolManager::GetFontManager().getGlobalFontScale();

		if (m_dockingViewport)
			ImGui::DockSpaceOverViewport();


		mvEventBus::Publish(mvEVT_CATEGORY_APP, mvEVT_FRAME, {CreateEventArgument("FRAME", ImGui::GetFrameCount() )});


		// route input callbacks
		mvInput::CheckInputs();

		mvToolManager::Draw();

		{
			std::lock_guard<std::mutex> lk(s_mutex);
			if (m_resetTheme)
			{
				mvApp::SetDefaultTheme();
				m_resetTheme = false;
			}

			mvEventBus::Publish(mvEVT_CATEGORY_APP, mvEVT_PRE_RENDER);
			mvEventBus::Publish(mvEVT_CATEGORY_APP, mvEVT_PRE_RENDER_RESET);
			mvEventBus::Publish(mvEVT_CATEGORY_APP, mvEVT_RENDER);
			mvEventBus::Publish(mvEVT_CATEGORY_APP, mvEVT_END_FRAME);
		}

		if (s_waitOneFrame == true)
			s_waitOneFrame = false;
	}

	std::map<std::string, mvPythonParser>& mvApp::getParsers()
	{ 
		return const_cast<std::map<std::string, mvPythonParser>&>(mvModule_DearPyGui::GetModuleParsers());
	}

	void mvApp::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "General" });
			parser.addArg<mvPyDataType::Bool>("dock_space", mvArgType::KEYWORD_ARG, "False", "add explicit dockspace over viewport");
			parser.finalize();
			parsers->insert({ "enable_docking", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "set dpg.ini file.", { "General" });
			parser.addArg<mvPyDataType::String>("file", mvArgType::KEYWORD_ARG, "'dpg.ini'", "dpg.ini by default");
			parser.finalize();
			parsers->insert({ "set_init_file", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Load dpg.ini file.", { "General" });
			parser.addArg<mvPyDataType::String>("file");
			parser.finalize();
			parsers->insert({ "load_init_file", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Save dpg.ini file.", { "General" });
			parser.addArg<mvPyDataType::String>("file");
			parser.finalize();
			parsers->insert({ "save_init_file", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Resets to default theme.", { "General" });
			parser.finalize();
			parsers->insert({ "reset_default_theme", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Waits one frame.", { "General" });
			parser.addArg<mvPyDataType::Integer>("delay", mvArgType::KEYWORD_ARG, "32", "Minimal delay in in milliseconds");
			parser.finalize();
			parsers->insert({ "split_frame", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Integer, "Get frame count.", { "General"});
			parser.finalize();
			parsers->insert({ "get_frame_count", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Object, "Loads an image. Returns width, height, channels, mvBuffer", { "Textures", "Widgets" });
			parser.addArg<mvPyDataType::String>("file");
			parser.addArg<mvPyDataType::Float>("gamma", mvArgType::KEYWORD_ARG, "1.0", "Gamma correction factor. (default is 1.0 to avoid automatic gamma correction on loading.");
			parser.addArg<mvPyDataType::Float>("gamma_scale_factor", mvArgType::KEYWORD_ARG, "1.0", "Gamma scale factor.");
			parser.finalize();
			parsers->insert({ "load_image", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUID, "Generate a new UUID", { "General" });
			parser.finalize();
			parsers->insert({ "generate_uuid", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Locks mutex", { "General" });
			parser.finalize();
			parsers->insert({ "lock_mutex", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Unlocks mutex", { "General" });
			parser.finalize();
			parsers->insert({ "unlock_mutex", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Bool, "Checks if dearpygui is running.", { "General" });
			parser.finalize();
			parsers->insert({ "is_dearpygui_running", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Sets up dearpygui", { "General" });
			parser.addArg<mvPyDataType::String>("viewport", mvArgType::KEYWORD_ARG, "''");
			parser.finalize();
			parsers->insert({ "setup_dearpygui", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Renders a dearpygui frame.", { "General" });
			parser.finalize();
			parsers->insert({ "render_dearpygui_frame", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Cleans up dearpygui.", { "General" });
			parser.finalize();
			parsers->insert({ "cleanup_dearpygui", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Stops dearpygui.", { "General" });
			parser.finalize();
			parsers->insert({ "stop_dearpygui", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Float, "Returns total time since Dear PyGui has started.", { "General" });
			parser.finalize();
			parsers->insert({ "get_total_time", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Float, "Returns time since last frame.", { "General" });
			parser.finalize();
			parsers->insert({ "get_delta_time", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String, "Returns the dearpygui version.", { "General" });
			parser.finalize();
			parsers->insert({ "get_dearpygui_version", parser });
		}

	}

	PyObject* mvApp::set_init_file(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* file;

		if (!(mvApp::GetApp()->getParsers())["set_init_file"].parse(args, kwargs, __FUNCTION__,
			&file))
			return GetPyNone();

		mvApp::GetApp()->setIniFile(file);

		return GetPyNone();
	}

	PyObject* mvApp::reset_default_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->m_resetTheme = true;

		return GetPyNone();
	}

	PyObject* mvApp::load_init_file(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* file;

		if (!(mvApp::GetApp()->getParsers())["load_init_file"].parse(args, kwargs, __FUNCTION__,
			&file))
			return GetPyNone();

		mvApp::GetApp()->loadIniFile(file);

		return GetPyNone();
	}

	PyObject* mvApp::save_init_file(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* file;

		if (!(mvApp::GetApp()->getParsers())["save_init_file"].parse(args, kwargs, __FUNCTION__,
			&file))
			return GetPyNone();

		if (mvApp::IsAppStarted())
			ImGui::SaveIniSettingsToDisk(file);
		else
			mvThrowPythonError(mvErrorCode::mvNone, "Dear PyGui must be started to use \"save_init_file\".");

		return GetPyNone();
	}

	PyObject* mvApp::split_frame(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int delay = 32;

		if (!(mvApp::GetApp()->getParsers())["split_frame"].parse(args, kwargs, __FUNCTION__,
			&delay))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		Py_BEGIN_ALLOW_THREADS;
		mvApp::s_waitOneFrame = true;
		while (s_waitOneFrame)
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		Py_END_ALLOW_THREADS;

		return GetPyNone();
	}

	PyObject* mvApp::lock_mutex(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::s_mutex.lock();
		mvApp::s_manualMutexControl = true;

		return GetPyNone();
	}

	PyObject* mvApp::unlock_mutex(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::s_mutex.unlock();
		mvApp::s_manualMutexControl = false;

		return GetPyNone();
	}

	PyObject* mvApp::get_frame_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int frame = 0;

		if (!(mvApp::GetApp()->getParsers())["get_frame_count"].parse(args, kwargs, __FUNCTION__,
			&frame))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		return ToPyInt(mvApp::s_frame);
	}

	PyObject* mvApp::enable_docking(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int dockspace = false;

		if (!(mvApp::GetApp()->getParsers())["enable_docking"].parse(args, kwargs, __FUNCTION__,
			&dockspace))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->turnOnDocking(dockspace);
			});

		return GetPyNone();
	}

	PyObject* mvApp::load_image(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* file;
		float gamma = 1.0f;
		float gamma_scale = 1.0f;

		if (!(mvApp::GetApp()->getParsers())["load_image"].parse(args, kwargs, __FUNCTION__,
			&file, &gamma, &gamma_scale))
			return GetPyNone();

		// Vout = (Vin / 255)^v; Where v = gamma

		if (stbi_is_hdr(file))
		{
			stbi_hdr_to_ldr_gamma(gamma);
			stbi_hdr_to_ldr_scale(gamma_scale);
		}
		else
		{
			stbi_ldr_to_hdr_gamma(gamma);
			stbi_ldr_to_hdr_scale(gamma_scale);
		}
		

		// Load from disk into a raw RGBA buffer
		int image_width = 0;
		int image_height = 0;

		// automatic gamma correction
		float* image_data = stbi_loadf(file, &image_width, &image_height, NULL, 4);
		if (image_data == NULL)
			return GetPyNone();

		PyObject* newbuffer = nullptr;
		PymvBuffer* newbufferview = nullptr;
		newbufferview = PyObject_New(PymvBuffer, &PymvBufferType);
		newbufferview->arr.length = image_width * image_height * 4;
		newbufferview->arr.data = (float*)image_data;
		newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvBufferType);

		PyObject* result = PyTuple_New(4);
		PyTuple_SetItem(result, 0, Py_BuildValue("i", image_width));
		PyTuple_SetItem(result, 1, Py_BuildValue("i", image_height));
		PyTuple_SetItem(result, 2, PyLong_FromLong(4));
		PyTuple_SetItem(result, 3, newbuffer);

		return result;
	}

	PyObject* mvApp::is_dearpygui_running(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyBool(mvApp::IsAppStarted());
	}

	PyObject* mvApp::setup_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* viewport = "";

		if (!(mvApp::GetApp()->getParsers())["setup_dearpygui"].parse(args, kwargs, __FUNCTION__,
			&viewport))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		Py_BEGIN_ALLOW_THREADS;
		mvLog::Init();

		if (mvApp::IsAppStarted())
		{
			mvThrowPythonError(mvErrorCode::mvNone, "Cannot call \"setup_dearpygui\" while a Dear PyGUI app is already running.");
			return GetPyNone();
		}

		GetApp()->getItemRegistry().emptyParents();
		s_started = true;
		GetApp()->m_future = std::async(std::launch::async, []() {return GetApp()->m_callbackRegistry->runCallbacks(); });

		MV_CORE_INFO("application starting");

		Py_END_ALLOW_THREADS;

		return GetPyNone();
	}

	PyObject* mvApp::render_dearpygui_frame(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_PROFILE_SCOPE("Frame")

		Py_BEGIN_ALLOW_THREADS;
		auto window = mvApp::GetApp()->getViewport();
		window->renderFrame();
		Py_END_ALLOW_THREADS;

		return GetPyNone();
	}

	PyObject* mvApp::cleanup_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		Py_BEGIN_ALLOW_THREADS;
		mvApp::GetApp()->cleanup();	
		mvApp::DeleteApp();
		mvEventBus::Reset();
		Py_END_ALLOW_THREADS;

		return GetPyNone();
	}

	PyObject* mvApp::stop_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::StopApp();
		auto viewport = mvApp::GetApp()->getViewport();
		if (viewport)
			viewport->stop();
		return GetPyNone();
	}

	PyObject* mvApp::get_total_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if(!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		return ToPyFloat((float)mvApp::s_time);
	}

	PyObject* mvApp::get_delta_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		return ToPyFloat(mvApp::s_deltaTime);

	}

	PyObject* mvApp::get_dearpygui_version(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyString(mvApp::GetApp()->GetVersion());
	}

	PyObject* mvApp::generate_uuid(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyUUID(mvApp::GenerateUUID());
	}
}
