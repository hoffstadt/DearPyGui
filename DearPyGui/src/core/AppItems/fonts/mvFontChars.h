#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFontChars, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvFontChars : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFontChars, add_font_chars)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvFontChars(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* dict) override;
		const std::vector<ImWchar>& getCharacters() const { return m_chars; }

	private:
		
		std::vector<ImWchar>  m_chars;

	};

}
