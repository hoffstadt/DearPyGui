#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDragLine, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Double, 0);
	class mvDragLine : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragLine, add_drag_line)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvDragLine(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		bool isParentCompatible(mvAppItemType type) override;

	private:

		mvRef<double> _value = CreateRef<double>(0.0);
		float         _disabled_value = 0.0;
		bool          _show_label = true;
		mvColor       _color = mvColor(0.0f, 0.0f, 0.0f, -1.0f);
		float         _thickness = 1.0f;
		bool          _vertical = true;

	};

}
