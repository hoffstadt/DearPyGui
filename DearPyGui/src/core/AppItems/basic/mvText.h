#pragma once

#include "mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvText
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvText
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvText, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvText : public mvStringPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvText, add_text)

		MV_CREATE_CONSTANT(mvThemeCol_Text_Text, ImGuiCol_Text, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Text_Text, mvImGuiCol_Text)
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvText(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		mvColor m_color = {-1.0f, 0.0f, 0.0f, 1.0f};
		int     m_wrap = -1;
		bool    m_bullet = false;
		bool	m_show_label = false;

	};
}