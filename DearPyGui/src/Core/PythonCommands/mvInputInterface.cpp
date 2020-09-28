#include "mvInputInterface.h"
#include "mvInterfaceCore.h"

namespace Marvel {

	void AddInputCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "set_mouse_move_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse move event. Data is the mouse position in local coordinates.", "None", "Input Polling") });

		parsers->insert({ "set_render_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"}
		}, "Sets the callback to be ran every frame.", "None", "Input Polling") });

		parsers->insert({ "get_mouse_pos", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "local"}
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
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a window resize event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_release_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse release event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_down_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse down event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_drag_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Float, "threshold"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse drag event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_wheel_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse wheel event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_double_click_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse double click event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_click_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a mouse click event.", "None", "Input Polling") });

		parsers->insert({ "set_key_down_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
		}, "Sets a callback for a key down event.", "None", "Input Polling") }),

		parsers->insert({ "set_key_press_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
			}, "Sets a callback for a key press event.", "None", "Input Polling") });

		parsers->insert({ "set_key_release_callback", mvPythonParser({
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "handler", "Callback will be run when event occurs while this window is active (default is main window)"},
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
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_down_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseDownCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseDownCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseDownCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_mouse_drag_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		float threshold;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_drag_callback"].parse(args, kwargs, __FUNCTION__, &callback, &threshold, &handler))
			return GetPyNone();

		mvInput::setMouseDragThreshold(threshold);

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseDragCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseDragCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseDragCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_mouse_double_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler;

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_double_click_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseDoubleClickCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseDoubleClickCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseDoubleClickCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_mouse_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_click_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseClickCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseClickCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseClickCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_mouse_release_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_release_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseReleaseCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseReleaseCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseReleaseCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_key_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_key_down_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setKeyDownCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setKeyDownCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setKeyDownCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_key_press_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_key_press_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setKeyPressCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setKeyPressCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setKeyPressCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_key_release_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_key_release_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setKeyReleaseCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setKeyReleaseCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setKeyReleaseCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_mouse_wheel_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_wheel_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setMouseWheelCallback(callback);
		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseWheelCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseWheelCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_mouse_move_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_mouse_move_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->getWindow("MainWindow")->setMouseMoveCallback(callback);

		else
		{
			mvAppItem* item = mvApp::GetApp()->getItem(handler);
			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					windowtype->setMouseMoveCallback(callback);
					return GetPyNone();
				}
				else
				{
					ThrowPythonException(std::string(handler) + " handler is not a window.");
					return GetPyNone();
				}
			}


			// check if item is a standard window
			mvStandardWindow* window = mvApp::GetApp()->getStandardWindow(handler);
			if (window == nullptr)
			{
				ThrowPythonException(std::string(handler) + " handler item was not found.");
				return GetPyNone();
			}

			window->setMouseMoveCallback(callback);
		}

		return GetPyNone();
	}

	PyObject* set_render_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;

		if (!(*mvApp::GetApp()->getParsers())["set_render_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();

		mvApp::GetApp()->setRenderCallback(callback);
		return GetPyNone();
	}

	PyObject* set_resize_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;
		const char* handler = "MainWindow";

		if (!(*mvApp::GetApp()->getParsers())["set_resize_callback"].parse(args, kwargs, __FUNCTION__, &callback, &handler))
			return GetPyNone();

		if (std::string(handler) == "MainWindow")
			mvApp::GetApp()->setResizeCallback(callback);
		else
		{
			mvAppItem* item;
			item = mvApp::GetApp()->getItem(handler);

			if (item)
			{
				if (item->getType() == mvAppItemType::Window)
				{
					auto windowtype = static_cast<mvWindowAppitem*>(item);
					mvEventHandler* eventhandler = static_cast<mvEventHandler*>(windowtype);
					eventhandler->setResizeCallback(callback);
				}
				else
					ThrowPythonException("Resize callback can only be set for window items");
			}
		}

		return GetPyNone();
	}

}