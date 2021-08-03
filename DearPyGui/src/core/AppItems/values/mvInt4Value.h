#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvInt4Value, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int4, 1);
	class mvInt4Value : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvInt4Value, add_int4_value)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvInt4Value(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;

	private:

		mvRef<std::array<int, 4>> _value = CreateRef<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
		int  _disabled_value[4]{};
	};

}
