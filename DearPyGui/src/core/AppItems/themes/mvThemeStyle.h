#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"
#include <imnodes.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvThemeStyle, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Color, 1);
	class mvThemeStyle : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvThemeStyle, add_theme_style)

		MV_CREATE_CONSTANT(mvStyleVar_Alpha, ImGuiStyleVar_Alpha);                             // float     Alpha
		MV_CREATE_CONSTANT(mvStyleVar_WindowPadding, ImGuiStyleVar_WindowPadding);             // ImVec2    WindowPadding
		MV_CREATE_CONSTANT(mvStyleVar_WindowRounding, ImGuiStyleVar_WindowRounding);           // float     WindowRounding
		MV_CREATE_CONSTANT(mvStyleVar_WindowBorderSize, ImGuiStyleVar_WindowBorderSize);       // float     WindowBorderSize
		MV_CREATE_CONSTANT(mvStyleVar_WindowMinSize, ImGuiStyleVar_WindowMinSize);             // ImVec2    WindowMinSize
		MV_CREATE_CONSTANT(mvStyleVar_WindowTitleAlign, ImGuiStyleVar_WindowTitleAlign);       // ImVec2    WindowTitleAlign
		MV_CREATE_CONSTANT(mvStyleVar_ChildRounding, ImGuiStyleVar_ChildRounding);             // float     ChildRounding
		MV_CREATE_CONSTANT(mvStyleVar_ChildBorderSize, ImGuiStyleVar_ChildBorderSize);         // float     ChildBorderSize
		MV_CREATE_CONSTANT(mvStyleVar_PopupRounding, ImGuiStyleVar_PopupRounding);             // float     PopupRounding
		MV_CREATE_CONSTANT(mvStyleVar_PopupBorderSize, ImGuiStyleVar_PopupBorderSize);         // float     PopupBorderSize
		MV_CREATE_CONSTANT(mvStyleVar_FramePadding, ImGuiStyleVar_FramePadding);               // ImVec2    FramePadding
		MV_CREATE_CONSTANT(mvStyleVar_FrameRounding, ImGuiStyleVar_FrameRounding);             // float     FrameRounding
		MV_CREATE_CONSTANT(mvStyleVar_FrameBorderSize, ImGuiStyleVar_FrameBorderSize);         // float     FrameBorderSize
		MV_CREATE_CONSTANT(mvStyleVar_ItemSpacing, ImGuiStyleVar_ItemSpacing);                 // ImVec2    ItemSpacing
		MV_CREATE_CONSTANT(mvStyleVar_ItemInnerSpacing, ImGuiStyleVar_ItemInnerSpacing);       // ImVec2    ItemInnerSpacing
		MV_CREATE_CONSTANT(mvStyleVar_IndentSpacing, ImGuiStyleVar_IndentSpacing);             // float     IndentSpacing
		MV_CREATE_CONSTANT(mvStyleVar_CellPadding, ImGuiStyleVar_CellPadding);                 // ImVec2    CellPadding
		MV_CREATE_CONSTANT(mvStyleVar_ScrollbarSize, ImGuiStyleVar_ScrollbarSize);             // float     ScrollbarSize
		MV_CREATE_CONSTANT(mvStyleVar_ScrollbarRounding, ImGuiStyleVar_ScrollbarRounding);     // float     ScrollbarRounding
		MV_CREATE_CONSTANT(mvStyleVar_GrabMinSize, ImGuiStyleVar_GrabMinSize);                 // float     GrabMinSize
		MV_CREATE_CONSTANT(mvStyleVar_GrabRounding, ImGuiStyleVar_GrabRounding);               // float     GrabRounding
		MV_CREATE_CONSTANT(mvStyleVar_TabRounding, ImGuiStyleVar_TabRounding);                 // float     TabRounding
		MV_CREATE_CONSTANT(mvStyleVar_ButtonTextAlign, ImGuiStyleVar_ButtonTextAlign);         // ImVec2    ButtonTextAlign
		MV_CREATE_CONSTANT(mvStyleVar_SelectableTextAlign, ImGuiStyleVar_SelectableTextAlign); // ImVec2    SelectableTextAlign

        // item styling variables
        MV_CREATE_CONSTANT(mvPlotStyleVar_LineWeight,         ImPlotStyleVar_LineWeight);         // float,  plot item line weight in pixels
        MV_CREATE_CONSTANT(mvPlotStyleVar_Marker,             ImPlotStyleVar_Marker);             // int,    marker specification
        MV_CREATE_CONSTANT(mvPlotStyleVar_MarkerSize,         ImPlotStyleVar_MarkerSize);         // float,  marker size in pixels (roughly the marker's "radius")
        MV_CREATE_CONSTANT(mvPlotStyleVar_MarkerWeight,       ImPlotStyleVar_MarkerWeight);       // float,  plot outline weight of markers in pixels
        MV_CREATE_CONSTANT(mvPlotStyleVar_FillAlpha,          ImPlotStyleVar_FillAlpha);          // float,  alpha modifier applied to all plot item fills
        MV_CREATE_CONSTANT(mvPlotStyleVar_ErrorBarSize,       ImPlotStyleVar_ErrorBarSize);       // float,  error bar whisker width in pixels
        MV_CREATE_CONSTANT(mvPlotStyleVar_ErrorBarWeight,     ImPlotStyleVar_ErrorBarWeight);     // float,  error bar whisker weight in pixels
        MV_CREATE_CONSTANT(mvPlotStyleVar_DigitalBitHeight,   ImPlotStyleVar_DigitalBitHeight);   // float,  digital channels bit height (at 1) in pixels
        MV_CREATE_CONSTANT(mvPlotStyleVar_DigitalBitGap,      ImPlotStyleVar_DigitalBitGap);      // float,  digital channels bit padding gap in pixels
    
	    // plot styling variables
		MV_CREATE_CONSTANT(mvPlotStyleVar_PlotBorderSize, ImPlotStyleVar_PlotBorderSize);         // float,  thickness of border around plot area
		MV_CREATE_CONSTANT(mvPlotStyleVar_MinorAlpha, ImPlotStyleVar_MinorAlpha);                 // float,  alpha multiplier applied to minor axis grid lines
		MV_CREATE_CONSTANT(mvPlotStyleVar_MajorTickLen, ImPlotStyleVar_MajorTickLen);             // ImVec2, major tick lengths for X and Y axes
		MV_CREATE_CONSTANT(mvPlotStyleVar_MinorTickLen, ImPlotStyleVar_MinorTickLen);             // ImVec2, minor tick lengths for X and Y axes
		MV_CREATE_CONSTANT(mvPlotStyleVar_MajorTickSize, ImPlotStyleVar_MajorTickSize);           // ImVec2, line thickness of major ticks
		MV_CREATE_CONSTANT(mvPlotStyleVar_MinorTickSize, ImPlotStyleVar_MinorTickSize);           // ImVec2, line thickness of minor ticks
		MV_CREATE_CONSTANT(mvPlotStyleVar_MajorGridSize, ImPlotStyleVar_MajorGridSize);           // ImVec2, line thickness of major grid lines
		MV_CREATE_CONSTANT(mvPlotStyleVar_MinorGridSize, ImPlotStyleVar_MinorGridSize);           // ImVec2, line thickness of minor grid lines
		MV_CREATE_CONSTANT(mvPlotStyleVar_PlotPadding, ImPlotStyleVar_PlotPadding);               // ImVec2, padding between widget frame and plot area, labels, or outside legends (i.e. main padding)
		MV_CREATE_CONSTANT(mvPlotStyleVar_LabelPadding, ImPlotStyleVar_LabelPadding);             // ImVec2, padding between axes labels, tick labels, and plot edge
		MV_CREATE_CONSTANT(mvPlotStyleVar_LegendPadding, ImPlotStyleVar_LegendPadding);           // ImVec2, legend padding from plot edges
		MV_CREATE_CONSTANT(mvPlotStyleVar_LegendInnerPadding, ImPlotStyleVar_LegendInnerPadding); // ImVec2, legend inner padding from legend edges
		MV_CREATE_CONSTANT(mvPlotStyleVar_LegendSpacing, ImPlotStyleVar_LegendSpacing);           // ImVec2, spacing between legend entries
		MV_CREATE_CONSTANT(mvPlotStyleVar_MousePosPadding, ImPlotStyleVar_MousePosPadding);       // ImVec2, padding between plot edge and interior info text
		MV_CREATE_CONSTANT(mvPlotStyleVar_AnnotationPadding, ImPlotStyleVar_AnnotationPadding);   // ImVec2, text padding around annotation labels
		MV_CREATE_CONSTANT(mvPlotStyleVar_FitPadding, ImPlotStyleVar_FitPadding);                 // ImVec2, additional fit padding as a percentage of the fit extents (e.g. ImVec2(0.1f,0.1f) adds 10% to the fit extents of X and Y)
		MV_CREATE_CONSTANT(mvPlotStyleVar_PlotDefaultSize, ImPlotStyleVar_PlotDefaultSize);       // ImVec2, default size used when ImVec2(0,0) is passed to BeginPlot
		MV_CREATE_CONSTANT(mvPlotStyleVar_PlotMinSize, ImPlotStyleVar_PlotMinSize);               // ImVec2, minimum size plot frame can be when shrunk

		// nodes
		MV_CREATE_CONSTANT(mvNodeStyleVar_GridSpacing, imnodes::StyleVar::StyleVar_GridSpacing);
		MV_CREATE_CONSTANT(mvNodeStyleVar_NodeCornerRounding, imnodes::StyleVar::StyleVar_NodeCornerRounding);
		MV_CREATE_CONSTANT(mvNodeStyleVar_NodePaddingHorizontal, imnodes::StyleVar::StyleVar_NodePaddingHorizontal);
		MV_CREATE_CONSTANT(mvNodeStyleVar_NodePaddingVertical, imnodes::StyleVar::StyleVar_NodePaddingVertical);
		MV_CREATE_CONSTANT(mvNodeStyleVar_NodeBorderThickness, imnodes::StyleVar::StyleVar_NodeBorderThickness);
		MV_CREATE_CONSTANT(mvNodeStyleVar_LinkThickness, imnodes::StyleVar::StyleVar_LinkThickness);
		MV_CREATE_CONSTANT(mvNodeStyleVar_LinkLineSegmentsPerLength, imnodes::StyleVar::StyleVar_LinkLineSegmentsPerLength);
		MV_CREATE_CONSTANT(mvNodeStyleVar_LinkHoverDistance, imnodes::StyleVar::StyleVar_LinkHoverDistance);
		MV_CREATE_CONSTANT(mvNodeStyleVar_PinCircleRadius, imnodes::StyleVar::StyleVar_PinCircleRadius);
		MV_CREATE_CONSTANT(mvNodeStyleVar_PinQuadSideLength, imnodes::StyleVar::StyleVar_PinQuadSideLength);
		MV_CREATE_CONSTANT(mvNodeStyleVar_PinTriangleSideLength, imnodes::StyleVar::StyleVar_PinTriangleSideLength);
		MV_CREATE_CONSTANT(mvNodeStyleVar_PinLineThickness, imnodes::StyleVar::StyleVar_PinLineThickness);
		MV_CREATE_CONSTANT(mvNodeStyleVar_PinHoverRadius, imnodes::StyleVar::StyleVar_PinHoverRadius);
		MV_CREATE_CONSTANT(mvNodeStyleVar_PinOffset, imnodes::StyleVar::StyleVar_PinOffset);

			MV_START_COMMANDS
			MV_END_COMMANDS

			MV_START_CONSTANTS
				MV_ADD_CONSTANT(mvStyleVar_Alpha),
				MV_ADD_CONSTANT(mvStyleVar_WindowPadding),
				MV_ADD_CONSTANT(mvStyleVar_WindowRounding),
				MV_ADD_CONSTANT(mvStyleVar_WindowBorderSize),
				MV_ADD_CONSTANT(mvStyleVar_WindowMinSize),
				MV_ADD_CONSTANT(mvStyleVar_WindowTitleAlign),
				MV_ADD_CONSTANT(mvStyleVar_ChildRounding),
				MV_ADD_CONSTANT(mvStyleVar_ChildBorderSize),
				MV_ADD_CONSTANT(mvStyleVar_PopupRounding),
				MV_ADD_CONSTANT(mvStyleVar_PopupBorderSize),
				MV_ADD_CONSTANT(mvStyleVar_FramePadding),
				MV_ADD_CONSTANT(mvStyleVar_FrameRounding),
				MV_ADD_CONSTANT(mvStyleVar_FrameBorderSize),
				MV_ADD_CONSTANT(mvStyleVar_ItemSpacing),
				MV_ADD_CONSTANT(mvStyleVar_ItemInnerSpacing),
				MV_ADD_CONSTANT(mvStyleVar_IndentSpacing),
				MV_ADD_CONSTANT(mvStyleVar_CellPadding),
				MV_ADD_CONSTANT(mvStyleVar_ScrollbarSize),
				MV_ADD_CONSTANT(mvStyleVar_ScrollbarRounding),
				MV_ADD_CONSTANT(mvStyleVar_GrabMinSize),
				MV_ADD_CONSTANT(mvStyleVar_GrabRounding),
				MV_ADD_CONSTANT(mvStyleVar_TabRounding),
				MV_ADD_CONSTANT(mvStyleVar_ButtonTextAlign),
				MV_ADD_CONSTANT(mvStyleVar_SelectableTextAlign),
				MV_ADD_CONSTANT(mvPlotStyleVar_LineWeight),
				MV_ADD_CONSTANT(mvPlotStyleVar_Marker),
				MV_ADD_CONSTANT(mvPlotStyleVar_MarkerSize),
				MV_ADD_CONSTANT(mvPlotStyleVar_MarkerWeight),
				MV_ADD_CONSTANT(mvPlotStyleVar_FillAlpha),
				MV_ADD_CONSTANT(mvPlotStyleVar_ErrorBarSize),
				MV_ADD_CONSTANT(mvPlotStyleVar_ErrorBarWeight),
				MV_ADD_CONSTANT(mvPlotStyleVar_DigitalBitHeight),
				MV_ADD_CONSTANT(mvPlotStyleVar_DigitalBitGap),
				MV_ADD_CONSTANT(mvPlotStyleVar_PlotBorderSize),
				MV_ADD_CONSTANT(mvPlotStyleVar_MinorAlpha),
				MV_ADD_CONSTANT(mvPlotStyleVar_MajorTickLen),
				MV_ADD_CONSTANT(mvPlotStyleVar_MinorTickLen),
				MV_ADD_CONSTANT(mvPlotStyleVar_MajorTickSize),
				MV_ADD_CONSTANT(mvPlotStyleVar_MinorTickSize),
				MV_ADD_CONSTANT(mvPlotStyleVar_MajorGridSize),
				MV_ADD_CONSTANT(mvPlotStyleVar_MinorGridSize),
				MV_ADD_CONSTANT(mvPlotStyleVar_PlotPadding),
				MV_ADD_CONSTANT(mvPlotStyleVar_LabelPadding),
				MV_ADD_CONSTANT(mvPlotStyleVar_LegendPadding),
				MV_ADD_CONSTANT(mvPlotStyleVar_LegendInnerPadding),
				MV_ADD_CONSTANT(mvPlotStyleVar_LegendSpacing),
				MV_ADD_CONSTANT(mvPlotStyleVar_MousePosPadding),
				MV_ADD_CONSTANT(mvPlotStyleVar_AnnotationPadding),
				MV_ADD_CONSTANT(mvPlotStyleVar_FitPadding),
				MV_ADD_CONSTANT(mvPlotStyleVar_PlotDefaultSize),
				MV_ADD_CONSTANT(mvPlotStyleVar_PlotMinSize),
				MV_ADD_CONSTANT(mvNodeStyleVar_GridSpacing),
				MV_ADD_CONSTANT(mvNodeStyleVar_NodeCornerRounding),
				MV_ADD_CONSTANT(mvNodeStyleVar_NodePaddingHorizontal),
				MV_ADD_CONSTANT(mvNodeStyleVar_NodePaddingVertical),
				MV_ADD_CONSTANT(mvNodeStyleVar_NodeBorderThickness),
				MV_ADD_CONSTANT(mvNodeStyleVar_LinkThickness),
				MV_ADD_CONSTANT(mvNodeStyleVar_LinkLineSegmentsPerLength),
				MV_ADD_CONSTANT(mvNodeStyleVar_LinkHoverDistance),
				MV_ADD_CONSTANT(mvNodeStyleVar_PinCircleRadius),
				MV_ADD_CONSTANT(mvNodeStyleVar_PinQuadSideLength),
				MV_ADD_CONSTANT(mvNodeStyleVar_PinTriangleSideLength),
				MV_ADD_CONSTANT(mvNodeStyleVar_PinLineThickness),
				MV_ADD_CONSTANT(mvNodeStyleVar_PinHoverRadius),
				MV_ADD_CONSTANT(mvNodeStyleVar_PinOffset)
			MV_END_CONSTANTS

	public:

		mvThemeStyle(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void customAction() override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;

		void setLibType(mvLibType libType) { m_libType = libType; }

	private:

		ImGuiCol m_targetStyle = 0;
		float m_x = 0.0f;
		float m_y = -1.0f;
		mvLibType m_libType = mvLibType::MV_IMGUI;
		bool m_twoComponent = false;
		

	};

}
