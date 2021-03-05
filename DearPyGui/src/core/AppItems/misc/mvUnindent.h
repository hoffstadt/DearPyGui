#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvUnindent : public mvFloatPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Unindent, mvUnindent, "unindent")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvUnindent(const std::string& name, float default_value)
			: mvFloatPtrBase(name, default_value)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			ImGui::Unindent(*m_value);
		}

	};

}