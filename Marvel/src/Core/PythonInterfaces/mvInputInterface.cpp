#include "mvPythonModule.h"
#include "mvPythonTranslator.h"
#include "Core/mvApp.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbInput");

	std::map<std::string, mvPythonTranslator> BuildInputsInterface() {

		std::map<std::string, mvPythonTranslator> translators = {

			{"get_mouse_pos", mvPythonTranslator({
			}, false, "Returns the current mouse position.")},

			{"is_mouse_button_pressed", mvPythonTranslator({
				{mvPythonDataType::Integer, "button"}
			}, false, "Checks if the mouse button is pressed.")},

			{"is_key_pressed", mvPythonTranslator({
				{mvPythonDataType::Integer, "key"}
			}, false, "Checks if the key is pressed.")},

			{"set_mouse_down_callback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Sets a callback for a mouse down event.")},

			{"set_mouse_double_click_callback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Sets a callback for a mouse double click event.")},

			{"set_mouse_click_callback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Sets a callback for a mouse click event.")},

			{ "set_key_down_callback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Sets a callback for a key down event.") },

			{ "set_key_press_callback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Sets a callback for a key press event.") },

			{ "set_key_release_callback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Sets a callback for a key release event.") }

		};

		return translators;
	}

	PyObject* get_mouse_pos(PyObject* self, PyObject* args)
	{
		mvMousePos pos = mvApp::GetApp()->getMousePosition();
		PyObject* pvalue = Py_BuildValue("(ff)", pos.x, pos.y);
		return pvalue;
	}

	PyObject* is_mouse_button_pressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!Translators["is_mouse_button_pressed"].parse(args, kwargs,__FUNCTION__, &button))
			Py_RETURN_NONE;

		bool pressed = mvApp::GetApp()->isMouseButtonPressed(button);

		PyObject* pvalue = Py_BuildValue("i", pressed);

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

	PyObject* set_mouse_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;

		if (!Translators["set_mouse_down_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMouseDownCallback(std::string(callback));



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

	mvPythonModule* CreateInputInterface()
	{

		auto pyModule = new mvPythonModule("sbInput", {});

		pyModule->addMethodD(get_mouse_pos);
		pyModule->addMethodD(is_mouse_button_pressed);
		pyModule->addMethodD(is_key_pressed);
		pyModule->addMethodD(set_mouse_click_callback);
		pyModule->addMethodD(set_mouse_down_callback);
		pyModule->addMethodD(set_mouse_double_click_callback);
		pyModule->addMethodD(set_key_down_callback);
		pyModule->addMethodD(set_key_press_callback);
		pyModule->addMethodD(set_key_release_callback);

		return pyModule;
	}
}