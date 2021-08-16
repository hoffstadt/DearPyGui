#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCollapsingHeader, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvCollapsingHeader : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCollapsingHeader, add_collapsing_header)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

	public:

		mvCollapsingHeader(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void applySpecificTemplate(mvAppItem* item) override;

	private:
		mvRef<bool>        _value = CreateRef<bool>(false);
		bool               _disabled_value = false;
		ImGuiTreeNodeFlags _flags = ImGuiTreeNodeFlags_None;
		bool               _closable = false;

	};

}