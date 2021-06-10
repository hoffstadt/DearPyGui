#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPopup, MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_AFTER, StorageValueTypes::Bool, 1);
	class mvPopup : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPopup, add_popup)

		MV_CREATE_EXTRA_COMMAND(close_popup);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(close_popup);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvPopup(mvUUID uuid);

		void closePopup();

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setLabel(const std::string& value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		bool       m_modal = false;
		int        m_button = 1;
		mvAppItem* m_parentAddress = nullptr;
		bool       m_close = false;

	};

}