#include "mvPythonModule.h"
#include "mvPythonTranslator.h"
#include "Core/mvApp.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"

namespace Marvel {

	static std::map<std::string, mvPythonParser> Translators = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbInput");

	std::map<std::string, mvPythonParser>& BuildInputsInterface() {

		std::map<std::string, mvPythonParser>* translators = new std::map< std::string, mvPythonParser>{

			{"get_key_down_callback", mvPythonParser({
			}, "Returns the key down callback.", "str")},

			{"get_key_press_callback", mvPythonParser({
			}, "Returns the key press callback.", "str")},

			{"get_key_release_callback", mvPythonParser({
			}, "Returns the key release callback.", "str")},

			{"get_mouse_click_callback", mvPythonParser({
			}, "Returns the mouse click callback.", "str")},

			{"get_mouse_double_click_callback", mvPythonParser({
			}, "Returns the mouse double click callback.", "str")},

			{"get_mouse_down_callback", mvPythonParser({
			}, "Returns the mouse down callback.", "str")},

			{"get_mouse_wheel_callback", mvPythonParser({
			}, "Returns the mouse wheel callback.", "str")},

			{"get_mouse_drag_callback", mvPythonParser({
			}, "Returns the mouse drag callback.", "str")},

			{"get_mouse_pos", mvPythonParser({
			}, "Returns the current mouse position in relation to the active window (minus titlebar).", "(int, int)")},

			{"get_mouse_drag_delta", mvPythonParser({
			}, "Returns the current mouse drag delta", "(float, float)")},

			{"is_mouse_button_dragging", mvPythonParser({
				{mvPythonDataType::Integer, "button"},
				{mvPythonDataType::Float, "threshold"},
			}, "Checks if the mouse is dragging.", "Boolean")},

			{"is_mouse_button_down", mvPythonParser({
				{mvPythonDataType::Integer, "button"}
			}, "Checks if the mouse button is pressed.", "Boolean")},

			{"is_mouse_button_clicked", mvPythonParser({
				{mvPythonDataType::Integer, "button"}
			}, "Checks if the mouse button is clicked.", "Boolean")},

			{"is_mouse_button_released", mvPythonParser({
				{mvPythonDataType::Integer, "button"}
			}, "Checks if the mouse button is released.", "Boolean")},

			{"is_mouse_button_double_clicked", mvPythonParser({
				{mvPythonDataType::Integer, "button"}
			}, "Checks if the mouse button is double clicked.", "Boolean")},

			{"is_key_pressed", mvPythonParser({
				{mvPythonDataType::Integer, "key"}
			}, "Checks if the key is pressed.", "Boolean")},

			{"is_key_released", mvPythonParser({
				{mvPythonDataType::Integer, "key"}
			}, "Checks if the key is released.", "Boolean")},

			{"is_key_down", mvPythonParser({
				{mvPythonDataType::Integer, "key"}
			}, "Checks if the key is down.", "Boolean")},

			{"set_mouse_down_callback", mvPythonParser({
				{mvPythonDataType::String, "callback"}
			}, "Sets a callback for a mouse down event.")},

			{"set_mouse_drag_callback", mvPythonParser({
				{mvPythonDataType::String, "callback"},
				{mvPythonDataType::Float, "threshold"}
			}, "Sets a callback for a mouse drag event.")},

			{"set_mouse_wheel_callback", mvPythonParser({
				{mvPythonDataType::String, "callback"}
			}, "Sets a callback for a mouse wheel event.")},

			{"set_mouse_double_click_callback", mvPythonParser({
				{mvPythonDataType::String, "callback"}
			}, "Sets a callback for a mouse double click event.")},

			{"set_mouse_click_callback", mvPythonParser({
				{mvPythonDataType::String, "callback"}
			}, "Sets a callback for a mouse click event.")},

			{ "set_key_down_callback", mvPythonParser({
				{mvPythonDataType::String, "callback"}
			}, "Sets a callback for a key down event.") },

			{ "set_key_press_callback", mvPythonParser({
				{mvPythonDataType::String, "callback"}
			}, "Sets a callback for a key press event.") },

			{ "set_key_release_callback", mvPythonParser({
				{mvPythonDataType::String, "callback"}
			}, "Sets a callback for a key release event.") }

		};

		return *translators;
	}

	PyObject* get_key_down_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMouseDragCallback());
	}

	PyObject* get_mouse_drag_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getKeyDownCallback());
	}

	PyObject* get_key_press_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getKeyPressCallback());
	}

	PyObject* get_key_release_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getKeyReleaseCallback());
	}

	PyObject* get_mouse_click_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMouseClickCallback());
	}

	PyObject* get_mouse_wheel_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMouseWheelCallback());
	}

	PyObject* get_mouse_double_click_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMouseDoubleClickCallback());
	}

	PyObject* get_mouse_down_callback(PyObject* self, PyObject* args)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMouseDownCallback());
	}

	PyObject* get_mouse_pos(PyObject* self, PyObject* args)
	{
		mvMousePos pos = mvApp::GetApp()->getMousePosition();
		PyObject* pvalue = Py_BuildValue("(ff)", pos.x, pos.y);
		return pvalue;
	}

	PyObject* get_mouse_drag_delta(PyObject* self, PyObject* args)
	{
		mvVec2 pos = mvApp::GetApp()->getMouseDragDelta();
		PyObject* pvalue = Py_BuildValue("(ff)", pos.x, pos.y);
		return pvalue;
	}

	PyObject* is_key_pressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!Translators["is_key_pressed"].parse(args, kwargs,__FUNCTION__, &key))
			Py_RETURN_NONE;

		bool pressed = mvApp::GetApp()->isKeyPressed(key);

		PyObject* pvalue = Py_BuildValue("i", pressed);

		return pvalue;
	}

	PyObject* is_key_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!Translators["is_key_released"].parse(args, kwargs, __FUNCTION__, &key))
			Py_RETURN_NONE;

		bool pressed = mvApp::GetApp()->isKeyReleased(key);

		PyObject* pvalue = Py_BuildValue("i", pressed);
		
		return pvalue;
	}

	PyObject* is_key_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!Translators["is_key_down"].parse(args, kwargs, __FUNCTION__, &key))
			Py_RETURN_NONE;

		bool pressed = mvApp::GetApp()->isKeyDown(key);

		PyObject* pvalue = Py_BuildValue("i", pressed);

		return pvalue;
	}

	PyObject* is_mouse_button_dragging(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;
		float threshold;

		if (!Translators["is_mouse_button_dragging"].parse(args, kwargs, __FUNCTION__, &button, &threshold))
			Py_RETURN_NONE;

		return Py_BuildValue("i", mvApp::GetApp()->isMouseDragging(button, threshold));
	}

	PyObject* is_mouse_button_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Translators["is_mouse_button_down"].parse(args, kwargs, __FUNCTION__, &button))
			Py_RETURN_NONE;

		return Py_BuildValue("i", mvApp::GetApp()->isMouseButtonDown(button));
	}

	PyObject* is_mouse_button_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Translators["is_mouse_button_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			Py_RETURN_NONE;

		return Py_BuildValue("i", mvApp::GetApp()->isMouseButtonClicked(button));
	}

	PyObject* is_mouse_button_double_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Translators["is_mouse_button_double_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			Py_RETURN_NONE;

		return Py_BuildValue("i", mvApp::GetApp()->isMouseButtonDoubleClicked(button));
	}

	PyObject* is_mouse_button_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Translators["is_mouse_button_released"].parse(args, kwargs, __FUNCTION__, &button))
			Py_RETURN_NONE;

		return Py_BuildValue("i", mvApp::GetApp()->isMouseButtonReleased(button));
	}

	PyObject* set_mouse_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["set_mouse_down_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseDownCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* set_mouse_drag_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		float threshold;

		if (!Translators["set_mouse_drag_callback"].parse(args, kwargs, __FUNCTION__, &callback, &threshold))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseDragCallback(std::string(callback));
		mvApp::GetApp()->setMouseDragThreshold(threshold);

		Py_RETURN_NONE;
	}

	PyObject* set_mouse_double_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["set_mouse_double_click_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseDoubleClickCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* set_mouse_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["set_mouse_click_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseClickCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* set_key_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["set_key_down_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setKeyDownCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* set_key_press_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["set_key_press_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setKeyPressCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* set_key_release_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["set_key_release_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setKeyReleaseCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	PyObject* set_mouse_wheel_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["set_mouse_wheel_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseWheelCallback(std::string(callback));

		Py_RETURN_NONE;
	}

	mvPythonModule* CreateInputInterface()
	{

		auto pyModule = new mvPythonModule("sbInput", {});

		pyModule->addMethodD(get_mouse_drag_callback);
		pyModule->addMethodD(set_mouse_drag_callback);
		pyModule->addMethodD(is_mouse_button_dragging);
		pyModule->addMethodD(is_mouse_button_down);
		pyModule->addMethodD(is_mouse_button_clicked);
		pyModule->addMethodD(is_mouse_button_double_clicked);
		pyModule->addMethodD(is_mouse_button_released);
		pyModule->addMethodD(get_mouse_drag_delta);
		pyModule->addMethodD(get_mouse_wheel_callback);
		pyModule->addMethodD(set_mouse_wheel_callback);
		pyModule->addMethodD(get_key_down_callback);
		pyModule->addMethodD(get_key_press_callback);
		pyModule->addMethodD(get_key_release_callback);
		pyModule->addMethodD(get_mouse_click_callback);
		pyModule->addMethodD(get_mouse_double_click_callback);
		pyModule->addMethodD(get_mouse_down_callback);
		pyModule->addMethodD(get_mouse_pos);
		pyModule->addMethodD(is_key_pressed);
		pyModule->addMethodD(is_key_released);
		pyModule->addMethodD(is_key_down);
		pyModule->addMethodD(set_mouse_click_callback);
		pyModule->addMethodD(set_mouse_down_callback);
		pyModule->addMethodD(set_mouse_double_click_callback);
		pyModule->addMethodD(set_key_down_callback);
		pyModule->addMethodD(set_key_press_callback);
		pyModule->addMethodD(set_key_release_callback);

		return pyModule;
	}
}