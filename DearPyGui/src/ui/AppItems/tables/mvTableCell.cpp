#include "mvTableCell.h"
#include "mvContext.h"
#include "mvCore.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvTableCell::mvTableCell(mvUUID uuid)
		: mvAppItem(uuid)
	{

	}

	void mvTableCell::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		for (auto& item : _children[1])
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
	}

}