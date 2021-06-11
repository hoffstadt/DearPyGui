#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPopup, MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_AFTER, StorageValueTypes::Bool, 1);
	class mvPopup : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPopup, add_popup)

		MV_CREATE_COMMAND(close_popup);

		MV_START_COMMANDS
			MV_ADD_COMMAND(close_popup);
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvPopup(mvUUID uuid);

		void closePopup();

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setLabel(const std::string& value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		bool isParentCompatible(mvAppItemType type) override;

	private:

		bool       m_modal = false;
		int        m_button = 1;
		mvAppItem* m_parentAddress = nullptr;
		bool       m_close = false;

	};

}