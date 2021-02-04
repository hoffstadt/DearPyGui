#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvIndent : public mvFloatPtrBase
	{

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::Indent, "add_indent")

		mvIndent(const std::string& name, float default_value)
			: mvFloatPtrBase(name, default_value, name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			ImGui::Indent(m_value[0]);
		}

	};

}