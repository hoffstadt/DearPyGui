#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvIndent);
	class mvIndent : public mvFloatPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvIndent, add_indent)

		MV_CREATE_CONSTANT(mvThemeStyle_Indent_Spacing, 11L, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_ADD_CONSTANT(mvThemeStyle_Indent_Spacing, 21, 30),
		MV_END_STYLE_CONSTANTS

		mvIndent(const std::string& name, float default_value);

		void draw() override;

	};

}