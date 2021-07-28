#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvColorPicker, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Color, 1);
	class mvColorPicker : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorPicker, add_color_picker)

		MV_CREATE_CONSTANT(mvColorPicker_bar, ImGuiColorEditFlags_PickerHueBar);
		MV_CREATE_CONSTANT(mvColorPicker_wheel, ImGuiColorEditFlags_PickerHueWheel);

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvColorPicker_bar),
			MV_ADD_CONSTANT(mvColorPicker_wheel)
		MV_END_CONSTANTS

	public:

		explicit mvColorPicker(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiColorEditFlags _flags = ImGuiColorEditFlags__OptionsDefault;

		bool                 _no_inputs = false;
		bool                 _no_label = false;
		bool                 _no_side_preview = false;
		bool                 _alpha_bar = false;

	};

}
