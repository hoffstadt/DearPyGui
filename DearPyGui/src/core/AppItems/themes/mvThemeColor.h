#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"
#include <imnodes.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvThemeColor, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Color, 1);
	class mvThemeColor : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvThemeColor, add_theme_color)

        MV_CREATE_CONSTANT(mvThemeCol_Text, ImGuiCol_Text);
        MV_CREATE_CONSTANT(mvThemeCol_TextDisabled, ImGuiCol_TextDisabled);
        MV_CREATE_CONSTANT(mvThemeCol_WindowBg, ImGuiCol_WindowBg);            // Background of normal windows
        MV_CREATE_CONSTANT(mvThemeCol_ChildBg, ImGuiCol_ChildBg);              // Background of child windows
        MV_CREATE_CONSTANT(mvThemeCol_Border, ImGuiCol_Border);                // Background of popups);menus);tooltips windows
        MV_CREATE_CONSTANT(mvThemeCol_PopupBg, ImGuiCol_PopupBg);              // Background of popups);menus);tooltips windows
        MV_CREATE_CONSTANT(mvThemeCol_BorderShadow, ImGuiCol_BorderShadow);
        MV_CREATE_CONSTANT(mvThemeCol_FrameBg, ImGuiCol_FrameBg);             // Background of checkbox);radio button);plot);slider);text input
        MV_CREATE_CONSTANT(mvThemeCol_FrameBgHovered, ImGuiCol_FrameBgHovered);
        MV_CREATE_CONSTANT(mvThemeCol_FrameBgActive, ImGuiCol_FrameBgActive);
        MV_CREATE_CONSTANT(mvThemeCol_TitleBg, ImGuiCol_TitleBg);
        MV_CREATE_CONSTANT(mvThemeCol_TitleBgActive, ImGuiCol_TitleBgActive);
        MV_CREATE_CONSTANT(mvThemeCol_TitleBgCollapsed, ImGuiCol_TitleBgCollapsed);
        MV_CREATE_CONSTANT(mvThemeCol_MenuBarBg, ImGuiCol_MenuBarBg);
        MV_CREATE_CONSTANT(mvThemeCol_ScrollbarBg, ImGuiCol_ScrollbarBg);
        MV_CREATE_CONSTANT(mvThemeCol_ScrollbarGrab, ImGuiCol_ScrollbarGrab);
        MV_CREATE_CONSTANT(mvThemeCol_ScrollbarGrabHovered, ImGuiCol_ScrollbarGrabHovered);
        MV_CREATE_CONSTANT(mvThemeCol_ScrollbarGrabActive, ImGuiCol_ScrollbarGrabActive);
        MV_CREATE_CONSTANT(mvThemeCol_CheckMark, ImGuiCol_CheckMark);
        MV_CREATE_CONSTANT(mvThemeCol_SliderGrab, ImGuiCol_SliderGrab);
        MV_CREATE_CONSTANT(mvThemeCol_SliderGrabActive, ImGuiCol_SliderGrabActive);
        MV_CREATE_CONSTANT(mvThemeCol_Button, ImGuiCol_Button);
        MV_CREATE_CONSTANT(mvThemeCol_ButtonHovered, ImGuiCol_ButtonHovered);
        MV_CREATE_CONSTANT(mvThemeCol_ButtonActive, ImGuiCol_ButtonActive);
        MV_CREATE_CONSTANT(mvThemeCol_Header, ImGuiCol_Header);              // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
        MV_CREATE_CONSTANT(mvThemeCol_HeaderHovered, ImGuiCol_HeaderHovered);
        MV_CREATE_CONSTANT(mvThemeCol_HeaderActive, ImGuiCol_HeaderActive);
        MV_CREATE_CONSTANT(mvThemeCol_Separator, ImGuiCol_Separator);
        MV_CREATE_CONSTANT(mvThemeCol_SeparatorHovered, ImGuiCol_SeparatorHovered);
        MV_CREATE_CONSTANT(mvThemeCol_SeparatorActive, ImGuiCol_SeparatorActive);
        MV_CREATE_CONSTANT(mvThemeCol_ResizeGrip, ImGuiCol_ResizeGrip);
        MV_CREATE_CONSTANT(mvThemeCol_ResizeGripHovered, ImGuiCol_ResizeGripHovered);
        MV_CREATE_CONSTANT(mvThemeCol_ResizeGripActive, ImGuiCol_ResizeGripActive);
        MV_CREATE_CONSTANT(mvThemeCol_Tab, ImGuiCol_Tab);
        MV_CREATE_CONSTANT(mvThemeCol_TabHovered, ImGuiCol_TabHovered);
        MV_CREATE_CONSTANT(mvThemeCol_TabActive, ImGuiCol_TabActive);
        MV_CREATE_CONSTANT(mvThemeCol_TabUnfocused, ImGuiCol_TabUnfocused);
        MV_CREATE_CONSTANT(mvThemeCol_TabUnfocusedActive, ImGuiCol_TabUnfocusedActive);
        MV_CREATE_CONSTANT(mvThemeCol_DockingPreview, ImGuiCol_DockingPreview);      // Preview overlay color when about to docking something
        MV_CREATE_CONSTANT(mvThemeCol_DockingEmptyBg, ImGuiCol_DockingEmptyBg);      // Background color for empty node (e.g. CentralNode with no window docked into it)
        MV_CREATE_CONSTANT(mvThemeCol_PlotLines, ImGuiCol_PlotLines);
        MV_CREATE_CONSTANT(mvThemeCol_PlotLinesHovered, ImGuiCol_PlotLinesHovered);
        MV_CREATE_CONSTANT(mvThemeCol_PlotHistogram, ImGuiCol_PlotHistogram);
        MV_CREATE_CONSTANT(mvThemeCol_PlotHistogramHovered, ImGuiCol_PlotHistogramHovered);
        MV_CREATE_CONSTANT(mvThemeCol_TableHeaderBg, ImGuiCol_TableHeaderBg);           // Table header background
        MV_CREATE_CONSTANT(mvThemeCol_TableBorderStrong, ImGuiCol_TableBorderStrong);   // Table outer and header borders (prefer using Alpha=1.0 here)
        MV_CREATE_CONSTANT(mvThemeCol_TableBorderLight, ImGuiCol_TableBorderLight);     // Table inner borders (prefer using Alpha=1.0 here)
        MV_CREATE_CONSTANT(mvThemeCol_TableRowBg, ImGuiCol_TableRowBg);                 // Table row background (even rows)
        MV_CREATE_CONSTANT(mvThemeCol_TableRowBgAlt, ImGuiCol_TableRowBgAlt);           // Table row background (odd rows)
        MV_CREATE_CONSTANT(mvThemeCol_TextSelectedBg, ImGuiCol_TextSelectedBg);
        MV_CREATE_CONSTANT(mvThemeCol_DragDropTarget, ImGuiCol_DragDropTarget);
        MV_CREATE_CONSTANT(mvThemeCol_NavHighlight, ImGuiCol_NavHighlight);                   // Gamepad/keyboard: current highlighted item
        MV_CREATE_CONSTANT(mvThemeCol_NavWindowingHighlight, ImGuiCol_NavWindowingHighlight); // Highlight window when using CTRL+TAB
        MV_CREATE_CONSTANT(mvThemeCol_NavWindowingDimBg, ImGuiCol_NavWindowingDimBg);         // Darken/colorize entire screen behind the CTRL+TAB window list, when active
        MV_CREATE_CONSTANT(mvThemeCol_ModalWindowDimBg, ImGuiCol_ModalWindowDimBg);           // Darken/colorize entire screen behind a modal window, when one is active
        
        // plotting
  
        // item styling colors
        MV_CREATE_CONSTANT(mvPlotCol_Line, ImPlotCol_Line);                   // plot line/outline color (defaults to next unused color in current colormap)
        MV_CREATE_CONSTANT(mvPlotCol_Fill, ImPlotCol_Fill);                   // plot fill color for bars (defaults to the current line color)
        MV_CREATE_CONSTANT(mvPlotCol_MarkerOutline, ImPlotCol_MarkerOutline); // marker outline color (defaults to the current line color)
        MV_CREATE_CONSTANT(mvPlotCol_MarkerFill, ImPlotCol_MarkerFill);       // marker fill color (defaults to the current line color)
        MV_CREATE_CONSTANT(mvPlotCol_ErrorBar, ImPlotCol_ErrorBar);           // error bar color (defaults to ImGuiCol_Text)
        
        // plot styling colors
        MV_CREATE_CONSTANT(mvPlotCol_FrameBg, ImPlotCol_FrameBg);           // plot frame background color (defaults to ImGuiCol_FrameBg)
        MV_CREATE_CONSTANT(mvPlotCol_PlotBg, ImPlotCol_PlotBg);             // plot area background color (defaults to ImGuiCol_WindowBg)
        MV_CREATE_CONSTANT(mvPlotCol_PlotBorder, ImPlotCol_PlotBorder);     // plot area border color (defaults to ImGuiCol_Border)
        MV_CREATE_CONSTANT(mvPlotCol_LegendBg, ImPlotCol_LegendBg);         // legend background color (defaults to ImGuiCol_PopupBg)
        MV_CREATE_CONSTANT(mvPlotCol_LegendBorder, ImPlotCol_LegendBorder); // legend border color (defaults to ImPlotCol_PlotBorder)
        MV_CREATE_CONSTANT(mvPlotCol_LegendText, ImPlotCol_LegendText);     // legend text color (defaults to ImPlotCol_InlayText)
        MV_CREATE_CONSTANT(mvPlotCol_TitleText, ImPlotCol_TitleText);       // plot title text color (defaults to ImGuiCol_Text)
        MV_CREATE_CONSTANT(mvPlotCol_InlayText, ImPlotCol_InlayText);       // color of text appearing inside of plots (defaults to ImGuiCol_Text)
        MV_CREATE_CONSTANT(mvPlotCol_XAxis, ImPlotCol_XAxis);               // x-axis label and tick lables color (defaults to ImGuiCol_Text)
        MV_CREATE_CONSTANT(mvPlotCol_XAxisGrid, ImPlotCol_XAxisGrid);       // x-axis grid color (defaults to 25% ImPlotCol_XAxis)
        MV_CREATE_CONSTANT(mvPlotCol_YAxis, ImPlotCol_YAxis);               // y-axis label and tick labels color (defaults to ImGuiCol_Text)
        MV_CREATE_CONSTANT(mvPlotCol_YAxisGrid, ImPlotCol_YAxisGrid);       // y-axis grid color (defaults to 25% ImPlotCol_YAxis)
        MV_CREATE_CONSTANT(mvPlotCol_YAxis2, ImPlotCol_YAxis2);             // 2nd y-axis label and tick labels color (defaults to ImGuiCol_Text)
        MV_CREATE_CONSTANT(mvPlotCol_YAxisGrid2, ImPlotCol_YAxisGrid2);     // 2nd y-axis grid/label color (defaults to 25% ImPlotCol_YAxis2)
        MV_CREATE_CONSTANT(mvPlotCol_YAxis3, ImPlotCol_YAxis3);             // 3rd y-axis label and tick labels color (defaults to ImGuiCol_Text)
        MV_CREATE_CONSTANT(mvPlotCol_YAxisGrid3, ImPlotCol_YAxisGrid3);     // 3rd y-axis grid/label color (defaults to 25% ImPlotCol_YAxis3)
        MV_CREATE_CONSTANT(mvPlotCol_Selection, ImPlotCol_Selection);       // box-selection color (defaults to yellow)
        MV_CREATE_CONSTANT(mvPlotCol_Query, ImPlotCol_Query);               // box-query color (defaults to green)
        MV_CREATE_CONSTANT(mvPlotCol_Crosshairs, ImPlotCol_Crosshairs);     // crosshairs color (defaults to ImPlotCol_PlotBorder)

        // nodes
        MV_CREATE_CONSTANT(mvNodeCol_NodeBackground, imnodes::ColorStyle::ColorStyle_NodeBackground);
        MV_CREATE_CONSTANT(mvNodeCol_NodeBackgroundHovered, imnodes::ColorStyle::ColorStyle_NodeBackgroundHovered);
        MV_CREATE_CONSTANT(mvNodeCol_NodeBackgroundSelected, imnodes::ColorStyle::ColorStyle_NodeBackgroundSelected);
        MV_CREATE_CONSTANT(mvNodeCol_NodeOutline, imnodes::ColorStyle::ColorStyle_NodeOutline);
        MV_CREATE_CONSTANT(mvNodeCol_TitleBar, imnodes::ColorStyle::ColorStyle_TitleBar);
        MV_CREATE_CONSTANT(mvNodeCol_TitleBarHovered, imnodes::ColorStyle::ColorStyle_TitleBarHovered);
        MV_CREATE_CONSTANT(mvNodeCol_TitleBarSelected, imnodes::ColorStyle::ColorStyle_TitleBarSelected);
        MV_CREATE_CONSTANT(mvNodeCol_Link, imnodes::ColorStyle::ColorStyle_Link);
        MV_CREATE_CONSTANT(mvNodeCol_LinkHovered, imnodes::ColorStyle::ColorStyle_LinkHovered);
        MV_CREATE_CONSTANT(mvNodeCol_LinkSelected, imnodes::ColorStyle::ColorStyle_LinkSelected);
        MV_CREATE_CONSTANT(mvNodeCol_Pin, imnodes::ColorStyle::ColorStyle_Pin);
        MV_CREATE_CONSTANT(mvNodeCol_PinHovered, imnodes::ColorStyle::ColorStyle_PinHovered);
        MV_CREATE_CONSTANT(mvNodeCol_BoxSelector, imnodes::ColorStyle::ColorStyle_BoxSelector);
        MV_CREATE_CONSTANT(mvNodeCol_BoxSelectorOutline, imnodes::ColorStyle::ColorStyle_BoxSelectorOutline);
        MV_CREATE_CONSTANT(mvNodeCol_GridBackground, imnodes::ColorStyle::ColorStyle_GridBackground);
        MV_CREATE_CONSTANT(mvNodeCol_GridLine, imnodes::ColorStyle::ColorStyle_GridLine);

			MV_START_COMMANDS
			MV_END_COMMANDS

			MV_START_CONSTANTS
            MV_ADD_CONSTANT(mvThemeCol_Text),
            MV_ADD_CONSTANT(mvThemeCol_TextDisabled),
            MV_ADD_CONSTANT(mvThemeCol_WindowBg),
            MV_ADD_CONSTANT(mvThemeCol_ChildBg),
            MV_ADD_CONSTANT(mvThemeCol_Border),
            MV_ADD_CONSTANT(mvThemeCol_PopupBg),
            MV_ADD_CONSTANT(mvThemeCol_BorderShadow),
            MV_ADD_CONSTANT(mvThemeCol_FrameBg),
            MV_ADD_CONSTANT(mvThemeCol_FrameBgHovered),
            MV_ADD_CONSTANT(mvThemeCol_FrameBgActive),
            MV_ADD_CONSTANT(mvThemeCol_TitleBg),
            MV_ADD_CONSTANT(mvThemeCol_TitleBgActive),
            MV_ADD_CONSTANT(mvThemeCol_TitleBgCollapsed),
            MV_ADD_CONSTANT(mvThemeCol_MenuBarBg),
            MV_ADD_CONSTANT(mvThemeCol_ScrollbarBg),
            MV_ADD_CONSTANT(mvThemeCol_ScrollbarGrab),
            MV_ADD_CONSTANT(mvThemeCol_ScrollbarGrabHovered),
            MV_ADD_CONSTANT(mvThemeCol_ScrollbarGrabActive),
            MV_ADD_CONSTANT(mvThemeCol_CheckMark),
            MV_ADD_CONSTANT(mvThemeCol_SliderGrab),
            MV_ADD_CONSTANT(mvThemeCol_SliderGrabActive),
            MV_ADD_CONSTANT(mvThemeCol_Button),
            MV_ADD_CONSTANT(mvThemeCol_ButtonHovered),
            MV_ADD_CONSTANT(mvThemeCol_ButtonActive),
            MV_ADD_CONSTANT(mvThemeCol_Header),
            MV_ADD_CONSTANT(mvThemeCol_HeaderHovered),
            MV_ADD_CONSTANT(mvThemeCol_HeaderActive),
            MV_ADD_CONSTANT(mvThemeCol_Separator),
            MV_ADD_CONSTANT(mvThemeCol_SeparatorHovered),
            MV_ADD_CONSTANT(mvThemeCol_SeparatorActive),
            MV_ADD_CONSTANT(mvThemeCol_ResizeGrip),
            MV_ADD_CONSTANT(mvThemeCol_ResizeGripHovered),
            MV_ADD_CONSTANT(mvThemeCol_ResizeGripActive),
            MV_ADD_CONSTANT(mvThemeCol_Tab),
            MV_ADD_CONSTANT(mvThemeCol_TabHovered),
            MV_ADD_CONSTANT(mvThemeCol_TabActive),
            MV_ADD_CONSTANT(mvThemeCol_TabUnfocused),
            MV_ADD_CONSTANT(mvThemeCol_TabUnfocusedActive),
            MV_ADD_CONSTANT(mvThemeCol_DockingPreview),
            MV_ADD_CONSTANT(mvThemeCol_DockingEmptyBg),
            MV_ADD_CONSTANT(mvThemeCol_PlotLines),
            MV_ADD_CONSTANT(mvThemeCol_PlotLinesHovered),
            MV_ADD_CONSTANT(mvThemeCol_PlotHistogram),
            MV_ADD_CONSTANT(mvThemeCol_PlotHistogramHovered),
            MV_ADD_CONSTANT(mvThemeCol_TableHeaderBg),
            MV_ADD_CONSTANT(mvThemeCol_TableBorderStrong),
            MV_ADD_CONSTANT(mvThemeCol_TableBorderLight),
            MV_ADD_CONSTANT(mvThemeCol_TableRowBg),
            MV_ADD_CONSTANT(mvThemeCol_TableRowBgAlt),
            MV_ADD_CONSTANT(mvThemeCol_TextSelectedBg),
            MV_ADD_CONSTANT(mvThemeCol_DragDropTarget),
            MV_ADD_CONSTANT(mvThemeCol_NavHighlight),
            MV_ADD_CONSTANT(mvThemeCol_NavWindowingHighlight),
            MV_ADD_CONSTANT(mvThemeCol_NavWindowingDimBg),
            MV_ADD_CONSTANT(mvThemeCol_ModalWindowDimBg),
            MV_ADD_CONSTANT(mvPlotCol_Line),
            MV_ADD_CONSTANT(mvPlotCol_Fill),
            MV_ADD_CONSTANT(mvPlotCol_MarkerOutline),
            MV_ADD_CONSTANT(mvPlotCol_MarkerFill),
            MV_ADD_CONSTANT(mvPlotCol_ErrorBar),
            MV_ADD_CONSTANT(mvPlotCol_FrameBg),
            MV_ADD_CONSTANT(mvPlotCol_PlotBg),
            MV_ADD_CONSTANT(mvPlotCol_PlotBorder),
            MV_ADD_CONSTANT(mvPlotCol_LegendBg),
            MV_ADD_CONSTANT(mvPlotCol_LegendBorder),
            MV_ADD_CONSTANT(mvPlotCol_LegendText),
            MV_ADD_CONSTANT(mvPlotCol_TitleText),
            MV_ADD_CONSTANT(mvPlotCol_InlayText),
            MV_ADD_CONSTANT(mvPlotCol_XAxis),
            MV_ADD_CONSTANT(mvPlotCol_XAxisGrid),
            MV_ADD_CONSTANT(mvPlotCol_YAxis),
            MV_ADD_CONSTANT(mvPlotCol_YAxisGrid),
            MV_ADD_CONSTANT(mvPlotCol_YAxis2),
            MV_ADD_CONSTANT(mvPlotCol_YAxisGrid2),
            MV_ADD_CONSTANT(mvPlotCol_YAxis3),
            MV_ADD_CONSTANT(mvPlotCol_YAxisGrid3),
            MV_ADD_CONSTANT(mvPlotCol_Selection),
            MV_ADD_CONSTANT(mvPlotCol_Query),
            MV_ADD_CONSTANT(mvPlotCol_Crosshairs),
            MV_ADD_CONSTANT(mvNodeCol_NodeBackground),
            MV_ADD_CONSTANT(mvNodeCol_NodeBackgroundHovered),
            MV_ADD_CONSTANT(mvNodeCol_NodeBackgroundSelected),
            MV_ADD_CONSTANT(mvNodeCol_NodeOutline),
            MV_ADD_CONSTANT(mvNodeCol_TitleBar),
            MV_ADD_CONSTANT(mvNodeCol_TitleBarHovered),
            MV_ADD_CONSTANT(mvNodeCol_TitleBarSelected),
            MV_ADD_CONSTANT(mvNodeCol_Link),
            MV_ADD_CONSTANT(mvNodeCol_LinkHovered),
            MV_ADD_CONSTANT(mvNodeCol_LinkSelected),
            MV_ADD_CONSTANT(mvNodeCol_Pin),
            MV_ADD_CONSTANT(mvNodeCol_PinHovered),
            MV_ADD_CONSTANT(mvNodeCol_BoxSelector),
            MV_ADD_CONSTANT(mvNodeCol_BoxSelectorOutline),
            MV_ADD_CONSTANT(mvNodeCol_GridBackground),
            MV_ADD_CONSTANT(mvNodeCol_GridLine)
			MV_END_CONSTANTS

	public:

		mvThemeColor(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void customAction() override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		bool isParentCompatible(mvAppItemType type) override;

		void setLibType(mvLibType libType) { m_libType = libType; }

	private:

		ImGuiCol m_targetColor = 0;
		mvColor m_color;
		mvLibType m_libType = mvLibType::MV_IMGUI;
		

	};

}
