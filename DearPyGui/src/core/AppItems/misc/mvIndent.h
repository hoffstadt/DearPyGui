#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvIndent : public mvFloatPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Indent, mvIndent, "add_indent")
		MV_CREATE_THEME_CONSTANT(mvAppItemType::SimplePlot, mvThemeStyle_Indent_Spacing, 11L, 0L);

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Indent_Spacing, 0, 30),
		MV_END_STYLE_CONSTANTS

		mvIndent(const std::string& name, float default_value)
			: mvFloatPtrBase(name, default_value)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			ImGui::Indent(*m_value);
		}

	};

}