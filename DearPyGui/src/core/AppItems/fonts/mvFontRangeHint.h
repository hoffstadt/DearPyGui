#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFontRangeHint, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 0);
	class mvFontRangeHint : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFontRangeHint, add_font_range_hint)
		MV_CREATE_CONSTANT(mvFontRangeHint_Default, 0);
		MV_CREATE_CONSTANT(mvFontRangeHint_Japanese, 1);
		MV_CREATE_CONSTANT(mvFontRangeHint_Korean, 2);
		MV_CREATE_CONSTANT(mvFontRangeHint_Chinese_Full, 3);
		MV_CREATE_CONSTANT(mvFontRangeHint_Chinese_Simplified_Common, 4);
		MV_CREATE_CONSTANT(mvFontRangeHint_Cyrillic, 5);
		MV_CREATE_CONSTANT(mvFontRangeHint_Thai, 6);
		MV_CREATE_CONSTANT(mvFontRangeHint_Vietnamese, 7);

			MV_START_COMMANDS
			MV_END_COMMANDS

			MV_START_CONSTANTS
				MV_ADD_CONSTANT(mvFontRangeHint_Default),
				MV_ADD_CONSTANT(mvFontRangeHint_Japanese),
				MV_ADD_CONSTANT(mvFontRangeHint_Korean),
				MV_ADD_CONSTANT(mvFontRangeHint_Chinese_Full),
				MV_ADD_CONSTANT(mvFontRangeHint_Chinese_Simplified_Common),
				MV_ADD_CONSTANT(mvFontRangeHint_Cyrillic),
				MV_ADD_CONSTANT(mvFontRangeHint_Thai),
				MV_ADD_CONSTANT(mvFontRangeHint_Vietnamese),
			MV_END_CONSTANTS

	public:

		mvFontRangeHint(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* dict) override;
		int getHint() const { return m_hint; }

	private:
		int m_hint = 0;

	};

}
