#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFocusHandler, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_HANDLER, StorageValueTypes::None, 3);
	class mvFocusHandler : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFocusHandler, add_focus_handler)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

	public:

		mvFocusHandler(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void handleSpecificRequiredArgs(PyObject* dict) override;

	};

}
