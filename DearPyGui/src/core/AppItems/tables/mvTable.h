#pragma once

#include "mvItemRegistry.h"
#include <vector>

namespace Marvel {

	MV_REGISTER_WIDGET(mvTable, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvTable : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTable, add_table)

		MV_CREATE_CONSTANT(mvTable_SizingFixedFit, ImGuiTableFlags_SizingFixedFit);
		MV_CREATE_CONSTANT(mvTable_SizingFixedSame, ImGuiTableFlags_SizingFixedSame);
		MV_CREATE_CONSTANT(mvTable_SizingStretchProp, ImGuiTableFlags_SizingStretchProp);
		MV_CREATE_CONSTANT(mvTable_SizingStretchSame, ImGuiTableFlags_SizingStretchSame);

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvTable_SizingFixedFit),
			MV_ADD_CONSTANT(mvTable_SizingFixedSame),
			MV_ADD_CONSTANT(mvTable_SizingStretchProp),
			MV_ADD_CONSTANT(mvTable_SizingStretchSame)
		MV_END_CONSTANTS

	public:

		mvTable(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

		void onChildAdd(mvRef<mvAppItem> item) override;
		void onChildRemoved(mvRef<mvAppItem> item) override;
		void onChildrenRemoved() override;

	private:

		int  _columns = 0;
		int  _inner_width = 0;
		int  _freezeRows = 0;
		int  _freezeColumns = 0;
		ImGuiTableFlags _flags = 0;
		bool _tableHeader = true;

		struct SortSpec 
		{
			mvUUID column;
			int direction;
		};
		
	};

}
