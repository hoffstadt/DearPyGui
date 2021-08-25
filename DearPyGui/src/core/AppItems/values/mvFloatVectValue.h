#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFloatVectValue, MV_ITEM_DESC_DEFAULT, StorageValueTypes::FloatVect, 1);
	class mvFloatVectValue : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFloatVectValue, add_float_vect_value)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_SET_STATES(MV_STATE_NONE);

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvValueRegistry)
		MV_END_PARENTS

	public:

		explicit mvFloatVectValue(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;

	private:

		mvRef<std::vector<float>> _value = CreateRef<std::vector<float>>(std::vector<float>{0.0f});

	};

}
