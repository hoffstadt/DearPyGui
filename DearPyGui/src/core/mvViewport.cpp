#include "mvViewport.h"
#include "mvCallbackRegistry.h"
#include "mvPythonTranslator.h"
#include <string>
#include <array>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

	mvViewport::mvViewport(unsigned width, unsigned height) :
		m_width(width), m_height(height)
	{
		m_app = mvApp::GetApp();
	}

	void mvViewport::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::String>("title", mvArgType::KEYWORD_ARG, "'Dear PyGui'");
			parser.addArg<mvPyDataType::String>("small_icon", mvArgType::KEYWORD_ARG, "''");
			parser.addArg<mvPyDataType::String>("large_icon", mvArgType::KEYWORD_ARG, "''");

			parser.addArg<mvPyDataType::String>("width", mvArgType::KEYWORD_ARG, "1280");
			parser.addArg<mvPyDataType::String>("height", mvArgType::KEYWORD_ARG, "800");
			parser.addArg<mvPyDataType::String>("x_pos", mvArgType::KEYWORD_ARG, "100");
			parser.addArg<mvPyDataType::String>("y_pos", mvArgType::KEYWORD_ARG, "100");
			parser.addArg<mvPyDataType::String>("min_width", mvArgType::KEYWORD_ARG, "250");
			parser.addArg<mvPyDataType::String>("max_width", mvArgType::KEYWORD_ARG, "10000");
			parser.addArg<mvPyDataType::String>("min_height", mvArgType::KEYWORD_ARG, "250");
			parser.addArg<mvPyDataType::String>("max_height", mvArgType::KEYWORD_ARG, "10000");

			parser.addArg<mvPyDataType::Bool>("resizable", mvArgType::KEYWORD_ARG, "True");
			parser.addArg<mvPyDataType::Bool>("vsync", mvArgType::KEYWORD_ARG, "True");
			parser.addArg<mvPyDataType::Bool>("always_on_top", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("maximized_box", mvArgType::KEYWORD_ARG, "True");
			parser.addArg<mvPyDataType::Bool>("minimized_box", mvArgType::KEYWORD_ARG, "True");
			parser.addArg<mvPyDataType::Bool>("border", mvArgType::KEYWORD_ARG, "True");
			parser.addArg<mvPyDataType::Bool>("caption", mvArgType::KEYWORD_ARG, "True");
			parser.addArg<mvPyDataType::Bool>("overlapped", mvArgType::KEYWORD_ARG, "True");

			parser.addArg<mvPyDataType::FloatList>("clear_color", mvArgType::KEYWORD_ARG, "(0, 0, 0, 255)");

			parser.finalize();
			parsers->insert({ "create_viewport", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("viewport");
			parser.addArg<mvPyDataType::Bool>("minimized", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("maximized", mvArgType::KEYWORD_ARG, "False");
			parser.finalize();
			parsers->insert({ "show_viewport", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.finalize();
			parsers->insert({ "configure_viewport", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.finalize();
			parsers->insert({ "maximize_viewport", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.finalize();
			parsers->insert({ "minimize_viewport", parser });
		}

	}

	void mvViewport::onResizeEvent()
	{
		mvApp::GetApp()->getCallbackRegistry().addCallback(
			mvApp::GetApp()->getCallbackRegistry().getResizeCallback(),
			0, nullptr, nullptr);
	}

	void mvViewport::setConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "clear_color")) m_clearColor = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "small_icon")) m_small_icon = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "large_icon")) m_large_icon = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "x_pos")) { m_posDirty = true;  m_xpos = ToInt(item);}
		if (PyObject* item = PyDict_GetItemString(dict, "y_pos")) { m_posDirty = true;  m_ypos = ToInt(item);}
		if (PyObject* item = PyDict_GetItemString(dict, "width")) { m_sizeDirty = true;  m_actualWidth = ToInt(item);}
		if (PyObject* item = PyDict_GetItemString(dict, "height")) { m_sizeDirty = true;  m_actualHeight = ToInt(item);}
		if (PyObject* item = PyDict_GetItemString(dict, "resizable")) m_resizable = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "vsync")) m_vsync = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "min_width")) m_minwidth = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "max_width")) m_maxwidth = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "min_height")) m_minheight = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "max_height")) m_maxheight = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "always_on_top")) { m_modesDirty = true; m_alwaysOnTop = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "maximized_box")) { m_modesDirty = true; m_maximizeBox = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "minimized_box")) { m_modesDirty = true; m_minimizeBox = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "border")) { m_modesDirty = true; m_border = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "title")) { m_modesDirty = true; m_title = ToString(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "caption")) { m_modesDirty = true; m_caption = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "overlapped")) { m_modesDirty = true; m_overlapped = ToBool(item); }

		if (m_sizeDirty)
		{
			if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
			mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
				CreateEventArgument("actual_width", m_actualWidth),
				CreateEventArgument("actual_height", m_actualHeight),
				CreateEventArgument("client_width", m_actualWidth),
				CreateEventArgument("client_height", m_actualHeight)
				});
		}

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

		return ToPyString("DPG_NOT_USED_YET");
	}

	PyObject* mvViewport::show_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* vp;
		int minimized = false;
		int maximized = false;

		if (!(mvApp::GetApp()->getParsers())["show_viewport"].parse(args, kwargs, __FUNCTION__,
			&vp, &minimized, &maximized))
			return GetPyNone();

		mvViewport* viewport = mvApp::GetApp()->getViewport();
		if (viewport)
			viewport->show(minimized, maximized);

		return GetPyNone();
	}

	PyObject* mvViewport::configure_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvViewport* viewport = mvApp::GetApp()->getViewport();
		if (viewport)
			viewport->setConfigDict(kwargs);

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

}