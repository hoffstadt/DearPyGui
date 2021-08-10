#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDragPayload, MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_HANDLER, StorageValueTypes::None, 4);
	class mvDragPayload : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragPayload, add_drag_payload)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

	public:

		mvDragPayload(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		PyObject* getDragData() const { return _dragData; }

	private:

		std::string _payloadType = "$$DPG_PAYLOAD";
		PyObject*   _dragData = nullptr;

	};

}