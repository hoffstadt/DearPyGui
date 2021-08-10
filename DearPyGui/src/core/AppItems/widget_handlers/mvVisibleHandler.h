#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvVisibleHandler, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_HANDLER, StorageValueTypes::None, 3);
	class mvVisibleHandler : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvVisibleHandler, add_visible_handler)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

	public:

		mvVisibleHandler(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void handleSpecificRequiredArgs(PyObject* dict) override;

	};

}
