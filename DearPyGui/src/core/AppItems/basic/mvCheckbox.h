#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCheckbox, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Bool, 1);
	class mvCheckbox : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCheckbox, add_checkbox)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_SET_STATES(MV_STATE_NONE);

	public:

		explicit mvCheckbox(mvUUID uuid);

		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void draw(ImDrawList* drawlist, float x, float y) override;
		void applySpecificTemplate(mvAppItem* item) override;

	private:

		mvRef<bool> _value = CreateRef<bool>(false);
		bool        _disabled_value = false;

	};

}
