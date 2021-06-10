#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTabButton, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvTabButton : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTabButton, add_tab_button)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvTabButton(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		bool isParentCompatible(mvAppItemType type) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiTabItemFlags m_flags = ImGuiTabItemFlags_None;

	};
}