#pragma once

#include <imgui.h>

namespace Marvel {

	struct mvVec2
	{
		float x, y;

		operator ImVec2()
		{
			return ImVec2{ x, y };
		}

		mvVec2 operator+(const ImVec2& other)
		{
			return mvVec2{ x + other.x, y + other.y };
		}
	};

	struct mvColor
	{
		float r, g, b, a;
		bool specified = true;

		operator ImU32()
		{
			return ImGui::ColorConvertFloat4ToU32({ r, g, b, a });
		}

	};

}

#define MV_DEFAULT_COLOR Marvel::mvColor{1.0f, 1.0f, 1.0f, 1.0f, false}
#define MV_TRANSPARENT Marvel::mvColor{1.0f, 1.0f, 1.0f, 0.0f, true}