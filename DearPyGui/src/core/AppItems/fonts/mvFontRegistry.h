#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFontRegistry, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvFontRegistry : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFontRegistry, add_font_registry)

			MV_START_EXTRA_COMMANDS
			MV_END_EXTRA_COMMANDS

			MV_START_GENERAL_CONSTANTS
			MV_END_GENERAL_CONSTANTS

			MV_START_COLOR_CONSTANTS
			MV_END_COLOR_CONSTANTS

			MV_START_STYLE_CONSTANTS
			MV_END_STYLE_CONSTANTS

	public:

		mvFontRegistry(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void customAction() override;
		bool canChildBeAdded(mvAppItemType type) override;
		void onChildAdd(mvRef<mvAppItem> item) override { m_show = true; }

		bool isInvalid() const { return m_dirty; }

	private:

		bool m_dirty = true;

	};

}
