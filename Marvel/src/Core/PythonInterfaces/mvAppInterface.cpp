#include "mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"
#include "mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"
#include "Core/mvUtilities.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbApp");

	std::map<std::string, mvPythonTranslator>& BuildAppInterface() {

		std::map<std::string, mvPythonTranslator>* translators = new std::map< std::string, mvPythonTranslator>{

			{"run_async_function", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Object, "data"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::String, "return_handler"},
			}, false, "Runs a function asyncronously.")},

			{"open_file_dialog", mvPythonTranslator({
				{mvPythonDataType::StringList, "extensions", "i.e [['Python', '*.py']]"},
			}, false, "Opens an 'open file' dialog.", "str")},

			{"save_file_dialog", mvPythonTranslator({
				{mvPythonDataType::StringList, "extensions", "i.e [['Python', '*.py']]"},
			}, false, "Opens an 'save file' dialog.", "str")},

			{"delete_item", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Bool, "children_only"}
			}, false, "Deletes an item if it exists.")},

			{"move_item_up", mvPythonTranslator({
				{mvPythonDataType::String, "item"}
			}, false, "Moves item up if possible and if it exists.")},

			{"move_item_down", mvPythonTranslator({
				{mvPythonDataType::String, "item"}
			}, false, "Moves down up if possible and if it exists.")},

			{"get_style_item", mvPythonTranslator({
				{mvPythonDataType::Integer, "item"}
			}, false, "Returns a style item's value", "(float, float)")},

			{"get_theme_item", mvPythonTranslator({
				{mvPythonDataType::Integer, "item"}
			}, false, "Returns a theme item's color", "(float, float, float, float)")},

			{"get_item_callback", mvPythonTranslator({
				{mvPythonDataType::String, "item"}
			}, false, "Returns an item' callback", "str")},

			{"get_item_height", mvPythonTranslator({
				{mvPythonDataType::String, "item"}
			}, false, "Returns an item's height.", "float")},

			{"get_item_width", mvPythonTranslator({
				{mvPythonDataType::String, "item"}
			}, false, "Returns an item's width.", "float")},

			{"get_item_popup", mvPythonTranslator({
				{mvPythonDataType::String, "item"}
			}, false, "Returns an item's popup.", "str")},

			{"get_item_tip", mvPythonTranslator({
				{mvPythonDataType::String, "item"}
			}, false, "Returns an item's tip.", "str")},

			{"get_main_callback", mvPythonTranslator({
			}, false, "Returns the main callback.", "str")},

			{"get_main_window_size", mvPythonTranslator({
			}, false, "Returns the size of the main window.", "(float, float)")},

			{"get_theme", mvPythonTranslator({
			}, false, "Returns the current theme.", "str")},

			{"get_thread_count", mvPythonTranslator({
			}, false, "Returns the allowable thread count.", "int")},

			{"is_threadpool_high_performance", mvPythonTranslator({
			}, false, "Checks if the threadpool is allowed to use the maximum number of threads.", "Boolean")},

			{"get_threadpool_threshold", mvPythonTranslator({
			}, false, "Returns the threadpool activation threshold.", "float")},

			{"get_active_window", mvPythonTranslator({
			}, false, "Returns the active window name.", "str")},

			{"get_marvel_version", mvPythonTranslator({
			}, false, "Returns the current version of Marvel.", "str")},

			{"set_threadpool_threshold", mvPythonTranslator({
				{mvPythonDataType::Float, "time"}
			}, false, "Sets the minimum duration for a callback before activating the threadpool.")},

			{"set_thread_count", mvPythonTranslator({
				{mvPythonDataType::Integer, "threads"}
			}, false, "Sets number of threads to use if the threadpool is active.")},

			{"show_documentation", mvPythonTranslator({
			}, false, "Shows the documentation window.")},

			{"show_about", mvPythonTranslator({
			}, false, "Shows the about window.")},

			{"show_metrics", mvPythonTranslator({
			}, false, "Shows the metrics window.")},

			{"show_debug", mvPythonTranslator({
			}, false, "Shows the debug window.")},

			{"close_popup", mvPythonTranslator({
			}, false, "Needs documentation")},

			{"show_source", mvPythonTranslator({
			}, false, "Shows the source code for the current app.")},

			{"set_threadpool_high_performance", mvPythonTranslator({
			}, false, "Set the thread count to the maximum number of threads on your computer.")},

			{"add_item_color_style", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
				{mvPythonDataType::Integer, "style"},
				{mvPythonDataType::FloatList, "color"}
			}, false, "Needs documentation")},

			{"is_item_hovered", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Checks if an item is hovered.", "Boolean")},

			{"set_main_window_size", mvPythonTranslator({
				{mvPythonDataType::Integer, "width"},
				{mvPythonDataType::Integer, "height"}
			}, false, "Sets the main window size.")},

			{"is_item_active", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Checks if an item is active.", "Boolean")},

			{"is_item_focused", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Checks if an item is focused.", "Boolean")},

			{"is_item_clicked", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Checks if an item is clicked.", "Boolean")},

			{"is_item_visible", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Checks if an item is visible.", "Boolean")},

			{"is_item_edited", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Checks if an item has been edited.", "Boolean")},

			{"is_item_activated", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Checks if an item has been activated.", "Boolean")},

			{"is_item_deactivated", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Checks if an item has been deactivated.", "Boolean")},

			{"is_item_deactivated_after_edit", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Checks if an item has been deactivated after editing.", "Boolean")},

			{"is_item_toggled_open", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Checks if an item is toggled.", "Boolean")},

			{"get_item_rect_min", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Returns an item's minimum allowable size.", "(float, float)")},

			{"get_item_rect_max", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Returns an item's maximum allowable size.", "(float, float)")},

			{"get_item_rect_size", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Returns an item's current size.", "(float, float)")},

			{"change_style_item", mvPythonTranslator({
				{mvPythonDataType::Integer, "item"},
				{mvPythonDataType::Float, "x"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "y"}
			}, false, "Needs documentation")},

			{"change_theme_item", mvPythonTranslator({
				{mvPythonDataType::Integer, "item"},
				{mvPythonDataType::Float, "r"},
				{mvPythonDataType::Float, "g"},
				{mvPythonDataType::Float, "b"},
				{mvPythonDataType::Float, "a"}
			}, false, "Needs documentation")},

			{"get_value", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, false, "Returns an item's value or None if there is none.", "Depends")},

			{"set_value", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Object, "value"}
			}, false, "Sets an item's value if applicable.")},

			{"show_item", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, false, "Shows an item if it was hidden.")},

			{"hide_item", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, false, "Hides an item.")},

			{"set_main_callback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Sets the callback to be ran every frame.")},

			{ "set_item_callback", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
				{mvPythonDataType::String, "callback"}
			}, false, "Sets an item's callback if applicable.") },

			{ "set_item_popup", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
				{mvPythonDataType::String, "popup"}
			}, false, "Sets an item's popup if applicable.") },

			{ "set_item_tip", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
				{mvPythonDataType::String, "tip"}
			}, false, "Sets a simple tooltip for an item.") },

			{ "set_item_width", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
				{mvPythonDataType::Integer, "width"}
			}, false, "Sets an item's width.") },

			{ "set_item_height", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
				{mvPythonDataType::Integer, "height"}
			}, false, "Sets an item's height if applicable.") },

			{ "set_theme", mvPythonTranslator({
				{mvPythonDataType::String, "theme"}
			}, false, "Set the application's theme to a built-in theme.") },

		};

		return *translators;
	}

	PyObject* run_async_function(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* return_handler = "";
		PyObject* data;

		if (!Translators["run_async_function"].parse(args, kwargs, __FUNCTION__, &name, &data, &return_handler))
			Py_RETURN_NONE;

		mvApp::GetApp()->addMTCallback(name, data, return_handler);

		Py_RETURN_NONE;

	}

	PyObject* delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Items can not be modified outside of the main thread.");
			Py_RETURN_NONE;
		}

		const char* item;
		int childrenOnly = false;

		if (!Translators["delete_item"].parse(args, kwargs, __FUNCTION__, &item, &childrenOnly))
			Py_RETURN_NONE;

		if(childrenOnly)
			mvApp::GetApp()->deleteItemChildren(item);
		else
			mvApp::GetApp()->deleteItem(item);

		Py_RETURN_NONE;

	}

	PyObject* open_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* extensions;

		if (!Translators["open_file_dialog"].parse(args, kwargs, __FUNCTION__, &extensions))
			Py_RETURN_NONE;

		std::string file = OpenFile(mvPythonTranslator::getStringPairVec(extensions));

		return Py_BuildValue("s", file.c_str());
	}

	PyObject* save_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* extensions;

		if (!Translators["save_file_dialog"].parse(args, kwargs, __FUNCTION__, &extensions))
			Py_RETURN_NONE;

		std::string file = SaveFile(mvPythonTranslator::getStringPairVec(extensions));

		return Py_BuildValue("s", SaveFile(mvPythonTranslator::getStringPairVec(extensions)).c_str());
	}

	PyObject* move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Items can not be modified outside of the main thread.");
			Py_RETURN_NONE;
		}

		const char* item;

		if (!Translators["move_item_up"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		mvApp::GetApp()->moveItemUp(item);

		Py_RETURN_NONE;

	}

	PyObject* move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Items can not be modified outside of the main thread.");
			Py_RETURN_NONE;
		}

		const char* item;

		if (!Translators["move_item_down"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		mvApp::GetApp()->moveItemDown(item);

		Py_RETURN_NONE;

	}

	PyObject* get_style_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;

		if (!Translators["get_style_item"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto values = mvApp::GetApp()->getStyleItem(item);

		return Py_BuildValue("(ff)", values.first, values.second);
	}

	PyObject* get_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;

		if (!Translators["get_theme_item"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto color = mvApp::GetApp()->getThemeItem(item);

		return Py_BuildValue("(iiii)", color.r, color.g, color.b, color.a);
	}

	PyObject* get_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["get_item_callback"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("s", appitem->getCallback());

		Py_RETURN_NONE;
	}

	PyObject* get_item_height(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["get_item_height"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->getHeight());

		Py_RETURN_NONE;
	}

	PyObject* get_item_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["get_item_width"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->getWidth());

		Py_RETURN_NONE;
	}

	PyObject* get_item_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["get_item_popup"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("s", appitem->getPopup());

		Py_RETURN_NONE;
	}

	PyObject* get_item_tip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["get_item_tip"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("s", appitem->getTip());

		Py_RETURN_NONE;
	}

	PyObject* get_main_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMainCallback());
	}

	PyObject* get_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("(i,i)", mvApp::GetApp()->getWindowWidth(), mvApp::GetApp()->getWindowWidth());
	}

	PyObject* get_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getAppTheme());
	}

	PyObject* get_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("i", mvApp::GetApp()->getThreadCount());
	}

	PyObject* is_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("b", mvApp::GetApp()->usingThreadPoolHighPerformance());
	}

	PyObject* get_threadpool_threshold(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("f", mvApp::GetApp()->getThreadPoolThreshold());
	}

	PyObject* get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getActiveWindow());
	}

	PyObject* get_marvel_version(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("s", mvApp::GetApp()->GetVersion());
	}

	PyObject* set_threadpool_threshold(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float time;

		if (!Translators["set_threadpool_threshold"].parse(args, kwargs, __FUNCTION__, &time))
			Py_RETURN_NONE;

		mvApp::GetApp()->setThreadPoolThreshold(time);

		Py_RETURN_NONE;
	}

	PyObject* set_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		int threads;

		if (!Translators["set_thread_count"].parse(args, kwargs, __FUNCTION__, &threads))
			Py_RETURN_NONE;

		mvApp::GetApp()->setThreadCount(threads);

		Py_RETURN_NONE;
	}

	PyObject* set_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->setThreadPoolHighPerformance();
		Py_RETURN_NONE;
	}

	PyObject* add_item_color_style(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int style;
		PyObject* color;

		if (!Translators["add_item_color_style"].parse(args, kwargs, __FUNCTION__, &item, &style, &color))
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);

		mvApp::GetApp()->addItemColorStyle(item, style, mcolor);

		Py_RETURN_NONE;
	}

	PyObject* is_item_hovered(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["is_item_hovered"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemHovered());

		Py_RETURN_NONE;
	}

	PyObject* is_item_active(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["is_item_active"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemActive());

		Py_RETURN_NONE;
	}

	PyObject* is_item_focused(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["is_item_focused"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemFocused());

		Py_RETURN_NONE;
	}

	PyObject* is_item_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["is_item_clicked"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemClicked());

		Py_RETURN_NONE;
	}

	PyObject* is_item_visible(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["is_item_visible"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemVisible());

		Py_RETURN_NONE;
	}

	PyObject* is_item_edited(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["is_item_edited"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemEdited());

		Py_RETURN_NONE;
	}

	PyObject* is_item_activated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["is_item_activated"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemActivated());

		Py_RETURN_NONE;
	}

	PyObject* is_item_deactivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["is_item_deactivated"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemDeactivated());

		Py_RETURN_NONE;
	}

	PyObject* is_item_deactivated_after_edit(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["is_item_deactivated_after_edit"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemDeactivatedAfterEdit());

		Py_RETURN_NONE;
	}

	PyObject* is_item_toggled_open(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["is_item_toggled_open"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemToogledOpen());

		Py_RETURN_NONE;
	}

	PyObject* get_item_rect_min(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["get_item_rect_min"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectMin();
			PyObject* pvalue = PyTuple_New(2);
			PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
			PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));
			return pvalue;
		}

		Py_RETURN_NONE;
	}

	PyObject* get_item_rect_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["get_item_rect_max"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectMax();
			PyObject* pvalue = PyTuple_New(2);
			PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
			PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));
			return pvalue;
		}

		Py_RETURN_NONE;
	}

	PyObject* get_item_rect_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Translators["get_item_rect_size"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectSize();
			PyObject* pvalue = PyTuple_New(2);
			PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
			PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));
			return pvalue;
		}

		Py_RETURN_NONE;
	}

	PyObject* change_style_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		float x = 0.0f;
		float y = 0.0f;

		if (!Translators["change_style_item"].parse(args, kwargs,__FUNCTION__, &item, &x, &y))
			Py_RETURN_NONE;

		mvApp::GetApp()->changeStyleItem(item, x, y);

		Py_RETURN_NONE;
	}

	PyObject* change_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		int r, g, b, a;

		if(!Translators["change_theme_item"].parse(args, kwargs,__FUNCTION__, &item, &r, &g, &b, &a))
			Py_RETURN_NONE;

		mvApp::GetApp()->changeThemeItem(item, { r, g, b, a });

		

		Py_RETURN_NONE;
	}

	PyObject* get_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if(!Translators["get_value"].parse(args, kwargs,__FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if(item == nullptr)
			Py_RETURN_NONE;

		return item->getPyValue();
	}

	PyObject* set_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		if(!Translators["set_value"].parse(args, kwargs,__FUNCTION__, &name, &value))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			Py_RETURN_NONE;

		item->setPyValue(value);
		Py_RETURN_NONE;
	}

	PyObject* show_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if(!Translators["show_item"].parse(args, kwargs,__FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->show();

		Py_RETURN_NONE;
	}

	PyObject* hide_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if(!Translators["hide_item"].parse(args, kwargs,__FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->hide();

		Py_RETURN_NONE;
	}

	PyObject* set_main_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;


		if(!Translators["set_main_callback"].parse(args, kwargs,__FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMainCallback(std::string(callback));

		

		Py_RETURN_NONE;
	}

	PyObject* set_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* item;
		
		if(!Translators["set_item_callback"].parse(args, kwargs,__FUNCTION__, &item, &callback))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setCallback(callback);

		Py_RETURN_NONE;
	}

	PyObject* set_item_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popup;
		const char* item;

		if (!Translators["set_item_popup"].parse(args, kwargs,__FUNCTION__, &item, &popup))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setPopup(popup);

		Py_RETURN_NONE;
	}

	PyObject* set_item_tip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tip, * item;

		if(!Translators["set_item_tip"].parse(args, kwargs,__FUNCTION__, &item, &tip))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setTip(tip);

		Py_RETURN_NONE;
	}

	PyObject* set_item_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int width;

		if(!Translators["set_item_width"].parse(args, kwargs,__FUNCTION__, &item, &width))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setWidth(width);
		

		Py_RETURN_NONE;
	}

	PyObject* set_item_height(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int height;

		if (!Translators["set_item_height"].parse(args, kwargs, __FUNCTION__, &item, &height))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setHeight(height);


		Py_RETURN_NONE;
	}

	PyObject* set_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int width;
		int height;

		if (!Translators["set_main_window_size"].parse(args, kwargs, __FUNCTION__, &width, &height))
			Py_RETURN_NONE;

		mvApp::GetApp()->setWindowSize(width, height);

		Py_RETURN_NONE;
	}

	PyObject* close_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::GetApp()->isStarted())
			Py_RETURN_NONE;

		ImGui::CloseCurrentPopup();
		
		Py_RETURN_NONE;
	}

	PyObject* set_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* theme;

		if(!Translators["set_theme"].parse(args, kwargs,__FUNCTION__, &theme))
			Py_RETURN_NONE;

		mvApp::GetApp()->setAppTheme(std::string(theme));

		Py_RETURN_NONE;
	}

	PyObject* show_metrics(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("metrics");
		Py_RETURN_NONE;
	}

	PyObject* show_about(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("about");
		Py_RETURN_NONE;
	}

	PyObject* show_source(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("source");
		Py_RETURN_NONE;
	}

	PyObject* show_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("debug");
		Py_RETURN_NONE;
	}

	PyObject* show_documentation(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("documentation");
		Py_RETURN_NONE;
	}

	mvPythonModule* CreatePythonInterface()
	{

		mvPythonModule* pyModule = new mvPythonModule("sbApp", {});

		pyModule->addMethodD(run_async_function);
		pyModule->addMethodD(save_file_dialog);
		pyModule->addMethodD(open_file_dialog);
		pyModule->addMethodD(delete_item);
		pyModule->addMethodD(move_item_down);
		pyModule->addMethodD(move_item_up);
		pyModule->addMethodD(get_style_item);
		pyModule->addMethodD(get_theme_item);
		pyModule->addMethodD(get_item_callback);
		pyModule->addMethodD(get_item_height);
		pyModule->addMethodD(get_item_popup);
		pyModule->addMethodD(get_item_tip);
		pyModule->addMethodD(get_main_callback);
		pyModule->addMethodD(get_main_window_size);
		pyModule->addMethodD(get_theme);
		pyModule->addMethodD(get_thread_count);
		pyModule->addMethodD(is_threadpool_high_performance);
		pyModule->addMethodD(get_threadpool_threshold);
		pyModule->addMethodD(get_active_window);
		pyModule->addMethodD(get_marvel_version);
		pyModule->addMethodD(show_source);
		pyModule->addMethodD(show_about);
		pyModule->addMethodD(show_debug);
		pyModule->addMethodD(show_metrics);
		pyModule->addMethodD(close_popup);
		pyModule->addMethodD(show_documentation);
		pyModule->addMethodD(set_threadpool_threshold);
		pyModule->addMethodD(set_thread_count);
		pyModule->addMethodD(set_threadpool_high_performance);
		pyModule->addMethodD(set_main_window_size);
		pyModule->addMethodD(add_item_color_style);
		pyModule->addMethodD(set_item_popup);
		pyModule->addMethodD(is_item_hovered);
		pyModule->addMethodD(is_item_active);
		pyModule->addMethodD(is_item_focused);
		pyModule->addMethodD(is_item_clicked);
		pyModule->addMethodD(is_item_visible);
		pyModule->addMethodD(is_item_edited);
		pyModule->addMethodD(is_item_activated);
		pyModule->addMethodD(is_item_deactivated);
		pyModule->addMethodD(is_item_deactivated_after_edit);
		pyModule->addMethodD(is_item_toggled_open);
		pyModule->addMethodD(get_item_rect_min);
		pyModule->addMethodD(get_item_rect_max);
		pyModule->addMethodD(get_item_rect_size);
		pyModule->addMethodD(change_style_item);
		pyModule->addMethodD(show_item);
		pyModule->addMethodD(hide_item);
		pyModule->addMethodD(change_theme_item);
		pyModule->addMethodD(set_theme);
		pyModule->addMethodD(set_main_callback);
		pyModule->addMethodD(set_item_callback);
		pyModule->addMethodD(set_item_tip);
		pyModule->addMethodD(set_item_width);
		pyModule->addMethodD(set_item_height);
		pyModule->addMethodD(get_value);
		pyModule->addMethodD(set_value);

		return pyModule;
	}
}