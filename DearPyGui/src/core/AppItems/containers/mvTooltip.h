#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvTooltip : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Tooltip, "add_tooltip")

		mvTooltip(const std::string& name);

		void draw() override;

	};

}