#pragma once

namespace Marvel {

	struct mvColor
	{
		float r, g, b, a;
		bool specified = true;
	};

}

#define MV_DEFAULT_COLOR {1.0f, 1.0f, 1.0f, 1.0f, false}