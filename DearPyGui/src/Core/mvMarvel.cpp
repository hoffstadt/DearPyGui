#include "mvMarvel.h"
#include "mvApp.h"
#include "Core/mvInput.h"
#include "Core/mvDataStorage.h"
#include "mvAppLog.h"
#include "Core/StandardWindows/mvSourceWindow.h"
#include "Core/StandardWindows/mvFileDialog.h"
#include "mvPythonTranslator.h"
#include "Core/mvAppItems.h"
#include "mvWindow.h"
#include "Core/mvPythonExceptions.h"
#include <ImGuiFileDialog.h>
#include <cstdlib>

// new includes
#include "Core/PythonCommands/mvInterfaceCore.h"
#include "Core/PythonCommands/mvPlotInterface.h"
#include "Core/PythonCommands/mvDrawingInterface.h"
#include "Core/PythonCommands/mvTableInterface.h"
#include "Core/PythonCommands/mvThemeInterface.h"
#include "Core/PythonCommands/mvInputInterface.h"
#include "Core/PythonCommands/mvWidgetInterface.h"
#include "Core/PythonCommands/mvAppItemInterface.h"
#include "Core/PythonCommands/mvAppInterface.h"

//-----------------------------------------------------------------------------
// Helper Macro
//-----------------------------------------------------------------------------
#define ADD_PYTHON_FUNCTION(Function) { #Function, (PyCFunction)Function, METH_VARARGS | METH_KEYWORDS, (*mvApp::GetApp()->getParsers())[#Function].getDocumentation() },

namespace Marvel {

	std::map<std::string, mvPythonParser>* BuildDearPyGuiInterface()
	{

		std::map<std::string, mvPythonParser>* parsers = new std::map< std::string, mvPythonParser>;

		AddDrawingCommands(parsers);
		AddPlotCommands(parsers);
		AddLogCommands(parsers);
		AddInputCommands(parsers);
		AddInputWidgets(parsers);
		AddSliderWidgets(parsers);
		AddDragWidgets(parsers);
		AddTableCommands(parsers);
		AddItemCommands(parsers);
		AddStdWindowCommands(parsers);
		AddMenuCommands(parsers);
		AddThemeCommands(parsers);
		AddBasicWidgets(parsers);
		AddContainerWidgets(parsers);
		AddAppCommands(parsers);
		return parsers;
	}

	std::vector<std::pair<std::string, std::string>> GetAllCommands()
	{
		auto mapping = BuildDearPyGuiInterface();

		std::vector<std::pair<std::string, std::string>> result;

		for (const auto& item : *mapping)
			result.emplace_back(item.first, item.second.getDocumentation());

		delete mapping;

		return result;
	}

	std::vector<std::pair<std::string, long>> GetModuleConstants()
	{
		std::vector<std::pair<std::string, long>> ModuleConstants =
		{

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
			// Key Codes
			//-----------------------------------------------------------------------------
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

#if defined (_WIN32)
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
#else
			{ "mvKey_Back",  259 },
			{"mvKey_Tab",  258 },
			{"mvKey_Clear",  259 },
			{"mvKey_Return",  257 },
			{"mvKey_Shift",  340 },
			{"mvKey_Control",  241 },
			{"mvKey_Alt",  342 },
			{"mvKey_Pause",  284 },
			{"mvKey_Capital",  280 },
			{"mvKey_Escape",  256 },
			{"mvKey_Spacebar",  32 },
			{"mvKey_Prior",  -1 },
			{"mvKey_Next",  -1 },
			{"mvKey_End",  269 },
			{"mvKey_Home",  268 },
			{"mvKey_Left",  263 },
			{"mvKey_Up",  265 },
			{"mvKey_Right",  262 },
			{"mvKey_Down",  264 },
			{"mvKey_Select",  -1 },
			{"mvKey_Print",  -1 },
			{"mvKey_Execute",  -1 },
			{"mvKey_PrintScreen",  286 },
			{"mvKey_Insert",  260 },
			{"mvKey_Delete",  261 },
			{"mvKey_Help",  -1 },
			{"mvKey_LWin",  343 },
			{"mvKey_RWin",  347 },
			{"mvKey_Apps",  -1 },
			{"mvKey_Sleep",  -1 },
			{"mvKey_NumPad0",  320 },
			{"mvKey_NumPad1",  321 },
			{"mvKey_NumPad2",  322 },
			{"mvKey_NumPad3",  323 },
			{"mvKey_NumPad4",  324 },
			{"mvKey_NumPad5",  325 },
			{"mvKey_NumPad6",  326 },
			{"mvKey_NumPad7",  327 },
			{"mvKey_NumPad8",  328 },
			{"mvKey_NumPad9",  329 },
			{"mvKey_Multiply",  332 },
			{"mvKey_Add",  334 },
			{"mvKey_Separator",  -1 },
			{"mvKey_Subtract",  333 },
			{"mvKey_Decimal",  330 },
			{"mvKey_Divide",  331 },
			{"mvKey_F1",  290 },
			{"mvKey_F2",  291 },
			{"mvKey_F3",  292 },
			{"mvKey_F4",  293 },
			{"mvKey_F5",  294 },
			{"mvKey_F6",  295 },
			{"mvKey_F7",  296 },
			{"mvKey_F8",  297 },
			{"mvKey_F9",  298 },
			{"mvKey_F10",  299 },
			{"mvKey_F11",  300 },
			{"mvKey_F12",  301 },
			{"mvKey_F13",  302 },
			{"mvKey_F14",  303 },
			{"mvKey_F15",  304 },
			{"mvKey_F16",  305 },
			{"mvKey_F17",  306 },
			{"mvKey_F18",  307 },
			{"mvKey_F19",  308 },
			{"mvKey_F20",  309 },
			{"mvKey_F21",  310 },
			{"mvKey_F22",  311 },
			{"mvKey_F23",  312 },
			{"mvKey_F24",  313 },
			{"mvKey_F24",  314 },
			{"mvKey_NumLock",  282 },
			{"mvKey_ScrollLock",  281 },
			{"mvKey_LShift",  340 },
			{"mvKey_RShift",  344 },
			{"mvKey_LControl",  341 },
			{"mvKey_RControl",  345 },
			{"mvKey_LMenu",  -1 },
			{"mvKey_RMenu",  -1 },
			{"mvKey_Browser_Back",  -1 },
			{"mvKey_Browser_Forward",  -1 },
			{"mvKey_Browser_Refresh",  -1 },
			{"mvKey_Browser_Stop",  -1 },
			{"mvKey_Browser_Search",  -1 },
			{"mvKey_Browser_Favorites",  -1 },
			{"mvKey_Browser_Home",  -1 },
			{"mvKey_Volume_Mute",  -1 },
			{"mvKey_Volume_Down",  -1 },
			{"mvKey_Volume_Up",  -1 },
			{"mvKey_Media_Next_Track",  -1 },
			{"mvKey_Media_Prev_Track",  -1 },
			{"mvKey_Media_Stop",  -1 },
			{"mvKey_Media_Play_Pause",  -1 },
			{"mvKey_Launch_Mail",  -1 },
			{"mvKey_Launch_Media_Select",  -1 },
			{"mvKey_Launch_App1",  -1 },
			{"mvKey_Launch_App2",  -1 },
			{"mvKey_Colon",  59 },
			{"mvKey_Plus",  61 },
			{"mvKey_Comma",  44 },
			{"mvKey_Minus",  45 },
			{"mvKey_Period",  46 },
			{"mvKey_Slash",  47 },
			{"mvKey_Tilde",  96 },
			{"mvKey_Open_Brace",  91 },
			{"mvKey_Backslash",  92 },
			{"mvKey_Close_Brace",  93 },
			{"mvKey_Quote",  39 },
#endif



			//-----------------------------------------------------------------------------
			// Mouse Codes
			//-----------------------------------------------------------------------------
			{ "mvMouseButton_Left"  , 0 },
			{ "mvMouseButton_Right" , 1 },
			{ "mvMouseButton_Middle", 2 },
			{ "mvMouseButton_X1", 3 },
			{ "mvMouseButton_X2", 4 },

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

	static PyMethodDef dearpyguimethods[]
	{
		ADD_PYTHON_FUNCTION(get_item_configuration)
		ADD_PYTHON_FUNCTION(configure_item)

		// app interface
		ADD_PYTHON_FUNCTION(set_threadpool_timeout)
		ADD_PYTHON_FUNCTION(set_thread_count)
		ADD_PYTHON_FUNCTION(set_threadpool_high_performance)
		ADD_PYTHON_FUNCTION(set_main_window_size)
		ADD_PYTHON_FUNCTION(get_thread_count)
		ADD_PYTHON_FUNCTION(is_threadpool_high_performance)
		ADD_PYTHON_FUNCTION(get_threadpool_timeout)
		ADD_PYTHON_FUNCTION(get_active_window)
		ADD_PYTHON_FUNCTION(get_dearpygui_version)
		ADD_PYTHON_FUNCTION(get_main_window_size)
		ADD_PYTHON_FUNCTION(setup_dearpygui)
		ADD_PYTHON_FUNCTION(render_dearpygui_frame)
		ADD_PYTHON_FUNCTION(cleanup_dearpygui)
		ADD_PYTHON_FUNCTION(start_dearpygui)
		ADD_PYTHON_FUNCTION(get_global_font_scale)
		ADD_PYTHON_FUNCTION(set_global_font_scale)
		ADD_PYTHON_FUNCTION(select_directory_dialog)
		ADD_PYTHON_FUNCTION(get_delta_time)
		ADD_PYTHON_FUNCTION(get_total_time)
		ADD_PYTHON_FUNCTION(get_data)
		ADD_PYTHON_FUNCTION(delete_data)
		ADD_PYTHON_FUNCTION(add_data)
		ADD_PYTHON_FUNCTION(run_async_function)
		ADD_PYTHON_FUNCTION(open_file_dialog)
		ADD_PYTHON_FUNCTION(set_exit_callback)
		ADD_PYTHON_FUNCTION(set_vsync)
		ADD_PYTHON_FUNCTION(stop_dearpygui)
		ADD_PYTHON_FUNCTION(is_dearpygui_running)
		ADD_PYTHON_FUNCTION(set_main_window_title)
		ADD_PYTHON_FUNCTION(set_main_window_pos)
		ADD_PYTHON_FUNCTION(set_main_window_resizable)
		ADD_PYTHON_FUNCTION(set_start_callback)
		ADD_PYTHON_FUNCTION(show_style_editor)
		ADD_PYTHON_FUNCTION(show_source)
		ADD_PYTHON_FUNCTION(show_about)
		ADD_PYTHON_FUNCTION(show_debug)
		ADD_PYTHON_FUNCTION(show_metrics)
		ADD_PYTHON_FUNCTION(close_popup)
		ADD_PYTHON_FUNCTION(show_documentation)
		ADD_PYTHON_FUNCTION(get_log_level)
		ADD_PYTHON_FUNCTION(clear_log)
		ADD_PYTHON_FUNCTION(show_logger)
		ADD_PYTHON_FUNCTION(set_log_level)
		ADD_PYTHON_FUNCTION(log)
		ADD_PYTHON_FUNCTION(log_debug)
		ADD_PYTHON_FUNCTION(log_info)
		ADD_PYTHON_FUNCTION(log_warning)
		ADD_PYTHON_FUNCTION(log_error)

		// app item interface
		ADD_PYTHON_FUNCTION(get_item_type)
		ADD_PYTHON_FUNCTION(set_item_callback)
		ADD_PYTHON_FUNCTION(get_value)
		ADD_PYTHON_FUNCTION(set_value)
		ADD_PYTHON_FUNCTION(is_item_hovered)
		ADD_PYTHON_FUNCTION(is_item_shown)
		ADD_PYTHON_FUNCTION(is_item_active)
		ADD_PYTHON_FUNCTION(is_item_focused)
		ADD_PYTHON_FUNCTION(is_item_clicked)
		ADD_PYTHON_FUNCTION(is_item_container)
		ADD_PYTHON_FUNCTION(is_item_visible)
		ADD_PYTHON_FUNCTION(is_item_edited)
		ADD_PYTHON_FUNCTION(is_item_activated)
		ADD_PYTHON_FUNCTION(is_item_deactivated)
		ADD_PYTHON_FUNCTION(is_item_deactivated_after_edit)
		ADD_PYTHON_FUNCTION(is_item_toggled_open)
		ADD_PYTHON_FUNCTION(get_item_rect_min)
		ADD_PYTHON_FUNCTION(get_item_rect_max)
		ADD_PYTHON_FUNCTION(get_item_rect_size)
		ADD_PYTHON_FUNCTION(get_item_callback)
		ADD_PYTHON_FUNCTION(get_item_parent)
		ADD_PYTHON_FUNCTION(delete_item)
		ADD_PYTHON_FUNCTION(does_item_exist)
		ADD_PYTHON_FUNCTION(move_item_down)
		ADD_PYTHON_FUNCTION(move_item_up)
		ADD_PYTHON_FUNCTION(get_windows)
		ADD_PYTHON_FUNCTION(get_all_items)
		ADD_PYTHON_FUNCTION(get_item_children)

		// widget commands
		ADD_PYTHON_FUNCTION(add_date_picker)
		ADD_PYTHON_FUNCTION(add_time_picker)
		ADD_PYTHON_FUNCTION(add_input_text)
		ADD_PYTHON_FUNCTION(add_input_int)
		ADD_PYTHON_FUNCTION(add_input_int2)
		ADD_PYTHON_FUNCTION(add_input_int3)
		ADD_PYTHON_FUNCTION(add_input_int4)
		ADD_PYTHON_FUNCTION(add_input_float)
		ADD_PYTHON_FUNCTION(add_input_float2)
		ADD_PYTHON_FUNCTION(add_input_float3)
		ADD_PYTHON_FUNCTION(add_input_float4)
		ADD_PYTHON_FUNCTION(end)
		ADD_PYTHON_FUNCTION(add_image)
		ADD_PYTHON_FUNCTION(add_image_button)
		ADD_PYTHON_FUNCTION(add_progress_bar)
		ADD_PYTHON_FUNCTION(add_drag_float)
		ADD_PYTHON_FUNCTION(add_drag_int)
		ADD_PYTHON_FUNCTION(add_drag_float2)
		ADD_PYTHON_FUNCTION(add_drag_float3)
		ADD_PYTHON_FUNCTION(add_drag_float4)
		ADD_PYTHON_FUNCTION(add_drag_int2)
		ADD_PYTHON_FUNCTION(add_drag_int3)
		ADD_PYTHON_FUNCTION(add_drag_int4)
		ADD_PYTHON_FUNCTION(add_slider_float)
		ADD_PYTHON_FUNCTION(add_slider_int)
		ADD_PYTHON_FUNCTION(add_slider_float2)
		ADD_PYTHON_FUNCTION(add_slider_float3)
		ADD_PYTHON_FUNCTION(add_slider_float4)
		ADD_PYTHON_FUNCTION(add_slider_int2)
		ADD_PYTHON_FUNCTION(add_slider_int3)
		ADD_PYTHON_FUNCTION(add_slider_int4)
		ADD_PYTHON_FUNCTION(add_tree_node)
		ADD_PYTHON_FUNCTION(add_selectable)
		ADD_PYTHON_FUNCTION(add_popup)
		ADD_PYTHON_FUNCTION(add_window)
		ADD_PYTHON_FUNCTION(add_indent)
		ADD_PYTHON_FUNCTION(unindent)
		ADD_PYTHON_FUNCTION(add_simple_plot)
		ADD_PYTHON_FUNCTION(add_combo)
		ADD_PYTHON_FUNCTION(add_text)
		ADD_PYTHON_FUNCTION(add_label_text)
		ADD_PYTHON_FUNCTION(add_listbox)
		ADD_PYTHON_FUNCTION(add_color_edit3)
		ADD_PYTHON_FUNCTION(add_color_edit4)
		ADD_PYTHON_FUNCTION(add_color_picker3)
		ADD_PYTHON_FUNCTION(add_color_picker4)
		ADD_PYTHON_FUNCTION(add_separator)
		ADD_PYTHON_FUNCTION(add_button)
		ADD_PYTHON_FUNCTION(add_radio_button)
		ADD_PYTHON_FUNCTION(add_checkbox)
		ADD_PYTHON_FUNCTION(add_group)
		ADD_PYTHON_FUNCTION(add_child)
		ADD_PYTHON_FUNCTION(add_tab_bar)
		ADD_PYTHON_FUNCTION(add_tab)
		ADD_PYTHON_FUNCTION(add_menu_bar)
		ADD_PYTHON_FUNCTION(add_menu)
		ADD_PYTHON_FUNCTION(add_menu_item)
		ADD_PYTHON_FUNCTION(add_spacing)
		ADD_PYTHON_FUNCTION(add_same_line)
		ADD_PYTHON_FUNCTION(add_tooltip)
		ADD_PYTHON_FUNCTION(add_collapsing_header)
		ADD_PYTHON_FUNCTION(add_dummy)

		// input commands
		ADD_PYTHON_FUNCTION(set_mouse_move_callback)
		ADD_PYTHON_FUNCTION(set_mouse_drag_callback)
		ADD_PYTHON_FUNCTION(is_mouse_button_dragging)
		ADD_PYTHON_FUNCTION(is_mouse_button_down)
		ADD_PYTHON_FUNCTION(is_mouse_button_clicked)
		ADD_PYTHON_FUNCTION(is_mouse_button_double_clicked)
		ADD_PYTHON_FUNCTION(is_mouse_button_released)
		ADD_PYTHON_FUNCTION(get_mouse_drag_delta)
		ADD_PYTHON_FUNCTION(set_mouse_wheel_callback)
		ADD_PYTHON_FUNCTION(get_mouse_pos)
		ADD_PYTHON_FUNCTION(get_plot_mouse_pos)
		ADD_PYTHON_FUNCTION(is_key_pressed)
		ADD_PYTHON_FUNCTION(is_key_released)
		ADD_PYTHON_FUNCTION(is_key_down)
		ADD_PYTHON_FUNCTION(set_mouse_click_callback)
		ADD_PYTHON_FUNCTION(set_mouse_down_callback)
		ADD_PYTHON_FUNCTION(set_mouse_double_click_callback)
		ADD_PYTHON_FUNCTION(set_key_down_callback)
		ADD_PYTHON_FUNCTION(set_key_press_callback)
		ADD_PYTHON_FUNCTION(set_key_release_callback)
		ADD_PYTHON_FUNCTION(set_mouse_release_callback)
		ADD_PYTHON_FUNCTION(set_render_callback)
		ADD_PYTHON_FUNCTION(set_resize_callback)

		// Theme commands
		ADD_PYTHON_FUNCTION(add_additional_font)
		ADD_PYTHON_FUNCTION(set_item_color)
		ADD_PYTHON_FUNCTION(clear_item_color)
		ADD_PYTHON_FUNCTION(get_theme_item)
		ADD_PYTHON_FUNCTION(get_theme)
		ADD_PYTHON_FUNCTION(set_theme_item)
		ADD_PYTHON_FUNCTION(set_theme)
		ADD_PYTHON_FUNCTION(set_style_window_padding)
		ADD_PYTHON_FUNCTION(set_style_frame_padding)
		ADD_PYTHON_FUNCTION(set_style_item_spacing)
		ADD_PYTHON_FUNCTION(set_style_item_inner_spacing)
		ADD_PYTHON_FUNCTION(set_style_touch_extra_padding)
		ADD_PYTHON_FUNCTION(set_style_indent_spacing)
		ADD_PYTHON_FUNCTION(set_style_scrollbar_size)
		ADD_PYTHON_FUNCTION(set_style_grab_min_size)
		ADD_PYTHON_FUNCTION(set_style_window_border_size)
		ADD_PYTHON_FUNCTION(set_style_child_border_size)
		ADD_PYTHON_FUNCTION(set_style_popup_border_size)
		ADD_PYTHON_FUNCTION(set_style_frame_border_size)
		ADD_PYTHON_FUNCTION(set_style_tab_border_size)
		ADD_PYTHON_FUNCTION(set_style_window_rounding)
		ADD_PYTHON_FUNCTION(set_style_child_rounding)
		ADD_PYTHON_FUNCTION(set_style_frame_rounding)
		ADD_PYTHON_FUNCTION(set_style_popup_rounding)
		ADD_PYTHON_FUNCTION(set_style_scrollbar_rounding)
		ADD_PYTHON_FUNCTION(set_style_grab_rounding)
		ADD_PYTHON_FUNCTION(set_style_tab_rounding)
		ADD_PYTHON_FUNCTION(set_style_window_title_align)
		ADD_PYTHON_FUNCTION(set_style_window_menu_button_position)
		ADD_PYTHON_FUNCTION(set_style_color_button_position)
		ADD_PYTHON_FUNCTION(set_style_button_text_align)
		ADD_PYTHON_FUNCTION(set_style_selectable_text_align)
		ADD_PYTHON_FUNCTION(set_style_display_safe_area_padding)
		ADD_PYTHON_FUNCTION(set_style_global_alpha)
		ADD_PYTHON_FUNCTION(set_style_antialiased_lines)
		ADD_PYTHON_FUNCTION(set_style_antialiased_fill)
		ADD_PYTHON_FUNCTION(set_style_curve_tessellation_tolerance)
		ADD_PYTHON_FUNCTION(set_style_circle_segment_max_error)
		ADD_PYTHON_FUNCTION(get_style_window_padding)
		ADD_PYTHON_FUNCTION(get_style_frame_padding)
		ADD_PYTHON_FUNCTION(get_style_item_spacing)
		ADD_PYTHON_FUNCTION(get_style_item_inner_spacing)
		ADD_PYTHON_FUNCTION(get_style_touch_extra_padding)
		ADD_PYTHON_FUNCTION(get_style_indent_spacing)
		ADD_PYTHON_FUNCTION(get_style_scrollbar_size)
		ADD_PYTHON_FUNCTION(get_style_grab_min_size)
		ADD_PYTHON_FUNCTION(get_style_window_border_size)
		ADD_PYTHON_FUNCTION(get_style_child_border_size)
		ADD_PYTHON_FUNCTION(get_style_popup_border_size)
		ADD_PYTHON_FUNCTION(get_style_frame_border_size)
		ADD_PYTHON_FUNCTION(get_style_tab_border_size)
		ADD_PYTHON_FUNCTION(get_style_window_rounding)
		ADD_PYTHON_FUNCTION(get_style_child_rounding)
		ADD_PYTHON_FUNCTION(get_style_frame_rounding)
		ADD_PYTHON_FUNCTION(get_style_popup_rounding)
		ADD_PYTHON_FUNCTION(get_style_scrollbar_rounding)
		ADD_PYTHON_FUNCTION(get_style_grab_rounding)
		ADD_PYTHON_FUNCTION(get_style_tab_rounding)
		ADD_PYTHON_FUNCTION(get_style_window_title_align)
		ADD_PYTHON_FUNCTION(get_style_window_menu_button_position)
		ADD_PYTHON_FUNCTION(get_style_color_button_position)
		ADD_PYTHON_FUNCTION(get_style_button_text_align)
		ADD_PYTHON_FUNCTION(get_style_selectable_text_align)
		ADD_PYTHON_FUNCTION(get_style_display_safe_area_padding)
		ADD_PYTHON_FUNCTION(get_style_global_alpha)
		ADD_PYTHON_FUNCTION(get_style_antialiased_lines)
		ADD_PYTHON_FUNCTION(get_style_antialiased_fill)
		ADD_PYTHON_FUNCTION(get_style_curve_tessellation_tolerance)
		ADD_PYTHON_FUNCTION(get_style_circle_segment_max_error)
		
		// table commands
		ADD_PYTHON_FUNCTION(add_table)
		ADD_PYTHON_FUNCTION(set_headers)
		ADD_PYTHON_FUNCTION(clear_table)
		ADD_PYTHON_FUNCTION(get_table_item)
		ADD_PYTHON_FUNCTION(set_table_item)
		ADD_PYTHON_FUNCTION(get_table_selections)
		ADD_PYTHON_FUNCTION(set_table_selection)
		ADD_PYTHON_FUNCTION(add_column)
		ADD_PYTHON_FUNCTION(insert_column)
		ADD_PYTHON_FUNCTION(delete_column)
		ADD_PYTHON_FUNCTION(add_row)
		ADD_PYTHON_FUNCTION(insert_row)
		ADD_PYTHON_FUNCTION(delete_row)

		// drawing commands
		ADD_PYTHON_FUNCTION(draw_arrow)
		ADD_PYTHON_FUNCTION(add_drawing)
		ADD_PYTHON_FUNCTION(draw_image)
		ADD_PYTHON_FUNCTION(draw_line)
		ADD_PYTHON_FUNCTION(draw_triangle)
		ADD_PYTHON_FUNCTION(draw_rectangle)
		ADD_PYTHON_FUNCTION(draw_quad)
		ADD_PYTHON_FUNCTION(draw_text)
		ADD_PYTHON_FUNCTION(draw_circle)
		ADD_PYTHON_FUNCTION(draw_polyline)
		ADD_PYTHON_FUNCTION(draw_polygon)
		ADD_PYTHON_FUNCTION(draw_bezier_curve)
		ADD_PYTHON_FUNCTION(clear_drawing)
		ADD_PYTHON_FUNCTION(delete_drawing_item)

		// plot commands
		ADD_PYTHON_FUNCTION(is_plot_queried)
		ADD_PYTHON_FUNCTION(get_plot_query_area)
		ADD_PYTHON_FUNCTION(clear_plot)
		ADD_PYTHON_FUNCTION(reset_xticks)
		ADD_PYTHON_FUNCTION(reset_yticks)
		ADD_PYTHON_FUNCTION(set_xticks)
		ADD_PYTHON_FUNCTION(set_yticks)
		ADD_PYTHON_FUNCTION(set_plot_xlimits_auto)
		ADD_PYTHON_FUNCTION(set_plot_ylimits_auto)
		ADD_PYTHON_FUNCTION(set_plot_xlimits)
		ADD_PYTHON_FUNCTION(set_plot_ylimits)
		ADD_PYTHON_FUNCTION(set_color_map)
		ADD_PYTHON_FUNCTION(add_plot)
		ADD_PYTHON_FUNCTION(add_shade_series)
		ADD_PYTHON_FUNCTION(add_bar_series)
		ADD_PYTHON_FUNCTION(add_line_series)
		ADD_PYTHON_FUNCTION(add_pie_series)
		ADD_PYTHON_FUNCTION(add_scatter_series)
		ADD_PYTHON_FUNCTION(add_area_series)
		ADD_PYTHON_FUNCTION(add_stem_series)
		ADD_PYTHON_FUNCTION(add_error_series)
		ADD_PYTHON_FUNCTION(delete_series)
		ADD_PYTHON_FUNCTION(add_heat_series)
		ADD_PYTHON_FUNCTION(add_text_point)
		{NULL, NULL, 0, NULL}
	};

	static PyModuleDef dearpyguiModule = {
		PyModuleDef_HEAD_INIT, "dearpygui", NULL, -1, dearpyguimethods,
		NULL, NULL, NULL, NULL
	};

	PyMODINIT_FUNC PyInit_dearpygui(void)
	{
		PyObject* m;

		m = PyModule_Create(&dearpyguiModule);
		if (m == NULL)
			return NULL;

		auto constants = GetModuleConstants();

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

		return m;
	}

	void start_dearpygui_error()
	{
		PyErr_Print();

		// create window
		auto window = mvWindow::CreatemvWindow(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight(), true);
		window->show();
		window->run();
		delete window;
		delete mvApp::GetApp();

	}
}