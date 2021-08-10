#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvRadioButton, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvRadioButton : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvRadioButton, add_radio_button)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

	public:

		mvRadioButton(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setPyValue(PyObject* value) override;
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		void updateIndex();

	private:

		mvRef<std::string>       _value = CreateRef<std::string>("");
		std::string              _disabled_value = "";
		std::vector<std::string> _itemnames;
		bool                     _horizontal = false;
		int                      _index = 0;
		int                      _disabledindex = 0;
	};

}
