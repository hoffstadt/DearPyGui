#include "mvAppInterface.h"
#include "mvAppItem.h"
#include "mvWindow.h"
#include "mvEvents.h"
#include <ImGuiFileDialog.h>
#include "mvDataStorage.h"
#include <iostream>

namespace Marvel {

	void AddAppCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_texture", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::IntList, "data", "RGBA format"},
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "format", "mvTEX_XXXX_XXXXX constants", "0"},
		}, "Adds a texture.") });

		parsers->insert({ "decrement_texture", mvPythonParser({
			{mvPythonDataType::String, "name"},
		}, "Decrements a texture.") });

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

		parsers->insert({ "set_logger_window_title", mvPythonParser({
			{mvPythonDataType::String, "title"}
		}, "Sets the title of the logger window.") });

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

		parsers->insert({ "open_file_dialog", mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::Callable, "callback", "function to call on completion", "None"},
			{mvPythonDataType::String, "extensions", "filters items with extensions i.e '.*, .py'", "''"},
		}, "Opens an 'open file' dialog.") });

		parsers->insert({ "select_directory_dialog", mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::Callable, "callback", "function to call on completion", "None"},
		}, "Opens a select directory dialog.") });

		parsers->insert({ "add_data", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "data"}
		}, "Adds data for later retrieval.") });

		parsers->insert({ "get_data", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Retrieves data from storage.", "object") });

		parsers->insert({ "delete_data", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Deletes data from storage.") });

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

	void AddLogCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "get_log_level", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "logger", "logger widget", "''"},
		}, "Returns the log level.", "int", "Logging") });

		parsers->insert({ "clear_log", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "logger", "logger widget", "''"},
		}, "Clears the logger.", "None", "Logging") });

		parsers->insert({ "set_log_level", mvPythonParser({
			{mvPythonDataType::Integer, "level"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "logger", "logger widget", "''"},
		}, "Sets the log level.", "None", "Logging") });

		parsers->insert({ "log", mvPythonParser({
			{mvPythonDataType::Object, "message"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "level", "logger widget", "'TRACE'"},
			{mvPythonDataType::String, "logger", "logger widget", "''"}
		}, "Logs a trace level log.", "None", "Logging") });

		parsers->insert({ "log_debug", mvPythonParser({
			{mvPythonDataType::Object, "message"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "logger", "logger widget", "''"},
		}, "Logs a debug level log.", "None", "Logging") });

		parsers->insert({ "log_info", mvPythonParser({
			{mvPythonDataType::Object, "message"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "logger", "logger widget", "''"},
		}, "Logs a info level log.", "None", "Logging") });

		parsers->insert({ "log_warning", mvPythonParser({
			{mvPythonDataType::Object, "message"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "logger", "logger widget", "''"},
		}, "Logs a warning level log.", "None", "Logging") });

		parsers->insert({ "log_error", mvPythonParser({
			{mvPythonDataType::Object, "message"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "logger", "logger widget", "''"},
		}, "Logs a error level log.", "None", "Logging") });
	}

	void AddStdWindowCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "show_logger", mvPythonParser({
		}, "Shows the logging window. The Default log level is Trace", "None", "Standard Windows") });

		parsers->insert({ "close_popup", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Closes a popup.") });

	}

	PyObject* add_texture(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		// TO DO, FIX THIS MESS

		const char* name;
		PyObject* data;
		int width;
		int height;
		int format = 0;

		if (!(*mvApp::GetApp()->getParsers())["add_texture"].parse(args, kwargs, __FUNCTION__, 
			&name, &data, &width, &height, &format))
			return GetPyNone();

		mvTextureFormat tformat = (mvTextureFormat)format;
		std::vector<float> fdata;

		if (tformat == mvTextureFormat::RGBA_INT)
		{
			std::vector<int> mdata = ToIntVect(data);

			for (auto& item : mdata)
				fdata.push_back(item / 255.0f);
		}

		else if (tformat == mvTextureFormat::RGB_INT)
		{

			std::vector<int> mdata = ToIntVect(data);

			for (int i = 0; i < mdata.size()-2; i = i + 3)
			{
				fdata.push_back(mdata[i] / 255.0f);
				fdata.push_back(mdata[i + 1] / 255.0f);
				fdata.push_back(mdata[i + 2] / 255.0f);
				fdata.push_back(1.0f);
			}

		}

		else if (tformat == mvTextureFormat::RGBA_FLOAT)
			fdata = ToFloatVect(data);

		else if (tformat == mvTextureFormat::RGB_FLOAT)
		{
			std::vector<float> mdata = ToFloatVect(data);

			for (int i = 0; i < mdata.size()-2; i = i + 3)
			{
				fdata.push_back(mdata[i]);
				fdata.push_back(mdata[i + 1]);
				fdata.push_back(mdata[i + 2]);
				fdata.push_back(1.0f);
			}

		}

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		if (mvApp::IsAppStarted())
			mvApp::GetApp()->getTextureStorage().addTexture(name, fdata.data(), width, height, tformat);

		else
			mvApp::GetApp()->getTextureStorage().addDelayedTexture(name, fdata, width, height, tformat);


		return GetPyNone();
	}

	PyObject* enable_docking(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int shift_only = true;
		int dockspace = false;

		if (!(*mvApp::GetApp()->getParsers())["enable_docking"].parse(args, kwargs, __FUNCTION__,
			&shift_only, &dockspace))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->turnOnDocking(shift_only, dockspace);
			});

		return GetPyNone();
	}

	PyObject* decrement_texture(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["decrement_texture"].parse(args, kwargs, __FUNCTION__,
			&name))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		mvEventBus::PublishEndFrame(mvEVT_CATEGORY_TEXTURE, mvEVT_DEC_TEXTURE, { CreateEventArgument("NAME", std::string(name)) });

		return GetPyNone();
	}

	PyObject* is_dearpygui_running(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyBool(mvApp::IsAppStarted());
	}

	PyObject* set_main_window_title(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* title;

		if (!(*mvApp::GetApp()->getParsers())["set_main_window_title"].parse(args, kwargs, __FUNCTION__,
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

		if (!(*mvApp::GetApp()->getParsers())["set_main_window_pos"].parse(args, kwargs, __FUNCTION__,
			&x, &y))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		mvApp::GetApp()->setMainPos(x, y);

		return GetPyNone();
	}

	PyObject* add_character_remap(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int destination;
		int source;

		if (!(*mvApp::GetApp()->getParsers())["add_character_remap"].parse(args, kwargs, __FUNCTION__,
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

		if (!(*mvApp::GetApp()->getParsers())["set_main_window_resizable"].parse(args, kwargs, __FUNCTION__,
			&resizable))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		mvApp::GetApp()->setResizable(resizable);

		return GetPyNone();
	}

	PyObject* set_vsync(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int value;

		if (!(*mvApp::GetApp()->getParsers())["set_vsync"].parse(args, kwargs, __FUNCTION__,
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
		mvApp::SetAppStarted();

		// create window
		auto window = mvWindow::CreatemvWindow(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight(), false);
		window->show();
		mvApp::GetApp()->setViewport(window);
		window->setup();
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
		auto window = mvApp::GetApp()->getViewport();
		delete window;
		mvApp::GetApp()->setViewport(nullptr);
		Py_BEGIN_ALLOW_THREADS;
		mvApp::SetAppStopped();
		mvApp::DeleteApp();
		Py_END_ALLOW_THREADS;

		return GetPyNone();
	}

	PyObject* start_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* primary_window = "";

		if (!(*mvApp::GetApp()->getParsers())["start_dearpygui"].parse(args, kwargs, __FUNCTION__, &primary_window))
			return GetPyNone();
		if (mvApp::IsAppStarted())
		{
			ThrowPythonException("Cannot call \"start_dearpygui\" while a Dear PyGUI app is already running.");
			return GetPyNone();
		}

		Py_BEGIN_ALLOW_THREADS;
		mvApp::GetApp()->start(primary_window);
		Py_END_ALLOW_THREADS;
		
		mvApp::DeleteApp();
		mvEventBus::Reset();
		mvAppLog::Clear();

		return GetPyNone();
	}

	PyObject* set_start_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_start_callback"].parse(args, kwargs, __FUNCTION__, &callback))
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

		if (!(*mvApp::GetApp()->getParsers())["set_exit_callback"].parse(args, kwargs, __FUNCTION__, &callback))
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

		if (!(*mvApp::GetApp()->getParsers())["set_accelerator_callback"].parse(args, kwargs, __FUNCTION__, &callback))
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

	PyObject* select_directory_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;

		if (!(*mvApp::GetApp()->getParsers())["select_directory_dialog"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		if (callback)
			Py_XINCREF(callback);

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				igfd::ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose Directory", 0, ".");
				auto window = mvApp::GetApp()->getItemRegistry().getItem("filedialog");
				auto dialog = static_cast<mvFileDialog*>(window.get());
				dialog->setCallback(callback);
				window->show();
			});

		return GetPyNone();
	}

	PyObject* open_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;
		const char* extensions = ".*";

		if (!(*mvApp::GetApp()->getParsers())["open_file_dialog"].parse(args, kwargs, __FUNCTION__,
			&callback, &extensions))
			return GetPyNone();

		if (callback)
			Py_XINCREF(callback);

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				igfd::ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose File", extensions, ".");
				auto window = mvApp::GetApp()->getItemRegistry().getItem("filedialog");
				auto dialog = static_cast<mvFileDialog*>(window.get());

				dialog->setCallback(callback);
				window->show();
			});

		return GetPyNone();
	}

	PyObject* get_total_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		return ToPyFloat((float)mvApp::GetApp()->getTotalTime());
	}

	PyObject* get_delta_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		return ToPyFloat(mvApp::GetApp()->getDeltaTime());

	}

	PyObject* get_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		return ToPyPairII(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight());
	}

	PyObject* get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		return ToPyString(mvApp::GetApp()->getItemRegistry().getActiveWindow());
	}

	PyObject* get_dearpygui_version(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyString(mvApp::GetApp()->GetVersion());
	}

	PyObject* add_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;
		PyObject* data;

		if (!(*mvApp::GetApp()->getParsers())["add_data"].parse(args, kwargs, __FUNCTION__, &name, &data))
			return GetPyNone();

		Py_XINCREF(data);

		mvDataStorage::AddData(name, data);
		
		return GetPyNone();
	}

	PyObject* get_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["get_data"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		return mvDataStorage::GetDataIncRef(name);

	}

	PyObject* delete_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["delete_data"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		mvDataStorage::DeleteData(name);
		
		return GetPyNone();
	}

	PyObject* set_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int width;
		int height;

		if (!(*mvApp::GetApp()->getParsers())["set_main_window_size"].parse(args, kwargs, __FUNCTION__, &width, &height))
			return GetPyNone();


		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
			CreateEventArgument("actual_width", width),
			CreateEventArgument("actual_height", height),
			CreateEventArgument("client_width", width),
			CreateEventArgument("client_height", height)
			});

		return GetPyNone();
	}

	PyObject* get_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* logger = "";

		if (!(*mvApp::GetApp()->getParsers())["get_log_level"].parse(args, kwargs, __FUNCTION__, &logger))
			return GetPyNone();


		if (!std::string(logger).empty())
		{
			auto loggeritem = mvApp::GetApp()->getItemRegistry().getItem(logger);
			if (loggeritem == nullptr)
			{
				ThrowPythonException(std::string(logger) + " logger does not exist.");
				return ToPyInt(-1);
			}

			if (loggeritem->getType() != mvAppItemType::Logger)
			{
				ThrowPythonException(std::string(logger) + " is not a logger.");
				return ToPyInt(-1);
			}

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem.get());
			return ToPyInt(loggerwidget->getLogLevel());

		}
		else
			return ToPyInt((int)mvAppLog::getLogLevel());

	}

	PyObject* set_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int level;
		const char* logger = "";
		if (!(*mvApp::GetApp()->getParsers())["set_log_level"].parse(args, kwargs, __FUNCTION__, 
			&level, &logger))
			return GetPyNone();


		if (!std::string(logger).empty())
		{
			auto loggeritem = mvApp::GetApp()->getItemRegistry().getItem(logger);
			if (loggeritem == nullptr)
			{
				ThrowPythonException(std::string(logger) + " logger does not exist.");
				return GetPyNone();
			}

			if (loggeritem->getType() != mvAppItemType::Logger)
			{
				ThrowPythonException(std::string(logger) + " is not a logger.");
				return GetPyNone();
			}

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem.get());
			loggerwidget->setLogLevel(level);

		}
		else
			mvAppLog::setLogLevel(level);
		
		return GetPyNone();
	}

	PyObject* log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;
		const char* level = "TRACE";
		const char* logger = "";
		if (!(*mvApp::GetApp()->getParsers())["log"].parse(args, kwargs, __FUNCTION__, &message, &level, &logger))
			return GetPyNone();

		std::string cmessage = ToString(message);

		if (!std::string(logger).empty())
		{
			auto loggeritem = mvApp::GetApp()->getItemRegistry().getItem(logger);
			if (loggeritem == nullptr)
			{
				ThrowPythonException(std::string(logger) + " logger does not exist.");
				return GetPyNone();
			}

			if (loggeritem->getType() != mvAppItemType::Logger)
			{
				ThrowPythonException(std::string(logger) + " is not a logger.");
				return GetPyNone();
			}

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem.get());
			loggerwidget->Log(cmessage, std::string(level));

		}
		else
			mvAppLog::Log(cmessage, std::string(level));

		return GetPyNone();
	}

	PyObject* log_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;
		const char* logger = "";

		if (!(*mvApp::GetApp()->getParsers())["log_debug"].parse(args, kwargs, __FUNCTION__, &message, &logger))
			return GetPyNone();

		std::string cmessage = ToString(message);

		if (!std::string(logger).empty())
		{
			auto loggeritem = mvApp::GetApp()->getItemRegistry().getItem(logger);
			if (loggeritem == nullptr)
			{
				ThrowPythonException(std::string(logger) + " logger does not exist.");
				return GetPyNone();
			}

			if (loggeritem->getType() != mvAppItemType::Logger)
			{
				ThrowPythonException(std::string(logger) + " is not a logger.");
				return GetPyNone();
			}

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem.get());
			loggerwidget->LogDebug(cmessage);

		}
		else
			mvAppLog::LogDebug(cmessage);

		return GetPyNone();
	}

	PyObject* log_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;
		const char* logger = "";

		if (!(*mvApp::GetApp()->getParsers())["log_info"].parse(args, kwargs, __FUNCTION__, &message, &logger))
			return GetPyNone();

		std::string cmessage = ToString(message);

		if (!std::string(logger).empty())
		{
			auto loggeritem = mvApp::GetApp()->getItemRegistry().getItem(logger);
			if (loggeritem == nullptr)
			{
				ThrowPythonException(std::string(logger) + " logger does not exist.");
				return GetPyNone();
			}

			if (loggeritem->getType() != mvAppItemType::Logger)
			{
				ThrowPythonException(std::string(logger) + " is not a logger.");
				return GetPyNone();
			}

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem.get());
			loggerwidget->LogInfo(cmessage);

		}
		else
			mvAppLog::LogInfo(cmessage);

		return GetPyNone();
	}

	PyObject* log_warning(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;
		const char* logger = "";

		if (!(*mvApp::GetApp()->getParsers())["log_warning"].parse(args, kwargs, __FUNCTION__, &message, &logger))
			return GetPyNone();

		std::string cmessage = ToString(message);

		if (!std::string(logger).empty())
		{
			auto loggeritem = mvApp::GetApp()->getItemRegistry().getItem(logger);
			if (loggeritem == nullptr)
			{
				ThrowPythonException(std::string(logger) + " logger does not exist.");
				return GetPyNone();
			}

			if (loggeritem->getType() != mvAppItemType::Logger)
			{
				ThrowPythonException(std::string(logger) + " is not a logger.");
				return GetPyNone();
			}

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem.get());
			loggerwidget->LogWarning(cmessage);

		}
		else
			mvAppLog::LogWarning(cmessage);
		return GetPyNone();
	}

	PyObject* log_error(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;
		const char* logger = "";

		if (!(*mvApp::GetApp()->getParsers())["log_error"].parse(args, kwargs, __FUNCTION__, &message, &logger))
			return GetPyNone();

		std::string cmessage = ToString(message);
		if (!std::string(logger).empty())
		{
			auto loggeritem = mvApp::GetApp()->getItemRegistry().getItem(logger);
			if (loggeritem == nullptr)
			{
				ThrowPythonException(std::string(logger) + " logger does not exist.");
				return GetPyNone();
			}

			if (loggeritem->getType() != mvAppItemType::Logger)
			{
				ThrowPythonException(std::string(logger) + " is not a logger.");
				return GetPyNone();
			}

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem.get());
			loggerwidget->LogError(cmessage);

		}
		else
			mvAppLog::LogError(cmessage);
		return GetPyNone();
	}

	PyObject* clear_log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* logger = "";

		if (!(*mvApp::GetApp()->getParsers())["clear_log"].parse(args, kwargs, __FUNCTION__, &logger))
			return GetPyNone();

		if (!std::string(logger).empty())
		{
			auto loggeritem = mvApp::GetApp()->getItemRegistry().getItem(logger);
			if (loggeritem == nullptr)
			{
				ThrowPythonException(std::string(logger) + " logger does not exist.");
				return GetPyNone();
			}

			if (loggeritem->getType() != mvAppItemType::Logger)
			{
				ThrowPythonException(std::string(logger) + " is not a logger.");
				return GetPyNone();
			}

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem.get());
			loggerwidget->ClearLog();

		}
		else
			mvAppLog::ClearLog();

		return GetPyNone();
	}

	PyObject* show_logger(PyObject* self, PyObject* args)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		mvAppLog::Show();
		return GetPyNone();
	}

	PyObject* set_logger_window_title(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* title;

		if (!(*mvApp::GetApp()->getParsers())["set_logger_window_title"].parse(args, kwargs, __FUNCTION__,
			&title))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		mvAppLog::setTitle(title);

		return GetPyNone();
	}

	PyObject* close_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popup;

		if (!(*mvApp::GetApp()->getParsers())["close_popup"].parse(args, kwargs, __FUNCTION__, &popup))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());

		auto item = mvApp::GetApp()->getItemRegistry().getItem(popup);

		if (item == nullptr)
		{
			std::string message = popup;
			ThrowPythonException(message + " popup does not exist.");
			return GetPyNone();
		}

		mvPopup* pop;
		if (item->getType() == mvAppItemType::Popup)
			pop = static_cast<mvPopup*>(item.get());
		else
		{
			ThrowPythonException(std::string(popup) + " is not a popup.");
			return GetPyNone();
		}

		pop->closePopup();

		return GetPyNone();
	}

	PyObject* set_primary_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int value;

		if (!(*mvApp::GetApp()->getParsers())["set_primary_window"].parse(args, kwargs, __FUNCTION__, &item, &value))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		mvApp::GetApp()->getItemRegistry().setPrimaryWindow(item, value);
		
		return GetPyNone();
	}
}
