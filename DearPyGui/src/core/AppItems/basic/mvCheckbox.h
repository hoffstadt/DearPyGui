#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvCheckbox : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Checkbox, "add_checkbox")

		mvCheckbox(const std::string& name, bool default_value, const std::string& dataSource);

		void draw() override;

	};

}
