#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFilterSet, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvFilterSet : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFilterSet, add_filter_set)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvFilterSet(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setPyValue(PyObject* value) override;
		PyObject* getPyValue() override;

	private:

		ImGuiTextFilter m_imguiFilter;
	};

}