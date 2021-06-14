#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFontRange, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvFontRange : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFontRange, add_font_range)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvFontRange(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* dict) override;
		const std::array<ImWchar, 3>& getRange() const { return m_range; }

	private:
		
		int m_min = 0x0370;
		int m_max = 0x03ff;
		std::array<ImWchar, 3> m_range = { 0x0370, 0x03ff, 0 };

	};

}
