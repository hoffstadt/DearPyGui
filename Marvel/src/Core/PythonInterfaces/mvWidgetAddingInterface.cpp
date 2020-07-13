#include "mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"
#include "mvPythonParser.h"
#include "mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"

namespace Marvel {

	static std::map<std::string, mvPythonParser> Parsers = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbWidgets");

	static void AddItemWithRuntimeChecks(mvAppItem* item, const char* parent, const char* before)
	{
		auto ma = mvApp::GetApp();

		// typical run time adding
		if ((!std::string(parent).empty() || !std::string(before).empty()) && ma->isStarted())
			ma->addRuntimeItem(parent, before, item);

		// adding without specifying before or parent, instead using parent stack
		else if (std::string(parent).empty() && std::string(before).empty() && ma->isStarted() && ma->topParent() != nullptr)
			ma->addRuntimeItem(ma->topParent()->getName(), before, item);

		// adding without specifying before or parent, but with empty stack (add to main window)
		else if (std::string(parent).empty() && std::string(before).empty() && ma->isStarted())
			ma->addRuntimeItem("MainWindow", "", item);

		// adding normally but using the runtime style of adding
		else if (!std::string(parent).empty() && !ma->isStarted())
			ma->addRuntimeItem(parent, before, item);

		// typical adding before runtime
		else if (std::string(parent).empty() && !ma->isStarted() && std::string(before).empty())
			ma->addItem(item);
	}

	std::map<std::string, mvPythonParser>& BuildWidgetsInterface() {
		
		std::map<std::string, mvPythonParser>* parsers = new std::map< std::string, mvPythonParser>;

		parsers->insert({ "end_tree_node", mvPythonParser({
		}, "Ends the tree node created by a call to add_tree_node.") });

		parsers->insert({ "end_popup", mvPythonParser({
		}, "Ends the popup created by a call to add_popup.") });

		parsers->insert({ "end_window", mvPythonParser({
		}, "Ends the window created by a call to add_window.") });

		parsers->insert({ "end_group", mvPythonParser({
		}, "Ends the group created by a call to add_group.") });

		parsers->insert({ "end_child", mvPythonParser({
		}, "Ends the child created by a call to add_child.") });

		parsers->insert({ "end_tab", mvPythonParser({
		}, "Ends the tab created by a call to add_tab.") });

		parsers->insert({ "end_tab_bar", mvPythonParser({
		}, "Ends the tab bar created by a call to add_tab_bar") });

		parsers->insert({ "end_menu", mvPythonParser({
		}, "Ends the menu created by a call to add_menu.") });

		parsers->insert({ "end_menu_bar", mvPythonParser({
		}, "Ends the menu bar created by a call to add_menu_bar.") });

		parsers->insert({ "end_tooltip", mvPythonParser({
		}, "Ends the tooltip created by a call to add_tooltip.") });

		parsers->insert({ "end_collapsing_header", mvPythonParser({
		}, "Ends the collapsing header created by a call to add_collapsing_header.") });

		parsers->insert({ "add_table", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "headers"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before","Item to add this item before. (runtime adding)"},
		}, "Adds table.") });

		parsers->insert({ "add_seperator", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before","Item to add this item before. (runtime adding)"},
		}, "Adds a horizontal line.") });

		parsers->insert({ "add_simple_plot", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "value", "Tuple of float values"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "autoscale", "autoscales range based on data (default is True)"},
			{mvPythonDataType::String, "overlay", "overlays text (similar to a plot title)"},
			{mvPythonDataType::Float, "minscale", "used if autoscale is false"},
			{mvPythonDataType::Float, "maxscale", "used if autoscale is false"},
			{mvPythonDataType::Bool, "histogram", "create a histogram"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before","Item to add this item before. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
		}, "A simple plot for visualization of a set of values") });

		parsers->insert({ "add_progress_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "value"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "overlay"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
		}, "Adds a progress bar.") });

		parsers->insert({ "add_image", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "tint_color"},
			{mvPythonDataType::FloatList, "border_color"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
		}, "Adds an image.") });

		parsers->insert({ "add_slider_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::Bool, "vertical"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height","Height of a vertical slider"},
		}, "Adds slider for a single float value") });

		parsers->insert({ "add_slider_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds slider for a 2 float values.") });

		parsers->insert({ "add_slider_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds slider for a 3 float values.") });

		parsers->insert({ "add_slider_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds slider for a 4 float values.") });

		parsers->insert({ "add_slider_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Bool, "vertical"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height","Height of a vertical slider"},
		}, "Adds slider for a single int value") });

		parsers->insert({ "add_slider_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds slider for a 2 int values.") });

		parsers->insert({ "add_slider_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds slider for a 3 int values.") });

		parsers->insert({ "add_slider_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds slider for a 4 int values.") });

		parsers->insert({ "add_drag_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a single float value") });

		parsers->insert({ "add_drag_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a 2 float values.") });

		parsers->insert({ "add_drag_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a 3 float values.") });

		parsers->insert({ "add_drag_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Float, "min_value"},
			{mvPythonDataType::Float, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::Float, "power"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a 4 float values.") });

		parsers->insert({ "add_drag_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a single int value") });

		parsers->insert({ "add_drag_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a 2 int values.") });

		parsers->insert({ "add_drag_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a 3 int values.") });

		parsers->insert({ "add_drag_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Float, "speed"},
			{mvPythonDataType::Integer, "min_value"},
			{mvPythonDataType::Integer, "max_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""}
		}, "Adds drag for a 4 int values.") });

		parsers->insert({ "add_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "wrap", "number of characters until wraping"},
			{mvPythonDataType::FloatList, "color", "color of the text (rgba)"},
			{mvPythonDataType::Bool, "bullet", "makes the text bulleted"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds text") });

		parsers->insert({ "add_label_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
		}, "Adds text with a label. Useful for output values.") });

		parsers->insert({ "add_listbox", mvPythonParser({
			{mvPythonDataType::String, "name", "Name of the listbox"},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", "number of items to show"},
		}, "Adds a listbox.") });

		parsers->insert({ "add_combo", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds a combo.") });

		parsers->insert({ "add_selectable", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
		}, "Adds a selectable.") });

		parsers->insert({ "add_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "small", "Small button, useful for embedding in text."},
			{mvPythonDataType::Bool, "arrow", "Arrow button."},
			{mvPythonDataType::Integer, "direction", "A cardinal direction"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds a button.") });

		parsers->insert({ "add_input_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "default_value"},
			{mvPythonDataType::String, "hint"},
			{mvPythonDataType::Bool, "multiline"},
			{mvPythonDataType::Bool, "no_spaces"},
			{mvPythonDataType::Bool, "uppercase"},
			{mvPythonDataType::Bool, "decimal"},
			{mvPythonDataType::Bool, "hexadecimal"},
			{mvPythonDataType::Bool, "readonly"},
			{mvPythonDataType::Bool, "password"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for text values.") });

		parsers->insert({ "add_input_int", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for integer values.") });

		parsers->insert({ "add_input_int2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for 2 integer values.") });

		parsers->insert({ "add_input_int3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for 3 integer values.") });

		parsers->insert({ "add_input_int4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for 4 integer values.") });

		parsers->insert({ "add_input_float", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for float values.") });

		parsers->insert({ "add_input_float2", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for 2 float values.") });

		parsers->insert({ "add_input_float3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for 3 float values.") });

		parsers->insert({ "add_input_float4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value"},
			{mvPythonDataType::String, "format"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
		}, "Adds input for 4 float values.") });

		parsers->insert({ "add_indent", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "offset"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds an indent to following items.") });

		parsers->insert({ "unindent", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "offset"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Unindents following items.") });

		parsers->insert({ "add_tab_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "reorderable"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
		}, "Adds a tab bar.") });

		parsers->insert({ "add_tab", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "closable"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds a tab to a tab bar.") });

		parsers->insert({ "add_menu_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds a menu bar to a window. Must be followed by a call to end_menu_bar.") });

		parsers->insert({ "add_menu", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds a menu to an existing menu bar. Must be followed by a call to end_menu.") });

		parsers->insert({ "add_menu_item", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds a menu item to an existing menu.") });

		parsers->insert({ "add_spacing", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Integer, "count"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds vertical spacing.") });

		parsers->insert({ "add_same_line", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "xoffset"},
			{mvPythonDataType::Float, "spacing"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Places a widget on the same line as the previous widget. Can also be used for horizontal spacing.") });

		parsers->insert({ "add_radio_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
		}, "Adds a set of radio buttons.") });

		parsers->insert({ "add_group", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""}
		}, "Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.\
				Must be closed with the end_group command unless added at runtime.") });

		parsers->insert({ "add_child", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end_child.") });

		parsers->insert({ "add_window", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "start_x"},
			{mvPythonDataType::Integer, "start_y"},
			{mvPythonDataType::Bool, "autosize"},
			{mvPythonDataType::Bool, "hide"}
		}, "Creates a new window for following items to be added to. Must call end_main_window command before.") });

		parsers->insert({ "add_tooltip", mvPythonParser({
			{mvPythonDataType::String, "tipparent"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"}
		}, "Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for.") });

		parsers->insert({ "add_popup", mvPythonParser({
			{mvPythonDataType::String, "popupparent"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "mousebutton"},
			{mvPythonDataType::Bool, "modal"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds a popup window for an item. This command must come immediately after the item the popup is for.") });

		parsers->insert({ "add_collapsing_header", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_open"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"}
		}, "Adds a collapsing header to add items to. Must be closed with the end_collapsing_header command.") });

		parsers->insert({ "add_tree_node", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_open"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
		}, "Adds a tree node to add items to. Must be closed with the end_tree_node command.") });

		parsers->insert({ "add_color_edit3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value (0, 0, 0)"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an rgb color editing widget.") });

		parsers->insert({ "add_color_edit4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value (0, 0, 0, 255)"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an rgba color editing widget.") });

		parsers->insert({ "add_color_picker3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value (0, 0, 0)"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an rgb color picking widget.") });

		parsers->insert({ "add_color_picker4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value (0, 0, 0, 255)"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
		}, "Adds an rgba color picking widget.") });

		parsers->insert({ "add_checkbox", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::String, "callback", "Registers a callback"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "Item to add this item before. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"}
		}, "Adds a checkbox widget.") });

		return *parsers;
	}

	PyObject* add_table(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* headers;
		const char* callback = "";
		const char* parent = "";
		const char* before = "";

		if (!Parsers["add_table"].parse(args, kwargs, __FUNCTION__, &name, &headers, &callback, &parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTable("", name, mvPythonTranslator::getStringVec(headers));
		item->setCallback(callback);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_simple_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* overlay = "";
		float minscale = 0.0f;
		float maxscale = 0.0f;
		int autoscale = true;
		int histogram = false;
		PyObject* value;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;

		if (!Parsers["add_simple_plot"].parse(args, kwargs,__FUNCTION__, &name, &value, &autoscale, &overlay, 
			&minscale, &maxscale, &histogram, &tip, &parent, &before, &width, &height))
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
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_progress_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		const char* overlay = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int height = 0;

		if (!Parsers["add_progress_bar"].parse(args, kwargs, __FUNCTION__, &name, &default_value, 
			&overlay, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvProgressBar("", name, default_value, overlay);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);
		item->setDataSource(data_source);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_image(PyObject* self, PyObject* args, PyObject* kwargs)
	{
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
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int height = 0;

		if (!Parsers["add_image"].parse(args, kwargs, __FUNCTION__, &name, 
			&value, &tintcolor, &bordercolor, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		auto mtintcolor = mvPythonTranslator::getColor(tintcolor);
		auto mbordercolor = mvPythonTranslator::getColor(bordercolor);

		mvAppItem* item = new mvImage("", name, value, mtintcolor, mbordercolor);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);
		item->setDataSource(data_source);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_drag_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		const char* callback = "";
		const char*tip = ""; 
		int width = 0; 
		const char* before = ""; 
		const char* parent = ""; 
		const char* data_source = "";

		if (!Parsers["add_drag_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		mvAppItem* item = new mvDragFloat("", name, default_value, speed, min_value, max_value, format, power);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		float speed = 1.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvDragFloat2("", name, vec.data(), speed, min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
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
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvDragFloat3("", name, vec.data(), speed, min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
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
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvDragFloat4("", name, vec.data(), speed, min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed, 
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		mvAppItem* item = new mvDragInt("", name, default_value, speed, min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvDragInt2("", name, vec.data(), speed, min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		float speed = 1.0f;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvDragInt3("", name, vec.data(), speed, min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_drag_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
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
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_drag_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvDragInt4("", name, vec.data(), speed, min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_slider_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		int vertical = false;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int height = 0;

		if (!Parsers["add_slider_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value, 
			&min_value, &max_value, &format, &power, &vertical, &callback, &tip, &parent, &before, 
			&data_source, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSliderFloat("", name, default_value, min_value, max_value, format, power, vertical);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_slider_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source,
			&width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvSliderFloat2("", name, vec.data(), min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
		float min_value = 0.0f;
		float max_value = 1.0f;
		const char* format = "%.3f";
		float power = 1.0f;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_slider_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source,
			&width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvSliderFloat3("", name, vec.data(), min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
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
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_slider_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &power, &callback, &tip, &parent, &before, &data_source,
			&width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvSliderFloat4("", name, vec.data(), min_value, max_value, format, power);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		int vertical = false;
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;
		int height = 0;

		if (!Parsers["add_slider_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &vertical, &callback, &tip, &parent, &before, &data_source,
			&width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSliderInt("", name, default_value, min_value, max_value, format, vertical);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_slider_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source,
			&width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvSliderInt2("", name, vec.data(), min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_slider_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source,
			&width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvSliderInt3("", name, vec.data(), min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_slider_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		int min_value = 0;
		int max_value = 100;
		const char* format = "%d";
		const char* callback = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";
		int width = 0;

		if (!Parsers["add_slider_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&min_value, &max_value, &format, &callback, &tip, &parent, &before, &data_source,
			&width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvSliderInt4("", name, vec.data(), min_value, max_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int wrap = 0;
		int bullet = false;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = ""; 


		if (!Parsers["add_text"].parse(args, kwargs,__FUNCTION__, &name, &wrap, 
			&color, &bullet, &tip, &parent, &before))
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);

		if (mcolor.r > 500)
			mcolor.specified = false;

		mvAppItem* item = new mvText("", name, wrap, mcolor, bullet);
		item->setTip(tip);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_label_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;
		const char* value;
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = ""; 
		const char* data_source = "";


		if (!Parsers["add_label_text"].parse(args, kwargs,__FUNCTION__, &name, &value, 
			&color, &tip, &parent, &before, &data_source))
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);
		if (mcolor.r > 500)
			mcolor.specified = false;

		mvAppItem* item = new mvLabelText("", std::string(name), value, mcolor);
		item->setTip(tip); 
		item->setDataSource(data_source);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_listbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* items;
		int default_value = 0;
		const char* callback = "";
		const char* tip = ""; 
		int width = 0; 
		int height = 3; 
		const char* before = ""; 
		const char* parent = ""; 
		const char* data_source = "";

		if (!Parsers["add_listbox"].parse(args, kwargs,__FUNCTION__, &name, &items, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvListbox("", name, mvPythonTranslator::getStringVec(items), default_value, height);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_combo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* default_value = "";
		PyObject* items;
		const char* callback = "";
		const char* tip = ""; 
		int width = 0;
		const char* before = ""; 
		const char* parent = ""; 
		const char* data_source = "";

		if (!Parsers["add_combo"].parse(args, kwargs,__FUNCTION__, &name, &items, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		mvAppItem* item = new mvCombo("", name, mvPythonTranslator::getStringVec(items), default_value);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_selectable(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = false;
		const char* callback = "";
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = ""; 
		const char* data_source = "";

		if (!Parsers["add_selectable"].parse(args, kwargs,__FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSelectable("", name, default_value);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int small = false;
		int arrow = false;
		int direction = -1;
		const char* callback = "";
		const char* tip = ""; 
		int width = 0; 
		int height = 0; 
		const char* before = ""; 
		const char* parent = ""; 

		if (!Parsers["add_button"].parse(args, kwargs,__FUNCTION__, &name, &small,
			&arrow, &direction, &callback, &tip, &parent, &before, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvButton("", name, small, arrow, direction);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* default_value = "";
		const char* hint = "";
		int multiline = 0;
		int no_spaces = false;
		int uppercase = false;
		int decimal = false;
		int hexadecimal = false;
		int readonly = false;
		int password = false;
		const char* callback = "";
		const char* tip = ""; 
		int width = 0; 
		const char* before = ""; 
		const char* parent = ""; 
		const char* data_source = "";

		int flags = 0;

		if (!Parsers["add_input_text"].parse(args, kwargs,__FUNCTION__, &name, &default_value, &hint, &multiline, &no_spaces, 
			&uppercase, &decimal, &hexadecimal, &readonly, &password, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		if (no_spaces) flags |= ImGuiInputTextFlags_CharsNoBlank;
		if (uppercase) flags |= ImGuiInputTextFlags_CharsUppercase;
		if (decimal) flags |= ImGuiInputTextFlags_CharsDecimal;
		if (hexadecimal) flags |= ImGuiInputTextFlags_CharsHexadecimal;
		if (readonly) flags |= ImGuiInputTextFlags_ReadOnly;
		if (password) flags |= ImGuiInputTextFlags_Password;

		mvAppItem* item = new mvInputText("", name, default_value, hint, multiline, flags);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width);
				
		AddItemWithRuntimeChecks(item, parent, before);

		Py_RETURN_NONE;
	}

	PyObject* add_input_int(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_int"].parse(args, kwargs,__FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		mvAppItem* item = new mvInputInt("", name, default_value);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_int2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_int2"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvInputInt2("", name, vec.data());
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_int3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_int3"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvInputInt3("", name, vec.data());
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
			
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_int4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_int4"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getIntVec(default_value);

		mvAppItem* item = new mvInputInt4("", name, vec.data());
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
				
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_float(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		const char* format = "%.3f";
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_float"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		mvAppItem* item = new mvInputFloat("", name, default_value, format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_float2(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(2);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		const char* format = "%.3f";
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_float2"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvInputFloat2("", name, vec.data(), format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_float3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		const char* format = "%.3f";
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_float3"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvInputFloat3("", name, vec.data(), format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_input_float4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		const char* format = "%.3f";
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_input_float4"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &format, &callback, &tip, &parent, &before, &data_source, &width))
			Py_RETURN_NONE;

		auto vec = mvPythonTranslator::getFloatVec(default_value);

		mvAppItem* item = new mvInputFloat4("", name, vec.data(), format);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);

		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_indent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("indent" + std::to_string(i));
		const char* name = sname.c_str();
		float offset = 0.0f;
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_indent"].parse(args, kwargs,__FUNCTION__, &name, &offset, 
			&parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvIndent("", name, offset);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* unindent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("unindent" + std::to_string(i));
		const char* name = sname.c_str();
		float offset = 0.0f;
		const char* before = "";
		const char* parent = "";

		if (!Parsers["unindent"].parse(args, kwargs,__FUNCTION__, &name, &offset, &parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvUnindent("", name, offset);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int reorderable = false;
		const char* callback = "";
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";

		if (!Parsers["add_tab_bar"].parse(args, kwargs, __FUNCTION__, &name, &reorderable, 
			&callback, &parent, &before, &data_source))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTabBar("", name, reorderable);
		item->setCallback(callback); 
		item->setDataSource(data_source);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);
		Py_RETURN_NONE;
	}

	PyObject* add_tab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int closeable = false;
		const char* tip = "";
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_tab"].parse(args, kwargs, __FUNCTION__, &name, &closeable,
			&tip, &parent, &before))
			Py_RETURN_NONE;
		
		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("add_tab must follow a call to add_tabbar.");

		else if (parentItem->getType() == mvAppItemType::TabBar)
		{
			mvAppItem* item = new mvTab("", name, closeable);
			item->setTip(tip);
			AddItemWithRuntimeChecks(item, parent, before);
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
		const char* name;
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_menu_bar"].parse(args, kwargs, __FUNCTION__, &name,
			&parent, &before))
			Py_RETURN_NONE;

		auto parentItem = mvApp::GetApp()->topParent();
		
		if (parentItem->getType() == mvAppItemType::Window)
		{
			auto window = static_cast<mvWindowAppitem*>(parentItem);
			window->addFlag(ImGuiWindowFlags_MenuBar);
			mvAppItem* item = new mvMenuBar(name);
			AddItemWithRuntimeChecks(item, parent, before);
			mvApp::GetApp()->pushParent(item);
		}
		

		Py_RETURN_NONE;
	}

	PyObject* add_menu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_menu"].parse(args, kwargs, __FUNCTION__, &name, 
			&tip, &parent, &before))
			Py_RETURN_NONE;

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem == nullptr)
			mvAppLog::getLogger()->LogError("add_menu must follow a call to add_menubar.");

		else if (parentItem->getType() == mvAppItemType::MenuBar || parentItem->getType() == mvAppItemType::Menu)
		{
			mvAppItem* item = new mvMenu("", name);
			item->setTip(tip);
			AddItemWithRuntimeChecks(item, parent, before);
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
		const char* name;
		const char* callback = "";
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_menu_item"].parse(args, kwargs,__FUNCTION__, &name, 
			&callback, &tip, &parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvMenuItem("", name);
		item->setCallback(callback); 
		item->setTip(tip); 
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_spacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("spacing" + std::to_string(i));
		const char* name = sname.c_str();
		int count = 1;
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_spacing"].parse(args, kwargs,__FUNCTION__, &name, &count,
			&parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSpacing("", name, count);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_same_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("sameline" + std::to_string(i));
		const char* name = sname.c_str();
		float xoffset = 0.0f;
		float spacing = 0.0f;
		const char* before = "";
		const char* parent = "";

		if (!Parsers["add_same_line"].parse(args, kwargs,__FUNCTION__, &name,
			&xoffset, &spacing, &parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSameLine("", name, xoffset, spacing);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_radio_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* items;
		int default_value = 0;
		const char* callback = "";
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_radio_button"].parse(args, kwargs,__FUNCTION__, &name, &items, 
			&default_value, &callback, &tip, &parent, &before, &data_source))
			Py_RETURN_NONE;

		mvAppItem* item = new mvRadioButton("", name, mvPythonTranslator::getStringVec(items), default_value);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_group(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = ""; 
		int width = 0; 
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_group"].parse(args, kwargs, __FUNCTION__, &name, 
			&tip, &parent, &before, &width))
			Py_RETURN_NONE;

		mvAppItem* item = new mvGroup("", name);
		item->setTip(tip);
		item->setWidth(width);
		AddItemWithRuntimeChecks(item, parent, before);
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
		const char* name;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";

		if (!Parsers["add_child"].parse(args, kwargs, __FUNCTION__, &name, 
			&tip, &parent, &before, &width, &height))
			Py_RETURN_NONE;

		mvAppItem* item = new mvChild("", name);
		item->setTip(tip); 
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		mvApp::GetApp()->pushParent(item);

		Py_RETURN_NONE;
	}

	PyObject* add_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width = -1;
		int height = -1;
		int startx = 0;
		int starty = 0;
		int autosize = false;
		int hide = false;

		if (!Parsers["add_window"].parse(args, kwargs, __FUNCTION__, &name, &width, 
			&height, &startx, &starty, &autosize, &hide))
			Py_RETURN_NONE;

		if (width == -1 && height == -1)
		{
			width = 500;
			height = 500;
		}

		mvAppItem* item = new mvWindowAppitem("", name, width, height, startx, starty, false, autosize);
		mvApp::GetApp()->addWindow(item);
		mvApp::GetApp()->pushParent(item);

		if (hide)
			item->hide();

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
		const char* tipparent;
		const char*name;
		const char* parent = ""; 
		const char* before = ""; 

		if (!Parsers["add_tooltip"].parse(args, kwargs, __FUNCTION__, &tipparent, 
			&name, &parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvTooltip(tipparent, name);
		AddItemWithRuntimeChecks(item, parent, before);
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
		const char* popupparent;
		const char* name;
		int mousebutton = 1;
		int modal = false;
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";

		if (!Parsers["add_popup"].parse(args, kwargs, __FUNCTION__, &popupparent, 
			&name, &mousebutton, &modal, &parent, &before, &width, &height))
			Py_RETURN_NONE;

		auto PopupParent = mvApp::GetApp()->getItem(popupparent);
		if (PopupParent)
			PopupParent->setPopup(name);

		if (std::string(popupparent) == "")
			mvApp::GetApp()->getItem("MainWindow")->setPopup(name);

		mvAppItem* item = new mvPopup(popupparent, name, mousebutton, modal);
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
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
		const char* name;
		int default_open = false;
		int flags = 0;
		const char* tip = ""; 
		const char* before = ""; 
		const char* parent = "";

		if (!Parsers["add_collapsing_header"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_open, &tip, &parent, &before))
			Py_RETURN_NONE;

		if (default_open) flags |= ImGuiTreeNodeFlags_DefaultOpen;

		mvAppItem* item = new mvCollapsingHeader("", name, flags);
		item->setTip(tip);
		AddItemWithRuntimeChecks(item, parent, before);
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
		const char* name;
		int default_open = false;
		int flags = 0;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";

		if (!Parsers["add_tree_node"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_open, &tip, &parent, &before))
			Py_RETURN_NONE;

		if (default_open) flags |= ImGuiTreeNodeFlags_DefaultOpen;

		mvAppItem* item = new mvTreeNode("", name, flags);
		item->setTip(tip);
		AddItemWithRuntimeChecks(item, parent, before);
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
		std::string sname = std::string("seperator" + std::to_string(i));
		const char* name = sname.c_str();
		const char* tip = "";
		const char* parent = "";
		const char* before = "";

		if (!Parsers["add_seperator"].parse(args, kwargs, __FUNCTION__, &name, &tip, &parent, &before))
			Py_RETURN_NONE;

		mvAppItem* item = new mvSeparator("", name);
		item->setTip(tip);

		AddItemWithRuntimeChecks(item, parent, before);
		
		Py_RETURN_NONE;
	}
	
	PyObject* add_color_edit3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_color_edit3"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorEdit3("", name, color);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source); 
		item->setWidth(width); 
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_color_edit4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_color_edit4"].parse(args, kwargs,__FUNCTION__, &name, &default_value, &callback, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorEdit4("", name, color);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_color_picker3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(3);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_color_picker3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &callback, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorPicker3("", name, color);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_color_picker4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(1));
		const char* callback = "";
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";

		if (!Parsers["add_color_picker4"].parse(args, kwargs, __FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source, &width, &height))
			Py_RETURN_NONE;

		auto color = mvPythonTranslator::getColor(default_value);

		mvAppItem* item = new mvColorPicker4("", name, color);
		item->setCallback(callback);
		item->setTip(tip);
		item->setDataSource(data_source);
		item->setWidth(width);
		item->setHeight(height);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	PyObject* add_checkbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		const char* callback = "";
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		const char* data_source = "";


		if (!Parsers["add_checkbox"].parse(args, kwargs,__FUNCTION__, &name, 
			&default_value, &callback, &tip, &parent, &before, &data_source))
			Py_RETURN_NONE;

		mvAppItem* item = new mvCheckbox("", name, default_value);
		item->setCallback(callback); 
		item->setTip(tip); 
		item->setDataSource(data_source);
		AddItemWithRuntimeChecks(item, parent, before);
		Py_RETURN_NONE;
	}

	mvPythonModule* CreateWidgetAddingInterface()
	{

		auto pyModule = new mvPythonModule("sbWidgets", {});

		pyModule->addMethodD(add_table);
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
		pyModule->addMethodD(add_input_int2);
		pyModule->addMethodD(add_input_int3);
		pyModule->addMethodD(add_input_int4);
		pyModule->addMethodD(add_input_float);
		pyModule->addMethodD(add_input_float2);
		pyModule->addMethodD(add_input_float3);
		pyModule->addMethodD(add_input_float4);
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