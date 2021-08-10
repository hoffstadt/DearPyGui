#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvIntValue, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int, 1);
	class mvIntValue : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvIntValue, add_int_value)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvValueRegistry)
		MV_END_PARENTS

	public:

		mvIntValue(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;

	private:

		mvRef<int> _value = CreateRef<int>(0);
		int        _disabled_value = 0;

	};

}
