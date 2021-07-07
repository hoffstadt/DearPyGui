#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCheckbox, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Bool, 1);
	class mvCheckbox : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCheckbox, add_checkbox)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvCheckbox(mvUUID uuid);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void draw(ImDrawList* drawlist, float x, float y) override;

	private:

		bool m_default_value = false;
		std::string m_source = "";

	};

}
