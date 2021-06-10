#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"

namespace Marvel {

	MV_REGISTER_WIDGET(mvThemeColor, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Color, 1);
	class mvThemeColor : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvThemeColor, add_theme_color)

			MV_START_EXTRA_COMMANDS
			MV_END_EXTRA_COMMANDS

			MV_START_GENERAL_CONSTANTS
			MV_END_GENERAL_CONSTANTS

			MV_START_COLOR_CONSTANTS
			MV_END_COLOR_CONSTANTS

			MV_START_STYLE_CONSTANTS
			MV_END_STYLE_CONSTANTS

	public:

		mvThemeColor(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void customAction() override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		bool isParentCompatible(mvAppItemType type) override;

		void setLibType(mvLibType libType) { m_libType = libType; }

	private:

		ImGuiCol m_targetColor = 0;
		mvColor m_color;
		mvLibType m_libType = mvLibType::MV_IMGUI;
		

	};

}
