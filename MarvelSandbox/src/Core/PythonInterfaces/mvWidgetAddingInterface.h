# pragma once

#include "Core/PythonUtilities/mvPythonTranslator.h"
#include <map>
#include <string>

namespace Marvel{

	static std::map<std::string, mvPythonTranslator> BuildTranslations()
	{

		std::map<std::string, mvPythonTranslator> translators = {

			{"addSimplePlot", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::FloatList, "value", "Tuple of float values"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Bool, "autoscale", "autoscales range based on data (default is True)"},
				{mvPythonDataType::String, "overlay", "overlays text (similar to a plot title)"},
				{mvPythonDataType::Float, "minscale", "used if autoscale is false"},
				{mvPythonDataType::Float, "maxscale", "used if autoscale is false"},
				{mvPythonDataType::Bool, "histogram", "create a histogram"}
			}, true, "A simple plot for visualization of a set of values")},

			{"addSliderFloat", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Float, "default_value"},
				{mvPythonDataType::Float, "min_value"},
				{mvPythonDataType::Float, "max_value"},
				{mvPythonDataType::String, "format"},
				{mvPythonDataType::Float, "power"},
				{mvPythonDataType::Bool, "vertical"}
			}, true, "A slider for a single float value")},

			{"addSliderFloat4", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "default_value"},
				{mvPythonDataType::Float, "min_value"},
				{mvPythonDataType::Float, "max_value"},
				{mvPythonDataType::String, "format"},
				{mvPythonDataType::Float, "power"}
			}, true, "A slider for a 4 float values.")},

			{"addSliderInt", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Integer, "default_value"},
				{mvPythonDataType::Integer, "min_value"},
				{mvPythonDataType::Integer, "max_value"},
				{mvPythonDataType::String, "format"},
				{mvPythonDataType::Bool, "vertical"}
			}, true, "A slider for a single int value")},

			{"addSliderInt4", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::IntList, "default_value"},
				{mvPythonDataType::Integer, "min_value"},
				{mvPythonDataType::Integer, "max_value"},
				{mvPythonDataType::String, "format"}
			}, true, "A slider for a 4 int values.")},

			{"addText", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "wrap", "number of characters until wraping"},
				{mvPythonDataType::FloatList, "color", "color of the text (rgba)"},
				{mvPythonDataType::Bool, "bullet", "makes the text bulleted"}
			}, true, "Adds text")},

			{"addLabelText", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::String, "value"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::FloatList, "color"},
			}, true, "A text with a label. Useful for output values.")},

			{"addListbox", mvPythonTranslator({
				{mvPythonDataType::String, "name", "Name of the listbox"},
				{mvPythonDataType::StringList, "items"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "default_value"},
			}, true, "A Listbox is a listbox")},

			{"addCombo", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::StringList, "items"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::String, "default_value"}
			}, true, "A Listbox is a listbox")},

			{"addSelectable", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Bool, "default_value"}
			}, true, "A Listbox is a listbox")},

			{"addButton", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Bool, "small", "Small button, useful for embedding in text."},
				{mvPythonDataType::Bool, "arrow", "Arrow button."},
				{mvPythonDataType::Integer, "direction", "A cardinal direction"},
			}, true, "A button.")},

			{"addInputText", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::String, "hint"},
				{mvPythonDataType::Bool, "multiline"},
				{mvPythonDataType::Integer, "flags"}
			}, true, "A input for text values.")},

			{"addInputInt", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "default_value"}
			}, true, "A Listbox is a listbox")},

			{"addInputFloat", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "default_value"}
			}, true, "A Listbox is a listbox")},

			{"indent", mvPythonTranslator({
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "offset"}
			}, false, "A Listbox is a listbox")},

			{"unindent", mvPythonTranslator({
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "offset"}
			}, false, "A Listbox is a listbox")},

			{"addTabBar", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Integer, "flags"}
			}, true, "A tab bar.")},

			{"addTab", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox")},

			{"addMenuBar", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox")},

			{"addMenu", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox")},

			{"addMenuItem", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox")},

			{"addSpacing", mvPythonTranslator({
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "count"}
			}, false, "A Listbox is a listbox")},

			{"addSameLine", mvPythonTranslator({
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "xoffset"},
				{mvPythonDataType::Float, "spacing"}
			}, false, "A Listbox is a listbox")},

			{"addRadioButton", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::StringList, "items"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "default_value"}
			}, true, "A Listbox is a listbox") },

			{"addGroup", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox") },

			{"addChild", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Integer, "width"},
				{mvPythonDataType::Integer, "height"}
			}, false, "A Listbox is a listbox") },

			{"addWindow", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Integer, "width"},
				{mvPythonDataType::Integer, "height"}
			}, false, "A Listbox is a listbox") },

			{"addTooltip", mvPythonTranslator({
				{mvPythonDataType::String, "parent"},
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox") },

			{"addPopup", mvPythonTranslator({
				{mvPythonDataType::String, "parent"},
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "mousebutton"},
				{mvPythonDataType::Integer, "modal"}
			}, true, "A Listbox is a listbox") },

			{"addCollapsingHeader", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Integer, "flags"}
			}, true, "A Listbox is a listbox") },

			{"addTreeNode", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Integer, "flags"}
			}, true, "A Listbox is a listbox") },

			{"addColorEdit4", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Float, "r"},
				{mvPythonDataType::Float, "g"},
				{mvPythonDataType::Float, "b"},
				{mvPythonDataType::Float, "a"}
			}, true, "A Listbox is a listbox") },

			{"addCheckbox", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Integer, "default_value"}
			}, true, "A Listbox is a listbox") },

			{"addDragFloat", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "default_value"}
			}, true, "This is useful for easy selection of a float withing a set range") }

		};

		return translators;

	}

}