#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFontRange, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvFontRange : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFontRange, add_font_range)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvFont)
		MV_END_PARENTS

	public:

		mvFontRange(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}
		void handleSpecificRequiredArgs(PyObject* dict) override;
		const std::array<ImWchar, 3>& getRange() const { return _range; }

	private:
		
		int _min = 0x0370;
		int _max = 0x03ff;
		std::array<ImWchar, 3> _range = { 0x0370, 0x03ff, 0 };

	};

}
