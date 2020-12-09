#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvMenuBar : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuBar, "add_menu_bar")

		explicit mvMenuBar(const std::string& name);

		void draw() override;

	};

}