#include "mvBasicWidgetInterface.h"
#include "mvInterfaceCore.h"

namespace Marvel {

	void AddBasicWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_time_picker", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Object, "default_value", "dict (i.e. {'hour': 14, 'min': 32, 'sec': 23}"},
			{mvPythonDataType::Bool, "hour24"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds a time selector widget.", "None", "Adding Widgets") });

		parsers->insert({ "add_date_picker", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Object, "default_value", "dict (i.e. {'month_day': 14, 'year':20, 'month':5})"},
			{mvPythonDataType::Integer, "level"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds a data selector widget.", "None", "Adding Widgets") });

		parsers->insert({ "add_separator", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a horizontal line.", "None", "Adding Widgets") });

		parsers->insert({ "add_simple_plot", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "value", "Tuple of float values"},
			{mvPythonDataType::String, "overlay", "overlays text (similar to a plot title)"},
			{mvPythonDataType::Float, "minscale", "used if autoscale is false"},
			{mvPythonDataType::Float, "maxscale", "used if autoscale is false"},
			{mvPythonDataType::Bool, "histogram", "create a histogram"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
			{mvPythonDataType::String, "source",""},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "A simple plot for visualization of a set of values", "None", "Adding Widgets") });

		parsers->insert({ "add_progress_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "value"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "overlay"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds a progress bar.", "None", "Adding Widgets") });

		parsers->insert({ "add_image", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "tint_color"},
			{mvPythonDataType::FloatList, "border_color"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
			{mvPythonDataType::FloatList, "uv_min"},
			{mvPythonDataType::FloatList, "uv_max"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds an image."
		"uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown."
		"Using(0,0)->(1,1) texture coordinates will generally display the entire texture", "None", "Adding Widgets") });

		parsers->insert({ "add_image_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::FloatList, "tint_color"},
			{mvPythonDataType::FloatList, "background_color"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""},
			{mvPythonDataType::Integer, "frame_padding",""},
			{mvPythonDataType::FloatList, "uv_min"},
			{mvPythonDataType::FloatList, "uv_max"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds an image button."
		"uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown."
		"Using(0,0)->(1,1) texture coordinates will generally display the entire texture", "None", "Adding Widgets") });

		parsers->insert({ "add_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "wrap", "number of characters until wraping"},
			{mvPythonDataType::FloatList, "color", "color of the text (rgba)"},
			{mvPythonDataType::Bool, "bullet", "makes the text bulleted"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds text", "None", "Adding Widgets") });

		parsers->insert({ "add_label_text", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "value"},
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds text with a label. Useful for output values.", "None", "Adding Widgets") });

		parsers->insert({ "add_listbox", mvPythonParser({
			{mvPythonDataType::String, "name", "Name of the listbox"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "num_items", "number of items to show"},
			{mvPythonDataType::String, "label",""},
			{mvPythonDataType::String, "popup",""},
			{mvPythonDataType::Bool, "show",""},
		}, "Adds a listbox.", "None", "Adding Widgets") });

		parsers->insert({ "add_combo", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::String, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::String, "label",""},
			{mvPythonDataType::String, "popup",""},
			{mvPythonDataType::Bool, "show",""},
			{mvPythonDataType::Bool, "popup_align_left","Align the popup toward the left by default"},
			{mvPythonDataType::Bool, "height_small","Max ~4 items visible"},
			{mvPythonDataType::Bool, "height_regular","Max ~8 items visible (default)"},
			{mvPythonDataType::Bool, "height_large","Max ~20 items visible"},
			{mvPythonDataType::Bool, "height_largest","As many items visible as possible"},
			{mvPythonDataType::Bool, "no_arrow_button","Display on the preview box without the square arrow button"},
			{mvPythonDataType::Bool, "no_preview","Display only a square arrow button"},
		}, "Adds a combo.", "None", "Adding Widgets") });

		parsers->insert({ "add_selectable", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Bool, "disabled", "Display grayed out text so selectable cannot be selected"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::Bool, "span_columns"},
		}, "Adds a selectable.", "None", "Adding Widgets") });

		parsers->insert({ "add_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "small", "Small button, useful for embedding in text."},
			{mvPythonDataType::Bool, "arrow", "Arrow button."},
			{mvPythonDataType::Integer, "direction", "A cardinal direction"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
			{mvPythonDataType::String, "label", ""},
			{mvPythonDataType::String, "popup", ""},
			{mvPythonDataType::Bool, "show", ""},
		}, "Adds a button.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::FloatList, "color"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
			{mvPythonDataType::String, "popup", ""},
			{mvPythonDataType::Bool, "show", ""},
			{mvPythonDataType::Bool, "no_alpha", "ignore Alpha component"},
			{mvPythonDataType::Bool, "no_border", "disable border (which is enforced by default)"},
			{mvPythonDataType::Bool, "no_drag_drop", "disable display of inline text label"},
		}, "Adds a color button.", "None", "Adding Widgets") });

		parsers->insert({ "add_indent", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "offset"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds an indent to following items. Must be closed with the unindent command.", "None", "Adding Widgets") });

		parsers->insert({ "unindent", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "offset"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Bool, "show"},
		}, "Unindents following items.", "None", "Adding Widgets") });

		parsers->insert({ "add_spacing", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Integer, "count"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds vertical spacing.", "None", "Adding Widgets") });

		parsers->insert({ "add_same_line", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Float, "xoffset"},
			{mvPythonDataType::Float, "spacing"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Bool, "show"},
		}, "Places a widget on the same line as the previous widget. Can also be used for horizontal spacing.",
		"None", "Adding Widgets") });

		parsers->insert({ "add_radio_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::StringList, "items"},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Bool, "horizontal"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
		}, "Adds a set of radio buttons. If items is empty, nothing will be shown.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_edit3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::Bool, "no_alpha", "ignore Alpha component"},
			{mvPythonDataType::Bool, "no_picker", "disable picker when clicking on colored square."},
			{mvPythonDataType::Bool, "no_options", " disable toggling options menu when right-clicking on inputs/small preview."},
			{mvPythonDataType::Bool, "no_small_preview", "disable colored square preview next to the inputs. For color pickers this only shows if the side preview is not shown."},
			{mvPythonDataType::Bool, "no_inputs", "disable inputs sliders/text widgets (e.g. to show only the small preview colored square)"},
			{mvPythonDataType::Bool, "no_tooltip", "disable tooltip when hovering the preview."},
			{mvPythonDataType::Bool, "no_label", "disable display of inline text label"},
			{mvPythonDataType::Bool, "no_drag_drop", "disable display of inline text label"},
			{mvPythonDataType::Bool, "alpha_bar", "show vertical alpha bar/gradient in picker."},
			{mvPythonDataType::Bool, "alpha_preview", "display preview as a transparent color over a checkerboard, instead of opaque."},
			{mvPythonDataType::Bool, "alpha_preview_half", "display half opaque / half checkerboard, instead of opaque."},
			{mvPythonDataType::Bool, "display_rgb", "override _display_ type among RGB/HSV/Hex."},
			{mvPythonDataType::Bool, "display_hsv", "override _display_ type among RGB/HSV/Hex."},
			{mvPythonDataType::Bool, "display_hex", "override _display_ type among RGB/HSV/Hex."},
			{mvPythonDataType::Bool, "uint8", "display values formatted as 0..255"},
			{mvPythonDataType::Bool, "floats", "display values formatted as 0.0f..1.0f floats instead of 0..255 integers."},
			{mvPythonDataType::Bool, "input_rgb", "input and output data in RGB format."},
			{mvPythonDataType::Bool, "input_hsv", "input and output data in HSV format."},

		}, "Adds an rgb color editing widget. Click and draging the color square will copy the color to be applied on any other color widget.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_edit4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::Bool, "no_alpha", "ignore Alpha component"},
			{mvPythonDataType::Bool, "no_picker", "disable picker when clicking on colored square."},
			{mvPythonDataType::Bool, "no_options", " disable toggling options menu when right-clicking on inputs/small preview."},
			{mvPythonDataType::Bool, "no_small_preview", "disable colored square preview next to the inputs. (e.g. to show only the inputs). This only displays if the side preview is not shown."},
			{mvPythonDataType::Bool, "no_inputs", "disable inputs sliders/text widgets (e.g. to show only the small preview colored square)"},
			{mvPythonDataType::Bool, "no_tooltip", "disable tooltip when hovering the preview."},
			{mvPythonDataType::Bool, "no_label", "disable display of inline text label"},
			{mvPythonDataType::Bool, "no_drag_drop", "disable display of inline text label"},
			{mvPythonDataType::Bool, "alpha_bar", "show vertical alpha bar/gradient in picker."},
			{mvPythonDataType::Bool, "alpha_preview", "display preview as a transparent color over a checkerboard, instead of opaque."},
			{mvPythonDataType::Bool, "alpha_preview_half", "display half opaque / half checkerboard, instead of opaque."},
			{mvPythonDataType::Bool, "display_rgb", "override _display_ type among RGB/HSV/Hex."},
			{mvPythonDataType::Bool, "display_hsv", "override _display_ type among RGB/HSV/Hex."},
			{mvPythonDataType::Bool, "display_hex", "override _display_ type among RGB/HSV/Hex."},
			{mvPythonDataType::Bool, "uint8", "display values formatted as 0..255"},
			{mvPythonDataType::Bool, "floats", "display values formatted as 0.0f..1.0f floats instead of 0..255 integers."},
			{mvPythonDataType::Bool, "input_rgb", "input and output data in RGB format."},
			{mvPythonDataType::Bool, "input_hsv", "input and output data in HSV format."},

		}, "Adds an rgba color editing widget. Click and draging the color square will copy the color to be applied on any other color widget.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_picker3", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::Bool, "no_alpha", "ignore Alpha component"},
			{mvPythonDataType::Bool, "no_small_preview", "disable colored square preview next to the inputs. (e.g. to show only the inputs). This only displays if the side preview is not shown."},
			{mvPythonDataType::Bool, "no_inputs", "disable inputs sliders/text widgets (e.g. to show only the small preview colored square)"},
			{mvPythonDataType::Bool, "no_tooltip", "disable tooltip when hovering the preview."},
			{mvPythonDataType::Bool, "no_label", "disable display of inline text label"},
			{mvPythonDataType::Bool, "no_side_preview", "disable bigger color preview on right side of the picker, use small colored square preview instead , unless small preview is also hidden"},
			{mvPythonDataType::Bool, "alpha_bar", "show vertical alpha bar/gradient in picker."},
			{mvPythonDataType::Bool, "alpha_preview", "display preview as a transparent color over a checkerboard, instead of opaque."},
			{mvPythonDataType::Bool, "alpha_preview_half", "display half opaque / half checkerboard, instead of opaque."},
			{mvPythonDataType::Bool, "display_rgb", "override _display_ type among RGB/HSV/Hex."},
			{mvPythonDataType::Bool, "display_hsv", "override _display_ type among RGB/HSV/Hex."},
			{mvPythonDataType::Bool, "display_hex", "override _display_ type among RGB/HSV/Hex."},
			{mvPythonDataType::Bool, "uint8", "display values formatted as 0..255"},
			{mvPythonDataType::Bool, "floats", "display values formatted as 0.0f..1.0f floats instead of 0..255 integers."},
			{mvPythonDataType::Bool, "picker_hue_bar", "bar for Hue, rectangle for Sat/Value."},
			{mvPythonDataType::Bool, "picker_hue_wheel", "wheel for Hue, triangle for Sat/Value."},
			{mvPythonDataType::Bool, "input_rgb", "input and output data in RGB format."},
			{mvPythonDataType::Bool, "input_hsv", "input and output data in HSV format."},
		}, "Adds an rgb color picking widget. Click and draging the color square will copy the color to be applied on any other color widget. Right Click allows the style of the color picker to be changed.", "None", "Adding Widgets") });

		parsers->insert({ "add_color_picker4", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::String, "popup"},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::Bool, "no_alpha", "ignore Alpha component"},
			{mvPythonDataType::Bool, "no_small_preview", "disable colored square preview next to the inputs. (e.g. to show only the inputs). This only displays if the side preview is not shown."},
			{mvPythonDataType::Bool, "no_inputs", "disable inputs sliders/text widgets (e.g. to show only the small preview colored square)"},
			{mvPythonDataType::Bool, "no_tooltip", "disable tooltip when hovering the preview."},
			{mvPythonDataType::Bool, "no_label", "disable display of inline text label"},
			{mvPythonDataType::Bool, "no_side_preview", "disable bigger color preview on right side of the picker, use small colored square preview instead , unless small preview is also hidden"},
			{mvPythonDataType::Bool, "alpha_bar", "show vertical alpha bar/gradient in picker."},
			{mvPythonDataType::Bool, "alpha_preview", "display preview as a transparent color over a checkerboard, instead of opaque."},
			{mvPythonDataType::Bool, "alpha_preview_half", "display half opaque / half checkerboard, instead of opaque."},
			{mvPythonDataType::Bool, "display_rgb", "override _display_ type among RGB/HSV/Hex."},
			{mvPythonDataType::Bool, "display_hsv", "override _display_ type among RGB/HSV/Hex."},
			{mvPythonDataType::Bool, "display_hex", "override _display_ type among RGB/HSV/Hex."},
			{mvPythonDataType::Bool, "uint8", "display values formatted as 0..255"},
			{mvPythonDataType::Bool, "floats", "display values formatted as 0.0f..1.0f floats instead of 0..255 integers."},
			{mvPythonDataType::Bool, "picker_hue_bar", "bar for Hue, rectangle for Sat/Value."},
			{mvPythonDataType::Bool, "picker_hue_wheel", "wheel for Hue, triangle for Sat/Value."},
			{mvPythonDataType::Bool, "input_rgb", "input and output data in RGB format."},
			{mvPythonDataType::Bool, "input_hsv", "input and output data in HSV format."},
		}, "Adds an rgba color picking widget. Click and draging the color square will copy the color to be applied on any other color widget. Right Click allows the style of the color picker to be changed", "None", "Adding Widgets") });

		parsers->insert({ "add_checkbox", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "default_value"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
			{mvPythonDataType::String, "label", ""},
			{mvPythonDataType::String, "popup", ""},
			{mvPythonDataType::Bool, "show", ""}
		}, "Adds a checkbox widget.", "None", "Adding Widgets") });

		parsers->insert({ "add_dummy", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Bool, "show"}
		}, "Adds a spacer or 'dummy' object.", "None", "Adding Widgets") });
	}

	PyObject* add_time_picker(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = nullptr;
		int hour24 = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_time_picker"].parse(args, kwargs, __FUNCTION__, 
			&name, &default_value, &hour24, &callback, &callback_data, &tip, &parent, &before, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvTimePicker(name, ToTime(default_value));
		item->setTip(tip);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_date_picker(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = nullptr;
		int level = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_date_picker"].parse(args, kwargs, __FUNCTION__,
			&name, &default_value, &level, &callback, &callback_data, &tip, &parent, &before, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvDatePicker(name, ToTime(default_value));
		item->setTip(tip);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_simple_plot(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* overlay = "";
		float minscale = 0.0f;
		float maxscale = 0.0f;
		int histogram = false;
		PyObject* value = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		const char* source = "";
		const char* label = "";
		const char* popup = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_simple_plot"].parse(args, kwargs, __FUNCTION__, &name, &value, &overlay,
			&minscale, &maxscale, &histogram, &tip, &parent, &before, &width, &height, &source, &label, &popup, &show))
			return ToPyBool(false);

		std::vector<float> values = ToFloatVect(value);

		mvAppItem* item = new mvSimplePlot(name, values);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_progress_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		float default_value = 0.0f;
		const char* overlay = "";
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		const char* source = "";
		int width = 0;
		int height = 0;
		const char* popup = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_progress_bar"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&overlay, &tip, &parent, &before, &source, &width, &height, &popup, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvProgressBar(name, default_value, source);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
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
		const char* source = "";
		int width = 0;
		int height = 0;
		PyObject* uv_min = PyTuple_New(2);
		PyTuple_SetItem(uv_min, 0, PyFloat_FromDouble(0));
		PyTuple_SetItem(uv_min, 1, PyFloat_FromDouble(0));
		PyObject* uv_max = PyTuple_New(2);
		PyTuple_SetItem(uv_max, 0, PyFloat_FromDouble(1));
		PyTuple_SetItem(uv_max, 1, PyFloat_FromDouble(1));
		const char* popup = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_image"].parse(args, kwargs, __FUNCTION__, &name,
			&value, &tintcolor, &bordercolor, &tip, &parent, &before, &source, &width,
			&height, &uv_min, &uv_max, &popup, &show))
			return ToPyBool(false);

		auto mtintcolor = ToColor(tintcolor);
		auto mbordercolor = ToColor(bordercolor);
		mvVec2 muv_min = ToVec2(uv_min);
		mvVec2 muv_max = ToVec2(uv_max);

		mvAppItem* item = new mvImage(name, value);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_image_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* value;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		PyObject* tintcolor = PyTuple_New(4);
		PyTuple_SetItem(tintcolor, 0, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 1, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 2, PyFloat_FromDouble(255.0));
		PyTuple_SetItem(tintcolor, 3, PyFloat_FromDouble(255.0));
		PyObject* backgroundColor = PyTuple_New(4);
		PyTuple_SetItem(backgroundColor, 0, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(backgroundColor, 1, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(backgroundColor, 2, PyFloat_FromDouble(0.0));
		PyTuple_SetItem(backgroundColor, 3, PyFloat_FromDouble(0.0));
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		int frame_padding = -1;
		PyObject* uv_min = PyTuple_New(2);
		PyTuple_SetItem(uv_min, 0, PyFloat_FromDouble(0));
		PyTuple_SetItem(uv_min, 1, PyFloat_FromDouble(0));
		PyObject* uv_max = PyTuple_New(2);
		PyTuple_SetItem(uv_max, 0, PyFloat_FromDouble(1));
		PyTuple_SetItem(uv_max, 1, PyFloat_FromDouble(1));
		const char* popup = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_image_button"].parse(args, kwargs, __FUNCTION__,
			&name, &value, &callback, &callback_data, &tintcolor, &backgroundColor, &tip, &parent,
			&before, &width, &height, &frame_padding, &uv_min, &uv_max, &popup, &show))
			return ToPyBool(false);

		auto mtintcolor = ToColor(tintcolor);
		auto mbackgroundColor = ToColor(backgroundColor);
		mvVec2 muv_min = ToVec2(uv_min);
		mvVec2 muv_max = ToVec2(uv_max);

		mvAppItem* item = new mvImageButton(name, value);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
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
		const char* popup = "";
		int show = true;


		if (!(*mvApp::GetApp()->getParsers())["add_text"].parse(args, kwargs, __FUNCTION__, &name, &wrap,
			&color, &bullet, &tip, &parent, &before, &popup, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvText(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_label_text(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;
		const char* value = "";
		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(1000));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(255));
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		const char* popup = "";
		int show = true;


		if (!(*mvApp::GetApp()->getParsers())["add_label_text"].parse(args, kwargs, __FUNCTION__, &name, &value,
			&color, &tip, &parent, &before, &source, &label, &popup, &show))
			return ToPyBool(false);


		mvAppItem* item = new mvLabelText(std::string(name), value, source);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_listbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* items;
		int default_value = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int num_items = 3;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		const char* popup = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_listbox"].parse(args, kwargs, __FUNCTION__, &name, &items,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &width,
			&num_items, &label, &popup, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvListbox(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_combo(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* default_value = "";
		PyObject* items;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		const char* popup = "";
		int show = true;
		int popup_align_left = false;
		int height_small = false;
		int height_regular = false;
		int height_large = false;
		int height_largest = false;
		int no_arrow_button = false;
		int no_preview = false;


		if (!(*mvApp::GetApp()->getParsers())["add_combo"].parse(args, kwargs, __FUNCTION__, &name, &items,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &width,
			&label, &popup, &show, &popup_align_left, &height_small, &height_regular, &height_large, 
			&height_largest, &no_arrow_button, &no_preview))
			return ToPyBool(false);

		mvAppItem* item = new mvCombo(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_selectable(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int disabled = false;
		const char* label = "";
		const char* popup = "";
		int show = true;
		int span_columns = false;

		ImGuiSelectableFlags flags = ImGuiSelectableFlags_None;

		if (!(*mvApp::GetApp()->getParsers())["add_selectable"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &disabled,
			&label, &popup, &show, &span_columns))
			return ToPyBool(false);

		mvAppItem* item = new mvSelectable(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int smallb = false;
		int arrow = false;
		int direction = 2;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* label = "";
		const char* popup = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_button"].parse(args, kwargs, __FUNCTION__, &name, &smallb,
			&arrow, &direction, &callback, &callback_data, &tip, &parent, &before, &width, &height,
			&label, &popup, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvButton(name);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_indent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("indent" + std::to_string(i));
		const char* name = sname.c_str();
		float offset = 0.0f;
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_indent"].parse(args, kwargs, __FUNCTION__, &name, &offset,
			&parent, &before, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvIndent(name, offset);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* unindent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("unindent" + std::to_string(i));
		const char* name = sname.c_str();
		float offset = 0.0f;
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["unindent"].parse(args, kwargs, __FUNCTION__, 
			&name, &offset, &parent, &before, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvUnindent(name, offset);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_spacing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("spacing" + std::to_string(i));
		const char* name = sname.c_str();
		int count = 1;
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_spacing"].parse(args, kwargs, __FUNCTION__, &name, &count,
			&parent, &before, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvSpacing(name, count);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_dummy(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("dummy" + std::to_string(i));
		const char* name = sname.c_str();
		int width;
		int height;
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_dummy"].parse(args, kwargs, __FUNCTION__, &width, &height, &name,
			&parent, &before, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvDummy(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_same_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("sameline" + std::to_string(i));
		const char* name = sname.c_str();
		float xoffset = 0.0f;
		float spacing = -1.0f;
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_same_line"].parse(args, kwargs, __FUNCTION__, &name,
			&xoffset, &spacing, &parent, &before, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvSameLine(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_radio_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* items;
		int default_value = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		int horizontal = false;
		const char* popup = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_radio_button"].parse(args, kwargs, __FUNCTION__, &name, &items,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source,&horizontal,
			&popup, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvRadioButton(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_separator(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("separator" + std::to_string(i));
		const char* name = sname.c_str();
		const char* tip = "";
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_separator"].parse(args, kwargs, __FUNCTION__, &name, &tip, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvSeparator(name);
		item->setTip(tip);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_color_edit3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(255));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		const char* popup = "";
		int show = true;
		int no_alpha = false;
		int no_picker = false;
		int no_options = false;
		int no_small_preview = false;
		int no_inputs = false;
		int no_tooltip = false;
		int no_label = false;
		int no_drag_drop = false;
		int alpha_bar = false;
		int alpha_preview = false;
		int alpha_preview_half = false;
		int display_rgb = false;
		int display_hsv = false;
		int display_hex = false;
		int unit8 = false;
		int floats = false;
		int input_rgb = false;
		int input_hsv = false;


		if (!(*mvApp::GetApp()->getParsers())["add_color_edit3"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &width, &height,
			&label, &popup, &show, &no_alpha, &no_picker, &no_options, &no_small_preview, &no_inputs, &no_tooltip, &no_label, &no_drag_drop,
			&alpha_bar, &alpha_preview, &alpha_preview_half, &display_rgb, &display_hsv, &display_hex, &unit8, &floats, &input_rgb, &input_hsv))
			return ToPyBool(false);

		auto color = ToColor(default_value);

		ImVec4 v = color.toVec4();
		mvAppItem* item = new mvColorEdit3(name, &v.x, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_color_edit4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(255));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		const char* popup = "";
		int show = true;
		int no_alpha = false;
		int no_picker = false;
		int no_options = false;
		int no_small_preview = false;
		int no_inputs = false;
		int no_tooltip = false;
		int no_label = false;
		int no_drag_drop = false;
		int alpha_bar = false;
		int alpha_preview = false;
		int alpha_preview_half = false;
		int display_rgb = false;
		int display_hsv = false;
		int display_hex = false;
		int unit8 = false;
		int floats = false;
		int input_rgb = false;
		int input_hsv = false;

		if (!(*mvApp::GetApp()->getParsers())["add_color_edit4"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&callback, &callback_data, &tip, &parent, &before, &source, &width, &height, &label, &popup, &show,
			&no_alpha, &no_picker, &no_options, &no_small_preview, &no_inputs, &no_tooltip, &no_label, &no_drag_drop,
			&alpha_bar, &alpha_preview, &alpha_preview_half, &display_rgb, &display_hsv, &display_hex, &unit8, &floats, &input_rgb, &input_hsv))
			return ToPyBool(false);

		auto color = ToColor(default_value);
		ImVec4 v = color.toVec4();
		mvAppItem* item = new mvColorEdit4(name, &v.x, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_color_picker3(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(255));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		const char* popup = "";
		int show = true;
		int no_alpha = false;
		int no_small_preview = false;
		int no_inputs = false;
		int no_tooltip = false;
		int no_label = false;
		int no_side_preview = false;
		int alpha_bar = false;
		int alpha_preview = false;
		int alpha_preview_half = false;
		int display_rgb = false;
		int display_hsv = false;
		int display_hex = false;
		int uint8 = false;
		int floats = false;
		int picker_hue_bar = false;
		int picker_hue_wheel = false;
		int input_rgb = false;
		int input_hsv = false;


		if (!(*mvApp::GetApp()->getParsers())["add_color_picker3"].parse(args, kwargs, __FUNCTION__, &name, &default_value,
			&callback, &callback_data, &tip, &parent, &before, &source, &width, &height, &label, &popup, &show, 
			&no_alpha, &no_small_preview, &no_inputs, &no_tooltip, &no_label, &no_side_preview, &alpha_bar,
			&alpha_preview, &alpha_preview_half, &display_rgb, &display_hsv, &display_hex, &uint8, &floats, &picker_hue_bar,
			&picker_hue_wheel, &input_rgb, &input_hsv))
			return ToPyBool(false);

		auto color = ToColor(default_value);
		ImVec4 v = color.toVec4();
		mvAppItem* item = new mvColorPicker3(name, &v.x, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_color_picker4(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* default_value = PyTuple_New(4);
		PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
		PyTuple_SetItem(default_value, 3, PyLong_FromLong(255));
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		const char* popup = "";
		int show = true;
		int no_alpha = false;
		int no_small_preview = false;
		int no_inputs = false;
		int no_tooltip = false;
		int no_label = false;
		int no_side_preview = false;
		int alpha_bar = false;
		int alpha_preview = false;
		int alpha_preview_half = false;
		int display_rgb = false;
		int display_hsv = false;
		int display_hex = false;
		int uint8 = false;
		int floats = false;
		int picker_hue_bar = false;
		int picker_hue_wheel = false;
		int input_rgb = false;
		int input_hsv = false;

		if (!(*mvApp::GetApp()->getParsers())["add_color_picker4"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source, &width, &height,
			&label, &popup, &show, &no_alpha, &no_small_preview, &no_inputs, &no_tooltip, &no_label, &no_side_preview, &alpha_bar, 
			&alpha_preview,	&alpha_preview_half, &display_rgb, &display_hsv, &display_hex, &uint8, &floats, &picker_hue_bar, 
			&picker_hue_wheel, &input_rgb, &input_hsv))
			return ToPyBool(false);

		auto color = ToColor(default_value);
		ImVec4 v = color.toVec4();
		mvAppItem* item = new mvColorPicker4(name, &v.x, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_color_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* color;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		const char* popup = "";
		int show = true;
		int no_alpha = false;
		int no_border = false;
		int no_drag_drop = false;

		if (!(*mvApp::GetApp()->getParsers())["add_color_button"].parse(args, kwargs, __FUNCTION__,
			&name, &color, &callback, &callback_data, &tip, &parent, &before, &width, &height,
			&popup, &show, &no_alpha, &no_border, &no_drag_drop))
			return ToPyBool(false);

		mvAppItem* item = new mvColorButton(name, ToColor(color));
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_checkbox(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_value = 0;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";
		const char* source = "";
		const char* label = "";
		const char* popup = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_checkbox"].parse(args, kwargs, __FUNCTION__, &name,
			&default_value, &callback, &callback_data, &tip, &parent, &before, &source,
			&label, &popup, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvCheckbox(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}
}
