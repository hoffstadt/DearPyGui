#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvMenuItem, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Bool, 1);
	class mvMenuItem : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMenuItem, add_menu_item)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		explicit mvMenuItem(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		std::string m_shortcut;
		bool        m_default_value = false;
		bool        m_check = false;
		std::string m_source = "";

	};

}