#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvColorMapSlider, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float, 1);
	class mvColorMapSlider : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorMapSlider, add_colormap_slider)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

	public:

		explicit mvColorMapSlider(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setColorMap(ImPlotColormap colormap) { _colormap = colormap; }
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;

	private:

		mvRef<float>    _value = CreateRef<float>(0.0f);
		ImVec4          _color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		ImPlotColormap  _colormap = 0;

	};

}
