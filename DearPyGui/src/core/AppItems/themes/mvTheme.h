#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTheme, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvTheme : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTheme, add_theme)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvTheme(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void customAction() override;
		bool canChildBeAdded(mvAppItemType type) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;

	private:

		bool m_defaultTheme = false;
	};

}
