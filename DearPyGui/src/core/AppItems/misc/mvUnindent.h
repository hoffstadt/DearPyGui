#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvUnindent : public mvFloatPtrBase
	{

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::Unindent, "unindent")

		mvUnindent(const std::string& name, float default_value)
			: mvFloatPtrBase(name, default_value, name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			ImGui::Unindent(m_value[0]);
		}

	};

}