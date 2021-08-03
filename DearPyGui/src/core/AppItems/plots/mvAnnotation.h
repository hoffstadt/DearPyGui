#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvAnnotation, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 0);
	class mvAnnotation : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvAnnotation, add_plot_annotation)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvAnnotation(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		bool isParentCompatible(mvAppItemType type) override;
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;

	private:

		mvRef<std::array<double, 4>> _value = CreateRef<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
		double                       _disabled_value[4]{};
		mvColor                      _color = mvColor(0.0f, 0.0f, 0.0f, -1.0f);
		bool                         _clamped = true;
		ImVec2                       _pixOffset;

	};

}
