#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvSameLine, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvSameLine : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSameLine, add_same_line)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvSameLine(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		float m_xoffset = 0.0f;
		float m_spacing = -1.0f;

	};

}