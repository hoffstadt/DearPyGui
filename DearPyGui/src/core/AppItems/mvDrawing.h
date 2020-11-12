#pragma once

#include "AppItems/mvTypeBases.h"
#include "Registries/mvDrawList.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDrawing
//
//-----------------------------------------------------------------------------


namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvDrawing
	//-----------------------------------------------------------------------------
	class mvDrawing : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Drawing, "add_drawing")

		mvDrawing(const std::string& name);

		void draw() override;

		mvDrawList& getDrawList();


	private:

		mvDrawList  m_drawList;
		float       m_startx = 0.0f;
		float       m_starty = 0.0f;

	};
}