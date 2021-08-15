#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFontChars, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvFontChars : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFontChars, add_font_chars)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvFont),
			MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
		MV_END_PARENTS

	public:

		mvFontChars(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}
		void handleSpecificRequiredArgs(PyObject* dict) override;
		const std::vector<ImWchar>& getCharacters() const { return _chars; }

	private:
		
		std::vector<ImWchar>  _chars;

	};

}
