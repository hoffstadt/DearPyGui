#pragma once
#include <imgui.h>
#include <string>
#include "mvCore.h"

#define MV_DRAWCMD_TYPE(x) mvDrawCmdType getType() const override { return x; }

namespace Marvel {

	class mvDrawList;

	//-----------------------------------------------------------------------------
	// Enums
	//-----------------------------------------------------------------------------
	enum class mvDrawCmdType
	{
		DrawLine, DrawTriangle, DrawCircle, DrawText, DrawRect, DrawQuad,
		DrawPolyline, DrawBezierCurve, DrawPolygon, DrawImage, DrawArrow
	};

	//-----------------------------------------------------------------------------
	// mvDrawingCommand
	//-----------------------------------------------------------------------------
	class mvDrawCmd
	{

	public:

		virtual ~mvDrawCmd() = default;
		virtual void          draw(ImDrawList* draw_list, float x, float y) = 0;
		virtual mvDrawCmdType getType() const = 0;

		std::string tag;

	};
}