#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	class mvTemplateRegistry : public mvAppItem
	{

	public:

		explicit mvTemplateRegistry(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}