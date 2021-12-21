#include "mvSeparator.h"
#include "mvItemRegistry.h"

namespace Marvel {

	mvSeparator::mvSeparator(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvSeparator::draw(ImDrawList* drawlist, float x, float y)
	{

		ImGui::Separator();

	}

}