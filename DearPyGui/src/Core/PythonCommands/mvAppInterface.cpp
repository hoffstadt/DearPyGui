#include "mvAppInterface.h"
#include "mvInterfaceCore.h"
#include "Core/mvAppItems.h"
#include "Core/mvWindow.h"
#include <ImGuiFileDialog.h>

namespace Marvel {

	void AddAppCommands(std::map<std::string, mvPythonParser>* parsers)
	{
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

		parsers->insert({ "setup_dearpygui", mvPythonParser({
		}, "Sets up DearPyGui for user controlled rendering. Only call once and you must call cleanup_deapygui when finished.") });

		parsers->insert({ "render_dearpygui_frame", mvPythonParser({
		}, "Renders a DearPyGui frame. Should be called within a user's event loop. Must first call setup_dearpygui outside of event loop.") });

		parsers->insert({ "cleanup_dearpygui", mvPythonParser({
		}, "Cleans up DearPyGui after calling setup_dearpygui.") });

		parsers->insert({ "start_dearpygui", mvPythonParser({
		}, "Starts DearPyGui.") });

		parsers->insert({ "stop_dearpygui", mvPythonParser({
		}, "Stops DearPyGui.") });

		parsers->insert({ "set_global_font_scale", mvPythonParser({
			{mvPythonDataType::Float, "scale", "default is 1.0"}
		}, "Changes the global font scale.") });

		parsers->insert({ "get_global_font_scale", mvPythonParser({
		}, "Returns the global font scale.", "float") });

		parsers->insert({ "run_async_function", mvPythonParser({
			{mvPythonDataType::Object, "name"},
			{mvPythonDataType::Object, "data", "Data that will be sent into the async function."},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Callable, "return_handler", "function called on completion", "None"},
		}, "Runs a function asyncronously.") });

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

		parsers->insert({ "get_thread_count", mvPythonParser({
		}, "Returns the allowable thread count.", "int") });

		parsers->insert({ "is_threadpool_high_performance", mvPythonParser({
		}, "Checks if the threadpool is allowed to use the maximum number of threads.", "bool") });

		parsers->insert({ "get_threadpool_timeout", mvPythonParser({
		}, "Returns the threadpool timeout in seconds.", "float") });

		parsers->insert({ "get_active_window", mvPythonParser({
		}, "Returns the active window name.", "str") });

		parsers->insert({ "get_dearpygui_version", mvPythonParser({
		}, "Returns the current version of Dear PyGui.", "str") });

		parsers->insert({ "set_threadpool_timeout", mvPythonParser({
			{mvPythonDataType::Float, "time"}
		}, "Sets the threadpool timeout.") });

		parsers->insert({ "set_thread_count", mvPythonParser({
			{mvPythonDataType::Integer, "threads"}
		}, "Sets number of threads to use if the threadpool is active.") });

		parsers->insert({ "set_threadpool_high_performance", mvPythonParser({
		}, "Set the thread count to the maximum number of threads on your computer.") });

		parsers->insert({ "set_main_window_size", mvPythonParser({
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"}
		}, "Sets the main window size.") });
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
		}, "Closes the current popup") });
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

		mvApp::GetApp()->setTitle(title);

		if (mvApp::IsAppStarted())
			mvApp::GetApp()->getViewport()->setWindowText(title);

		return GetPyNone();
	}

	PyObject* set_main_window_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int x;
		int y;

		if (!(*mvApp::GetApp()->getParsers())["set_main_window_pos"].parse(args, kwargs, __FUNCTION__,
			&x, &y))
			return GetPyNone();

		mvApp::GetApp()->setMainPos(x, y);

		return GetPyNone();
	}

	PyObject* set_main_window_resizable(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int resizable = true;

		if (!(*mvApp::GetApp()->getParsers())["set_main_window_resizable"].parse(args, kwargs, __FUNCTION__,
			&resizable))
			return GetPyNone();

		mvApp::GetApp()->setResizable(resizable);

		return GetPyNone();
	}

	PyObject* set_vsync(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		int value;

		if (!(*mvApp::GetApp()->getParsers())["set_vsync"].parse(args, kwargs, __FUNCTION__,
			&value))
			return GetPyNone();

		mvApp::GetApp()->setVSync(value);
		return GetPyNone();
	}

	PyObject* setup_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvApp::GetApp()->precheck();
		mvApp::SetAppStarted();

		// create window
		auto window = mvWindow::CreatemvWindow(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight(), false);
		window->show();
		mvApp::GetApp()->setViewport(window);
		window->setup();

		return GetPyNone();
	}

	PyObject* render_dearpygui_frame(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		auto window = mvApp::GetApp()->getViewport();
		window->renderFrame();

		return GetPyNone();
	}

	PyObject* cleanup_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		auto window = mvApp::GetApp()->getViewport();
		delete window;
		mvApp::GetApp()->setViewport(nullptr);
		mvApp::SetAppStopped();
		mvApp::DeleteApp();

		return GetPyNone();
	}

	PyObject* start_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvApp::GetApp()->precheck();
		mvApp::SetAppStarted();

		// create window
		auto window = mvWindow::CreatemvWindow(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight(), false);
		mvApp::GetApp()->setViewport(window);
		window->show();
		window->run();
		delete window;
		mvApp::SetAppStopped();
		mvApp::DeleteApp();

		return GetPyNone();
	}

	PyObject* set_start_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_start_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		mvApp::GetApp()->setOnStartCallback(callback);
		return GetPyNone();
	}

	PyObject* set_exit_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_exit_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		mvApp::GetApp()->setOnCloseCallback(callback);
		return GetPyNone();
	}

	PyObject* set_accelerator_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_accelerator_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		mvApp::GetApp()->setAcceleratorCallback(callback);
		return GetPyNone();
	}

	PyObject* stop_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::SetAppStopped();
		return GetPyNone();
	}

	PyObject* run_async_function(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		PyObject* return_handler = nullptr;
		PyObject* data;

		if (!(*mvApp::GetApp()->getParsers())["run_async_function"].parse(args, kwargs, __FUNCTION__, &callback, &data, &return_handler))
			return GetPyNone();

		mvApp::GetApp()->addMTCallback(callback, data, return_handler);

		return GetPyNone();

	}

	PyObject* select_directory_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;

		if (!(*mvApp::GetApp()->getParsers())["select_directory_dialog"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		igfd::ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose Directory", 0, ".");
		mvAppItem* window = mvApp::GetApp()->getItem("filedialog");
		auto dialog = static_cast<mvFileDialog*>(window);
		if (callback)
			Py_XINCREF(callback);
		dialog->setCallback(callback);
		window->show();

		return GetPyNone();
	}

	PyObject* open_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;
		const char* extensions = ".*";

		if (!(*mvApp::GetApp()->getParsers())["open_file_dialog"].parse(args, kwargs, __FUNCTION__,
			&callback, &extensions))
			return GetPyNone();

		igfd::ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose File", extensions, ".");
		mvAppItem* window = mvApp::GetApp()->getItem("filedialog");
		auto dialog = static_cast<mvFileDialog*>(window);
		if (callback)
			Py_XINCREF(callback);
		dialog->setCallback(callback);
		window->show();

		return GetPyNone();
	}

	PyObject* get_total_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyFloat(mvApp::GetApp()->getTotalTime());
	}

	PyObject* get_delta_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyFloat(mvApp::GetApp()->getDeltaTime());
	}

	PyObject* get_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyPairII(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight());
	}

	PyObject* get_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyInt(mvApp::GetApp()->getThreadCount());
	}

	PyObject* is_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyBool(mvApp::GetApp()->usingThreadPoolHighPerformance());
	}

	PyObject* get_threadpool_timeout(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyFloat(mvApp::GetApp()->getThreadPoolTimeout());
	}

	PyObject* get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyString(mvApp::GetApp()->getActiveWindow());
	}

	PyObject* get_dearpygui_version(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyString(mvApp::GetApp()->GetVersion());
	}

	PyObject* set_threadpool_timeout(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float time;

		if (!(*mvApp::GetApp()->getParsers())["set_threadpool_timeout"].parse(args, kwargs, __FUNCTION__, &time))
			return GetPyNone();

		mvApp::GetApp()->setThreadPoolTimeout(time);

		return GetPyNone();
	}

	PyObject* set_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int threads;

		if (!(*mvApp::GetApp()->getParsers())["set_thread_count"].parse(args, kwargs, __FUNCTION__, &threads))
			return GetPyNone();

		mvApp::GetApp()->setThreadCount(threads);

		return GetPyNone();
	}

	PyObject* add_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;
		PyObject* data;

		if (!(*mvApp::GetApp()->getParsers())["add_data"].parse(args, kwargs, __FUNCTION__, &name, &data))
			return GetPyNone();

		mvDataStorage::AddData(name, data);
		Py_XINCREF(data);

		return GetPyNone();
	}

	PyObject* get_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["get_data"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		auto result = mvDataStorage::GetDataIncRef(name);

		if (result)
			return result;

		return GetPyNone();
	}

	PyObject* delete_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["delete_data"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		mvDataStorage::DeleteData(name);

		return GetPyNone();
	}

	PyObject* set_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->setThreadPoolHighPerformance();
		return GetPyNone();
	}

	PyObject* set_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int width;
		int height;

		if (!(*mvApp::GetApp()->getParsers())["set_main_window_size"].parse(args, kwargs, __FUNCTION__, &width, &height))
			return GetPyNone();

		mvApp::GetApp()->setActualSize(width, height);

		return GetPyNone();
	}

	PyObject* get_log_level(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* logger = "";

		if (!(*mvApp::GetApp()->getParsers())["get_log_level"].parse(args, kwargs, __FUNCTION__, &logger))
			return GetPyNone();

		if (!std::string(logger).empty())
		{
			mvAppItem* loggeritem = mvApp::GetApp()->getItem(logger);
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

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem);
			return ToPyInt(loggerwidget->getLogLevel());

		}
		return ToPyInt(mvAppLog::getLogLevel());
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
			mvAppItem* loggeritem = mvApp::GetApp()->getItem(logger);
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

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem);
			loggerwidget->setLogLevel(level);
			return GetPyNone();

		}

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

		if (!std::string(logger).empty())
		{
			mvAppItem* loggeritem = mvApp::GetApp()->getItem(logger);
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

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem);
			loggerwidget->Log(ToString(message), std::string(level));
			return GetPyNone();

		}

		mvAppLog::Log(ToString(message), std::string(level));
		return GetPyNone();
	}

	PyObject* log_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;
		const char* logger = "";

		if (!(*mvApp::GetApp()->getParsers())["log_debug"].parse(args, kwargs, __FUNCTION__, &message, &logger))
			return GetPyNone();

		if (!std::string(logger).empty())
		{
			mvAppItem* loggeritem = mvApp::GetApp()->getItem(logger);
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

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem);
			loggerwidget->LogDebug(ToString(message));
			return GetPyNone();

		}

		mvAppLog::LogDebug(ToString(message));
		return GetPyNone();
	}

	PyObject* log_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;
		const char* logger = "";

		if (!(*mvApp::GetApp()->getParsers())["log_info"].parse(args, kwargs, __FUNCTION__, &message, &logger))
			return GetPyNone();

		if (!std::string(logger).empty())
		{
			mvAppItem* loggeritem = mvApp::GetApp()->getItem(logger);
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

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem);
			loggerwidget->LogInfo(ToString(message));
			return GetPyNone();
		}

		mvAppLog::LogInfo(ToString(message));
		return GetPyNone();
	}

	PyObject* log_warning(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;
		const char* logger = "";

		if (!(*mvApp::GetApp()->getParsers())["log_warning"].parse(args, kwargs, __FUNCTION__, &message, &logger))
			return GetPyNone();

		if (!std::string(logger).empty())
		{
			mvAppItem* loggeritem = mvApp::GetApp()->getItem(logger);
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

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem);
			loggerwidget->LogWarning(ToString(message));
			return GetPyNone();
		}

		mvAppLog::LogWarning(ToString(message));
		return GetPyNone();
	}

	PyObject* log_error(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* message;
		const char* logger = "";

		if (!(*mvApp::GetApp()->getParsers())["log_error"].parse(args, kwargs, __FUNCTION__, &message, &logger))
			return GetPyNone();

		if (!std::string(logger).empty())
		{
			mvAppItem* loggeritem = mvApp::GetApp()->getItem(logger);
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

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem);
			loggerwidget->LogError(ToString(message));
			return GetPyNone();
		}

		mvAppLog::LogError(ToString(message));
		return GetPyNone();
	}

	PyObject* clear_log(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* logger = "";

		if (!(*mvApp::GetApp()->getParsers())["clear_log"].parse(args, kwargs, __FUNCTION__, &logger))
			return GetPyNone();

		if (!std::string(logger).empty())
		{
			mvAppItem* loggeritem = mvApp::GetApp()->getItem(logger);
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

			auto loggerwidget = static_cast<mvLoggerItem*>(loggeritem);
			loggerwidget->ClearLog();
			return GetPyNone();
		}

		mvAppLog::ClearLog();
		return GetPyNone();
	}

	PyObject* show_logger(PyObject* self, PyObject* args)
	{
		mvAppLog::Show();
		return GetPyNone();
	}

	PyObject* close_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::IsAppStarted())
			return GetPyNone();

		ImGui::CloseCurrentPopup();

		return GetPyNone();
	}

}
