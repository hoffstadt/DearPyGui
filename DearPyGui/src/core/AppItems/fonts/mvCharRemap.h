#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCharRemap, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvCharRemap : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCharRemap, add_char_remap)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvCharRemap(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* dict) override;
		int getSourceChar() const { return m_source; }
		int getTargetChar() const { return m_target; }

	private:
		
		int m_source = 0;
		int m_target = 0;

	};

}
