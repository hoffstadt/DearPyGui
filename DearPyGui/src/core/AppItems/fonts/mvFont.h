#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFont, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvFont : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFont, add_font)
		MV_NO_COMMANDS
		MV_NO_CONSTANTS

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvFontRegistry)
		MV_END_PARENTS

		MV_START_CHILDREN
			MV_ADD_CHILD(mvAppItemType::mvFontChars),
			MV_ADD_CHILD(mvAppItemType::mvFontRange),
			MV_ADD_CHILD(mvAppItemType::mvCharRemap),
			MV_ADD_CHILD(mvAppItemType::mvFontRangeHint)
		MV_END_CHILDREN

	public:

		mvFont(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void customAction() override;
		void handleSpecificRequiredArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		ImFont* getFontPtr() { return _fontPtr; }

	private:

		// config
		std::string _file;
		float       _size = 13.0f;
		bool        _default = false;

		// finalized
		ImFont* _fontPtr = nullptr;
		ImVector<ImWchar> _ranges;

	};

}
