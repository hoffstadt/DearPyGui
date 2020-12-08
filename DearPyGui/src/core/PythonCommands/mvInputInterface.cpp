#include "mvInputInterface.h"
#include "mvInterfaceCore.h"

namespace Marvel {

	void AddInputCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "set_mouse_move_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a mouse move event. Data is the mouse position in local coordinates.", "None", "Input Polling") });

		parsers->insert({ "set_render_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets the callback to be ran every frame.", "None", "Input Polling") });

		parsers->insert({ "get_mouse_pos", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "local", "", "True"}
		}, "Returns the current mouse position in relation to the active window (minus titlebar) unless local flag is unset.",
		"(int, int)", "Input Polling") });

		parsers->insert({ "get_plot_mouse_pos", mvPythonParser({
		}, "Returns the current mouse position in the currently hovered plot.",
		"(int, int)", "Input Polling") });

		parsers->insert({ "get_mouse_drag_delta", mvPythonParser({
		}, "Returns the current mouse drag delta in pixels", "(float, float)", "Input Polling") });

		parsers->insert({ "is_mouse_button_dragging", mvPythonParser({
			{mvPythonDataType::Integer, "button"},
			{mvPythonDataType::Float, "threshold"},
		}, "Checks if the mouse is dragging.", "bool", "Input Polling") });

		parsers->insert({ "is_mouse_button_down", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is pressed.", "bool", "Input Polling") });

		parsers->insert({ "is_mouse_button_clicked", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is clicked.", "bool", "Input Polling") });

		parsers->insert({ "is_mouse_button_released", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is released.", "bool", "Input Polling") });

		parsers->insert({ "is_mouse_button_double_clicked", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is double clicked.", "bool", "Input Polling") });

		parsers->insert({ "is_key_pressed", mvPythonParser({
			{mvPythonDataType::Integer, "key"}
		}, "Checks if the key is pressed.", "bool", "Input Polling") });

		parsers->insert({ "is_key_released", mvPythonParser({
			{mvPythonDataType::Integer, "key"}
		}, "Checks if the key is released.", "bool", "Input Polling") });

		parsers->insert({ "is_key_down", mvPythonParser({
			{mvPythonDataType::Integer, "key"}
		}, "Checks if the key is down.", "bool", "Input Polling") });

		parsers->insert({ "set_resize_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)", "''"},
		}, "Sets a callback for a window resize event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_release_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a mouse release event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_down_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a mouse down event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_drag_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"},
			{mvPythonDataType::Float, "threshold"}
		}, "Sets a callback for a mouse drag event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_wheel_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a mouse wheel event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_double_click_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a mouse double click event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_click_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a mouse click event.", "None", "Input Polling") });

		parsers->insert({ "set_key_down_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a key down event.", "None", "Input Polling") }),

		parsers->insert({ "set_key_press_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
			}, "Sets a callback for a key press event.", "None", "Input Polling") });

		parsers->insert({ "set_key_release_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a key release event.", "None", "Input Polling") });
	}

	PyObject* get_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int local = true;

		if (!(*mvApp::GetApp()->getParsers())["get_mouse_pos"].parse(args, kwargs, __FUNCTION__, &local))
			return GetPyNone();

		mvVec2 pos = mvInput::getMousePosition();
		if (!local)
			pos = mvInput::getGlobalMousePosition();
		PyObject* pvalue = ToPyPair(pos.x, pos.y);
		return pvalue;
	}

	PyObject* get_plot_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int local = true;

		if (!(*mvApp::GetApp()->getParsers())["get_plot_mouse_pos"].parse(args, kwargs, __FUNCTION__, &local))
			return GetPyNone();

		mvVec2 pos = mvInput::getPlotMousePosition();
		PyObject* pvalue = ToPyPair(pos.x, pos.y);
		return pvalue;
	}

	PyObject* get_mouse_drag_delta(PyObject* self, PyObject* args)
	{
		mvVec2 pos = mvInput::getMouseDragDelta();
		PyObject* pvalue = ToPyPair(pos.x, pos.y);
		return pvalue;
	}

	PyObject* is_key_pressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(*mvApp::GetApp()->getParsers())["is_key_pressed"].parse(args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		bool pressed = mvInput::isKeyPressed(key);

		PyObject* pvalue = ToPyBool(pressed);

		return pvalue;
	}

	PyObject* is_key_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(*mvApp::GetApp()->getParsers())["is_key_released"].parse(args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		bool pressed = mvInput::isKeyReleased(key);

		PyObject* pvalue = ToPyBool(pressed);

		return pvalue;
	}

	PyObject* is_key_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(*mvApp::GetApp()->getParsers())["is_key_down"].parse(args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		bool pressed = mvInput::isKeyDown(key);

		PyObject* pvalue = ToPyBool(pressed);

		return pvalue;
	}

	PyObject* is_mouse_button_dragging(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;
		float threshold;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_dragging"].parse(args, kwargs, __FUNCTION__, &button, &threshold))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseDragging(button, threshold));
	}

	PyObject* is_mouse_button_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_down"].parse(args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseButtonDown(button));
	}

	PyObject* is_mouse_button_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseButtonClicked(button));
	}

	PyObject* is_mouse_button_double_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_double_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseButtonDoubleClicked(button));
	}

	PyObject* is_mouse_button_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(*mvApp::GetApp()->getParsers())["is_mouse_button_released"].parse(args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseButtonReleased(button));
	}

	PyObject* set_mouse_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_down_callback"].parse(args, kwargs, __FUNCTION__, 
			&callback))
			return GetPyNone();

		if (callback)
			Py_XINCREF(callback);

		mvCallbackRegistry::GetCallbackRegistry()->setMouseDownCallback(callback);

		return GetPyNone();
	}

	PyObject* set_mouse_drag_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		float threshold;

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_drag_callback"].parse(args, kwargs, __FUNCTION__, 
			&callback, &threshold))
			return GetPyNone();

		mvInput::setMouseDragThreshold(threshold);

		if (callback)
			Py_XINCREF(callback);
		mvCallbackRegistry::GetCallbackRegistry()->setMouseDragCallback(callback);

		return GetPyNone();
	}

	PyObject* set_mouse_double_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_double_click_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();

		if (callback)
			Py_XINCREF(callback);
		mvCallbackRegistry::GetCallbackRegistry()->setMouseDoubleClickCallback(callback);

		return GetPyNone();
	}

	PyObject* set_mouse_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_click_callback"].parse(args, kwargs, __FUNCTION__, 
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvCallbackRegistry::GetCallbackRegistry()->setMouseClickCallback(callback);
		return GetPyNone();
	}

	PyObject* set_mouse_release_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_release_callback"].parse(args, kwargs, __FUNCTION__, 
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvCallbackRegistry::GetCallbackRegistry()->setMouseReleaseCallback(callback);

		return GetPyNone();
	}

	PyObject* set_key_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_key_down_callback"].parse(args, kwargs, __FUNCTION__, 
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvCallbackRegistry::GetCallbackRegistry()->setKeyDownCallback(callback);

		return GetPyNone();
	}

	PyObject* set_key_press_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_key_press_callback"].parse(args, kwargs, __FUNCTION__, 
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvCallbackRegistry::GetCallbackRegistry()->setKeyPressCallback(callback);

		return GetPyNone();
	}

	PyObject* set_key_release_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_key_release_callback"].parse(args, kwargs, __FUNCTION__, 
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvCallbackRegistry::GetCallbackRegistry()->setKeyReleaseCallback(callback);
		return GetPyNone();
	}

	PyObject* set_mouse_wheel_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_wheel_callback"].parse(args, kwargs, __FUNCTION__, 
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvCallbackRegistry::GetCallbackRegistry()->setMouseWheelCallback(callback);

		return GetPyNone();
	}

	PyObject* set_mouse_move_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_move_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvCallbackRegistry::GetCallbackRegistry()->setMouseMoveCallback(callback);

		return GetPyNone();
	}

	PyObject* set_render_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;

		if (!(*mvApp::GetApp()->getParsers())["set_render_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvCallbackRegistry::GetCallbackRegistry()->setRenderCallback(callback);
		return GetPyNone();
	}

	PyObject* set_resize_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_resize_callback"].parse(args, kwargs, __FUNCTION__, 
			&callback, &handler))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);

		//mvAppItem* item;
		//item = mvItemRegistry::GetItemRegistry()->getItem(handler);

		//if (item)
		//{
		//	if (item->getDescription().root)
		//	{
		//		auto windowtype = static_cast<mvWindowAppitem*>(item);
		//		mvOldEventHandler* eventhandler = static_cast<mvOldEventHandler*>(windowtype);
		//		eventhandler->setResizeCallback(callback);
		//	}
		//	else
		//		ThrowPythonException("Resize callback can only be set for window items");
		//}


		return GetPyNone();
	}

}