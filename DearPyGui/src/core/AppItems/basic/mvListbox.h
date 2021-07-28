#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvListbox, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvListbox : public mvStringPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvListbox, add_listbox)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvListbox(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setPyValue(PyObject* value) override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		void updateIndex();

	private:

		std::vector<std::string> _names;
		int                      _itemsHeight = 3; // number of items to show (default -1)
		std::vector<const char*> _charNames;
		int                      _index = 0;
		int                      _disabledindex = 0;

	};

}
