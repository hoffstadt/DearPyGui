#pragma once

#include <vector>
#include <string>

namespace Marvel {

	std::vector<std::pair<std::string, long>> ModuleConstants =
	{
		// Plot flags
		{"mvPlotFlags_MousePos"   , 1 << 0},  // the mouse position, in plot coordinates, will be displayed in the bottom-right
		{"mvPlotFlags_Legend"     , 1 << 1},  // a legend will be displayed in the top-left
		{"mvPlotFlags_Highlight"  , 1 << 2},  // plot items will be highlighted when their legend entry is hovered
		{"mvPlotFlags_BoxSelect"  , 1 << 3},  // the user will be able to box-select with right-mouse
		{"mvPlotFlags_Query"      , 1 << 4},  // the user will be able to draw query rects with middle-mouse
		{"mvPlotFlags_ContextMenu", 1 << 5},  // the user will be able to open a context menu with double-right click
		{"mvPlotFlags_Crosshairs" , 1 << 6},  // the default mouse cursor will be replaced with a crosshair when hovered
		{"mvPlotFlags_CullData"   , 1 << 7},  // plot data outside the plot area will be culled from rendering
		{"mvPlotFlags_AntiAliased", 1 << 8},  // lines and fills will be anti-aliased (not recommended)
		{"mvPlotFlags_NoChild"    , 1 << 9},  // a child window region will not be used to capture mouse scroll (can boost performance for single ImGui window applications)
		{"mvPlotFlags_YAxis2"     , 1 << 10}, // enable a 2nd y axis
		{"mvPlotFlags_YAxis3"     , 1 << 11}, // enable a 3rd y axis
		{"mvPlotFlags_Default"    ,     175},

		// Axis flags
		{"mvPlotAxisFlags_GridLines" , 1 << 0}, // grid lines will be displayed
		{"mvPlotAxisFlags_TickMarks" , 1 << 1}, // tick marks will be displayed for each grid line
		{"mvPlotAxisFlags_TickLabels", 1 << 2}, // text labels will be displayed for each grid line
		{"mvPlotAxisFlags_Invert"    , 1 << 3}, // the axis will be inverted
		{"mvPlotAxisFlags_LockMin"   , 1 << 4}, // the axis minimum value will be locked when panning/zooming
		{"mvPlotAxisFlags_LockMax"   , 1 << 5}, // the axis maximum value will be locked when panning/zooming
		{"mvPlotAxisFlags_Adaptive"  , 1 << 6}, // grid divisions will adapt to the current pixel size the axis
		{"mvPlotAxisFlags_LogScale"  , 1 << 7}, // a logartithmic (base 10) axis scale will be used
		{"mvPlotAxisFlags_Scientific", 1 << 8}, // scientific notation will be used for tick labels if displayed (WIP, not very good yet)
		{"mvPlotAxisFlags_Default"   ,     71},
		{"mvPlotAxisFlags_Auxiliary" ,     70},

		// Plot styling colors
		{"mvPlotCol_Line"         , 0}, // plot line/outline color (defaults to a rotating color set)
		{"mvPlotCol_Fill"         , 1}, // plot fill color for bars (defaults to the current line color)
		{"mvPlotCol_MarkerOutline", 2}, // marker outline color (defaults to the current line color)
		{"mvPlotCol_MarkerFill"   , 3}, // marker fill color (defaults to the current line color)
		{"mvPlotCol_ErrorBar"     , 4}, // error bar color (defaults to ImGuiCol_Text)
		{"mvPlotCol_FrameBg"      , 5}, // plot frame background color (defaults to ImGuiCol_FrameBg)
		{"mvPlotCol_PlotBg"       , 6}, // plot area background color (defaults to ImGuiCol_WindowBg)
		{"mvPlotCol_PlotBorder"   , 7}, // plot area border color (defaults to ImGuiCol_Text)
		{"mvPlotCol_XAxis"        , 8}, // x-axis grid/label color (defaults to 25% ImGuiCol_Text)
		{"mvPlotCol_YAxis"        , 9}, // y-axis grid/label color (defaults to 25% ImGuiCol_Text)
		{"mvPlotCol_YAxis2"       ,10}, // 2nd y-axis grid/label color (defaults to 25% ImGuiCol_Text)
		{"mvPlotCol_YAxis3"       ,11}, // 3rd y-axis grid/label color (defaults to 25% ImGuiCol_Text)
		{"mvPlotCol_Selection"    ,12}, // box-selection color (defaults to yellow)
		{"mvPlotCol_Query"        ,13}, // box-query color (defaults to green)
		{"mvPlotCol_COUNT"        ,14},

		// Plot styling variables
		{"mvPlotStyleVar_LineWeight"      , 0}, // float, line weight in pixels
		{"mvPlotStyleVar_Marker"          , 1}, // int,   marker specification
		{"mvPlotStyleVar_MarkerSize"      , 2}, // float, marker size in pixels (roughly the marker's "radius")
		{"mvPlotStyleVar_MarkerWeight"    , 3}, // float, outline weight of markers in pixels
		{"mvPlotStyleVar_ErrorBarSize"    , 4}, // float, error bar whisker width in pixels
		{"mvPlotStyleVar_ErrorBarWeight"  , 5}, // float, error bar whisker weight in pixels
		{"mvPlotStyleVar_DigitalBitHeight", 6}, // float, digital channels bit height (at 1) in pixels
		{"mvPlotStyleVar_DigitalBitGap"   , 7}, // float, digital channels bit padding gap in pixels
		{"mvPlotStyleVar_COUNT"           , 8},

		// Marker specifications
		{"mvPlotMarker_None"    , 1 << 0},  // no marker
		{"mvPlotMarker_Circle"  , 1 << 1},  // a circle marker will be rendered at each point
		{"mvPlotMarker_Square"  , 1 << 2},  // a square maker will be rendered at each point
		{"mvPlotMarker_Diamond" , 1 << 3},  // a diamond marker will be rendered at each point
		{"mvPlotMarker_Up"      , 1 << 4},  // an upward-pointing triangle marker will up rendered at each point
		{"mvPlotMarker_Down"    , 1 << 5},  // an downward-pointing triangle marker will up rendered at each point
		{"mvPlotMarker_Left"    , 1 << 6},  // an leftward-pointing triangle marker will up rendered at each point
		{"mvPlotMarker_Right"   , 1 << 7},  // an rightward-pointing triangle marker will up rendered at each point
		{"mvPlotMarker_Cross"   , 1 << 8},  // a cross marker will be rendered at each point (not filled)
		{"mvPlotMarker_Plus"    , 1 << 9},  // a plus marker will be rendered at each point (not filled)
		{"mvPlotMarker_Asterisk", 1 << 10}, // a asterisk marker will be rendered at each point (not filled)
	};

	void CreateConstantsInterface(mvPythonModule& pyModule, PyObject* (*initfunc)())
	{
		PyImport_AppendInittab(pyModule.getName(), initfunc);
	}

}