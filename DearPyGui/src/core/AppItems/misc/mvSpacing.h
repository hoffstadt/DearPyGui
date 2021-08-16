#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvSpacing, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int, 1);
	class mvSpacing : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSpacing, add_spacing)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

	public:

		mvSpacing(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void applySpecificTemplate(mvAppItem* item) override;

	private:

		mvRef<int> _value = CreateRef<int>(0);
		int        _disabled_value = 0;

	};

}