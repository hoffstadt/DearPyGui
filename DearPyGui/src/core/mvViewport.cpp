#include "mvViewport.h"
#include "mvCallbackRegistry.h"
#include "mvPythonTranslator.h"
#include <string>
#include <array>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "mvPythonExceptions.h"
#include "mvPyObject.h"

namespace Marvel {

	mvViewport::mvViewport(unsigned width, unsigned height) :
		_width(width), _height(height)
	{
		_app = mvApp::GetApp();
	}

	void mvViewport::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::String, "Creates a viewport.", { "General" });
			parser.addArg<mvPyDataType::String>("title", mvArgType::KEYWORD_ARG, "'Dear PyGui'");
			parser.addArg<mvPyDataType::String>("small_icon", mvArgType::KEYWORD_ARG, "''");
			parser.addArg<mvPyDataType::String>("large_icon", mvArgType::KEYWORD_ARG, "''");

			parser.addArg<mvPyDataType::Integer>("width", mvArgType::KEYWORD_ARG, "1280");
			parser.addArg<mvPyDataType::Integer>("height", mvArgType::KEYWORD_ARG, "800");
			parser.addArg<mvPyDataType::Integer>("x_pos", mvArgType::KEYWORD_ARG, "100");
			parser.addArg<mvPyDataType::Integer>("y_pos", mvArgType::KEYWORD_ARG, "100");
			parser.addArg<mvPyDataType::Integer>("min_width", mvArgType::KEYWORD_ARG, "250");
			parser.addArg<mvPyDataType::Integer>("max_width", mvArgType::KEYWORD_ARG, "10000");
			parser.addArg<mvPyDataType::Integer>("min_height", mvArgType::KEYWORD_ARG, "250");
			parser.addArg<mvPyDataType::Integer>("max_height", mvArgType::KEYWORD_ARG, "10000");

			parser.addArg<mvPyDataType::Bool>("resizable", mvArgType::KEYWORD_ARG, "True");
			parser.addArg<mvPyDataType::Bool>("vsync", mvArgType::KEYWORD_ARG, "True");
			parser.addArg<mvPyDataType::Bool>("always_on_top", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("decorated", mvArgType::KEYWORD_ARG, "True");

			parser.addArg<mvPyDataType::FloatList>("clear_color", mvArgType::KEYWORD_ARG, "(0, 0, 0, 255)");

			parser.finalize();
			parsers->insert({ "create_viewport", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Shows the viewport", { "General" });
			parser.addArg<mvPyDataType::Bool>("minimized", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("maximized", mvArgType::KEYWORD_ARG, "False");
			parser.finalize();
			parsers->insert({ "show_viewport", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Configures the viewport.", { "General" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addKwargs();
			parser.makeInternal();
			parser.finalize();
			parsers->insert({ "configure_viewport", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Dict, "Returns viewport configuration.", { "General" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "get_viewport_configuration", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Bool, "Checks if a viewport has been created and shown.", { "General" });
			parser.finalize();
			parsers->insert({ "is_viewport_ok", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Maximizes the viewport.", { "General" });
			parser.finalize();
			parsers->insert({ "maximize_viewport", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Minimizes the viewport.", { "General" });
			parser.finalize();
			parsers->insert({ "minimize_viewport", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Toggles fullscreen on/off.", { "General" });
			parser.finalize();
			parsers->insert({ "toggle_fulscreen", parser });
		}

	}

	void mvViewport::onResizeEvent()
	{
		mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
			PyObject* dimensions = PyTuple_New(4);
			PyTuple_SetItem(dimensions, 0, PyLong_FromLong(_actualWidth));
			PyTuple_SetItem(dimensions, 1, PyLong_FromLong(_actualHeight));
			PyTuple_SetItem(dimensions, 2, PyLong_FromLong(_clientWidth));
			PyTuple_SetItem(dimensions, 3, PyLong_FromLong(_clientHeight));
			mvApp::GetApp()->getCallbackRegistry().addCallback(
				mvApp::GetApp()->getCallbackRegistry().getResizeCallback(), MV_APP_UUID, dimensions, nullptr);
			});
	}

	void mvViewport::setConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "clear_color")) _clearColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "small_icon")) _small_icon = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "large_icon")) _large_icon = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "x_pos")) { _posDirty = true;  _xpos = ToInt(item);}
		if (PyObject* item = PyDict_GetItemString(dict, "y_pos")) { _posDirty = true;  _ypos = ToInt(item);}
		if (PyObject* item = PyDict_GetItemString(dict, "width")) { _sizeDirty = true;  _actualWidth = ToInt(item);}
		if (PyObject* item = PyDict_GetItemString(dict, "height")) { _sizeDirty = true;  _actualHeight = ToInt(item);}
		if (PyObject* item = PyDict_GetItemString(dict, "resizable")) { _modesDirty = true;  _resizable = ToBool(item);}
		if (PyObject* item = PyDict_GetItemString(dict, "vsync")) _vsync = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "min_width")) _minwidth = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "max_width")) _maxwidth = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "min_height")) _minheight = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "max_height")) _maxheight = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "always_on_top")) { _modesDirty = true; _alwaysOnTop = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "decorated")) { _modesDirty = true; _decorated = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "title")) { _titleDirty = true; _title = ToString(item); }

		if (_sizeDirty)
		{
			if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
			mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
				CreateEventArgument("actual_width", _actualWidth),
				CreateEventArgument("actual_height", _actualHeight),
				CreateEventArgument("client_width", _actualWidth),
				CreateEventArgument("client_height", _actualHeight)
				});
		}

	}

	void mvViewport::getConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "clear_color", mvPyObject(ToPyColor(_clearColor)));
		PyDict_SetItemString(dict, "small_icon", mvPyObject(ToPyString(_small_icon)));
		PyDict_SetItemString(dict, "large_icon", mvPyObject(ToPyString(_large_icon)));
		PyDict_SetItemString(dict, "x_pos", mvPyObject(ToPyInt(_xpos)));
		PyDict_SetItemString(dict, "y_pos", mvPyObject(ToPyInt(_ypos)));
		PyDict_SetItemString(dict, "width", mvPyObject(ToPyInt(_actualWidth)));
		PyDict_SetItemString(dict, "height",mvPyObject( ToPyInt(_actualHeight)));
		PyDict_SetItemString(dict, "client_width", mvPyObject(ToPyInt(_clientWidth)));
		PyDict_SetItemString(dict, "client_height", mvPyObject(ToPyInt(_clientHeight)));
		PyDict_SetItemString(dict, "resizable", mvPyObject(ToPyBool(_resizable)));
		PyDict_SetItemString(dict, "vsync", mvPyObject(ToPyBool(_vsync)));
		PyDict_SetItemString(dict, "min_width",  mvPyObject(ToPyInt(_minwidth )));
		PyDict_SetItemString(dict, "max_width",  mvPyObject(ToPyInt(_maxwidth )));
		PyDict_SetItemString(dict, "min_height", mvPyObject(ToPyInt(_minheight)));
		PyDict_SetItemString(dict, "max_height", mvPyObject(ToPyInt(_maxheight)));
		PyDict_SetItemString(dict, "always_on_top", mvPyObject(ToPyBool(_alwaysOnTop)));
		PyDict_SetItemString(dict, "decorated", mvPyObject(ToPyBool(_decorated)));
		PyDict_SetItemString(dict, "title", mvPyObject(ToPyString(_title)));

	}

	PyObject* mvViewport::get_viewport_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		PyObject* pdict = PyDict_New();

		mvViewport* viewport = mvApp::GetApp()->getViewport();
		if (viewport)
			viewport->getConfigDict(pdict);
		else
			mvThrowPythonError(mvErrorCode::mvNone, "No viewport created");

		return pdict;
	}

	PyObject* mvViewport::is_viewport_ok(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvViewport* viewport = mvApp::GetApp()->getViewport();
		if (viewport)
		{
			if(viewport->_shown)
				return ToPyBool(true);
		}

		return ToPyBool(false);
	}

	PyObject* mvViewport::create_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* title = "Dear PyGui";
		const char* small_icon = "";
		const char* large_icon = "";
		int width = 1280;
		int height = 800;
		int x_pos = 100;
		int y_pos = 100;
		int min_width = 250;
		int max_width = 10000;
		int min_height = 250;
		int max_height = 10000;

		int resizable = true;
		int vsync = true;
		int always_on_top = true;
		int maximized_box = true;
		int minimized_box = true;
		int border = true;
		int caption = true;
		int overlapped = true;

		PyObject* color = PyList_New(4);
		PyList_SetItem(color, 0, PyFloat_FromDouble(0.0));
		PyList_SetItem(color, 1, PyFloat_FromDouble(0.0));
		PyList_SetItem(color, 2, PyFloat_FromDouble(0.0));
		PyList_SetItem(color, 3, PyFloat_FromDouble(1.0));


		if (!(mvApp::GetApp()->getParsers())["create_viewport"].parse(args, kwargs, __FUNCTION__,
			&title, &small_icon, &large_icon, &width, &height, &x_pos, &y_pos, &min_width, &max_width, &min_height, &max_height ,
			&resizable, &vsync, &always_on_top,
			&maximized_box, &minimized_box, &border, &caption, &overlapped, &color
			))
			return GetPyNone();

		mvViewport* viewport = CreateViewport(width, height);
		viewport->setConfigDict(kwargs);

		mvApp::GetApp()->setViewport(viewport);

		return GetPyNone();
	}

	PyObject* mvViewport::show_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int minimized = false;
		int maximized = false;

		if (!(mvApp::GetApp()->getParsers())["show_viewport"].parse(args, kwargs, __FUNCTION__,
			&minimized, &maximized))
			return GetPyNone();

		mvViewport* viewport = mvApp::GetApp()->getViewport();
		if (viewport)
		{
			viewport->show(minimized, maximized);
			viewport->_shown = true;
		}
		else
			mvThrowPythonError(mvErrorCode::mvNone, "No viewport created");

		return GetPyNone();
	}

	PyObject* mvViewport::configure_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvViewport* viewport = mvApp::GetApp()->getViewport();
		if (viewport)
			viewport->setConfigDict(kwargs);
		else
			mvThrowPythonError(mvErrorCode::mvNone, "No viewport created");

		return GetPyNone();
	}

	PyObject* mvViewport::maximize_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getViewport()->maximize();
			});

		return GetPyNone();
	}

	PyObject* mvViewport::minimize_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getViewport()->minimize();
			});

		return GetPyNone();
	}

	PyObject* mvViewport::toggle_fullscreen(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getViewport()->fullscreen();
			});

		return GetPyNone();
	}

}
