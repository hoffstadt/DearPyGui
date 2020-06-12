# pragma once

#include "Core/mvPythonTranslator.h"
#include <map>
#include <string>

namespace Marvel{

	static std::map<std::string, mvPythonTranslator> BuildTranslations()
	{
		std::map<std::string, mvPythonTranslator> translators = {

			{"isItemHovered", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"isItemActive", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"isItemFocused", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"isItemClicked", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"isItemVisible", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"isItemEdited", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"isItemActivated", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"isItemDeactivated", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"isItemDeactivatedAfterEdit", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"isItemToggledOpen", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"getItemRectMin", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"getItemRectMax", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"getItemRectSize", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
			}, false, "Needs documentation")},

			{"changeStyleItem", mvPythonTranslator({
				{mvPythonDataType::Integer, "item"},
				{mvPythonDataType::Float, "x"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::Float, "y"}
			}, false, "Needs documentation")},

			{"changeThemeItem", mvPythonTranslator({
				{mvPythonDataType::Integer, "item"},
				{mvPythonDataType::Float, "r"},
				{mvPythonDataType::Float, "g"},
				{mvPythonDataType::Float, "b"},
				{mvPythonDataType::Float, "a"}
			}, false, "Needs documentation")},

			{"getValue", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, false, "Needs documentation")},

			{"setValue", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Object, "value"}
			}, false, "Needs documentation")},

			{"showItem", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, false, "Needs documentation")},

			{"hideItem", mvPythonTranslator({
				{mvPythonDataType::String, "name"}
			}, false, "Needs documentation")},

			{"setMainCallback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Needs documentation")},

			{"setMouseDownCallback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Needs documentation")},

			{"setMouseDoubleClickCallback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Needs documentation")},

			{"setMouseClickCallback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Needs documentation")},

			{ "setKeyDownCallback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Needs documentation") },

			{ "setKeyPressCallback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Needs documentation") },

			{ "setKeyReleaseCallback", mvPythonTranslator({
				{mvPythonDataType::String, "callback"}
			}, false, "Needs documentation") },

			{ "setItemCallback", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
				{mvPythonDataType::String, "callback"}
			}, false, "Needs documentation") },

			{ "setItemPopup", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
				{mvPythonDataType::String, "popup"}
			}, false, "Needs documentation") },

			{ "setItemTip", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
				{mvPythonDataType::String, "tip"}
			}, false, "Needs documentation") },

			{ "setItemWidth", mvPythonTranslator({
				{mvPythonDataType::String, "item"},
				{mvPythonDataType::Integer, "width"}
			}, false, "Needs documentation") },

			{ "setTheme", mvPythonTranslator({
				{mvPythonDataType::String, "theme"}
			}, false, "Needs documentation") },

		};

		return translators;
	}

}