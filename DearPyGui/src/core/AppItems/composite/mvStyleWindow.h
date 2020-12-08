#pragma once
#include "core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvStyleWindow : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE(mvAppItemType::StyleWindow, "add_style_window")

	public:

		mvStyleWindow(const std::string& name) 
			: mvBaseWindowAppitem(name) 
		{
			m_description.deleteAllowed = false;
		}

		void draw() override;

	};

}