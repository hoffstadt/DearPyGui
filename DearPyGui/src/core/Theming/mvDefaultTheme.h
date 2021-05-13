#pragma once

#define MV_BASE_COL_bgColor mvColor(37, 37, 38, 255)
#define MV_BASE_COL_lightBgColor mvColor(82, 82, 85, 255)
#define MV_BASE_COL_veryLightBgColor mvColor(90, 90, 95, 255)
#define MV_BASE_COL_panelColor mvColor(51, 51, 55, 255)
#define MV_BASE_COL_panelHoverColor mvColor(29, 151, 236, 255)
#define MV_BASE_COL_panelActiveColor mvColor(0, 119, 200, 255)
#define MV_BASE_COL_textColor mvColor(255, 255, 255, 255)
#define MV_BASE_COL_textDisabledColor mvColor(151, 151, 151, 255)
#define MV_BASE_COL_borderColor mvColor(78, 78, 78, 255)

#define mvImGuiCol_Text						mvColor( 255, 255, 255, 255)
#define mvImGuiCol_TextDisabled				mvColor( 186, 191, 189, 255)
#define mvImGuiCol_WindowBg					mvColor( 23, 23, 23, 240)
#define mvImGuiCol_ChildBg					mvColor( 0, 0, 0, 0)
#define mvImGuiCol_PopupBg					mvColor( 20, 20, 20, 240)
#define mvImGuiCol_Border					mvColor( 51, 51, 51, 128)
#define mvImGuiCol_BorderShadow				mvColor( 0, 0, 0, 0)
#define mvImGuiCol_FrameBg					mvColor( 181, 99, 99, 138)
#define mvImGuiCol_FrameBgHovered			mvColor( 214, 168, 168, 102)
#define mvImGuiCol_FrameBgActive			mvColor( 214, 168, 168, 171)
#define mvImGuiCol_TitleBg					mvColor( 120, 56, 56, 171)
#define mvImGuiCol_TitleBgActive			mvColor( 120, 56, 56, 255)
#define mvImGuiCol_TitleBgCollapsed			mvColor( 120, 56, 56, 171)
#define mvImGuiCol_MenuBarBg				mvColor( 87, 41, 41, 255)
#define mvImGuiCol_ScrollbarBg				mvColor( 5, 5, 5, 135)
#define mvImGuiCol_ScrollbarGrab			mvColor( 79, 79, 79, 255)
#define mvImGuiCol_ScrollbarGrabHovered		mvColor( 105, 105, 105, 255)
#define mvImGuiCol_ScrollbarGrabActive		mvColor( 130, 130, 130, 255)
#define mvImGuiCol_CheckMark				mvColor( 255, 255, 255, 255)
#define mvImGuiCol_SliderGrab				mvColor( 181, 99, 99, 255)
#define mvImGuiCol_SliderGrabActive			mvColor( 214, 168, 168, 255)
#define mvImGuiCol_Button					mvColor( 120, 56, 56, 166)
#define mvImGuiCol_ButtonHovered			mvColor( 181, 99, 99, 166)
#define mvImGuiCol_ButtonActive				mvColor( 51, 51, 51, 128)
#define mvImGuiCol_Header					mvColor( 181, 99, 99, 138)
#define mvImGuiCol_HeaderHovered			mvColor( 214, 168, 168, 166)
#define mvImGuiCol_HeaderActive				mvColor( 214, 168, 168, 0)
#define mvImGuiCol_Separator				mvColor( 110, 110, 128, 128)
#define mvImGuiCol_SeparatorHovered			mvColor( 181, 99, 99, 138)
#define mvImGuiCol_SeparatorActive			mvColor( 181, 99, 99, 138)
#define mvImGuiCol_ResizeGrip				mvColor( 181, 99, 99, 138)
#define mvImGuiCol_ResizeGripHovered		mvColor( 214, 168, 168, 168)
#define mvImGuiCol_ResizeGripActive			mvColor( 214, 168, 168, 168)
#define mvImGuiCol_Tab						mvColor( 181, 99, 99, 138)
#define mvImGuiCol_TabHovered				mvColor( 214, 168, 168, 168)
#define mvImGuiCol_TabActive				mvColor( 214, 168, 168, 168)
#define mvImGuiCol_TabUnfocused				mvColor( 18, 26, 38, 247)
#define mvImGuiCol_TabUnfocusedActive		mvColor( 36, 66, 107, 255)
#define mvImGuiCol_DockingPreview			mvColor( 66, 150, 250, 179)
#define mvImGuiCol_DockingEmptyBg			mvColor( 51, 51, 51, 255)
#define mvImGuiCol_PlotLines				mvColor( 156, 156, 156, 255)
#define mvImGuiCol_PlotLinesHovered			mvColor( 255, 110, 89, 255)
#define mvImGuiCol_PlotHistogram			mvColor( 230, 179, 0, 255)
#define mvImGuiCol_PlotHistogramHovered		mvColor( 255, 153, 0, 255)
#define mvImGuiCol_TextSelectedBg			mvColor( 66, 150, 250, 89)
#define mvImGuiCol_DragDropTarget			mvColor( 255, 255, 0, 230)
#define mvImGuiCol_NavHighlight				mvColor( 105, 105, 105, 255)
#define mvImGuiCol_NavWindowingHighlight	mvColor( 255, 255, 255, 179)
#define mvImGuiCol_NavWindowingDimBg		mvColor( 204, 204, 204, 51)
#define mvImGuiCol_ModalWindowDimBg			mvColor( 204, 204, 204, 89)

#define mvImGuiCol_NodeBackground			mvImGuiCol_TitleBg
#define mvImGuiCol_NodeBackgroundHovered	mvImGuiCol_TitleBgCollapsed
#define mvImGuiCol_NodeBackgroundSelected	mvImGuiCol_TitleBgActive
#define mvImGuiCol_NodeOutline				mvImGuiCol_Border
#define mvImGuiCol_Link						mvImGuiCol_Button
#define mvImGuiCol_LinkHovered				mvImGuiCol_ButtonHovered
#define mvImGuiCol_LinkSelected				mvImGuiCol_ButtonActive
#define mvImGuiCol_Pin						mvImGuiCol_FrameBg
#define mvImGuiCol_PinHovered				mvImGuiCol_FrameBgHovered
#define mvImGuiCol_BoxSelector				mvImGuiCol_FrameBg
#define mvImGuiCol_BoxSelectorOutline		mvImGuiCol_Border
#define mvImGuiCol_GridBackground			mvImGuiCol_ChildBg
#define mvImGuiCol_GridLine					mvImGuiCol_ScrollbarBg

#define mvImGuiCol_TableHeaderBg			mvImGuiCol_TitleBgActive
#define mvImGuiCol_TableBorderStrong		mvImGuiCol_TitleBg
#define mvImGuiCol_TableBorderLight			mvImGuiCol_TitleBgCollapsed
#define mvImGuiCol_TableRowBg				mvImGuiCol_TitleBg
#define mvImGuiCol_TableRowBgAlt			mvImGuiCol_TitleBgCollapsed


