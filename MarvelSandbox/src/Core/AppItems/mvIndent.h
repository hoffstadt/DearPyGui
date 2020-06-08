#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvIndent : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Indent)

		mvIndent(const std::string& parent, float default_value)
			: mvFloatItemBase(parent, "indent", 1, default_value)
		{
		}

		virtual void draw() override
		{
			ImGui::Indent(m_value[0]);
		}

	};

	class mvUnindent : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputFloat)

		mvUnindent(const std::string& parent, float default_value)
			: mvFloatItemBase(parent, "indent", 1, default_value)
		{
		}

		virtual void draw() override
		{
			ImGui::Unindent(m_value[0]);
		}

	};
}