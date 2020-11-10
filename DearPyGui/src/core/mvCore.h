#pragma once

//-----------------------------------------------------------------------------
// mvCore
//
//     - This file contains commonly used items throughout the core library
//     
//-----------------------------------------------------------------------------

#include <imgui.h>
#include <utility>

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
			if (x < 0 || y < 0 || z < 0 || w < 0)
				return ImVec4(0, 0, 0, -1);
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
		int r=0, g=0, b=0, a=255;
		bool specified = true;

		mvColor(int r, int g, int b, int a, bool specified = true)
			: r(r), g(g), b(b), a(a), specified(specified)
		{
		}

		explicit mvColor(ImVec4 color)
		{
			r = (int)(color.x*255.0f);
			g = (int)(color.y*255.0f);
			b = (int)(color.z*255.0f);
			a = (int)(color.w*255.0f);
		}

		operator ImU32()
		{
			return ImGui::ColorConvertFloat4ToU32(toVec4());
		}

		ImVec4 toVec4() const
		{
			if (r < 0 || g < 0 || b < 0 || a < 0)
				return ImVec4(0, 0, 0, -1);
			return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		}

	};

}

#define MV_DEFAULT_COLOR Marvel::mvColor{255, 255, 255, 255, false}
#define MV_TRANSPARENT   Marvel::mvColor{255, 255, 255,   0, true }