#include "mvModule_DearPyGui.h"
#include "mvAppItemCommons.h"
#include "mvViewportOps.h"
#include "mvFontManager.h"
#include "mvItemRegistry.h"
#include <ImGuiFileDialog.h>
#include <cstdlib>
#include "mvToolManager.h"
#include "mvBuffer.h"
#include "mvVec4Type.h"
#include "mvMat4Type.h"

namespace Marvel {

	mv_internal PyMethodDef* 
	GetMethods()
	{

		mv_local_persist std::vector<PyMethodDef> methods;
		methods.clear();

		#define X(el) methods.push_back(el::GetMethodDefinition()); el::FillExtraCommands(methods);
		MV_ITEM_TYPES
		#undef X

		mvToolManager::FillExtraCommands(methods);
		mvFontManager::FillExtraCommands(methods);

		mvCallbackRegCommands(methods);
		mvViewportCommands(methods);
		mvContextCommands(methods);
		mvItemRegistryCommands(methods);

		methods.push_back({ NULL, NULL, 0, NULL });

		return methods.data();
	}

	const std::map<std::string, mvPythonParser>& 
	GetModuleParsers()
	{

		mv_local_persist auto parsers = std::map<std::string, mvPythonParser>();

		if (parsers.empty())
		{
			#define X(el) el::InsertParser(&parsers);
			MV_ITEM_TYPES
			#undef X

			mvToolManager::InsertParser(&parsers);
			mvFontManager::InsertParser(&parsers);

			InsertParser_mvCallbackRegistry(&parsers);
			InsertParser_mvViewport(&parsers);
			InsertParser_mvContext(&parsers);
			InsertParser_mvItemRegistry(&parsers);
		}
		return parsers;
	}

	const std::vector<std::pair<std::string, long>>&
	GetModuleConstants()
	{
		mv_local_persist bool First_Run = true;
		mv_local_persist std::vector<std::pair<std::string, long>> ModuleConstants;

		if (First_Run)
		{
			InsertConstants_mvContext(ModuleConstants);
			mvToolManager::InsertConstants(ModuleConstants);

			ModuleConstants.push_back({ "mvAll", 0L });
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

			ModuleConstants.push_back({ "mvNode_PinShape_Circle", imnodes::PinShape::PinShape_Circle });
			ModuleConstants.push_back({ "mvNode_PinShape_CircleFilled", imnodes::PinShape::PinShape_CircleFilled });
			ModuleConstants.push_back({ "mvNode_PinShape_Triangle", imnodes::PinShape::PinShape_Triangle });
			ModuleConstants.push_back({ "mvNode_PinShape_TriangleFilled", imnodes::PinShape::PinShape_TriangleFilled });
			ModuleConstants.push_back({ "mvNode_PinShape_Quad", imnodes::PinShape::PinShape_Quad });
			ModuleConstants.push_back({ "mvNode_PinShape_QuadFilled", imnodes::PinShape::PinShape_QuadFilled });

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
			ModuleConstants.push_back({ "mvNodeCol_NodeBackground", imnodes::ColorStyle::ColorStyle_NodeBackground });
			ModuleConstants.push_back({ "mvNodeCol_NodeBackgroundHovered", imnodes::ColorStyle::ColorStyle_NodeBackgroundHovered });
			ModuleConstants.push_back({ "mvNodeCol_NodeBackgroundSelected", imnodes::ColorStyle::ColorStyle_NodeBackgroundSelected });
			ModuleConstants.push_back({ "mvNodeCol_NodeOutline", imnodes::ColorStyle::ColorStyle_NodeOutline });
			ModuleConstants.push_back({ "mvNodeCol_TitleBar", imnodes::ColorStyle::ColorStyle_TitleBar });
			ModuleConstants.push_back({ "mvNodeCol_TitleBarHovered", imnodes::ColorStyle::ColorStyle_TitleBarHovered });
			ModuleConstants.push_back({ "mvNodeCol_TitleBarSelected", imnodes::ColorStyle::ColorStyle_TitleBarSelected });
			ModuleConstants.push_back({ "mvNodeCol_Link", imnodes::ColorStyle::ColorStyle_Link });
			ModuleConstants.push_back({ "mvNodeCol_LinkHovered", imnodes::ColorStyle::ColorStyle_LinkHovered });
			ModuleConstants.push_back({ "mvNodeCol_LinkSelected", imnodes::ColorStyle::ColorStyle_LinkSelected });
			ModuleConstants.push_back({ "mvNodeCol_Pin", imnodes::ColorStyle::ColorStyle_Pin });
			ModuleConstants.push_back({ "mvNodeCol_PinHovered", imnodes::ColorStyle::ColorStyle_PinHovered });
			ModuleConstants.push_back({ "mvNodeCol_BoxSelector", imnodes::ColorStyle::ColorStyle_BoxSelector });
			ModuleConstants.push_back({ "mvNodeCol_BoxSelectorOutline", imnodes::ColorStyle::ColorStyle_BoxSelectorOutline });
			ModuleConstants.push_back({ "mvNodeCol_GridBackground", imnodes::ColorStyle::ColorStyle_GridBackground });
			ModuleConstants.push_back({ "mvNodeCol_GridLine", imnodes::ColorStyle::ColorStyle_GridLine });

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
			ModuleConstants.push_back({ "mvNodeStyleVar_GridSpacing", imnodes::StyleVar::StyleVar_GridSpacing });
			ModuleConstants.push_back({ "mvNodeStyleVar_NodeCornerRounding", imnodes::StyleVar::StyleVar_NodeCornerRounding });
			ModuleConstants.push_back({ "mvNodeStyleVar_NodePaddingHorizontal", imnodes::StyleVar::StyleVar_NodePaddingHorizontal });
			ModuleConstants.push_back({ "mvNodeStyleVar_NodePaddingVertical", imnodes::StyleVar::StyleVar_NodePaddingVertical });
			ModuleConstants.push_back({ "mvNodeStyleVar_NodeBorderThickness", imnodes::StyleVar::StyleVar_NodeBorderThickness });
			ModuleConstants.push_back({ "mvNodeStyleVar_LinkThickness", imnodes::StyleVar::StyleVar_LinkThickness });
			ModuleConstants.push_back({ "mvNodeStyleVar_LinkLineSegmentsPerLength", imnodes::StyleVar::StyleVar_LinkLineSegmentsPerLength });
			ModuleConstants.push_back({ "mvNodeStyleVar_LinkHoverDistance", imnodes::StyleVar::StyleVar_LinkHoverDistance });
			ModuleConstants.push_back({ "mvNodeStyleVar_PinCircleRadius", imnodes::StyleVar::StyleVar_PinCircleRadius });
			ModuleConstants.push_back({ "mvNodeStyleVar_PinQuadSideLength", imnodes::StyleVar::StyleVar_PinQuadSideLength });
			ModuleConstants.push_back({ "mvNodeStyleVar_PinTriangleSideLength", imnodes::StyleVar::StyleVar_PinTriangleSideLength });
			ModuleConstants.push_back({ "mvNodeStyleVar_PinLineThickness", imnodes::StyleVar::StyleVar_PinLineThickness });
			ModuleConstants.push_back({ "mvNodeStyleVar_PinHoverRadius", imnodes::StyleVar::StyleVar_PinHoverRadius });
			ModuleConstants.push_back({ "mvNodeStyleVar_PinOffset", imnodes::StyleVar::StyleVar_PinOffset });

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

	PyMODINIT_FUNC
	PyInit__dearpygui(void)
	{
		mv_local_persist PyModuleDef dearpyguiModule = {
			PyModuleDef_HEAD_INIT, "_dearpygui", NULL, -1, GetMethods(),
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
}