#pragma once

#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvInputText, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvInputText : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvInputText, add_input_text)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_SET_STATES(MV_STATE_NONE);

	public:

		explicit mvInputText(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void applySpecificTemplate(mvAppItem* item) override;

	private:

		mvRef<std::string>  _value = CreateRef<std::string>("");
		std::string         _disabled_value = "";
		std::string         _hint;
		bool                _multiline = false;
		ImGuiInputTextFlags _flags = 0;
		ImGuiInputTextFlags _stor_flags = 0;

	};

}