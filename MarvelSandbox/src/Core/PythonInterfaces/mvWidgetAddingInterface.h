# pragma once

#include "Core/mvPythonTranslator.h"
#include <map>
#include <string>

namespace Marvel{

	static std::map<std::string, mvPythonTranslator> BuildTranslations()
	{

		std::map<std::string, mvPythonTranslator> translators = {

			{"addSimplePlot", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::FloatList, "value"},
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Bool, "autoscale"},
				{mvPythonDataType::String, "overlay"},
				{mvPythonDataType::Float, "minscale"},
				{mvPythonDataType::Float, "maxscale"},
				{mvPythonDataType::Float, "height"},
				{mvPythonDataType::Bool, "histogram"}
			}, true, "A Listbox is a listbox")},

			{"addText", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Integer, "wrap"},
				{mvPythonDataType::FloatList, "color"},
				{mvPythonDataType::Bool, "bullet"}
			}, true, "A Listbox is a listbox")},

			{"addLabelText", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::String, "value"},
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Integer, "wrap"},
				{mvPythonDataType::FloatList, "color"},
				{mvPythonDataType::Bool, "bullet"}
			}, true, "A Listbox is a listbox")},

			{"addListbox", mvPythonTranslator({
				{mvPythonDataType::String, "name", false, "Name of the listbox"},
				{mvPythonDataType::StringList, "items"},
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Integer, "default_value"},
				{mvPythonDataType::Integer, "height"},
			}, true, "A Listbox is a listbox")},

			{"addCombo", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::StringList, "items"},
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::String, "default_value"}
			}, true, "A Listbox is a listbox")},

			{"addSelectable", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Bool, "default_value"}
			}, true, "A Listbox is a listbox")},

			{"addButton", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox")},

			{"addInputText", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::String, "hint"},
				{mvPythonDataType::Bool, "multiline"}
			}, true, "A Listbox is a listbox")},

			{"addInputInt", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Integer, "default_value"}
			}, true, "A Listbox is a listbox")},

			{"addInputFloat", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Float, "default_value"}
			}, true, "A Listbox is a listbox")},

			{"indent", mvPythonTranslator({
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Float, "offset"}
			}, false, "A Listbox is a listbox")},

			{"unindent", mvPythonTranslator({
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Float, "offset"}
			}, false, "A Listbox is a listbox")},

			{"addTabBar", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox")},

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
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Integer, "count"}
			}, false, "A Listbox is a listbox")},

			{"addSameLine", mvPythonTranslator({
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Float, "xoffset"},
				{mvPythonDataType::Float, "spacing"}
			}, false, "A Listbox is a listbox")},

			{"addRadioButton", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::StringList, "items"},
				{mvPythonDataType::Optional, ""},
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
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Integer, "mousebutton"},
				{mvPythonDataType::Integer, "modal"}
			}, true, "A Listbox is a listbox") },

			{"addCollapsingHeader", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, true, "A Listbox is a listbox") },

			{"addTreeNode", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
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
				{mvPythonDataType::Optional, ""},
				{mvPythonDataType::Integer, "default_value"}
			}, true, "A Listbox is a listbox") }

		};

		return translators;

	}

}