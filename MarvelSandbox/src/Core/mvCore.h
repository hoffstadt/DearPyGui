#pragma once

#include <imgui.h>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvVec2
	//-----------------------------------------------------------------------------
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

	//-----------------------------------------------------------------------------
	// mvVec4
	//-----------------------------------------------------------------------------
	struct mvVec4
	{
		float x, y, z, w;

		operator ImVec4()
		{
			return ImVec4{ x, y , z, w};
		}

		mvVec4 operator+(const ImVec4& other)
		{
			return mvVec4{ x + other.x, y + other.y, z + other.z, w + other.w };
		}
	};

	//-----------------------------------------------------------------------------
	// mvColor
	//-----------------------------------------------------------------------------
	struct mvColor
	{
		int r, g, b, a;
		bool specified = true;

		operator ImU32()
		{
			return ImGui::ColorConvertFloat4ToU32({ r/255.0f, g/255.0f, b/255.0f, a/255.0f });
		}


	};

}

#define MV_DEFAULT_COLOR Marvel::mvColor{255, 255, 255, 255, false}
#define MV_TRANSPARENT Marvel::mvColor{255, 255, 255, 0, true}

//#include "mvlogging.h"
//
//#define MV_ENABLE_ASSERTS
//
//#ifdef MV_ENABLE_ASSERTS
//#define MV_ASSERT(x, ...) { if(!(x)) { MV_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
//#else
//#define MV_ASSERT(x, ...)
//#endif