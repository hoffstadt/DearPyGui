#include "mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"
#include "mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"

namespace Marvel {

	static std::map<std::string, mvPythonTranslator> Translators = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbWidgets");

	std::map<std::string, mvPythonTranslator>& BuildWidgetsInterface() {
		
		std::map<std::string, mvPythonTranslator>* translators = new std::map< std::string, mvPythonTranslator>;

		translators->insert({ "end_tree_node", mvPythonTranslator({
		}, false, "Ends the tree node created by a call to add_tree_node.") });

		translators->insert({ "end_popup", mvPythonTranslator({
		}, false, "Ends the popup created by a call to add_popup.") });

		translators->insert({ "end_window", mvPythonTranslator({
		}, false, "Ends the window created by a call to add_window.") });

		translators->insert({ "end_group", mvPythonTranslator({
		}, false, "Ends the group created by a call to add_group.") });

		translators->insert({ "end_child", mvPythonTranslator({
		}, false, "Ends the child created by a call to add_child.") });

		translators->insert({ "end_tab", mvPythonTranslator({
		}, false, "Ends the tab created by a call to add_tab.") });

		translators->insert({ "end_tab_bar", mvPythonTranslator({
		}, false, "Ends the tab bar created by a call to add_tab_bar") });

		translators->insert({ "end_menu", mvPythonTranslator({
		}, false, "Ends the menu created by a call to add_menu.") });

		translators->insert({ "end_menu_bar", mvPythonTranslator({
		}, false, "Ends the menu bar created by a call to add_menu_bar.") });

		translators->insert({ "end_tooltip", mvPythonTranslator({
		}, false, "Ends the tooltip created by a call to add_tooltip.") });

		translators->insert({ "end_collapsing_header", mvPythonTranslator({
		}, false, "Ends the collapsing header created by a call to add_collapsing_header.") });

		translators->insert({ "add_seperator", mvPythonTranslator({
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
		}, true, "Adds a horizontal line.") });

		translators->insert({ "add_simple_plot", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "value", "Tuple of float values"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Bool, "autoscale", "autoscales range based on data (default is True)"},
			{mvPythonDataType::String, "overlay", "overlays text (similar to a plot title)"},
			{mvPythonDataType::Float, "minscale", "used if autoscale is false"},
			{mvPythonDataType::Float, "maxscale", "used if autoscale is false"},
			{mvPythonDataType::Bool, "histogram", "create a histogram"}
		}, true, "A simple plot for visualization of a set of values") });

		translators->insert({ "add_progress_bar", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "value"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "overlay"}
		}, true, "Adds a progress bar.") });

		translators->insert({ "add_image", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "tint_color"},
			{mvPythonDataType::FloatList, "border_color"}
		}, true, "Adds an image.") });

		translators->insert({ "add_slider_float", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::Bool, "vertical"}
		}, true, "Adds slider for a single float value") });

		translators->insert({ "add_slider_float2", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"}
		}, true, "Adds slider for a 2 float values.") });

		translators->insert({ "add_slider_float3", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"}
		}, true, "Adds slider for a 3 float values.") });

		translators->insert({ "add_slider_float4", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"}
		}, true, "Adds slider for a 4 float values.") });

		translators->insert({ "add_slider_int", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Bool, "vertical"}
		}, true, "Adds slider for a single int value") });

		translators->insert({ "add_slider_int2", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"}
		}, true, "Adds slider for a 2 int values.") });

		translators->insert({ "add_slider_int3", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"}
		}, true, "Adds slider for a 3 int values.") });

		translators->insert({ "add_slider_int4", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"}
		}, true, "Adds slider for a 4 int values.") });

		translators->insert({ "add_drag_float", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"}
		}, true, "Adds drag for a single float value") });

		translators->insert({ "add_drag_float2", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"}
		}, true, "Adds drag for a 2 float values.") });

		translators->insert({ "add_drag_float3", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"}
		}, true, "Adds drag for a 3 float values.") });

		translators->insert({ "add_drag_float4", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"}
		}, true, "Adds drag for a 4 float values.") });

		translators->insert({ "add_drag_int", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"}
		}, true, "Adds drag for a single int value") });

		translators->insert({ "add_drag_int2", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"}
		}, true, "Adds drag for a 2 int values.") });

		translators->insert({ "add_drag_int3", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"}
		}, true, "Adds drag for a 3 int values.") });

		translators->insert({ "add_drag_int4", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"}
		}, true, "Adds drag for a 4 int values.") });

		translators->insert({ "add_text", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "wrap", "number of characters until wraping"},
			{mvPythonDataType::FloatList, "color", "color of the text (rgba)"},
			{mvPythonDataType::Bool, "bullet", "makes the text bulleted"}
		}, true, "Adds text") });

		translators->insert({ "add_label_text", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "color"},
		}, true, "Adds text with a label. Useful for output values.") });

		translators->insert({ "add_listbox", mvPythonTranslator({
			{mvPythonDataType::String, "name", "Name of the listbox"},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "default_value"},
		}, true, "Adds a listbox.") });

		translators->insert({ "add_combo", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "default_value"}
		}, true, "Adds a combo.") });

		translators->insert({ "add_selectable", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Bool, "default_value"}
		}, true, "Adds a selectable.") });

		translators->insert({ "add_button", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "small", "Small button, useful for embedding in text."},
			{mvPythonDataType::Bool, "arrow", "Arrow button."},
			{mvPythonDataType::Integer, "direction", "A cardinal direction"},
		}, true, "Adds a button.") });

		translators->insert({ "add_input_text", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "hint"},
			{mvPythonDataType::Bool, "multiline"},
			{mvPythonDataType::Integer, "flags"}
		}, true, "Adds input for text values.") });

		translators->insert({ "add_input_int", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "default_value"},
		}, true, "Adds input for integer values.") });

		translators->insert({ "add_input_int2", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::IntList, "default_value"},
		}, true, "Adds input for 2 integer values.") });

		translators->insert({ "add_input_int3", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::IntList, "default_value"},
		}, true, "Adds input for 3 integer values.") });

		translators->insert({ "add_input_int4", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::IntList, "default_value"},
		}, true, "Adds input for 4 integer values.") });

		translators->insert({ "add_input_float", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::String, "format"}
		}, true, "Adds input for float values.") });

		translators->insert({ "add_input_float2", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"}
		}, true, "Adds input for 2 float values.") });

		translators->insert({ "add_input_float3", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"}
		}, true, "Adds input for 3 float values.") });

		translators->insert({ "add_input_float4", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"}
		}, true, "Adds input for 4 float values.") });

		translators->insert({ "add_indent", mvPythonTranslator({
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "offset"}
		}, true, "Adds an indent to following items.") });

		translators->insert({ "unindent", mvPythonTranslator({
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "offset"}
		}, true, "Unindents following items.") });

		translators->insert({ "add_tab_bar", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "flags"}
		}, true, "Adds a tab bar.") });

		translators->insert({ "add_tab", mvPythonTranslator({
			{mvPythonDataType::String, "name"}
		}, true, "Adds a tab to a tab bar.") });

		translators->insert({ "add_menu_bar", mvPythonTranslator({
			{mvPythonDataType::String, "name"}
		}, true, "Adds a menu bar to a window. Must be followed by a call to end_menu_bar.") });

		translators->insert({ "add_menu", mvPythonTranslator({
			{mvPythonDataType::String, "name"}
		}, true, "Adds a menu to an existing menu bar. Must be followed by a call to end_menu.") });

		translators->insert({ "add_menu_item", mvPythonTranslator({
			{mvPythonDataType::String, "name"}
		}, true, "Adds a menu item to an existing menu.") });

		translators->insert({ "add_spacing", mvPythonTranslator({
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Integer, "count"}
		}, true, "Adds vertical spacing.") });

		translators->insert({ "add_same_line", mvPythonTranslator({
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "xoffset"},
			{mvPythonDataType::Float, "spacing"}
		}, true, "Places a widget on the same line as the previous widget. Can also be used for horizontal spacing.") });

		translators->insert({ "add_radio_button", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "default_value"}
		}, true, "Adds a set of radio buttons.") });

		translators->insert({ "add_group", mvPythonTranslator({
			{mvPythonDataType::String, "name"}
		}, true, "Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.\
				Must be closed with the end_group command unless added at runtime.") });

		translators->insert({ "add_child", mvPythonTranslator({
			{mvPythonDataType::String, "name"}
		}, true, "Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end_child.") });

		translators->insert({ "add_window", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"}
		}, false, "Creates a new window for following items to be added to. Must call end_main_window command before.") });

		translators->insert({ "add_tooltip", mvPythonTranslator({
			{mvPythonDataType::String, "tipparent"},
			{mvPythonDataType::String, "name"}
		}, true, "Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for.") });

		translators->insert({ "add_popup", mvPythonTranslator({
			{mvPythonDataType::String, "popupparent"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "mousebutton"},
			{mvPythonDataType::Integer, "modal"}
		}, true, "Adds a popup window for an item. This command must come immediately after the item the popup is for.") });

		translators->insert({ "add_collapsing_header", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "flags"}
		}, true, "Adds a collapsing header to add items to. Must be closed with the end_collapsing_header command.") });

		translators->insert({ "add_tree_node", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "flags"}
		}, true, "Adds a tree node to add items to. Must be closed with the end_tree_node command.") });

		translators->insert({ "add_color_edit3", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::IntList, "default_value (0, 0, 0)"},
		}, true, "Adds an rgb color editing widget.") });

		translators->insert({ "add_color_edit4", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::IntList, "default_value (0, 0, 0, 255)"},
		}, true, "Adds an rgba color editing widget.") });

		translators->insert({ "add_color_picker3", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::IntList, "default_value (0, 0, 0)"},
		}, true, "Adds an rgb color picking widget.") });

		translators->insert({ "add_color_picker4", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::IntList, "default_value (0, 0, 0, 255)"},
		}, true, "Adds an rgba color picking widget.") });

		translators->insert({ "add_checkbox", mvPythonTranslator({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "default_value"}
		}, true, "Adds a checkbox widget.") });

		return *translators;
	}

	PyObject* add_simple_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		const char* overlay = "";
		float minscale = 0.0f;
		float maxscale = 0.0f;
		int autoscale = true;
		int histogram = false;
		PyObject* value;

		if (!Translators["add_simple_plot"].parse(args, kwargs,__FUNCTION__, &name, &value, &autoscale, &overlay, 
			&minscale, &maxscale, &histogram, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		std::vector<float> values = mvPythonTranslator::getFloatVec(value);

		if (autoscale)
		{
			maxscale = values[0];
			minscale = values[0];

			for (auto& item : values)
			{
				if (item > maxscale)
					maxscale = item;
				if (item < minscale)
					minscale = item;
			}
		}

		mvAppItem* item = new mvSimplePlot("", name, values, overlay, minscale, maxscale, height, histogram);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_progress_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		float default_value;
		const char* overlay = "";

		if (!Translators["add_progress_bar"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &overlay, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvProgressBar("", name, default_value, overlay);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_image(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		const char* value;
		PyObject* tintcolor = PyTuple_New(4);
		PyTuple_SetItem(tintcolor, 0, PyFloat_FromDouble(1.0));
		PyTuple_SetItem(tintcolor, 1, PyFloat_FromDouble(1.0));
		PyTuple_SetItem(tintcolor, 2, PyFloat_FromDouble(1.0));
		PyTuple_SetItem(tintcolor, 3, PyFloat_FromDouble(1.0));
		PyObject* bordercolor = PyTuple_New(4);
		PyTuple_SetItem(bordercolor, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(bordercolor, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(bordercolor, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(bordercolor, 3, PyFloat_FromDouble(0.0));

		if (!Translators["add_image"].parse(args, kwargs, __FUNCTION__, &name, &value, &tintcolor, &bordercolor, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto mtintcolor = mvPythonTranslator::getColor(tintcolor);
		auto mbordercolor = mvPythonTranslator::getColor(bordercolor);

		mvAppItem* item = new mvImage("", name, value, mtintcolor, mbordercolor);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_drag_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		float default_value = 0.0f;
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["add_drag_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvDragFloat("", name, default_value, speed, min_value, max_value, format, power);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_drag_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["add_drag_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvDragFloat2("", name, vec.data(), speed, min_value, max_value, format, power);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_drag_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["add_drag_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvDragFloat3("", name, vec.data(), speed, min_value, max_value, format, power);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_drag_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["add_drag_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvDragFloat4("", name, vec.data(), speed, min_value, max_value, format, power);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_drag_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		int default_value = 0;
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["add_drag_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed, 
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvDragInt("", name, default_value, speed, min_value, max_value, format);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_drag_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["add_drag_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvDragInt2("", name, vec.data(), speed, min_value, max_value, format);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_drag_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["add_drag_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvDragInt3("", name, vec.data(), speed, min_value, max_value, format);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_drag_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["add_drag_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvDragInt4("", name, vec.data(), speed, min_value, max_value, format);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_slider_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		float default_value = 0.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		int vertical = false;

		if (!Translators["add_slider_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value, 
			&min_value, &max_value, &format, &power, &vertical, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSliderFloat("", name, default_value, min_value, max_value, format, power, vertical);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_slider_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["add_slider_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvSliderFloat2("", name, vec.data(), min_value, max_value, format, power);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_slider_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["add_slider_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvSliderFloat3("", name, vec.data(), min_value, max_value, format, power);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_slider_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;

		if (!Translators["add_slider_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &power, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvSliderFloat4("", name, vec.data(), min_value, max_value, format, power);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_slider_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		int default_value = 0;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		int vertical = false;

		if (!Translators["add_slider_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &vertical, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSliderInt("", name, default_value, min_value, max_value, format, vertical);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_slider_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["add_slider_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvSliderInt2("", name, vec.data(), min_value, max_value, format);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_slider_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["add_slider_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvSliderInt3("", name, vec.data(), min_value, max_value, format);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_slider_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";

		if (!Translators["add_slider_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvSliderInt4("", name, vec.data(), min_value, max_value, format);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		int wrap = 0, bullet = 0;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));

		if (!Translators["add_text"].parse(args, kwargs,__FUNCTION__, &name, &wrap, &color, &bullet, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);

		if (mcolor.r > 500)
			mcolor.specified = false;

		mvAppItem* item = new mvText("", name, wrap, mcolor, bullet);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_label_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		const char* value;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));

		if (!Translators["add_label_text"].parse(args, kwargs,__FUNCTION__, &name, &value, &color, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);
		if (mcolor.r > 500)
			mcolor.specified = false;

		mvAppItem* item = new mvLabelText("", std::string(name), value, mcolor);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_listbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* items;
		int default_value = 0;

		if (!Translators["add_listbox"].parse(args, kwargs,__FUNCTION__, &name, &items, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvListbox("", name, mvPythonTranslator::getStringVec(items), default_value, height);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_combo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		const char* default_value = "";
		PyObject* items;

		if (!Translators["add_combo"].parse(args, kwargs,__FUNCTION__, &name, &items, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvCombo("", name, mvPythonTranslator::getStringVec(items), default_value);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_selectable(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int default_value = false;

		if (!Translators["add_selectable"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSelectable("", name, default_value);

		MV_STANDARD_CALLBACK_EVAL();
		

		Py_RETURN_NONE;
	}

	PyObject* add_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		int small = false;
		int arrow = false;
		int direction = -1;

		if (!Translators["add_button"].parse(args, kwargs,__FUNCTION__, &name, &small,
			&arrow, &direction, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvButton("", name, small, arrow, direction);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_input_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		const char* hint = "";
		int multiline = 0;
		int flags = 0;

		if (!Translators["add_input_text"].parse(args, kwargs,__FUNCTION__, &name, &hint, &multiline, &flags, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvInputText("", name, hint, multiline, flags);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_input_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int default_value = 0;

		if (!Translators["add_input_int"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvInputInt("", name, default_value);

		MV_STANDARD_CALLBACK_EVAL();
		
		Py_RETURN_NONE;
	}

	PyObject* add_input_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));

		if (!Translators["add_input_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvInputInt2("", name, vec.data());

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_input_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));

		if (!Translators["add_input_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvInputInt3("", name, vec.data());

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_input_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));

		if (!Translators["add_input_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvInputInt4("", name, vec.data());

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_input_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		float default_value = 0.0f;
		const char* format = "%.3f";

		if (!Translators["add_input_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvInputFloat("", name, default_value, format);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_input_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		const char* format = "%.3f";

		if (!Translators["add_input_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvInputFloat2("", name, vec.data(), format);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_input_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		const char* format = "%.3f";

		if (!Translators["add_input_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvInputFloat3("", name, vec.data(), format);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_input_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		const char* format = "%.3f";

		if (!Translators["add_input_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &format, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvInputFloat4("", name, vec.data(), format);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_indent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		MV_STANDARD_CALLBACK_INIT();
		std::string sname = std::string("indent" + std::to_string(i));
		const char* name = sname.c_str();
		float offset = 0.0f;

		if (!Translators["add_indent"].parse(args, kwargs,__FUNCTION__, &name, &offset, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvIndent("", name, offset);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* unindent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		MV_STANDARD_CALLBACK_INIT();
		std::string sname = std::string("unindent" + std::to_string(i));
		const char* name = sname.c_str();
		float offset = 0.0f;

		if (!Translators["unindent"].parse(args, kwargs,__FUNCTION__, &name, &offset, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvUnindent("", name, offset);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int flags = 0;

		if (!Translators["add_tab_bar"].parse(args, kwargs, __FUNCTION__, &name, &flags, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTabBar("", name, flags);
		MV_STANDARD_CALLBACK_EVAL();
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* add_tab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["add_tab"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;
		
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("add_tab must follow a call to add_tabbar.");

		else if (parentItem->getType() == mvAppItemType::TabBar)
		{
			mvAppItem* item = new mvTab("", name);
			MV_STANDARD_CALLBACK_EVAL();
			mvApp::GetApp()->pushParent(item);
		}

		else
			mvAppLog::getLogger()->LogError("add_tab was called incorrectly. Did you forget to call end_tab?");

		Py_RETURN_NONE;
	}

	PyObject* end_tab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		Py_RETURN_NONE;

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_tab must follow a call to add_tab.");

		else if (parentItem->getType() == mvAppItemType::TabItem)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_tab was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* end_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		Py_RETURN_NONE;

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_tabbar must follow a call to add_menubar.");

		else if (parentItem->getType() == mvAppItemType::TabBar)
			mvApp::GetApp()->popParent();

		else
		{
			mvAppLog::getLogger()->LogError("add_menubar was called incorrectly. Did you forget to call end_menu?");
			mvAppLog::getLogger()->LogWarning("Taking corrective action. Clearing parent stack.");

			auto item = mvApp::GetApp()->popParent();
			while (item->getType() != mvAppItemType::TabBar)
			{
				mvApp::GetApp()->popParent();
				item = mvApp::GetApp()->topParent();

				if (item == nullptr)
					break;
			}

		}

		Py_RETURN_NONE;
	}

	PyObject* add_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["add_menu_bar"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto parentItem = mvApp::GetApp()->topParent();
		
		if (parentItem->getType() == mvAppItemType::Window)
		{
			auto window = static_cast<mvWindowAppitem*>(parentItem);
			window->addFlag(ImGuiWindowFlags_MenuBar);
			mvAppItem* item = new mvMenuBar(name);
			MV_STANDARD_CALLBACK_EVAL();
			mvApp::GetApp()->pushParent(item);
		}
		

		Py_RETURN_NONE;
	}

	PyObject* add_menu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["add_menu"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("add_menu must follow a call to add_menubar.");

		else if (parentItem->getType() == mvAppItemType::MenuBar || parentItem->getType() == mvAppItemType::Menu)
		{
			mvAppItem* item = new mvMenu("", name);
			MV_STANDARD_CALLBACK_EVAL();
			mvApp::GetApp()->pushParent(item);
		}

		else
			mvAppLog::getLogger()->LogError("add_menu was called incorrectly. Did you forget to call end_menu?");

		Py_RETURN_NONE;
	}

	PyObject* end_menu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();
		
		if(parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_menu must follow a call to add_menu.");

		else if (parentItem->getType() == mvAppItemType::Menu)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_menu was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* end_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_menu_bar must follow a call to add_menu_bar.");

		else if (parentItem->getType() == mvAppItemType::MenuBar)
			mvApp::GetApp()->popParent();

		else
		{
			mvAppLog::getLogger()->LogError("end_menu_bar was called incorrectly. Did you forget to call end_menu?");
			mvAppLog::getLogger()->LogWarning("Taking corrective action. Clearing parent stack.");

			auto item = mvApp::GetApp()->popParent();
			while (item != nullptr)
			{
				mvApp::GetApp()->popParent();
				item = mvApp::GetApp()->topParent();
			}

		}

		Py_RETURN_NONE;
	}

	PyObject* add_menu_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["add_menu_item"].parse(args, kwargs,__FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvMenuItem("", name);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_spacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;

		MV_STANDARD_CALLBACK_INIT();
		std::string sname = std::string("spacing" + std::to_string(i));
		const char* name = sname.c_str();
		int count = 1;

		if (!Translators["add_spacing"].parse(args, kwargs,__FUNCTION__, &name, &count, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSpacing("", name, count);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_same_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		MV_STANDARD_CALLBACK_INIT();
		std::string sname = std::string("sameline" + std::to_string(i));
		const char* name = sname.c_str();
		float xoffset = 0.0f;
		float spacing = 0.0f;

		if (!Translators["add_same_line"].parse(args, kwargs,__FUNCTION__, &name, &xoffset, &spacing, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSameLine("", name, xoffset, spacing);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_radio_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* items;
		int default_value = 0;

		if (!Translators["add_radio_button"].parse(args, kwargs,__FUNCTION__, &name, &items, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvRadioButton("", name, mvPythonTranslator::getStringVec(items), default_value);

		MV_STANDARD_CALLBACK_EVAL();
		
		Py_RETURN_NONE;
	}

	PyObject* add_group(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;

		if (!Translators["add_group"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvGroup("", name);
		MV_STANDARD_CALLBACK_EVAL();
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* end_group(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_group must follow a call to add_group.");

		else if (parentItem->getType() == mvAppItemType::Group)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_group was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* add_child(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();

		const char* name;

		if (!Translators["add_child"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvChild("", name);
		MV_STANDARD_CALLBACK_EVAL();
		mvApp::GetApp()->pushParent(item);

		Py_RETURN_NONE;
	}

	PyObject* add_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width, height;

		if (!Translators["add_window"].parse(args, kwargs, __FUNCTION__, &name, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvWindowAppitem("", name, width, height, 0, 0, false);
		mvApp::GetApp()->addWindow(item);
		mvApp::GetApp()->pushParent(item);

		Py_RETURN_NONE;
	}

	PyObject* end_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_window must follow a call to add_window.");

		else if (parentItem->getType() == mvAppItemType::Window)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_window was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* end_child(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_child must follow a call to add_child.");

		else if (parentItem->getType() == mvAppItemType::Child)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_child was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* add_tooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* tipparent, * name;

		if (!Translators["add_tooltip"].parse(args, kwargs, __FUNCTION__, &tipparent, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTooltip(tipparent, name);
		MV_STANDARD_CALLBACK_EVAL();
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* end_tooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_tooltip must follow a call to add_tooltip.");

		else if (parentItem->getType() == mvAppItemType::Tooltip)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_tooltip was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* add_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* popupparent, * name;
		int mousebutton = 1;
		int modal = false;

		if (!Translators["add_popup"].parse(args, kwargs, __FUNCTION__, &popupparent, &name, &mousebutton, &modal, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvPopup(popupparent, name, mousebutton, modal);
		MV_STANDARD_CALLBACK_EVAL();
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* end_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_popup must follow a call to add_group.");

		else if (parentItem->getType() == mvAppItemType::Popup)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_popup was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* add_collapsing_header(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int flags = 0;

		if (!Translators["add_collapsing_header"].parse(args, kwargs, __FUNCTION__, &name, &flags, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvCollapsingHeader("", name, flags);
		MV_STANDARD_CALLBACK_EVAL();
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* end_collapsing_header(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_collapsing_header must follow a call to add_collapsing_header.");

		else if (parentItem->getType() == mvAppItemType::CollapsingHeader)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_collapsing_header was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* add_tree_node(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int flags = 0;

		if (!Translators["add_tree_node"].parse(args, kwargs, __FUNCTION__, &name, &flags, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTreeNode("", name, flags);
		MV_STANDARD_CALLBACK_EVAL();
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* end_tree_node(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("end_tree_node must follow a call to add_tree_node.");

		else if (parentItem->getType() == mvAppItemType::TreeNode)
			mvApp::GetApp()->popParent();

		else
			mvAppLog::getLogger()->LogError("end_tree_node was called incorrectly and will be ignored");

		Py_RETURN_NONE;
	}

	PyObject* add_seperator(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		MV_STANDARD_CALLBACK_INIT();
		std::string sname = std::string("seperator" + std::to_string(i));
		const char* name = sname.c_str();

		if (!Translators["add_seperator"].parse(args, kwargs, __FUNCTION__, &name, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSeparator("", name);

		MV_STANDARD_CALLBACK_EVAL();
		
		Py_RETURN_NONE;
	}
	
	PyObject* add_color_edit3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));

		if (!Translators["add_color_edit3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorEdit3("", name, color);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_color_edit4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));

		if (!Translators["add_color_edit4"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorEdit4("", name, color);
		MV_STANDARD_CALLBACK_EVAL();
		
		Py_RETURN_NONE;
	}

	PyObject* add_color_picker3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));

		if (!Translators["add_color_picker3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorPicker3("", name, color);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_color_picker4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(1));

		if (!Translators["add_color_picker4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorPicker4("", name, color);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	PyObject* add_checkbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		MV_STANDARD_CALLBACK_INIT();
		const char* name;
		int default_value = 0;

		if (!Translators["add_checkbox"].parse(args, kwargs,__FUNCTION__, &name, &default_value, MV_STANDARD_CALLBACK_PARSE))
			Py_RETURN_NONE;

		mvAppItem* item = new mvCheckbox("", name, default_value);

		MV_STANDARD_CALLBACK_EVAL();

		Py_RETURN_NONE;
	}

	mvPythonModule* CreateWidgetAddingInterface()
	{

		auto pyModule = new mvPythonModule("sbWidgets", {});

		pyModule->addMethodD(end_tree_node);
		pyModule->addMethodD(end_popup);
		pyModule->addMethodD(end_window);
		pyModule->addMethodD(end_group);
		pyModule->addMethodD(end_child);
		pyModule->addMethodD(end_tab);
		pyModule->addMethodD(end_tab_bar);
		pyModule->addMethodD(end_menu);
		pyModule->addMethodD(end_menu_bar);
		pyModule->addMethodD(end_tooltip);
		pyModule->addMethodD(end_collapsing_header);
		pyModule->addMethodD(add_image);
		pyModule->addMethodD(add_progress_bar);
		pyModule->addMethodD(add_drag_float);
		pyModule->addMethodD(add_drag_int);
		pyModule->addMethodD(add_drag_float2);
		pyModule->addMethodD(add_drag_float3);
		pyModule->addMethodD(add_drag_float4);
		pyModule->addMethodD(add_drag_int2);
		pyModule->addMethodD(add_drag_int3);
		pyModule->addMethodD(add_drag_int4);
		pyModule->addMethodD(add_slider_float);
		pyModule->addMethodD(add_slider_int);
		pyModule->addMethodD(add_slider_float2);
		pyModule->addMethodD(add_slider_float3);
		pyModule->addMethodD(add_slider_float4);
		pyModule->addMethodD(add_slider_int2);
		pyModule->addMethodD(add_slider_int3);
		pyModule->addMethodD(add_slider_int4);
		pyModule->addMethodD(add_tree_node);
		pyModule->addMethodD(add_selectable);
		pyModule->addMethodD(add_popup);
		pyModule->addMethodD(add_window);
		pyModule->addMethodD(add_indent);
		pyModule->addMethodD(unindent);
		pyModule->addMethodD(add_simple_plot);
		pyModule->addMethodD(add_combo);
		pyModule->addMethodD(add_text);
		pyModule->addMethodD(add_label_text);
		pyModule->addMethodD(add_listbox);
		pyModule->addMethodD(add_color_edit3);
		pyModule->addMethodD(add_color_edit4);
		pyModule->addMethodD(add_color_picker3);
		pyModule->addMethodD(add_color_picker4);
		pyModule->addMethodD(add_seperator);
		pyModule->addMethodD(add_button);
		pyModule->addMethodD(add_input_text);
		pyModule->addMethodD(add_input_int);
		pyModule->addMethodD(add_input_float);
		pyModule->addMethodD(add_radio_button);
		pyModule->addMethodD(add_checkbox);
		pyModule->addMethodD(add_group);
		pyModule->addMethodD(add_child);
		pyModule->addMethodD(add_tab_bar);
		pyModule->addMethodD(add_tab);
		pyModule->addMethodD(add_menu_bar);
		pyModule->addMethodD(add_menu);
		pyModule->addMethodD(add_menu_item);
		pyModule->addMethodD(add_spacing);
		pyModule->addMethodD(add_same_line);
		pyModule->addMethodD(add_tooltip);
		pyModule->addMethodD(add_collapsing_header);
		
		return pyModule;
	}
}