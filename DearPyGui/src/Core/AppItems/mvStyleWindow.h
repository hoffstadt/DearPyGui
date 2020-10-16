#pragma once
#include "Core/AppItems/mvTypeBases.h"
#include "mvAppItem.h"

namespace Marvel {

	class mvStyleWindow : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE(mvAppItemType::StyleWindow, "add_style_window")

	public:

		mvStyleWindow(const std::string& name) 
			: mvBaseWindowAppitem(name) 
		{
		}

		bool canBeDeleted() const override { return false; }

		void draw() override;

	};

}