#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvSeparator : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Separator)
		MV_NORETURN_VALUE()

		mvSeparator(const std::string& parent)
			: mvAppItem(parent, "separator")
		{
		}

		virtual void draw() override
		{
			ImGui::Separator();
		}

	};

}