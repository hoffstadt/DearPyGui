#pragma once
#include <imgui.h>
#include <string>
#include "mvCore.h"
#include "mvApp.h"
#include "mvAppItems.h"
#include "mvWindow.h"
#include "mvPython.h"
#include "mvItemRegistry.h"

#define MV_DRAWCMD_TYPE(x) mvDrawCmdType getType() const override { return x; }

inline const std::string DrawForeground = "##FOREGROUND";
inline const std::string DrawBackground = "##BACKGROUND";

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
		virtual void          setConfigDict(PyObject* dict) = 0;
		virtual void          getConfigDict(PyObject* dict) = 0;

		std::string tag;

	};

	static mvRef<mvDrawList> GetDrawListFromTarget(const char* name)
	{
		if (name == DrawForeground)
		{
			auto viewport = mvApp::GetApp()->getViewport();
			if (viewport == nullptr)
			{
				ThrowPythonException(std::string(name) + " viewport can't be accessed until the application is running");
				return nullptr;
			}
			return mvApp::GetApp()->getViewport()->getFrontDrawList();
		}

		if (name == DrawBackground)
		{
			auto viewport = mvApp::GetApp()->getViewport();
			if (viewport == nullptr)
			{
				ThrowPythonException(std::string(name) + " viewport can't be accessed until the application is running");
				return nullptr;
			}
			return mvApp::GetApp()->getViewport()->getBackDrawList();
		}

		auto item = mvApp::GetApp()->getItemRegistry().getItem(name);

		if (item == nullptr)
			return nullptr;

		if (item->getType() == mvAppItemType::Drawing)
			return static_cast<mvDrawing*>(item.get())->getDrawList();
		if (item->getType() == mvAppItemType::Window)
			return static_cast<mvWindowAppItem*>(item.get())->getDrawList();
		if (item->getType() == mvAppItemType::Plot)
			return static_cast<mvPlot*>(item.get())->getDrawList();

		ThrowPythonException(std::string(name) + " draw target does not exist or is not a valid target.");
		return nullptr;
	}
}
