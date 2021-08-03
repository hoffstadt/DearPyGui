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

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvStringValue(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;

	private:

		mvRef<std::string> _value = CreateRef<std::string>("");
		std::string  _disabled_value = "";
	};

}
