#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvMouseDoubleClickHandler, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvMouseDoubleClickHandler : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMouseDoubleClickHandler, add_mouse_double_click_handler)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_SET_STATES(MV_STATE_NONE);

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
			MV_ADD_PARENT(mvAppItemType::mvStage),
			MV_ADD_PARENT(mvAppItemType::mvHandlerRegistry)
		MV_END_PARENTS

	public:

		explicit mvMouseDoubleClickHandler(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void applySpecificTemplate(mvAppItem* item) override;

	private:

		int _button = -1;

	};

}
