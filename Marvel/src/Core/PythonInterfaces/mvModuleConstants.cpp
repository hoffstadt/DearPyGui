#include "mvPythonModule.h"
#include "mvPythonTranslator.h"
#include "Core/mvApp.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"

namespace Marvel {

	std::vector<std::pair<std::string, long>> BuildConstantsInterface()
	{
		std::vector<std::pair<std::string, long>> ModuleConstants =
		{
			//-----------------------------------------------------------------------------
			// Plot Flags
			//-----------------------------------------------------------------------------
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

			//-----------------------------------------------------------------------------
			// Axis Flags
			//-----------------------------------------------------------------------------
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

			//-----------------------------------------------------------------------------
			// Plot Colors
			//-----------------------------------------------------------------------------
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

			//-----------------------------------------------------------------------------
			// Plot Marker Specifications
			//-----------------------------------------------------------------------------
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

			//-----------------------------------------------------------------------------
			// Built-in ImPlot Color maps
			//-----------------------------------------------------------------------------
			{"mvPlotColormap_Default",  0}, // ImPlot default colormap         (n=10)
			{"mvPlotColormap_Dark"   ,  1}, // a.k.a. matplotlib "Set1"        (n=9)
			{"mvPlotColormap_Pastel" ,  2}, // a.k.a. matplotlib "Pastel1"     (n=9)
			{"mvPlotColormap_Paired" ,  3}, // a.k.a. matplotlib "Paired"      (n=12)
			{"mvPlotColormap_Viridis",  4}, // a.k.a. matplotlib "viridis"     (n=11)
			{"mvPlotColormap_Plasma" ,  5}, // a.k.a. matplotlib "plasma"      (n=11)
			{"mvPlotColormap_Hot"    ,  6}, // a.k.a. matplotlib/MATLAB "hot"  (n=11)
			{"mvPlotColormap_Cool"   ,  7}, // a.k.a. matplotlib/MATLAB "cool" (n=11)
			{"mvPlotColormap_Pink"   ,  8}, // a.k.a. matplotlib/MATLAB "pink" (n=11)
			{"mvPlotColormap_Jet"    ,  9}, // a.k.a. MATLAB "jet"             (n=11)

			//-----------------------------------------------------------------------------
			// Theme Color IDs
			//-----------------------------------------------------------------------------
			{"mvGuiCol_Text"                 , 0},
			{"mvGuiCol_TextDisabled"         , 1},
			{"mvGuiCol_WindowBg"             , 2}, // Background of normal windows
			{"mvGuiCol_ChildBg"              , 3}, // Background of child windows
			{"mvGuiCol_PopupBg"              , 4}, // Background of popups, menus, tooltips windows
			{"mvGuiCol_Border"               , 5},
			{"mvGuiCol_BorderShadow"         , 6},
			{"mvGuiCol_FrameBg"              , 7},
			{"mvGuiCol_FrameBgHovered"       , 8},
			{"mvGuiCol_FrameBgActive"        , 9},
			{"mvGuiCol_TitleBg"              , 10},
			{"mvGuiCol_TitleBgActive"        , 11},
			{"mvGuiCol_TitleBgCollapsed"     , 12},
			{"mvGuiCol_MenuBarBg"            , 13},
			{"mvGuiCol_ScrollbarBg"          , 14},
			{"mvGuiCol_ScrollbarGrab"        , 15},
			{"mvGuiCol_ScrollbarGrabHovered" , 16},
			{"mvGuiCol_ScrollbarGrabActive"  , 17},
			{"mvGuiCol_CheckMark"            , 18},
			{"mvGuiCol_SliderGrab"           , 19},
			{"mvGuiCol_SliderGrabActive"     , 20},
			{"mvGuiCol_Button"               , 21},
			{"mvGuiCol_ButtonHovered"        , 22},
			{"mvGuiCol_ButtonActive"         , 23},
			{"mvGuiCol_Header"               , 24}, // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
			{"mvGuiCol_HeaderHovered"        , 25},
			{"mvGuiCol_HeaderActive"         , 26},
			{"mvGuiCol_Separator"            , 27},
			{"mvGuiCol_SeparatorHovered"     , 28},
			{"mvGuiCol_SeparatorActive"      , 29},
			{"mvGuiCol_ResizeGrip"           , 30},
			{"mvGuiCol_ResizeGripHovered"    , 31},
			{"mvGuiCol_ResizeGripActive"     , 32},
			{"mvGuiCol_Tab"                  , 33},
			{"mvGuiCol_TabHovered"           , 34},
			{"mvGuiCol_TabActive"            , 35},
			{"mvGuiCol_TabUnfocused"         , 36},
			{"mvGuiCol_TabUnfocusedActive"   , 37},
			{"mvGuiCol_PlotLines"            , 38},
			{"mvGuiCol_PlotLinesHovered"     , 39},
			{"mvGuiCol_PlotHistogram"        , 40},
			{"mvGuiCol_PlotHistogramHovered" , 41},
			{"mvGuiCol_TextSelectedBg"       , 42},
			{"mvGuiCol_DragDropTarget"       , 43},
			{"mvGuiCol_NavHighlight"         , 44}, // Gamepad/keyboard: current highlighted item
			{"mvGuiCol_NavWindowingHighlight", 45}, // Highlight window when using CTRL+TAB
			{"mvGuiCol_NavWindowingDimBg"    , 46}, // Darken/colorize entire screen behind the CTRL+TAB window list, when active 
			{"mvGuiCol_ModalWindowDimBg"     , 47}, // Darken/colorize entire screen behind a modal window, when one is active

			//-----------------------------------------------------------------------------
			// Style IDs
			//-----------------------------------------------------------------------------
			// Enum name --------------------- // Member in mvGuiStyle structure (see GuiStyle for descriptions)
			{ "mvGuiStyleVar_Alpha",               0}, // float     Alpha
			{ "mvGuiStyleVar_WindowPadding",       1}, // mvVec2    WindowPadding
			{ "mvGuiStyleVar_WindowRounding",      2}, // float     WindowRounding
			{ "mvGuiStyleVar_WindowBorderSize",    3}, // float     WindowBorderSize
			{ "mvGuiStyleVar_WindowMinSize",       4}, // mvVec2    WindowMinSize
			{ "mvGuiStyleVar_WindowTitleAlign",    5}, // mvVec2    WindowTitleAlign
			{ "mvGuiStyleVar_ChildRounding",       6}, // float     ChildRounding
			{ "mvGuiStyleVar_ChildBorderSize",     7}, // float     ChildBorderSize
			{ "mvGuiStyleVar_PopupRounding",       8}, // float     PopupRounding
			{ "mvGuiStyleVar_PopupBorderSize",     9}, // float     PopupBorderSize
			{ "mvGuiStyleVar_FramePadding",        10}, // mvVec2    FramePadding
			{ "mvGuiStyleVar_FrameRounding",       11}, // float     FrameRounding
			{ "mvGuiStyleVar_FrameBorderSize",     12}, // float     FrameBorderSize
			{ "mvGuiStyleVar_ItemSpacing",         13}, // mvVec2    ItemSpacing
			{ "mvGuiStyleVar_ItemInnerSpacing",    14}, // mvVec2    ItemInnerSpacing
			{ "mvGuiStyleVar_IndentSpacing",       15}, // float     IndentSpacing
			{ "mvGuiStyleVar_ScrollbarSize",       16}, // float     ScrollbarSize
			{ "mvGuiStyleVar_ScrollbarRounding",   17}, // float     ScrollbarRounding
			{ "mvGuiStyleVar_GrabMinSize",         18}, // float     GrabMinSize
			{ "mvGuiStyleVar_GrabRounding",        19}, // float     GrabRounding
			{ "mvGuiStyleVar_TabRounding",         20}, // float     TabRounding
			{ "mvGuiStyleVar_ButtonTextAlign",     21}, // mvVec2    ButtonTextAlign
			{ "mvGuiStyleVar_SelectableTextAlign", 22}, // mvVec2    SelectableTextAlign

			{ "ImGuiStyleVar_TouchExtraPadding", 23}, // mvVec2    SelectableTextAlign
			{ "ImGuiStyleVar_TabBorderSize", 24}, // mvVec2    SelectableTextAlign
			{ "ImGuiStyleVar_DisplaySafeAreaPadding", 25}, // mvVec2    SelectableTextAlign
			{ "ImGuiStyleVar_AntiAliasedLines", 26}, // mvVec2    SelectableTextAlign
			{ "ImGuiStyleVar_AntiAliasedFill", 27}, // mvVec2    SelectableTextAlign
			{ "ImGuiStyleVar_CurveTessellationTol", 28}, // mvVec2    SelectableTextAlign
			{ "ImGuiStyleVar_CircleSegmentMaxError", 29}, // mvVec2    SelectableTextAlign

			//-----------------------------------------------------------------------------
			// Key Codes
			//-----------------------------------------------------------------------------
			{ "mvKey_Back"			, 0x08 },
			{ "mvKey_Tab"			, 0x09 },
			{ "mvKey_Clear"			, 0x0C },
			{ "mvKey_Return"		, 0x0D },
			{ "mvKey_Shift"			, 0x10 },
			{ "mvKey_Control"		, 0x11 },
			{ "mvKey_Alt"			, 0x12 },
			{ "mvKey_Pause"			, 0x13 },
			{ "mvKey_Capital"		, 0x14 },
			{ "mvKey_Escape"		, 0x1B },
			{ "mvKey_Spacebar"		, 0x20 },
			{ "mvKey_Prior"			, 0x21 },
			{ "mvKey_Next"			, 0x22 },
			{ "mvKey_End"			, 0x23 },
			{ "mvKey_Home"			, 0x24 },
			{ "mvKey_Left"			, 0x25 },
			{ "mvKey_Up"			, 0x26 },
			{ "mvKey_Right"			, 0x27 },
			{ "mvKey_Down"			, 0x28 },
			{ "mvKey_Select"		, 0x29 },
			{ "mvKey_Print"			, 0x2A },
			{ "mvKey_Execute"		, 0x2B },
			{ "mvKey_PrintScreen"	, 0x2C },
			{ "mvKey_Insert"		, 0x2D },
			{ "mvKey_Delete"		, 0x2E },
			{ "mvKey_Help"			, 0x2F },
			{ "mvKey_0"			, 0x30 },
			{ "mvKey_1"			, 0x31 },
			{ "mvKey_2"			, 0x32 },
			{ "mvKey_3"			, 0x33 },
			{ "mvKey_4"			, 0x34 },
			{ "mvKey_5"			, 0x35 },
			{ "mvKey_6"			, 0x36 },
			{ "mvKey_7"			, 0x37 },
			{ "mvKey_8"			, 0x38 },
			{ "mvKey_9"			, 0x39 },
			{ "mvKey_A"			, 0x41 },
			{ "mvKey_B"			, 0x42 },
			{ "mvKey_C"			, 0x43 },
			{ "mvKey_D"			, 0x44 },
			{ "mvKey_E"			, 0x45 },
			{ "mvKey_F"			, 0x46 },
			{ "mvKey_G"			, 0x47 },
			{ "mvKey_H"			, 0x48 },
			{ "mvKey_I"			, 0x49 },
			{ "mvKey_J"			, 0x4A },
			{ "mvKey_K"			, 0x4B },
			{ "mvKey_L"			, 0x4C },
			{ "mvKey_M"			, 0x4D },
			{ "mvKey_N"			, 0x4E },
			{ "mvKey_O"			, 0x4F },
			{ "mvKey_P"			, 0x50 },
			{ "mvKey_Q"			, 0x51 },
			{ "mvKey_R"			, 0x52 },
			{ "mvKey_S"			, 0x53 },
			{ "mvKey_T"			, 0x54 },
			{ "mvKey_U"			, 0x55 },
			{ "mvKey_V"			, 0x56 },
			{ "mvKey_W"			, 0x57 },
			{ "mvKey_X"			, 0x58 },
			{ "mvKey_Y"			, 0x59 },
			{ "mvKey_Z"			, 0x5A },
			{ "mvKey_LWin"		, 0x5B },
			{ "mvKey_RWin"		, 0x5C },
			{ "mvKey_Apps"		, 0x5D },
			{ "mvKey_Sleep"		, 0x5F },
			{ "mvKey_NumPad0"   , 0x60 },
			{ "mvKey_NumPad1"   , 0x61 },
			{ "mvKey_NumPad2"   , 0x62 },
			{ "mvKey_NumPad3"   , 0x63 },
			{ "mvKey_NumPad4"   , 0x64 },
			{ "mvKey_NumPad5"   , 0x65 },
			{ "mvKey_NumPad6"	, 0x66 },
			{ "mvKey_NumPad7"	, 0x67 },
			{ "mvKey_NumPad8"	, 0x68 },
			{ "mvKey_NumPad9"	, 0x69 },
			{ "mvKey_Multiply"	, 0x6A },
			{ "mvKey_Add"		, 0x6B },
			{ "mvKey_Separator"	, 0x6C },
			{ "mvKey_Subtract"	, 0x6D },
			{ "mvKey_Decimal"	, 0x6E },
			{ "mvKey_Divide"	, 0x6F },
			{ "mvKey_F1"		, 0x70 },
			{ "mvKey_F2"		, 0x71 },
			{ "mvKey_F3"		, 0x72 },
			{ "mvKey_F4"		, 0x73 },
			{ "mvKey_F5"		, 0x74 },
			{ "mvKey_F6"		, 0x75 },
			{ "mvKey_F7"		, 0x76 },
			{ "mvKey_F8"		, 0x77 },
			{ "mvKey_F9"		, 0x78 },
			{ "mvKey_F10"		, 0x79 },
			{ "mvKey_F11"		, 0x7A },
			{ "mvKey_F12"		, 0x7B },
			{ "mvKey_F13"		, 0x7C },
			{ "mvKey_F14"		, 0x7D },
			{ "mvKey_F15"		, 0x7E },
			{ "mvKey_F16"		, 0x7F },
			{ "mvKey_F17"		, 0x80 },
			{ "mvKey_F18"		, 0x81 },
			{ "mvKey_F19"		, 0x82 },
			{ "mvKey_F20"		, 0x83 },
			{ "mvKey_F21"		, 0x84 },
			{ "mvKey_F22"		, 0x85 },
			{ "mvKey_F23"		, 0x86 },
			{ "mvKey_F24"		, 0x87 },
			{ "mvKey_NumLock"				, 0x90 },
			{ "mvKey_ScrollLock"			, 0x91 },
			{ "mvKey_LShift"				, 0xA0 },
			{ "mvKey_RShift"				, 0xA1 },
			{ "mvKey_LControl"				, 0xA2 },
			{ "mvKey_RControl"				, 0xA3 },
			{ "mvKey_LMenu"					, 0xA4 },
			{ "mvKey_RMenu"					, 0xA5 },
			{ "mvKey_Browser_Back"			, 0xA6 },
			{ "mvKey_Browser_Forward"		, 0xA7 },
			{ "mvKey_Browser_Refresh"		, 0xA8 },
			{ "mvKey_Browser_Stop"			, 0xA9 },
			{ "mvKey_Browser_Search"		, 0xAA },
			{ "mvKey_Browser_Favorites"		, 0xAB },
			{ "mvKey_Browser_Home"			, 0xAC },
			{ "mvKey_Volume_Mute"			, 0xAD },
			{ "mvKey_Volume_Down"			, 0xAE },
			{ "mvKey_Volume_Up"				, 0xAF },
			{ "mvKey_Media_Next_Track"		, 0xB0 },
			{ "mvKey_Media_Prev_Track"		, 0xB1 },
			{ "mvKey_Media_Stop"			, 0xB2 },
			{ "mvKey_Media_Play_Pause"		, 0xB3 },
			{ "mvKey_Launch_Mail"			, 0xB4 },
			{ "mvKey_Launch_Media_Select"	, 0xB5 },
			{ "mvKey_Launch_App1"			, 0xB6 },
			{ "mvKey_Launch_App2"			, 0xB7 },
			{ "mvKey_Colon"					, 0xBA },
			{ "mvKey_Plus"					, 0xBB },
			{ "mvKey_Comma"					, 0xBC },
			{ "mvKey_Minus"					, 0xBD },
			{ "mvKey_Period"				, 0xBE },
			{ "mvKey_Slash"					, 0xBF },
			{ "mvKey_Tilde"					, 0xC0 },
			{ "mvKey_Open_Brace"			, 0xDB },
			{ "mvKey_Backslash"				, 0xDC },
			{ "mvKey_Close_Brace"			, 0xDD },
			{ "mvKey_Quote"					, 0xDE },



			//-----------------------------------------------------------------------------
			// Mouse Codes
			//-----------------------------------------------------------------------------
			{"mvMouseButton_Left"  , 0},
			{"mvMouseButton_Right" , 1},
			{"mvMouseButton_Middle", 2},
			{"mvMouseButton_X1", 3},
			{"mvMouseButton_X2", 4},

			//-----------------------------------------------------------------------------
			// Cardinal directions
			//-----------------------------------------------------------------------------
			{ "mvDir_None" ,-1 },
			{ "mvDir_Left" , 0 },
			{ "mvDir_Right", 1 },
			{ "mvDir_Up"   , 2 },
			{ "mvDir_Down" , 3 },

			//-----------------------------------------------------------------------------
			// Log Levels
			//-----------------------------------------------------------------------------
			{ "mvTRACE"   , 0 },
			{ "mvDEBUG"   , 1 },
			{ "mvINFO"    , 2 },
			{ "mvWARNING" , 3 },
			{ "mvERROR"   , 4 },
			{ "mvOFF"     , 5 }
		};

		return ModuleConstants;
	}


	mvPythonModule* CreateConstantsInterface()
	{

		auto pyModule = new mvPythonModule("sbConstants", BuildConstantsInterface());

		return pyModule;
	}
}