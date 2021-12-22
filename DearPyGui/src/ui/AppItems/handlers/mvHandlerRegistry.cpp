#include "mvHandlerRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	mvHandlerRegistry::mvHandlerRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvHandlerRegistry::draw(ImDrawList* drawlist, float x, float y)
	{

		for (auto& item : childslots[1])
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

	}
}