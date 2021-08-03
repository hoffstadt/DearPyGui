#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvColorButton, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvColorButton : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorButton, add_color_button)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		explicit mvColorButton(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		mvRef<std::array<float, 4>> _value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
		float                       _disabled_value[4]{};
		ImGuiColorEditFlags         _flags = ImGuiColorEditFlags_None;
		bool                        _no_border = false;

	};

}
