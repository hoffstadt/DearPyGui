#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFontRegistry, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvFontRegistry : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFontRegistry, add_font_registry)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_NO_CONSTANTS

		MV_SET_STATES(MV_STATE_NONE);

		MV_START_CHILDREN
			MV_ADD_CHILD(mvAppItemType::mvFont)
		MV_END_CHILDREN

	public:

		mvFontRegistry(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void customAction(void* data = nullptr) override;
		void onChildAdd(mvRef<mvAppItem> item) override { _show = true; }
		bool isInvalid() const { return _dirty; }
		void resetFont();

	private:

		bool _dirty = true;

	};

}
