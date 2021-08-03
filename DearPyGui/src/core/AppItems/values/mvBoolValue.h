#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvBoolValue, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Bool, 1);
	class mvBoolValue : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvBoolValue, add_bool_value)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvBoolValue(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;

	private:

		mvRef<bool> _value = CreateRef<bool>(false);
		bool  _disabled_value = false;
	};

}
