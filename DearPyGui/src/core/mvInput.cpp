#include "mvInput.h"
#include "mvApp.h"
#include "mvProfiler.h"
#include "mvCallbackRegistry.h"
#include "mvItemRegistry.h"
#include "containers/mvWindowAppItem.h"

namespace Marvel {

	mvInput::AtomicVec2      mvInput::s_mousePos = {0, 0};
	mvInput::AtomicVec2      mvInput::s_mouseGlobalPos = {0, 0};
	mvInput::AtomicFloatVec2 mvInput::s_mousePlotPos = {0, 0};
	mvInput::AtomicVec2      mvInput::s_mouseDrawingPos = {0, 0};
	std::atomic_int          mvInput::s_mouseDragThreshold = 20;
	mvInput::AtomicVec2      mvInput::s_mouseDragDelta = { 0, 0 };
	std::atomic_bool         mvInput::s_keysdown[512];
	std::atomic_int          mvInput::s_keysdownduration[512]; // 1/100 seconds
	std::atomic_bool         mvInput::s_keyspressed[512];
	std::atomic_bool         mvInput::s_keysreleased[512];
	std::atomic_int          mvInput::s_mousewheel;
	std::atomic_bool         mvInput::s_mousedown[5];
	std::atomic_bool         mvInput::s_mouseDragging[5];
	std::atomic_int          mvInput::s_mousedownduration[5]; // 1/100 seconds
	std::atomic_bool         mvInput::s_mouseclick[5];
	std::atomic_bool         mvInput::s_mousedoubleclick[5];
	std::atomic_bool         mvInput::s_mousereleased[5];

	void mvInput::CheckInputs()
	{
		MV_PROFILE_FUNCTION();

		// update mouse
		// mouse move event
		ImVec2 mousepos = ImGui::GetMousePos();
		if (ImGui::IsMousePosValid(&mousepos))
		{
			if (s_mouseGlobalPos.x != mousepos.x || s_mouseGlobalPos.y !=mousepos.y)
			{
				mvInput::setGlobalMousePosition(mousepos.x, mousepos.y);

				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_MOVE,
					{
					CreateEventArgument("X", mousepos.x),
					CreateEventArgument("Y", mousepos.y)
					});
			}
		}
		

		// route key events
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
		{
			s_keysdown[i] = ImGui::GetIO().KeysDown[i];

			// route key pressed event
			if (s_keyspressed[i] = ImGui::IsKeyPressed(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_KEY_PRESS, { CreateEventArgument("KEY", i) });

			// route key down event
			if (ImGui::GetIO().KeysDownDuration[i] >= 0.0f)
			{
				s_keysdownduration[i] = (int)(ImGui::GetIO().KeysDownDuration[i] * 100.0);
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_KEY_DOWN,
					{ CreateEventArgument("KEY", i), CreateEventArgument("DURATION", ImGui::GetIO().KeysDownDuration[i]) });
			}

			// route key released event
			if (s_keysreleased[i] = ImGui::IsKeyReleased(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_KEY_RELEASE, { CreateEventArgument("KEY", i) });
		}

		// route mouse wheel event
		if (ImGui::GetIO().MouseWheel != 0.0f)
		{
			s_mousewheel = (int)ImGui::GetIO().MouseWheel;
			mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_WHEEL, { CreateEventArgument("DELTA", ImGui::GetIO().MouseWheel) });
		}

		// route mouse dragging event
		for (int i = 0; i < 3; i++)
		{
			s_mouseDragging[i] = ImGui::IsMouseDragging(i, mvInput::getMouseDragThreshold());
			
			if (ImGui::IsMouseDragging(i, mvInput::getMouseDragThreshold()))
			{
				mvInput::setMouseDragDelta({ ImGui::GetMouseDragDelta().x, ImGui::GetMouseDragDelta().y });
				// TODO: send delta
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_DRAG,
					{ CreateEventArgument("BUTTON", i),
					CreateEventArgument("X", ImGui::GetMouseDragDelta().x),
					CreateEventArgument("Y", ImGui::GetMouseDragDelta().y)
					});
				ImGui::ResetMouseDragDelta(i);
				break;
			}
			
		}

		// route other mouse events (note mouse move callbacks are handled in mvWindowAppItem)
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
		{
			s_mousedown[i] = ImGui::GetIO().MouseDown[i];

			// route mouse click event
			if (s_mouseclick[i] = ImGui::IsMouseClicked(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_CLICK, { CreateEventArgument("BUTTON", i) });

			// route mouse down event
			if (ImGui::GetIO().MouseDownDuration[i] >= 0.0f)
			{
				s_mousedownduration[i] = (int)(ImGui::GetIO().MouseDownDuration[i] * 100.0);
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_DOWN,
					{ CreateEventArgument("BUTTON", i), CreateEventArgument("DURATION",  ImGui::GetIO().MouseDownDuration[i]) });
			}
			else
				s_mousedownduration[i] = 0;

			// route mouse double clicked event
			if (s_mousedoubleclick[i] = ImGui::IsMouseDoubleClicked(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_DBL_CLK, { CreateEventArgument("BUTTON", i) });

			// route mouse released event
			if (s_mousereleased[i] = ImGui::IsMouseReleased(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_RELEASE, { CreateEventArgument("BUTTON", i) });
		}
	}

	void mvInput::setMousePosition(float x, float y)
	{
		s_mousePos.x = (int)x;
		s_mousePos.y = (int)y;
	}

	void mvInput::setGlobalMousePosition(float x, float y)
	{
		s_mouseGlobalPos.x = (int)x;
		s_mouseGlobalPos.y = (int)y;
	}

	void mvInput::setPlotMousePosition(float x, float y)
	{
		s_mousePlotPos.x = x;
		s_mousePlotPos.y = y;
	}

	void mvInput::setDrawingMousePosition(float x, float y)
	{
		s_mouseDrawingPos.x = (int)x;
		s_mouseDrawingPos.y = (int)y;
	}

	void mvInput::setMouseDragThreshold(float threshold)
	{
		s_mouseDragThreshold = threshold;
	}

	void mvInput::setMouseDragDelta(const mvVec2& delta)
	{
		s_mouseDragDelta.x = delta.x;
		s_mouseDragDelta.y = delta.y;
	}

	int mvInput::getMouseDragThreshold()
	{
		return s_mouseDragThreshold;
	}

	mvVec2 mvInput::getMouseDragDelta()
	{
		return { (float)s_mouseDragDelta.x, (float)s_mouseDragDelta.y };
	}

	mvVec2 mvInput::getMousePosition()
	{
		return { (float)s_mousePos.x, (float)s_mousePos.y };
	}

	mvVec2 mvInput::getGlobalMousePosition()
	{
		return { (float)s_mouseGlobalPos.x, (float)s_mouseGlobalPos.y };
	}

	mvVec2 mvInput::getPlotMousePosition()
	{
		return { (float)s_mousePlotPos.x, (float)s_mousePlotPos.y };
	}

	mvVec2 mvInput::getDrawingMousePosition()
	{
		return { (float)s_mouseDrawingPos.x, (float)s_mouseDrawingPos.y };
	}

	bool mvInput::isMouseDragging(int button, float threshold)
	{
		return s_mousedownduration[button]/100.0f >= threshold;
	}

	bool mvInput::isMouseButtonDown(int button)
	{
		return s_mousedown[button];
	}

	bool mvInput::isMouseButtonClicked(int button)
	{
		return s_mouseclick[button];
	}

	bool mvInput::isMouseButtonDoubleClicked(int button)
	{
		return s_mousedoubleclick[button];
	}

	bool mvInput::isMouseButtonReleased(int button)
	{
		return s_mousereleased[button];
	}

	bool mvInput::isKeyPressed(int keycode)
	{
		return s_keyspressed[keycode];
	}

	bool mvInput::isKeyReleased(int keycode)
	{
		return s_keysreleased[keycode];
	}

	bool mvInput::isKeyDown(int keycode)
	{
		return s_keysdown[keycode];
	}

#ifdef MV_CPP
#else
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

		parsers->insert({ "get_drawing_mouse_pos", mvPythonParser({
		}, "Returns the current mouse position in the currently hovered drawing.",
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
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "handler", "Callback will be run when window is resized (default is viewport)", "''"},
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

		mvVec2 pos;

		if (local)
			pos = mvInput::getMousePosition();
		else
			pos = mvInput::getGlobalMousePosition();

		return ToPyPair(pos.x, pos.y);
	}

	PyObject* get_plot_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!(*mvApp::GetApp()->getParsers())["get_plot_mouse_pos"].parse(args, kwargs, __FUNCTION__))
			return GetPyNone();

		mvVec2 pos = mvInput::getPlotMousePosition();

		return ToPyPair(pos.x, pos.y);
	}

	PyObject* get_drawing_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!(*mvApp::GetApp()->getParsers())["get_drawing_mouse_pos"].parse(args, kwargs, __FUNCTION__))
			return GetPyNone();

		mvVec2 pos = mvInput::getDrawingMousePosition();

		return ToPyPair(pos.x, pos.y);
	}

	PyObject* get_mouse_drag_delta(PyObject* self, PyObject* args)
	{

		mvVec2 pos = mvInput::getMouseDragDelta();
		return ToPyPair(pos.x, pos.y);
	}

	PyObject* is_key_pressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(*mvApp::GetApp()->getParsers())["is_key_pressed"].parse(args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		return ToPyBool(mvInput::isKeyPressed(key));
	}

	PyObject* is_key_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(*mvApp::GetApp()->getParsers())["is_key_released"].parse(args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		return ToPyBool(mvInput::isKeyReleased(key));
	}

	PyObject* is_key_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(*mvApp::GetApp()->getParsers())["is_key_down"].parse(args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		return ToPyBool(mvInput::isKeyDown(key));
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

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseDownCallback(callback);
			});
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

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseDragCallback(callback);
			});

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
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseDoubleClickCallback(callback);
			});

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
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseClickCallback(callback);
			});
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
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseReleaseCallback(callback);
			});

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
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setKeyDownCallback(callback);
			});

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
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setKeyPressCallback(callback);
			});

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
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setKeyReleaseCallback(callback);
			});
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
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseWheelCallback(callback);
			});

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
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseMoveCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* set_render_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;

		if (!(*mvApp::GetApp()->getParsers())["set_render_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setRenderCallback(callback);
			});
		return GetPyNone();
	}

	PyObject* set_resize_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;
		const char* handler = "";

		if (!(*mvApp::GetApp()->getParsers())["set_resize_callback"].parse(args, kwargs, __FUNCTION__,
			&callback, &handler))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);


		auto fut = mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{

				if (std::string(handler).empty())
				{
					mvApp::GetApp()->getCallbackRegistry().setResizeCallback(callback);
					return std::string("");
				}

				mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(handler);

				if (item)
				{
					if (item->getDescription().root)
					{
						auto windowtype = static_cast<mvWindowAppItem*>(item.get());
						windowtype->setResizeCallback(callback);
					}
					else
						return std::string("Resize callback can only be set for window items");
				}
				return std::string("");
			});

		auto message = fut.get();

		if (!message.empty())
			ThrowPythonException(message);

		return GetPyNone();
	}
#endif // MV_CPP

}
