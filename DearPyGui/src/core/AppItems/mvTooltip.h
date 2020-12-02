#pragma once

#include "core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvTooltip
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvTooltip
	//-----------------------------------------------------------------------------
	class mvTooltip : public mvPtrBase<bool, 1>
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Tooltip, "add_tooltip")

		mvTooltip(const std::string& name);

		void draw() override;

	};

}