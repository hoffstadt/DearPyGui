#include "dearpygui.h"
#include "mvAppItemCommons.h"
#include "mvFontManager.h"
#include "mvItemRegistry.h"
#include <ImGuiFileDialog.h>
#include <cstdlib>
#include "mvToolManager.h"
#include "mvCustomTypes.h"
#include "mvPyUtils.h"
#include "mvViewport.h"
#include <stb_image.h>
#include "mvProfiler.h"
#include "dearpygui_commands.h"
#include "dearpygui_parsers.h"

#define MV_ADD_COMMAND(x) methods.push_back({ #x, (PyCFunction)x, METH_VARARGS | METH_KEYWORDS, GetParsers()[#x].documentation.c_str() });

const std::map<std::string, mvPythonParser>& 
GetModuleParsers()
{

	static auto parsers = std::map<std::string, mvPythonParser>();

	if (parsers.empty())
	{
		#define X(el) parsers.insert({GetEntityCommand(mvAppItemType::el), DearPyGui::GetEntityParser(mvAppItemType::el)});
		MV_ITEM_TYPES
		#undef X

		// to prevent exceeding function stack size
		InsertParser_Block0(parsers);
		InsertParser_Block1(parsers);
		InsertParser_Block2(parsers);
		InsertParser_Block3(parsers);
		InsertParser_Block4(parsers);
	}
	return parsers;
}

const std::vector<std::pair<std::string, long>>&
GetModuleConstants()
{
	static bool First_Run = true;
	static std::vector<std::pair<std::string, long>> ModuleConstants;

	if (First_Run)
	{
		InsertConstants_mvContext(ModuleConstants);

		ModuleConstants.push_back({"mvAll", 0L });
		ModuleConstants.push_back({"mvTool_About", MV_TOOL_ABOUT_UUID });
		ModuleConstants.push_back({"mvTool_Debug", MV_TOOL_DEBUG_UUID });
		ModuleConstants.push_back({"mvTool_Doc", MV_TOOL_DOC_UUID });
		ModuleConstants.push_back({"mvTool_ItemRegistry", MV_TOOL_ITEM_REGISTRY_UUID });
		ModuleConstants.push_back({"mvTool_Metrics", MV_TOOL_METRICS_UUID });
		ModuleConstants.push_back({"mvTool_Style", MV_TOOL_STYLE_UUID });
		ModuleConstants.push_back({"mvTool_Font", MV_TOOL_FONT_UUID });
		ModuleConstants.push_back({"mvFontAtlas", MV_ATLAS_UUID });
		ModuleConstants.push_back({"mvAppUUID", MV_APP_UUID });
		ModuleConstants.push_back({"mvInvalidUUID", MV_INVALID_UUID });
		ModuleConstants.push_back({"mvDir_None", ImGuiDir_None });
		ModuleConstants.push_back({"mvDir_Left", ImGuiDir_Left });
		ModuleConstants.push_back({"mvDir_Right", ImGuiDir_Right });
		ModuleConstants.push_back({"mvDir_Up", ImGuiDir_Up });
		ModuleConstants.push_back({"mvDir_Down", ImGuiDir_Down });
		ModuleConstants.push_back({"mvComboHeight_Small", 0L });
		ModuleConstants.push_back({"mvComboHeight_Regular", 1L });
		ModuleConstants.push_back({"mvComboHeight_Large", 2L });
		ModuleConstants.push_back({"mvComboHeight_Largest", 3L });

		ModuleConstants.push_back({"mvPlatform_Windows", 0L });
		ModuleConstants.push_back({"mvPlatform_Apple", 1L });
		ModuleConstants.push_back({"mvPlatform_Linux", 2L });

		ModuleConstants.push_back({"mvColorEdit_AlphaPreviewNone", 0L });
		ModuleConstants.push_back({"mvColorEdit_AlphaPreview", ImGuiColorEditFlags_AlphaPreview });
		ModuleConstants.push_back({"mvColorEdit_AlphaPreviewHalf", ImGuiColorEditFlags_AlphaPreviewHalf });
		ModuleConstants.push_back({"mvColorEdit_uint8", ImGuiColorEditFlags_Uint8});
		ModuleConstants.push_back({"mvColorEdit_float", ImGuiColorEditFlags_Float});
		ModuleConstants.push_back({"mvColorEdit_rgb", ImGuiColorEditFlags_DisplayRGB});
		ModuleConstants.push_back({"mvColorEdit_hsv", ImGuiColorEditFlags_DisplayHSV});
		ModuleConstants.push_back({"mvColorEdit_hex", ImGuiColorEditFlags_DisplayHex});
		ModuleConstants.push_back({"mvColorEdit_input_rgb", ImGuiColorEditFlags_InputRGB});
		ModuleConstants.push_back({"mvColorEdit_input_hsv", ImGuiColorEditFlags_InputHSV});

		ModuleConstants.push_back({ "mvPlotColormap_Default", 0L }); // ImPlot default colormap         (n=10)
		ModuleConstants.push_back({ "mvPlotColormap_Deep", 0L }); // ImPlot default colormap         (n=10)
		ModuleConstants.push_back({ "mvPlotColormap_Dark", 1L }); // a.k.a. matplotlib "Set1"        (n=9)
		ModuleConstants.push_back({ "mvPlotColormap_Pastel", 2L}); // a.k.a. matplotlib "Pastel1"     (n=9)
		ModuleConstants.push_back({ "mvPlotColormap_Paired", 3L}); // a.k.a. matplotlib "Paired"      (n=12)
		ModuleConstants.push_back({ "mvPlotColormap_Viridis", 4L }); // a.k.a. matplotlib "viridis"     (n=11)
		ModuleConstants.push_back({ "mvPlotColormap_Plasma", 5L }); // a.k.a. matplotlib "plasma"      (n=11)
		ModuleConstants.push_back({ "mvPlotColormap_Hot", 6L }); // a.k.a. matplotlib/MATLAB "hot"  (n=11)
		ModuleConstants.push_back({ "mvPlotColormap_Cool", 7L }); // a.k.a. matplotlib/MATLAB "cool" (n=11)
		ModuleConstants.push_back({ "mvPlotColormap_Pink", 8L }); // a.k.a. matplotlib/MATLAB "pink" (n=11)
		ModuleConstants.push_back({ "mvPlotColormap_Jet", 9L }); // a.k.a. MATLAB "jet"             (n=11)
		ModuleConstants.push_back({ "mvPlotColormap_Twilight", 10L }); // a.k.a. MATLAB "jet"             (n=11)
		ModuleConstants.push_back({ "mvPlotColormap_RdBu", 11L }); // a.k.a. MATLAB "jet"             (n=11)
		ModuleConstants.push_back({ "mvPlotColormap_BrBG", 12L }); // a.k.a. MATLAB "jet"             (n=11)
		ModuleConstants.push_back({ "mvPlotColormap_PiYG", 13L }); // a.k.a. MATLAB "jet"             (n=11)
		ModuleConstants.push_back({ "mvPlotColormap_Spectral", 14L }); // a.k.a. MATLAB "jet"             (n=11)
		ModuleConstants.push_back({ "mvPlotColormap_Greys", 15L }); // a.k.a. MATLAB "jet"             (n=11)

		ModuleConstants.push_back({ "mvColorPicker_bar", ImGuiColorEditFlags_PickerHueBar });
		ModuleConstants.push_back({ "mvColorPicker_wheel", ImGuiColorEditFlags_PickerHueWheel });

		ModuleConstants.push_back({ "mvTabOrder_Reorderable", 0L });
		ModuleConstants.push_back({ "mvTabOrder_Fixed", 1L });
		ModuleConstants.push_back({ "mvTabOrder_Leading", 2L });
		ModuleConstants.push_back({ "mvTabOrder_Trailing", 3L });

		ModuleConstants.push_back({ "mvTimeUnit_Us", 0L });
		ModuleConstants.push_back({ "mvTimeUnit_Ms", 1L });
		ModuleConstants.push_back({ "mvTimeUnit_S", 2L });
		ModuleConstants.push_back({ "mvTimeUnit_Min", 3L });
		ModuleConstants.push_back({ "mvTimeUnit_Hr", 4L });
		ModuleConstants.push_back({ "mvTimeUnit_Day", 5L });
		ModuleConstants.push_back({ "mvTimeUnit_Mo", 6L });
		ModuleConstants.push_back({ "mvTimeUnit_Yr", 7L });

		ModuleConstants.push_back({ "mvDatePickerLevel_Day", 0L });
		ModuleConstants.push_back({ "mvDatePickerLevel_Month", 1L });
		ModuleConstants.push_back({ "mvDatePickerLevel_Year", 2L });

		ModuleConstants.push_back({ "mvCullMode_None", 0L });
		ModuleConstants.push_back({ "mvCullMode_Back", 1L });
		ModuleConstants.push_back({ "mvCullMode_Front", 2L });

		ModuleConstants.push_back({ "mvFontRangeHint_Default", 0L });
		ModuleConstants.push_back({ "mvFontRangeHint_Japanese", 1L });
		ModuleConstants.push_back({ "mvFontRangeHint_Korean", 2L });
		ModuleConstants.push_back({ "mvFontRangeHint_Chinese_Full", 3L });
		ModuleConstants.push_back({ "mvFontRangeHint_Chinese_Simplified_Common", 4L });
		ModuleConstants.push_back({ "mvFontRangeHint_Cyrillic", 5L });
		ModuleConstants.push_back({ "mvFontRangeHint_Thai", 6L });
		ModuleConstants.push_back({ "mvFontRangeHint_Vietnamese", 7L });

		ModuleConstants.push_back({ "mvNode_PinShape_Circle", ImNodesPinShape_Circle });
		ModuleConstants.push_back({ "mvNode_PinShape_CircleFilled", ImNodesPinShape_CircleFilled });
		ModuleConstants.push_back({ "mvNode_PinShape_Triangle", ImNodesPinShape_Triangle });
		ModuleConstants.push_back({ "mvNode_PinShape_TriangleFilled", ImNodesPinShape_TriangleFilled });
		ModuleConstants.push_back({ "mvNode_PinShape_Quad", ImNodesPinShape_Quad });
		ModuleConstants.push_back({ "mvNode_PinShape_QuadFilled", ImNodesPinShape_QuadFilled });

		ModuleConstants.push_back({ "mvNode_Attr_Input", 0L });
		ModuleConstants.push_back({ "mvNode_Attr_Output", 1L});
		ModuleConstants.push_back({ "mvNode_Attr_Static", 2L});

		ModuleConstants.push_back({ "mvPlotBin_Sqrt", -1L });
		ModuleConstants.push_back({ "mvPlotBin_Sturges", -2L });
		ModuleConstants.push_back({ "mvPlotBin_Rice", -3L });
		ModuleConstants.push_back({ "mvPlotBin_Scott", -4L });

		ModuleConstants.push_back({ "mvXAxis", 0L});
		ModuleConstants.push_back({ "mvYAxis", 1L});

		ModuleConstants.push_back({ "mvPlotMarker_None", -1L});  // no marker
		ModuleConstants.push_back({ "mvPlotMarker_Circle",  0L});  // a circle marker will be rendered at each point
		ModuleConstants.push_back({ "mvPlotMarker_Square",  1L});  // a square maker will be rendered at each point
		ModuleConstants.push_back({ "mvPlotMarker_Diamond",  2L});  // a diamond marker will be rendered at each point
		ModuleConstants.push_back({ "mvPlotMarker_Up",  3L});  // an upward-pointing triangle marker will up rendered at each point
		ModuleConstants.push_back({ "mvPlotMarker_Down",  4L});  // an downward-pointing triangle marker will up rendered at each point
		ModuleConstants.push_back({ "mvPlotMarker_Left",  5L});  // an leftward-pointing triangle marker will up rendered at each point
		ModuleConstants.push_back({ "mvPlotMarker_Right",  6L});  // an rightward-pointing triangle marker will up rendered at each point
		ModuleConstants.push_back({ "mvPlotMarker_Cross",  7L});  // a cross marker will be rendered at each point (not filled)
		ModuleConstants.push_back({ "mvPlotMarker_Plus",  8L});  // a plus marker will be rendered at each point (not filled)
		ModuleConstants.push_back({ "mvPlotMarker_Asterisk",  9L}); // a asterisk marker will be rendered at each point (not filled)

		ModuleConstants.push_back({ "mvPlot_Location_Center", ImPlotLocation_Center });
		ModuleConstants.push_back({ "mvPlot_Location_North", ImPlotLocation_North });
		ModuleConstants.push_back({ "mvPlot_Location_South", ImPlotLocation_South });
		ModuleConstants.push_back({ "mvPlot_Location_West", ImPlotLocation_West});
		ModuleConstants.push_back({ "mvPlot_Location_East", ImPlotLocation_East});
		ModuleConstants.push_back({ "mvPlot_Location_NorthWest", ImPlotLocation_NorthWest});
		ModuleConstants.push_back({ "mvPlot_Location_NorthEast", ImPlotLocation_NorthEast});
		ModuleConstants.push_back({ "mvPlot_Location_SouthWest", ImPlotLocation_SouthWest});
		ModuleConstants.push_back({ "mvPlot_Location_SouthEast", ImPlotLocation_SouthEast});

		ModuleConstants.push_back({ "mvNodeMiniMap_Location_BottomLeft", ImNodesMiniMapLocation_BottomLeft });
		ModuleConstants.push_back({ "mvNodeMiniMap_Location_BottomRight", ImNodesMiniMapLocation_BottomRight });
		ModuleConstants.push_back({ "mvNodeMiniMap_Location_TopLeft", ImNodesMiniMapLocation_TopLeft });
		ModuleConstants.push_back({ "mvNodeMiniMap_Location_TopRight", ImNodesMiniMapLocation_TopRight });

		ModuleConstants.push_back({ "mvTable_SizingFixedFit", ImGuiTableFlags_SizingFixedFit });
		ModuleConstants.push_back({ "mvTable_SizingFixedSame", ImGuiTableFlags_SizingFixedSame });
		ModuleConstants.push_back({ "mvTable_SizingStretchProp", ImGuiTableFlags_SizingStretchProp});
		ModuleConstants.push_back({ "mvTable_SizingStretchSame", ImGuiTableFlags_SizingStretchSame});

		ModuleConstants.push_back({ "mvFormat_Float_rgba", 0L });
		ModuleConstants.push_back({ "mvFormat_Float_rgb", 1L });

		ModuleConstants.push_back({ "mvThemeCat_Core", 0L });
		ModuleConstants.push_back({ "mvThemeCat_Plots", 1L});
		ModuleConstants.push_back({ "mvThemeCat_Nodes", 2L});

		ModuleConstants.push_back({ "mvThemeCol_Text", ImGuiCol_Text });
		ModuleConstants.push_back({ "mvThemeCol_TextDisabled", ImGuiCol_TextDisabled });
		ModuleConstants.push_back({ "mvThemeCol_WindowBg", ImGuiCol_WindowBg });            // Background of normal windows
		ModuleConstants.push_back({ "mvThemeCol_ChildBg", ImGuiCol_ChildBg });              // Background of child windows
		ModuleConstants.push_back({ "mvThemeCol_Border", ImGuiCol_Border });                // Background of popups, menus, tooltips windows
		ModuleConstants.push_back({ "mvThemeCol_PopupBg", ImGuiCol_PopupBg });              // Background of popups, menus, tooltips windows
		ModuleConstants.push_back({ "mvThemeCol_BorderShadow", ImGuiCol_BorderShadow });
		ModuleConstants.push_back({ "mvThemeCol_FrameBg", ImGuiCol_FrameBg });             // Background of checkbox, radio button, plot, slider, text input
		ModuleConstants.push_back({ "mvThemeCol_FrameBgHovered", ImGuiCol_FrameBgHovered });
		ModuleConstants.push_back({ "mvThemeCol_FrameBgActive", ImGuiCol_FrameBgActive });
		ModuleConstants.push_back({ "mvThemeCol_TitleBg", ImGuiCol_TitleBg });
		ModuleConstants.push_back({ "mvThemeCol_TitleBgActive", ImGuiCol_TitleBgActive });
		ModuleConstants.push_back({ "mvThemeCol_TitleBgCollapsed", ImGuiCol_TitleBgCollapsed });
		ModuleConstants.push_back({ "mvThemeCol_MenuBarBg", ImGuiCol_MenuBarBg });
		ModuleConstants.push_back({ "mvThemeCol_ScrollbarBg", ImGuiCol_ScrollbarBg });
		ModuleConstants.push_back({ "mvThemeCol_ScrollbarGrab", ImGuiCol_ScrollbarGrab });
		ModuleConstants.push_back({ "mvThemeCol_ScrollbarGrabHovered", ImGuiCol_ScrollbarGrabHovered });
		ModuleConstants.push_back({ "mvThemeCol_ScrollbarGrabActive", ImGuiCol_ScrollbarGrabActive });
		ModuleConstants.push_back({ "mvThemeCol_CheckMark", ImGuiCol_CheckMark });
		ModuleConstants.push_back({ "mvThemeCol_SliderGrab", ImGuiCol_SliderGrab });
		ModuleConstants.push_back({ "mvThemeCol_SliderGrabActive", ImGuiCol_SliderGrabActive });
		ModuleConstants.push_back({ "mvThemeCol_Button", ImGuiCol_Button });
		ModuleConstants.push_back({ "mvThemeCol_ButtonHovered", ImGuiCol_ButtonHovered });
		ModuleConstants.push_back({ "mvThemeCol_ButtonActive", ImGuiCol_ButtonActive });
		ModuleConstants.push_back({ "mvThemeCol_Header", ImGuiCol_Header });              // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		ModuleConstants.push_back({ "mvThemeCol_HeaderHovered", ImGuiCol_HeaderHovered });
		ModuleConstants.push_back({ "mvThemeCol_HeaderActive", ImGuiCol_HeaderActive });
		ModuleConstants.push_back({ "mvThemeCol_Separator", ImGuiCol_Separator });
		ModuleConstants.push_back({ "mvThemeCol_SeparatorHovered", ImGuiCol_SeparatorHovered });
		ModuleConstants.push_back({ "mvThemeCol_SeparatorActive", ImGuiCol_SeparatorActive });
		ModuleConstants.push_back({ "mvThemeCol_ResizeGrip", ImGuiCol_ResizeGrip });
		ModuleConstants.push_back({ "mvThemeCol_ResizeGripHovered", ImGuiCol_ResizeGripHovered });
		ModuleConstants.push_back({ "mvThemeCol_ResizeGripActive", ImGuiCol_ResizeGripActive });
		ModuleConstants.push_back({ "mvThemeCol_Tab", ImGuiCol_Tab });
		ModuleConstants.push_back({ "mvThemeCol_TabHovered", ImGuiCol_TabHovered });
		ModuleConstants.push_back({ "mvThemeCol_TabActive", ImGuiCol_TabActive });
		ModuleConstants.push_back({ "mvThemeCol_TabUnfocused", ImGuiCol_TabUnfocused });
		ModuleConstants.push_back({ "mvThemeCol_TabUnfocusedActive", ImGuiCol_TabUnfocusedActive });
		ModuleConstants.push_back({ "mvThemeCol_DockingPreview", ImGuiCol_DockingPreview });      // Preview overlay color when about to docking something
		ModuleConstants.push_back({ "mvThemeCol_DockingEmptyBg", ImGuiCol_DockingEmptyBg });      // Background color for empty node (e.g. CentralNode with no window docked into it)
		ModuleConstants.push_back({ "mvThemeCol_PlotLines", ImGuiCol_PlotLines });
		ModuleConstants.push_back({ "mvThemeCol_PlotLinesHovered", ImGuiCol_PlotLinesHovered });
		ModuleConstants.push_back({ "mvThemeCol_PlotHistogram", ImGuiCol_PlotHistogram });
		ModuleConstants.push_back({ "mvThemeCol_PlotHistogramHovered", ImGuiCol_PlotHistogramHovered });
		ModuleConstants.push_back({ "mvThemeCol_TableHeaderBg", ImGuiCol_TableHeaderBg });           // Table header background
		ModuleConstants.push_back({ "mvThemeCol_TableBorderStrong", ImGuiCol_TableBorderStrong });   // Table outer and header borders (prefer using Alpha=1.0 here)
		ModuleConstants.push_back({ "mvThemeCol_TableBorderLight", ImGuiCol_TableBorderLight });     // Table inner borders (prefer using Alpha=1.0 here)
		ModuleConstants.push_back({ "mvThemeCol_TableRowBg", ImGuiCol_TableRowBg });                 // Table row background (even rows)
		ModuleConstants.push_back({ "mvThemeCol_TableRowBgAlt", ImGuiCol_TableRowBgAlt });           // Table row background (odd rows)
		ModuleConstants.push_back({ "mvThemeCol_TextSelectedBg", ImGuiCol_TextSelectedBg });
		ModuleConstants.push_back({ "mvThemeCol_DragDropTarget", ImGuiCol_DragDropTarget });
		ModuleConstants.push_back({ "mvThemeCol_NavHighlight", ImGuiCol_NavHighlight });                   // Gamepad/keyboard: current highlighted item
		ModuleConstants.push_back({ "mvThemeCol_NavWindowingHighlight", ImGuiCol_NavWindowingHighlight }); // Highlight window when using CTRL+TAB
		ModuleConstants.push_back({ "mvThemeCol_NavWindowingDimBg", ImGuiCol_NavWindowingDimBg });         // Darken/colorize entire screen behind the CTRL+TAB window list", when active
		ModuleConstants.push_back({ "mvThemeCol_ModalWindowDimBg", ImGuiCol_ModalWindowDimBg });           // Darken/colorize entire screen behind a modal window", when one is active

		// plotting

		// item styling colors
		ModuleConstants.push_back({ "mvPlotCol_Line", ImPlotCol_Line });                   // plot line/outline color (defaults to next unused color in current colormap)
		ModuleConstants.push_back({ "mvPlotCol_Fill", ImPlotCol_Fill });                   // plot fill color for bars (defaults to the current line color)
		ModuleConstants.push_back({ "mvPlotCol_MarkerOutline", ImPlotCol_MarkerOutline }); // marker outline color (defaults to the current line color)
		ModuleConstants.push_back({ "mvPlotCol_MarkerFill", ImPlotCol_MarkerFill });       // marker fill color (defaults to the current line color)
		ModuleConstants.push_back({ "mvPlotCol_ErrorBar", ImPlotCol_ErrorBar });           // error bar color (defaults to ImGuiCol_Text)

		// plot styling colors
		ModuleConstants.push_back({ "mvPlotCol_FrameBg", ImPlotCol_FrameBg });           // plot frame background color (defaults to ImGuiCol_FrameBg)
		ModuleConstants.push_back({ "mvPlotCol_PlotBg", ImPlotCol_PlotBg });             // plot area background color (defaults to ImGuiCol_WindowBg)
		ModuleConstants.push_back({ "mvPlotCol_PlotBorder", ImPlotCol_PlotBorder });     // plot area border color (defaults to ImGuiCol_Border)
		ModuleConstants.push_back({ "mvPlotCol_LegendBg", ImPlotCol_LegendBg });         // legend background color (defaults to ImGuiCol_PopupBg)
		ModuleConstants.push_back({ "mvPlotCol_LegendBorder", ImPlotCol_LegendBorder }); // legend border color (defaults to ImPlotCol_PlotBorder)
		ModuleConstants.push_back({ "mvPlotCol_LegendText", ImPlotCol_LegendText });     // legend text color (defaults to ImPlotCol_InlayText)
		ModuleConstants.push_back({ "mvPlotCol_TitleText", ImPlotCol_TitleText });       // plot title text color (defaults to ImGuiCol_Text)
		ModuleConstants.push_back({ "mvPlotCol_InlayText", ImPlotCol_InlayText });       // color of text appearing inside of plots (defaults to ImGuiCol_Text)
		ModuleConstants.push_back({ "mvPlotCol_XAxis", ImPlotCol_XAxis });               // x-axis label and tick lables color (defaults to ImGuiCol_Text)
		ModuleConstants.push_back({ "mvPlotCol_XAxisGrid", ImPlotCol_XAxisGrid });       // x-axis grid color (defaults to 25% ImPlotCol_XAxis)
		ModuleConstants.push_back({ "mvPlotCol_YAxis", ImPlotCol_YAxis });               // y-axis label and tick labels color (defaults to ImGuiCol_Text)
		ModuleConstants.push_back({ "mvPlotCol_YAxisGrid", ImPlotCol_YAxisGrid });       // y-axis grid color (defaults to 25% ImPlotCol_YAxis)
		ModuleConstants.push_back({ "mvPlotCol_YAxis2", ImPlotCol_YAxis2 });             // 2nd y-axis label and tick labels color (defaults to ImGuiCol_Text)
		ModuleConstants.push_back({ "mvPlotCol_YAxisGrid2", ImPlotCol_YAxisGrid2 });     // 2nd y-axis grid/label color (defaults to 25% ImPlotCol_YAxis2)
		ModuleConstants.push_back({ "mvPlotCol_YAxis3", ImPlotCol_YAxis3 });             // 3rd y-axis label and tick labels color (defaults to ImGuiCol_Text)
		ModuleConstants.push_back({ "mvPlotCol_YAxisGrid3", ImPlotCol_YAxisGrid3 });     // 3rd y-axis grid/label color (defaults to 25% ImPlotCol_YAxis3)
		ModuleConstants.push_back({ "mvPlotCol_Selection", ImPlotCol_Selection });       // box-selection color (defaults to yellow)
		ModuleConstants.push_back({ "mvPlotCol_Query", ImPlotCol_Query });               // box-query color (defaults to green)
		ModuleConstants.push_back({ "mvPlotCol_Crosshairs", ImPlotCol_Crosshairs });     // crosshairs color (defaults to ImPlotCol_PlotBorder)

		// nodes
		ModuleConstants.push_back({ "mvNodeCol_NodeBackground", ImNodesCol_NodeBackground });
		ModuleConstants.push_back({ "mvNodeCol_NodeBackgroundHovered", ImNodesCol_NodeBackgroundHovered });
		ModuleConstants.push_back({ "mvNodeCol_NodeBackgroundSelected", ImNodesCol_NodeBackgroundSelected });
		ModuleConstants.push_back({ "mvNodeCol_NodeOutline", ImNodesCol_NodeOutline });
		ModuleConstants.push_back({ "mvNodeCol_TitleBar", ImNodesCol_TitleBar });
		ModuleConstants.push_back({ "mvNodeCol_TitleBarHovered", ImNodesCol_TitleBarHovered });
		ModuleConstants.push_back({ "mvNodeCol_TitleBarSelected", ImNodesCol_TitleBarSelected });
		ModuleConstants.push_back({ "mvNodeCol_Link", ImNodesCol_Link });
		ModuleConstants.push_back({ "mvNodeCol_LinkHovered", ImNodesCol_LinkHovered });
		ModuleConstants.push_back({ "mvNodeCol_LinkSelected", ImNodesCol_LinkSelected });
		ModuleConstants.push_back({ "mvNodeCol_Pin", ImNodesCol_Pin });
		ModuleConstants.push_back({ "mvNodeCol_PinHovered", ImNodesCol_PinHovered });
		ModuleConstants.push_back({ "mvNodeCol_BoxSelector", ImNodesCol_BoxSelector });
		ModuleConstants.push_back({ "mvNodeCol_BoxSelectorOutline", ImNodesCol_BoxSelectorOutline });
		ModuleConstants.push_back({ "mvNodeCol_GridBackground", ImNodesCol_GridBackground });
		ModuleConstants.push_back({ "mvNodeCol_GridLine", ImNodesCol_GridLine });
		ModuleConstants.push_back({ "mvNodesCol_GridLinePrimary", ImNodesCol_GridLinePrimary });
		ModuleConstants.push_back({ "mvNodesCol_MiniMapBackground", ImNodesCol_MiniMapBackground });
		ModuleConstants.push_back({ "mvNodesCol_MiniMapBackgroundHovered", ImNodesCol_MiniMapBackgroundHovered });
		ModuleConstants.push_back({ "mvNodesCol_MiniMapOutline", ImNodesCol_MiniMapOutline });
		ModuleConstants.push_back({ "mvNodesCol_MiniMapOutlineHovered", ImNodesCol_MiniMapOutlineHovered });
		ModuleConstants.push_back({ "mvNodesCol_MiniMapNodeBackground", ImNodesCol_MiniMapNodeBackground });
		ModuleConstants.push_back({ "mvNodesCol_MiniMapNodeBackgroundHovered", ImNodesCol_MiniMapNodeBackgroundHovered });
		ModuleConstants.push_back({ "mvNodesCol_MiniMapNodeBackgroundSelected", ImNodesCol_MiniMapNodeBackgroundSelected });
		ModuleConstants.push_back({ "mvNodesCol_MiniMapNodeOutline", ImNodesCol_MiniMapNodeOutline });
		ModuleConstants.push_back({ "mvNodesCol_MiniMapLink", ImNodesCol_MiniMapLink });
		ModuleConstants.push_back({ "mvNodesCol_MiniMapLinkSelected", ImNodesCol_MiniMapLinkSelected });
		ModuleConstants.push_back({ "mvNodesCol_MiniMapCanvas", ImNodesCol_MiniMapCanvas });
		ModuleConstants.push_back({ "mvNodesCol_MiniMapCanvasOutline", ImNodesCol_MiniMapCanvasOutline });


		ModuleConstants.push_back({ "mvStyleVar_Alpha", ImGuiStyleVar_Alpha });                             // float     Alpha
		ModuleConstants.push_back({ "mvStyleVar_WindowPadding", ImGuiStyleVar_WindowPadding });             // ImVec2    WindowPadding
		ModuleConstants.push_back({ "mvStyleVar_WindowRounding", ImGuiStyleVar_WindowRounding });           // float     WindowRounding
		ModuleConstants.push_back({ "mvStyleVar_WindowBorderSize", ImGuiStyleVar_WindowBorderSize });       // float     WindowBorderSize
		ModuleConstants.push_back({ "mvStyleVar_WindowMinSize", ImGuiStyleVar_WindowMinSize });             // ImVec2    WindowMinSize
		ModuleConstants.push_back({ "mvStyleVar_WindowTitleAlign", ImGuiStyleVar_WindowTitleAlign });       // ImVec2    WindowTitleAlign
		ModuleConstants.push_back({ "mvStyleVar_ChildRounding", ImGuiStyleVar_ChildRounding });             // float     ChildRounding
		ModuleConstants.push_back({ "mvStyleVar_ChildBorderSize", ImGuiStyleVar_ChildBorderSize });         // float     ChildBorderSize
		ModuleConstants.push_back({ "mvStyleVar_PopupRounding", ImGuiStyleVar_PopupRounding });             // float     PopupRounding
		ModuleConstants.push_back({ "mvStyleVar_PopupBorderSize", ImGuiStyleVar_PopupBorderSize });         // float     PopupBorderSize
		ModuleConstants.push_back({ "mvStyleVar_FramePadding", ImGuiStyleVar_FramePadding });               // ImVec2    FramePadding
		ModuleConstants.push_back({ "mvStyleVar_FrameRounding", ImGuiStyleVar_FrameRounding });             // float     FrameRounding
		ModuleConstants.push_back({ "mvStyleVar_FrameBorderSize", ImGuiStyleVar_FrameBorderSize });         // float     FrameBorderSize
		ModuleConstants.push_back({ "mvStyleVar_ItemSpacing", ImGuiStyleVar_ItemSpacing });                 // ImVec2    ItemSpacing
		ModuleConstants.push_back({ "mvStyleVar_ItemInnerSpacing", ImGuiStyleVar_ItemInnerSpacing });       // ImVec2    ItemInnerSpacing
		ModuleConstants.push_back({ "mvStyleVar_IndentSpacing", ImGuiStyleVar_IndentSpacing });             // float     IndentSpacing
		ModuleConstants.push_back({ "mvStyleVar_CellPadding", ImGuiStyleVar_CellPadding });                 // ImVec2    CellPadding
		ModuleConstants.push_back({ "mvStyleVar_ScrollbarSize", ImGuiStyleVar_ScrollbarSize });             // float     ScrollbarSize
		ModuleConstants.push_back({ "mvStyleVar_ScrollbarRounding", ImGuiStyleVar_ScrollbarRounding });     // float     ScrollbarRounding
		ModuleConstants.push_back({ "mvStyleVar_GrabMinSize", ImGuiStyleVar_GrabMinSize });                 // float     GrabMinSize
		ModuleConstants.push_back({ "mvStyleVar_GrabRounding", ImGuiStyleVar_GrabRounding });               // float     GrabRounding
		ModuleConstants.push_back({ "mvStyleVar_TabRounding", ImGuiStyleVar_TabRounding });                 // float     TabRounding
		ModuleConstants.push_back({ "mvStyleVar_ButtonTextAlign", ImGuiStyleVar_ButtonTextAlign });         // ImVec2    ButtonTextAlign
		ModuleConstants.push_back({ "mvStyleVar_SelectableTextAlign", ImGuiStyleVar_SelectableTextAlign }); // ImVec2    SelectableTextAlign

		// item styling variables
		ModuleConstants.push_back({ "mvPlotStyleVar_LineWeight",         ImPlotStyleVar_LineWeight });         // float,  plot item line weight in pixels
		ModuleConstants.push_back({ "mvPlotStyleVar_Marker",             ImPlotStyleVar_Marker });             // int,    marker specification
		ModuleConstants.push_back({ "mvPlotStyleVar_MarkerSize",         ImPlotStyleVar_MarkerSize });         // float,  marker size in pixels (roughly the marker's "radius")
		ModuleConstants.push_back({ "mvPlotStyleVar_MarkerWeight",       ImPlotStyleVar_MarkerWeight });       // float,  plot outline weight of markers in pixels
		ModuleConstants.push_back({ "mvPlotStyleVar_FillAlpha",          ImPlotStyleVar_FillAlpha });          // float,  alpha modifier applied to all plot item fills
		ModuleConstants.push_back({ "mvPlotStyleVar_ErrorBarSize",       ImPlotStyleVar_ErrorBarSize });       // float,  error bar whisker width in pixels
		ModuleConstants.push_back({ "mvPlotStyleVar_ErrorBarWeight",     ImPlotStyleVar_ErrorBarWeight });     // float,  error bar whisker weight in pixels
		ModuleConstants.push_back({ "mvPlotStyleVar_DigitalBitHeight",   ImPlotStyleVar_DigitalBitHeight });   // float,  digital channels bit height (at 1) in pixels
		ModuleConstants.push_back({ "mvPlotStyleVar_DigitalBitGap",      ImPlotStyleVar_DigitalBitGap });      // float,  digital channels bit padding gap in pixels

		// plot styling variables
		ModuleConstants.push_back({ "mvPlotStyleVar_PlotBorderSize", ImPlotStyleVar_PlotBorderSize });         // float,  thickness of border around plot area
		ModuleConstants.push_back({ "mvPlotStyleVar_MinorAlpha", ImPlotStyleVar_MinorAlpha });                 // float,  alpha multiplier applied to minor axis grid lines
		ModuleConstants.push_back({ "mvPlotStyleVar_MajorTickLen", ImPlotStyleVar_MajorTickLen });             // ImVec2, major tick lengths for X and Y axes
		ModuleConstants.push_back({ "mvPlotStyleVar_MinorTickLen", ImPlotStyleVar_MinorTickLen });             // ImVec2, minor tick lengths for X and Y axes
		ModuleConstants.push_back({ "mvPlotStyleVar_MajorTickSize", ImPlotStyleVar_MajorTickSize });           // ImVec2, line thickness of major ticks
		ModuleConstants.push_back({ "mvPlotStyleVar_MinorTickSize", ImPlotStyleVar_MinorTickSize });           // ImVec2, line thickness of minor ticks
		ModuleConstants.push_back({ "mvPlotStyleVar_MajorGridSize", ImPlotStyleVar_MajorGridSize });           // ImVec2, line thickness of major grid lines
		ModuleConstants.push_back({ "mvPlotStyleVar_MinorGridSize", ImPlotStyleVar_MinorGridSize });           // ImVec2, line thickness of minor grid lines
		ModuleConstants.push_back({ "mvPlotStyleVar_PlotPadding", ImPlotStyleVar_PlotPadding });               // ImVec2, padding between widget frame and plot area, labels, or outside legends (i.e. main padding)
		ModuleConstants.push_back({ "mvPlotStyleVar_LabelPadding", ImPlotStyleVar_LabelPadding });             // ImVec2, padding between axes labels, tick labels, and plot edge
		ModuleConstants.push_back({ "mvPlotStyleVar_LegendPadding", ImPlotStyleVar_LegendPadding });           // ImVec2, legend padding from plot edges
		ModuleConstants.push_back({ "mvPlotStyleVar_LegendInnerPadding", ImPlotStyleVar_LegendInnerPadding }); // ImVec2, legend inner padding from legend edges
		ModuleConstants.push_back({ "mvPlotStyleVar_LegendSpacing", ImPlotStyleVar_LegendSpacing });           // ImVec2, spacing between legend entries
		ModuleConstants.push_back({ "mvPlotStyleVar_MousePosPadding", ImPlotStyleVar_MousePosPadding });       // ImVec2, padding between plot edge and interior info text
		ModuleConstants.push_back({ "mvPlotStyleVar_AnnotationPadding", ImPlotStyleVar_AnnotationPadding });   // ImVec2, text padding around annotation labels
		ModuleConstants.push_back({ "mvPlotStyleVar_FitPadding", ImPlotStyleVar_FitPadding });                 // ImVec2, additional fit padding as a percentage of the fit extents (e.g. ImVec2(0.1f,0.1f) adds 10% to the fit extents of X and Y)
		ModuleConstants.push_back({ "mvPlotStyleVar_PlotDefaultSize", ImPlotStyleVar_PlotDefaultSize });       // ImVec2, default size used when ImVec2(0,0) is passed to BeginPlot
		ModuleConstants.push_back({ "mvPlotStyleVar_PlotMinSize", ImPlotStyleVar_PlotMinSize });               // ImVec2, minimum size plot frame can be when shrunk

		// nodes
		ModuleConstants.push_back({ "mvNodeStyleVar_GridSpacing", ImNodesStyleVar_GridSpacing });
		ModuleConstants.push_back({ "mvNodeStyleVar_NodeCornerRounding", ImNodesStyleVar_NodeCornerRounding });
		ModuleConstants.push_back({ "mvNodeStyleVar_NodePadding", ImNodesStyleVar_NodePadding });
		ModuleConstants.push_back({ "mvNodeStyleVar_NodeBorderThickness", ImNodesStyleVar_NodeBorderThickness });
		ModuleConstants.push_back({ "mvNodeStyleVar_LinkThickness", ImNodesStyleVar_LinkThickness });
		ModuleConstants.push_back({ "mvNodeStyleVar_LinkLineSegmentsPerLength", ImNodesStyleVar_LinkLineSegmentsPerLength });
		ModuleConstants.push_back({ "mvNodeStyleVar_LinkHoverDistance", ImNodesStyleVar_LinkHoverDistance });
		ModuleConstants.push_back({ "mvNodeStyleVar_PinCircleRadius", ImNodesStyleVar_PinCircleRadius });
		ModuleConstants.push_back({ "mvNodeStyleVar_PinQuadSideLength", ImNodesStyleVar_PinQuadSideLength });
		ModuleConstants.push_back({ "mvNodeStyleVar_PinTriangleSideLength", ImNodesStyleVar_PinTriangleSideLength });
		ModuleConstants.push_back({ "mvNodeStyleVar_PinLineThickness", ImNodesStyleVar_PinLineThickness });
		ModuleConstants.push_back({ "mvNodeStyleVar_PinHoverRadius", ImNodesStyleVar_PinHoverRadius });
		ModuleConstants.push_back({ "mvNodeStyleVar_PinOffset", ImNodesStyleVar_PinOffset });
		ModuleConstants.push_back({ "mvNodesStyleVar_MiniMapPadding", ImNodesStyleVar_MiniMapPadding });
		ModuleConstants.push_back({ "mvNodesStyleVar_MiniMapOffset", ImNodesStyleVar_MiniMapOffset });

		#define X(el) ModuleConstants.push_back({ std::string(#el), (long)mvAppItemType::el });
		MV_ITEM_TYPES
		#undef X

		int j = 0;
		for (int i = MV_RESERVED_UUID_start; i < MV_RESERVED_UUID_start+MV_RESERVED_UUIDs; i++)
		{
			ModuleConstants.push_back({"mvReservedUUID_" + std::to_string(j), (long)i});
			j++;
		}

	}

	First_Run = false;
	return ModuleConstants;
}

static void
PreFetchItemInfo(mvUUID* out_name, mvUUID* out_parent, mvUUID* out_before, std::string* out_alias, PyObject* args, PyObject* kwargs)
{

	if (kwargs)
	{
		if (PyObject* item = PyDict_GetItemString(kwargs, "parent"))
		{
			if (PyUnicode_Check(item))
				*out_parent = GetIdFromAlias(*GContext->itemRegistry, ToString(item));
			else
				*out_parent = ToUUID(item);
		}

		if (PyObject* item = PyDict_GetItemString(kwargs, "before"))
		{
			if (PyUnicode_Check(item))
				*out_before = GetIdFromAlias(*GContext->itemRegistry, ToString(item));
			else
				*out_before = ToUUID(item);
		}

		if (PyObject* item = PyDict_GetItemString(kwargs, "tag"))
		{
			if (PyUnicode_Check(item))
				*out_alias = ToString(item);
			else
			{
				auto id = ToUUID(item);
				if (id > 0)
					*out_name = id;
			}
		};
	}
}

static PyObject*
common_constructor(const char* command, mvAppItemType type, PyObject* self, PyObject* args, PyObject* kwargs)
{

	std::shared_ptr<mvAppItem> item = nullptr;
	mvUUID id = 0;
	mvUUID parent = 0;
	mvUUID before = 0;
	std::string alias;

	PreFetchItemInfo(&id, &parent, &before, &alias, args, kwargs);

	// generate id if not specified
	if (id == 0)
		id = GenerateUUID();

	item = DearPyGui::CreateEntity(type, id);

	i32 flags = DearPyGui::GetEntityDesciptionFlags(type);
	if (flags & MV_ITEM_DESC_DRAW_CMP)
		item->drawInfo = std::make_shared<mvAppItemDrawInfo>();

	// register alias if present
	if (!alias.empty())
	{
		RemoveAlias(*GContext->itemRegistry, item->config.alias, true);
		item->config.alias = alias;
		AddAlias(*GContext->itemRegistry, item->config.alias, item->uuid);
	}

	VerifyArgumentCount(GetParsers()[command], args);

	if(!GContext->IO.skipRequiredArgs)
		item->handleSpecificRequiredArgs(args);

	if(!GContext->IO.skipPositionalArgs)
		item->handleSpecificPositionalArgs(args);

	if(!GContext->IO.skipKeywordArgs)
		item->handleKeywordArgs(kwargs, command);

	AddItemWithRuntimeChecks((*GContext->itemRegistry), item, parent, before);

	// return raw UUID if alias not used
	if (item->config.alias.empty())
		return Py_BuildValue("K", id);

	return ToPyString(item->config.alias);
}

#define X(el) static PyObject* el##_command(PyObject* self, PyObject* args, PyObject* kwargs){return common_constructor(GetEntityCommand(mvAppItemType::el), mvAppItemType::el, self, args, kwargs);}
MV_ITEM_TYPES
#undef X

PyMODINIT_FUNC
PyInit__dearpygui(void)
{

	static std::vector<PyMethodDef> methods;
	methods.clear();

	#define X(el) methods.push_back({GetEntityCommand(mvAppItemType::el), (PyCFunction)el##_command, METH_VARARGS | METH_KEYWORDS, GetParsers()[GetEntityCommand(mvAppItemType::el)].documentation.c_str()});
	MV_ITEM_TYPES
	#undef X

	// color maps
	MV_ADD_COMMAND(bind_colormap);
	MV_ADD_COMMAND(sample_colormap);
	MV_ADD_COMMAND(get_colormap_color);

	// file dialog
	MV_ADD_COMMAND(get_file_dialog_info);

	// window commands
	MV_ADD_COMMAND(set_x_scroll);
	MV_ADD_COMMAND(set_y_scroll);
	MV_ADD_COMMAND(get_x_scroll);
	MV_ADD_COMMAND(get_y_scroll);
	MV_ADD_COMMAND(get_x_scroll_max);
	MV_ADD_COMMAND(get_y_scroll_max);

	// draw layer
	MV_ADD_COMMAND(set_clip_space);

	// draw node
	MV_ADD_COMMAND(apply_transform);
	MV_ADD_COMMAND(create_rotation_matrix);
	MV_ADD_COMMAND(create_translation_matrix);
	MV_ADD_COMMAND(create_scale_matrix);
	MV_ADD_COMMAND(create_lookat_matrix);
	MV_ADD_COMMAND(create_perspective_matrix);
	MV_ADD_COMMAND(create_orthographic_matrix);
	MV_ADD_COMMAND(create_fps_matrix);

	// fonts
	MV_ADD_COMMAND(bind_font);
	MV_ADD_COMMAND(get_text_size);
	MV_ADD_COMMAND(get_global_font_scale);
	MV_ADD_COMMAND(set_global_font_scale);

	// node editor
	MV_ADD_COMMAND(get_selected_nodes);
	MV_ADD_COMMAND(get_selected_links);
	MV_ADD_COMMAND(clear_selected_nodes);
	MV_ADD_COMMAND(clear_selected_links);

	// plots
	MV_ADD_COMMAND(is_plot_queried);
	MV_ADD_COMMAND(get_plot_query_area);

	// plot axes
	MV_ADD_COMMAND(reset_axis_ticks);
	MV_ADD_COMMAND(set_axis_ticks);
	MV_ADD_COMMAND(set_axis_limits);
	MV_ADD_COMMAND(set_axis_limits_auto);
	MV_ADD_COMMAND(get_axis_limits);
	MV_ADD_COMMAND(fit_axis_data);

	// tables
	MV_ADD_COMMAND(highlight_table_column);
	MV_ADD_COMMAND(unhighlight_table_column);
	MV_ADD_COMMAND(set_table_row_color);
	MV_ADD_COMMAND(unset_table_row_color);
	MV_ADD_COMMAND(highlight_table_cell);
	MV_ADD_COMMAND(unhighlight_table_cell);
	MV_ADD_COMMAND(highlight_table_row);
	MV_ADD_COMMAND(unhighlight_table_row);
	MV_ADD_COMMAND(is_table_column_highlighted);
	MV_ADD_COMMAND(is_table_row_highlighted);
	MV_ADD_COMMAND(is_table_cell_highlighted);

	// themes
	MV_ADD_COMMAND(bind_theme);

	// tools
	MV_ADD_COMMAND(show_tool);

	// callback registry
	MV_ADD_COMMAND(set_frame_callback);
	MV_ADD_COMMAND(set_exit_callback);
	MV_ADD_COMMAND(set_viewport_resize_callback);

	// viewport
	MV_ADD_COMMAND(show_viewport);
	MV_ADD_COMMAND(create_viewport);
	MV_ADD_COMMAND(configure_viewport);
	MV_ADD_COMMAND(maximize_viewport);
	MV_ADD_COMMAND(minimize_viewport);
	MV_ADD_COMMAND(get_viewport_configuration);
	MV_ADD_COMMAND(is_viewport_ok);
	MV_ADD_COMMAND(toggle_viewport_fullscreen);

	// context
	MV_ADD_COMMAND(create_context);
	MV_ADD_COMMAND(destroy_context);
	MV_ADD_COMMAND(lock_mutex);
	MV_ADD_COMMAND(unlock_mutex);
	MV_ADD_COMMAND(setup_dearpygui);
	MV_ADD_COMMAND(render_dearpygui_frame);
	MV_ADD_COMMAND(get_delta_time);
	MV_ADD_COMMAND(get_total_time);
	MV_ADD_COMMAND(stop_dearpygui);
	MV_ADD_COMMAND(is_dearpygui_running);
	MV_ADD_COMMAND(generate_uuid);
	MV_ADD_COMMAND(save_init_file);
	MV_ADD_COMMAND(output_frame_buffer);
	MV_ADD_COMMAND(load_image);
	MV_ADD_COMMAND(save_image);
	MV_ADD_COMMAND(split_frame);
	MV_ADD_COMMAND(get_frame_count);
	MV_ADD_COMMAND(get_frame_rate);
	MV_ADD_COMMAND(get_app_configuration);
	MV_ADD_COMMAND(configure_app);
	MV_ADD_COMMAND(get_drawing_mouse_pos);
	MV_ADD_COMMAND(is_mouse_button_dragging);
	MV_ADD_COMMAND(is_mouse_button_down);
	MV_ADD_COMMAND(is_mouse_button_clicked);
	MV_ADD_COMMAND(is_mouse_button_double_clicked);
	MV_ADD_COMMAND(is_mouse_button_released);
	MV_ADD_COMMAND(get_mouse_drag_delta);
	MV_ADD_COMMAND(get_mouse_pos);
	MV_ADD_COMMAND(get_plot_mouse_pos);
	MV_ADD_COMMAND(is_key_pressed);
	MV_ADD_COMMAND(is_key_released);
	MV_ADD_COMMAND(is_key_down);
	MV_ADD_COMMAND(get_callback_queue);
	MV_ADD_COMMAND(set_clipboard_text);
	MV_ADD_COMMAND(get_clipboard_text);

	// item registry
	MV_ADD_COMMAND(focus_item);
	MV_ADD_COMMAND(get_aliases);
	MV_ADD_COMMAND(add_alias);
	MV_ADD_COMMAND(remove_alias);
	MV_ADD_COMMAND(does_alias_exist);
	MV_ADD_COMMAND(get_alias_id);
	MV_ADD_COMMAND(move_item);
	MV_ADD_COMMAND(delete_item);
	MV_ADD_COMMAND(does_item_exist);
	MV_ADD_COMMAND(move_item_down);
	MV_ADD_COMMAND(move_item_up);
	MV_ADD_COMMAND(get_windows);
	MV_ADD_COMMAND(get_all_items);
	MV_ADD_COMMAND(get_active_window);
	MV_ADD_COMMAND(set_primary_window);
	MV_ADD_COMMAND(push_container_stack);
	MV_ADD_COMMAND(pop_container_stack);
	MV_ADD_COMMAND(top_container_stack);
	MV_ADD_COMMAND(empty_container_stack);
	MV_ADD_COMMAND(last_item);
	MV_ADD_COMMAND(last_container);
	MV_ADD_COMMAND(last_root);
	MV_ADD_COMMAND(unstage);
	MV_ADD_COMMAND(reorder_items);
	MV_ADD_COMMAND(show_imgui_demo);
	MV_ADD_COMMAND(show_implot_demo);
	MV_ADD_COMMAND(show_item_debug);
	MV_ADD_COMMAND(get_item_info);
	MV_ADD_COMMAND(set_item_alias);
	MV_ADD_COMMAND(get_item_alias);
	MV_ADD_COMMAND(get_item_types);
	MV_ADD_COMMAND(get_item_configuration);
	MV_ADD_COMMAND(get_item_state);
	MV_ADD_COMMAND(configure_item);
	MV_ADD_COMMAND(get_value);
	MV_ADD_COMMAND(get_values);
	MV_ADD_COMMAND(set_value);
	MV_ADD_COMMAND(reset_pos);
	MV_ADD_COMMAND(set_item_children);
	MV_ADD_COMMAND(bind_item_handler_registry);
	MV_ADD_COMMAND(bind_item_font);
	MV_ADD_COMMAND(bind_item_theme);
	MV_ADD_COMMAND(capture_next_item);
	MV_ADD_COMMAND(get_platform);

	methods.push_back({ NULL, NULL, 0, NULL });

	static PyModuleDef dearpyguiModule = {
		PyModuleDef_HEAD_INIT, "_dearpygui", NULL, -1, methods.data(),
		NULL, NULL, NULL, NULL
	};

	PyObject* m;
	PymvBufferType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&PymvBufferType) < 0)
		return NULL;

	PymvVec4Type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&PymvVec4Type) < 0)
		return NULL;

	PymvMat4Type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&PymvMat4Type) < 0)
		return NULL;

	m = PyModule_Create(&dearpyguiModule);
	if (m == NULL)
		return NULL;

	const auto& constants = GetModuleConstants();

	// handled in the stub file
	for (auto& item : constants)
		PyModule_AddIntConstant(m, item.first.c_str(), item.second);

	auto MarvelError = PyErr_NewException("dearpygui.error", NULL, NULL);
	Py_XINCREF(MarvelError);
	if (PyModule_AddObject(m, "error", MarvelError) < 0) {
		Py_XDECREF(MarvelError);
		Py_CLEAR(MarvelError);
		Py_DECREF(m);
		return NULL;
	}

	Py_INCREF(&PymvBufferType);
	PyModule_AddObject(m, "mvBuffer", (PyObject*)&PymvBufferType);

	Py_INCREF(&PymvVec4Type);
	PyModule_AddObject(m, "mvVec4", (PyObject*)&PymvVec4Type);

	Py_INCREF(&PymvMat4Type);
	PyModule_AddObject(m, "mvMat4", (PyObject*)&PymvMat4Type);

	return m;
}