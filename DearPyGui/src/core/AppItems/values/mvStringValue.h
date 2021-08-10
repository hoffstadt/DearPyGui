#pragma once

#include <string>
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvStringValue, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvStringValue : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvStringValue, add_string_value)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvValueRegistry)
		MV_END_PARENTS

	public:

		mvStringValue(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;

	private:

		mvRef<std::string> _value = CreateRef<std::string>("");
		std::string  _disabled_value = "";
	};

}
