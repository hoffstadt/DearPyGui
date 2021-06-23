#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTab, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvTab : public mvBoolPtrBase
	{

		enum class TabOrdering {
			mvTabOrder_Reorderable = 0L,
			mvTabOrder_Fixed,
			mvTabOrder_Leading,
			mvTabOrder_Trailing
		};

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTab, add_tab)

		MV_CREATE_CONSTANT(mvTabOrder_Reorderable, 0L);
		MV_CREATE_CONSTANT(mvTabOrder_Fixed, 1L);
		MV_CREATE_CONSTANT(mvTabOrder_Leading, 2L);
		MV_CREATE_CONSTANT(mvTabOrder_Trailing, 3L);

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvTabOrder_Reorderable),
			MV_ADD_CONSTANT(mvTabOrder_Fixed),
			MV_ADD_CONSTANT(mvTabOrder_Leading),
			MV_ADD_CONSTANT(mvTabOrder_Trailing)
		MV_END_CONSTANTS

	public:

		mvTab(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void addFlag   (ImGuiTabItemFlags flag);
		void removeFlag(ImGuiTabItemFlags flag);
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		bool              m_closable = false;
		ImGuiTabItemFlags m_flags = ImGuiTabItemFlags_None;

	};

}