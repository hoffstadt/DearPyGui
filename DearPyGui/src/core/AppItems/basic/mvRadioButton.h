#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvRadioButton, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvRadioButton : public mvStringPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvRadioButton, add_radio_button)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvRadioButton(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setPyValue(PyObject* value) override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		void updateIndex();

	private:

		std::vector<std::string> m_itemnames;
		bool                     m_horizontal = false;
		int                      m_index = 0;
		int                      m_disabledindex = 0;
	};

}
