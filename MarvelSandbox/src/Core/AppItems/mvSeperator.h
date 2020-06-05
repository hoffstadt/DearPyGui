#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvSeparator : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Separator)

		mvSeparator(const std::string& parent)
			: mvNoneItemBase(parent, "separator")
		{
		}

		virtual void draw() override
		{
			ImGui::Separator();
		}

	};

}