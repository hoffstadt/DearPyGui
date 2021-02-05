#include "mvMarvel.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvAppLog.h"
#include "mvAppItems.h"
#include "mvDrawList.h"
#include "mvDrawCmdCommon.h"
#include "mvWindow.h"
#include <ImGuiFileDialog.h>
#include <cstdlib>

#ifndef MV_CPP
#include "mvPlotInterface.h"
#include "mvTableInterface.h"
#include "mvThemeInterface.h"
#include "mvInputInterface.h"
#include "mvBasicWidgetInterface.h"
#include "mvSliderInterface.h"
#include "mvInputsInterface.h"
#include "mvDragInterface.h"
#include "mvContainerInterface.h"
#include "mvAppItemInterface.h"
#include "mvAppInterface.h"
#endif // !MV_CPP



//-----------------------------------------------------------------------------
// Helper Macro
//-----------------------------------------------------------------------------
#define ADD_PYTHON_FUNCTION(Function) { #Function, (PyCFunction)Function, METH_VARARGS | METH_KEYWORDS, (*mvApp::GetApp()->getParsers())[#Function].getDocumentation() },

namespace Marvel {

	template <typename T>
	static void AddWidgetColorConstants(std::vector<std::pair<std::string, long>>& constants)
	{
		for (const auto& item : T::GetColorConstants())
			constants.push_back(item);
	}

	mvRef<std::map<std::string, mvPythonParser>> BuildDearPyGuiInterface()
	{

		auto parsers = CreateRef<std::map< std::string, mvPythonParser>>();

		// new system
		mvDrawing::InsertParser(parsers.get());
		mvListbox::InsertParser(parsers.get());
		mvText::InsertParser(parsers.get());
		mvCombo::InsertParser(parsers.get());
		mvRadioButton::InsertParser(parsers.get());
		mvButton::InsertParser(parsers.get());
		mvSelectable::InsertParser(parsers.get());
		mvCheckbox::InsertParser(parsers.get());
		mvColorButton::InsertParser(parsers.get());
		mvColorEdit3::InsertParser(parsers.get());
		mvColorEdit4::InsertParser(parsers.get());
		mvColorPicker3::InsertParser(parsers.get());
		mvColorPicker4::InsertParser(parsers.get());
		mvImage::InsertParser(parsers.get());
		mvImageButton::InsertParser(parsers.get());
		mvProgressBar::InsertParser(parsers.get());
		mvLoggerItem::InsertParser(parsers.get());
		mvDatePicker::InsertParser(parsers.get());
		mvTimePicker::InsertParser(parsers.get());
		mvLabelText::InsertParser(parsers.get());
		mvMenuBar::InsertParser(parsers.get());
		mvMenu::InsertParser(parsers.get());
		mvMenuItem::InsertParser(parsers.get());
		mvTabBar::InsertParser(parsers.get());
		mvTab::InsertParser(parsers.get());
		mvTabButton::InsertParser(parsers.get());
		mvGroup::InsertParser(parsers.get());
		mvChild::InsertParser(parsers.get());
		mvWindowAppItem::InsertParser(parsers.get());
		mvDocWindow::InsertParser(parsers.get());
		mvDebugWindow::InsertParser(parsers.get());
		mvStyleWindow::InsertParser(parsers.get());
		mvTooltip::InsertParser(parsers.get());
		mvCollapsingHeader::InsertParser(parsers.get());
		mvTreeNode::InsertParser(parsers.get());
		mvManagedColumns::InsertParser(parsers.get());
		mvColumn::InsertParser(parsers.get());
		mvNextColumn::InsertParser(parsers.get());
		mvInputText::InsertParser(parsers.get());
		mvTable::InsertParser(parsers.get());
		mvPlot::InsertParser(parsers.get());
		mvDrawList::InsertParser(parsers.get());

#ifndef MV_CPP
		AddPlotCommands(parsers.get());
		AddLogCommands(parsers.get());
		AddInputCommands(parsers.get());
		AddInputWidgets(parsers.get());
		AddSliderWidgets(parsers.get());
		AddDragWidgets(parsers.get());
		AddTableCommands(parsers.get());
		AddItemCommands(parsers.get());
		AddStdWindowCommands(parsers.get());
		AddThemeCommands(parsers.get());
		AddBasicWidgets(parsers.get());
		AddContainerWidgets(parsers.get());
		AddAppCommands(parsers.get());
#endif
		return parsers;
	}

	std::vector<std::pair<std::string, std::string>> GetAllCommands()
	{
		auto mapping = BuildDearPyGuiInterface();

		std::vector<std::pair<std::string, std::string>> result;

		for (const auto& item : *mapping)
			result.emplace_back(item.first, item.second.getDocumentation());

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
			{"mvPlotMarker_None"    , -1},  // no marker
			{"mvPlotMarker_Circle"  , 0},  // a circle marker will be rendered at each point
			{"mvPlotMarker_Square"  , 1},  // a square maker will be rendered at each point
			{"mvPlotMarker_Diamond" , 2},  // a diamond marker will be rendered at each point
			{"mvPlotMarker_Up"      , 3},  // an upward-pointing triangle marker will up rendered at each point
			{"mvPlotMarker_Down"    , 4},  // an downward-pointing triangle marker will up rendered at each point
			{"mvPlotMarker_Left"    , 5},  // an leftward-pointing triangle marker will up rendered at each point
			{"mvPlotMarker_Right"   , 6},  // an rightward-pointing triangle marker will up rendered at each point
			{"mvPlotMarker_Cross"   , 7},  // a cross marker will be rendered at each point (not filled)
			{"mvPlotMarker_Plus"    , 8},  // a plus marker will be rendered at each point (not filled)
			{"mvPlotMarker_Asterisk", 9}, // a asterisk marker will be rendered at each point (not filled)

			//-----------------------------------------------------------------------------
			// Built-in ImPlot Color maps
			//-----------------------------------------------------------------------------
			{"mvPlotColormap_Default",  0}, // ImPlot default colormap         (n=10)
			{"mvPlotColormap_Deep"   ,  1}, // a.k.a. matplotlib "Set1"        (n=9)
			{"mvPlotColormap_Dark"   ,  2}, // a.k.a. matplotlib "Set1"        (n=9)
			{"mvPlotColormap_Pastel" ,  3}, // a.k.a. matplotlib "Pastel1"     (n=9)
			{"mvPlotColormap_Paired" ,  4}, // a.k.a. matplotlib "Paired"      (n=12)
			{"mvPlotColormap_Viridis",  5}, // a.k.a. matplotlib "viridis"     (n=11)
			{"mvPlotColormap_Plasma" ,  6}, // a.k.a. matplotlib "plasma"      (n=11)
			{"mvPlotColormap_Hot"    ,  7}, // a.k.a. matplotlib/MATLAB "hot"  (n=11)
			{"mvPlotColormap_Cool"   ,  8}, // a.k.a. matplotlib/MATLAB "cool" (n=11)
			{"mvPlotColormap_Pink"   ,  9}, // a.k.a. matplotlib/MATLAB "pink" (n=11)
			{"mvPlotColormap_Jet"    ,  10}, // a.k.a. MATLAB "jet"             (n=11)

			//-----------------------------------------------------------------------------
			// NEW Theme Color IDs
			//-----------------------------------------------------------------------------
			//{ "mvThemeCol_InputText_Text"                  ,  300 },
			//{ "mvThemeCol_InputText_TextHighlight"         ,  344 },
			//{ "mvThemeCol_InputText_Bg"                    ,  307 },
			//{ "mvThemeCol_InputText_Hint"                  ,  301 },
			//{ "mvThemeCol_InputText_Border"                ,  305 },
			//{ "mvThemeCol_InputText_BorderShadow"          ,  306 },
			//{ "mvThemeCol_RadioButton_Text"                ,  500 },
			//{ "mvThemeCol_RadioButton_Bg"                  ,  507 },
			//{ "mvThemeCol_RadioButton_BgHovered"           ,  508 },
			//{ "mvThemeCol_RadioButton_BgActive"            ,  518 },
			//{ "mvThemeCol_RadioButton_Border"              ,  505 },
			//{ "mvThemeCol_RadioButton_BorderShadow"        ,  506 },
			//{ "mvThemeCol_TabBar"                          ,  635 },
			//{ "mvThemeCol_Tab_Text"                        ,  700 },
			//{ "mvThemeCol_Tab_Bg"                          ,  733 },
			//{ "mvThemeCol_Tab_BgHovered"                   ,  734 },
			//{ "mvThemeCol_Tab_BgActive"                    ,  735 },
			//{ "mvThemeCol_Tab_PopupBg"                     ,  704 },
			//{ "mvThemeCol_Menu_Text"                       , 1000 },
			//{ "mvThemeCol_Menu_Bg"                         , 1004 },
			//{ "mvThemeCol_Menu_BgHovered"                  , 1025 },
			//{ "mvThemeCol_Menu_BgActive"                   , 1024 },
			//{ "mvThemeCol_Menu_Border"                     , 1005 },
			//{ "mvThemeCol_MenuItem_Text"                   , 1200 },
			//{ "mvThemeCol_MenuItem_BgHovered"              , 1225 },
			//{ "mvThemeCol_Child_Bg"                        , 1503 },
			//{ "mvThemeCol_Child_MenuBar"                   , 1513 },
			//{ "mvThemeCol_Child_Border"                    , 1505 },
			//{ "mvThemeCol_Child_Scrollbar"                 , 1514 },
			//{ "mvThemeCol_Child_ScrollbarGrab"             , 1515 },
			//{ "mvThemeCol_Child_ScrollbarGrabHovered"      , 1516 },
			//{ "mvThemeCol_Child_ScrollbarGrabActive"       , 1517 },
			//{ "mvThemeCol_SliderFloat_Text"                , 1600 },
			//{ "mvThemeCol_SliderFloat_Bg"                  , 1607 },
			//{ "mvThemeCol_SliderFloat_BgHovered"           , 1608 },
			//{ "mvThemeCol_SliderFloat_BgActive"            , 1609 },
			//{ "mvThemeCol_SliderFloat_Grab"                , 1619 },
			//{ "mvThemeCol_SliderFloat_GrabActive"          , 1620 },
			//{ "mvThemeCol_SliderFloat_Border"              , 1605 },
			//{ "mvThemeCol_SliderFloat_BorderShadow"        , 1606 },
			//{ "mvThemeCol_SliderFloat2_Text"               , 2000 },
			//{ "mvThemeCol_SliderFloat2_Bg"                 , 2007 },
			//{ "mvThemeCol_SliderFloat2_BgHovered"          , 2008 },
			//{ "mvThemeCol_SliderFloat2_BgActive"           , 2009 },
			//{ "mvThemeCol_SliderFloat2_Grab"               , 2019 },
			//{ "mvThemeCol_SliderFloat2_GrabActive"         , 2020 },
			//{ "mvThemeCol_SliderFloat2_Border"             , 2005 },
			//{ "mvThemeCol_SliderFloat2_BorderShadow"       , 2006 },
			//{ "mvThemeCol_SliderFloat3_Text"               , 2100 },
			//{ "mvThemeCol_SliderFloat3_Bg"                 , 2107 },
			//{ "mvThemeCol_SliderFloat3_BgHovered"          , 2108 },
			//{ "mvThemeCol_SliderFloat3_BgActive"           , 2109 },
			//{ "mvThemeCol_SliderFloat3_Grab"               , 2119 },
			//{ "mvThemeCol_SliderFloat3_GrabActive"         , 2120 },
			//{ "mvThemeCol_SliderFloat3_Border"             , 2105 },
			//{ "mvThemeCol_SliderFloat3_BorderShadow"       , 2106 },
			//{ "mvThemeCol_SliderFloat4_Text"               , 1700 },
			//{ "mvThemeCol_SliderFloat4_Bg"                 , 1707 },
			//{ "mvThemeCol_SliderFloat4_BgHovered"          , 1708 },
			//{ "mvThemeCol_SliderFloat4_BgActive"           , 1709 },
			//{ "mvThemeCol_SliderFloat4_Grab"               , 1719 },
			//{ "mvThemeCol_SliderFloat4_GrabActive"         , 1720 },
			//{ "mvThemeCol_SliderFloat4_Border"             , 1705 },
			//{ "mvThemeCol_SliderFloat4_BorderShadow"       , 1706 },
			//{ "mvThemeCol_SliderInt_Text"                  , 1800 },
			//{ "mvThemeCol_SliderInt_Bg"                    , 1807 },
			//{ "mvThemeCol_SliderInt_BgHovered"             , 1808 },
			//{ "mvThemeCol_SliderInt_BgActive"              , 1809 },
			//{ "mvThemeCol_SliderInt_Grab"                  , 1819 },
			//{ "mvThemeCol_SliderInt_GrabActive"            , 1820 },
			//{ "mvThemeCol_SliderInt_Border"                , 1805 },
			//{ "mvThemeCol_SliderInt_BorderShadow"          , 1806 },
			//{ "mvThemeCol_SliderInt2_Text"                 , 2200 },
			//{ "mvThemeCol_SliderInt2_Bg"                   , 2207 },
			//{ "mvThemeCol_SliderInt2_BgHovered"            , 2208 },
			//{ "mvThemeCol_SliderInt2_BgActive"             , 2209 },
			//{ "mvThemeCol_SliderInt2_Grab"                 , 2219 },
			//{ "mvThemeCol_SliderInt2_GrabActive"           , 2220 },
			//{ "mvThemeCol_SliderInt2_Border"               , 2205 },
			//{ "mvThemeCol_SliderInt2_BorderShadow"         , 2206 },
			//{ "mvThemeCol_SliderInt3_Text"                 , 2300 },
			//{ "mvThemeCol_SliderInt3_Bg"                   , 2307 },
			//{ "mvThemeCol_SliderInt3_BgHovered"            , 2308 },
			//{ "mvThemeCol_SliderInt3_BgActive"             , 2309 },
			//{ "mvThemeCol_SliderInt3_Grab"                 , 2319 },
			//{ "mvThemeCol_SliderInt3_GrabActive"           , 2320 },
			//{ "mvThemeCol_SliderInt3_Border"               , 2305 },
			//{ "mvThemeCol_SliderInt3_BorderShadow"         , 2306 },
			//{ "mvThemeCol_SliderInt4_Text"                 , 1900 },
			//{ "mvThemeCol_SliderInt4_Bg"                   , 1907 },
			//{ "mvThemeCol_SliderInt4_BgHovered"            , 1908 },
			//{ "mvThemeCol_SliderInt4_BgActive"             , 1909 },
			//{ "mvThemeCol_SliderInt4_Grab"                 , 1919 },
			//{ "mvThemeCol_SliderInt4_GrabActive"           , 1920 },
			//{ "mvThemeCol_SliderInt4_Border"               , 1905 },
			//{ "mvThemeCol_SliderInt4_BorderShadow"         , 1906 },
			//{ "mvThemeCol_DragFloat_Text"                  , 2400 },
			//{ "mvThemeCol_DragFloat_Bg"                    , 2407 },
			//{ "mvThemeCol_DragFloat_BgHovered"             , 2408 },
			//{ "mvThemeCol_DragFloat_BgActive"              , 2409 },
			//{ "mvThemeCol_DragFloat_Border"                , 2405 },
			//{ "mvThemeCol_DragFloat_BorderShadow"          , 2406 },
			//{ "mvThemeCol_DragFloat2_Text"                 , 2800 },
			//{ "mvThemeCol_DragFloat2_Bg"                   , 2807 },
			//{ "mvThemeCol_DragFloat2_BgHovered"            , 2808 },
			//{ "mvThemeCol_DragFloat2_BgActive"             , 2809 },
			//{ "mvThemeCol_DragFloat2_Border"               , 2805 },
			//{ "mvThemeCol_DragFloat2_BorderShadow"         , 2806 },
			//{ "mvThemeCol_DragFloat3_Text"                 , 2900 },
			//{ "mvThemeCol_DragFloat3_Bg"                   , 2907 },
			//{ "mvThemeCol_DragFloat3_BgHovered"            , 2908 },
			//{ "mvThemeCol_DragFloat3_BgActive"             , 2909 },
			//{ "mvThemeCol_DragFloat3_Border"               , 2905 },
			//{ "mvThemeCol_DragFloat3_BorderShadow"         , 2906 },
			//{ "mvThemeCol_DragFloat4_Text"                 , 2500 },
			//{ "mvThemeCol_DragFloat4_Bg"                   , 2507 },
			//{ "mvThemeCol_DragFloat4_BgHovered"            , 2508 },
			//{ "mvThemeCol_DragFloat4_BgActive"             , 2509 },
			//{ "mvThemeCol_DragFloat4_Border"               , 2505 },
			//{ "mvThemeCol_DragFloat4_BorderShadow"         , 2506 }, 
			//{ "mvThemeCol_DragInt_Text"                    , 2600 },
			//{ "mvThemeCol_DragInt_Bg"                      , 2607 },
			//{ "mvThemeCol_DragInt_BgHovered"               , 2608 },
			//{ "mvThemeCol_DragInt_BgActive"                , 2609 },
			//{ "mvThemeCol_DragInt_Border"                  , 2605 },
			//{ "mvThemeCol_DragInt_BorderShadow"            , 2606 },
			//{ "mvThemeCol_DragInt2_Text"                   , 3000 },
			//{ "mvThemeCol_DragInt2_Bg"                     , 3007 },
			//{ "mvThemeCol_DragInt2_BgHovered"              , 3008 },
			//{ "mvThemeCol_DragInt2_BgActive"               , 3009 },
			//{ "mvThemeCol_DragInt2_Border"                 , 3005 },
			//{ "mvThemeCol_DragInt2_BorderShadow"           , 3006 },
			//{ "mvThemeCol_DragInt3_Text"                   , 3100 },
			//{ "mvThemeCol_DragInt3_Bg"                     , 3107 },
			//{ "mvThemeCol_DragInt3_BgHovered"              , 3108 },
			//{ "mvThemeCol_DragInt3_BgActive"               , 3109 },
			//{ "mvThemeCol_DragInt3_Border"                 , 3105 },
			//{ "mvThemeCol_DragInt3_BorderShadow"           , 3106 },
			//{ "mvThemeCol_DragInt4_Text"                   , 2700 },
			//{ "mvThemeCol_DragInt4_Bg"                     , 2707 },
			//{ "mvThemeCol_DragInt4_BgHovered"              , 2708 },
			//{ "mvThemeCol_DragInt4_BgActive"               , 2709 },
			//{ "mvThemeCol_DragInt4_Border"                 , 2705 },
			//{ "mvThemeCol_DragInt4_BorderShadow"           , 2706 },
			//{ "mvThemeCol_InputFloat_Text"                 , 3200 },
			//{ "mvThemeCol_InputFloat_TextHighlight"        , 3244 },
			//{ "mvThemeCol_InputFloat_Bg"                   , 3207 },
			//{ "mvThemeCol_InputFloat_ButtonBg"             , 3221 },
			//{ "mvThemeCol_InputFloat_ButtonBgHovered"      , 3222 },
			//{ "mvThemeCol_InputFloat_ButtonBgActive"       , 3223 },
			//{ "mvThemeCol_InputFloat_Border"               , 3205 },
			//{ "mvThemeCol_InputFloat_BorderShadow"         , 3206 },
			//{ "mvThemeCol_InputFloat2_Text"                , 3600 },
			//{ "mvThemeCol_InputFloat2_TextHighlight"       , 3644 },
			//{ "mvThemeCol_InputFloat2_Bg"                  , 3607 },
			//{ "mvThemeCol_InputFloat2_Border"              , 3605 },
			//{ "mvThemeCol_InputFloat2_BorderShadow"        , 3606 },
			//{ "mvThemeCol_InputFloat3_Text"                , 3700 },
			//{ "mvThemeCol_InputFloat3_TextHighlight"       , 3744 },
			//{ "mvThemeCol_InputFloat3_Bg"                  , 3707 },
			//{ "mvThemeCol_InputFloat3_Border"              , 3705 },
			//{ "mvThemeCol_InputFloat3_BorderShadow"        , 3706 },
			//{ "mvThemeCol_InputFloat4_Text"                , 3300 },
			//{ "mvThemeCol_InputFloat4_TextHighlight"       , 3344 },
			//{ "mvThemeCol_InputFloat4_Bg"                  , 3307 },
			//{ "mvThemeCol_InputFloat4_Border"              , 3305 },
			//{ "mvThemeCol_InputFloat4_BorderShadow"        , 3306 },
			//{ "mvThemeCol_InputInt_Text"                   , 3400 },
			//{ "mvThemeCol_InputInt_TextHighlight"          , 3444 },
			//{ "mvThemeCol_InputInt_Bg"                     , 3407 },
			//{ "mvThemeCol_InputInt_ButtonBg"               , 3421 },
			//{ "mvThemeCol_InputInt_ButtonBgHovered"        , 3422 },
			//{ "mvThemeCol_InputInt_ButtonBgActive"         , 3423 },
			//{ "mvThemeCol_InputInt_Border"                 , 3405 },
			//{ "mvThemeCol_InputInt_BorderShadow"           , 3406 },
			//{ "mvThemeCol_InputInt2_Text"                  , 3800 },
			//{ "mvThemeCol_InputInt2_TextHighlight"         , 3844 },
			//{ "mvThemeCol_InputInt2_Bg"                    , 3807 },
			//{ "mvThemeCol_InputInt2_Border"                , 3805 },
			//{ "mvThemeCol_InputInt2_BorderShadow"          , 3806 },
			//{ "mvThemeCol_InputInt3_Text"                  , 3900 },
			//{ "mvThemeCol_InputInt3_TextHighlight"         , 3944 },
			//{ "mvThemeCol_InputInt3_Bg"                    , 3907 },
			//{ "mvThemeCol_InputInt3_Border"                , 3905 },
			//{ "mvThemeCol_InputInt3_BorderShadow"          , 3906 },
			//{ "mvThemeCol_InputInt4_Text"                  , 3500 },
			//{ "mvThemeCol_InputInt4_TextHighlight"         , 3544 },
			//{ "mvThemeCol_InputInt4_Bg"                    , 3507 },
			//{ "mvThemeCol_InputInt4_Border"                , 3505 },
			//{ "mvThemeCol_InputInt4_BorderShadow"          , 3506 },
			//{ "mvThemeCol_ColorEdit3_Text"                 , 4000 },
			//{ "mvThemeCol_ColorEdit3_TextHighlight"        , 4044 },
			//{ "mvThemeCol_ColorEdit3_Bg"                   , 4007 },
			//{ "mvThemeCol_ColorEdit3_BgHovered"            , 4008 },
			//{ "mvThemeCol_ColorEdit3_BgActive"             , 4009 },
			//{ "mvThemeCol_ColorEdit3_PopupBg"              , 4004 },
			//{ "mvThemeCol_ColorEdit3_Border"               , 4005 },
			//{ "mvThemeCol_ColorEdit3_BorderShadow"         , 4006 },
			//{ "mvThemeCol_ColorEdit4_Text"                 , 4100 },
			//{ "mvThemeCol_ColorEdit4_TextHighlight"        , 4144 },
			//{ "mvThemeCol_ColorEdit4_Bg"                   , 4107 },
			//{ "mvThemeCol_ColorEdit4_BgHovered"            , 4108 },
			//{ "mvThemeCol_ColorEdit4_BgActive"             , 4109 },
			//{ "mvThemeCol_ColorEdit4_PopupBg"              , 4104 },
			//{ "mvThemeCol_ColorEdit4_Border"               , 4105 },
			//{ "mvThemeCol_ColorEdit4_BorderShadow"         , 4106 },
			//{ "mvThemeCol_ColorPicker3_Text"               , 4200 },
			//{ "mvThemeCol_ColorPicker3_Bg"                 , 4207 },
			//{ "mvThemeCol_ColorPicker3_BgHovered"          , 4208 },
			//{ "mvThemeCol_ColorPicker3_BgActive"           , 4209 },
			//{ "mvThemeCol_ColorPicker3_Border"             , 4205 },
			//{ "mvThemeCol_ColorPicker3_BorderShadow"       , 4206 },
			//{ "mvThemeCol_ColorPicker4_Text"               , 4300 },
			//{ "mvThemeCol_ColorPicker4_Bg"                 , 4307 },
			//{ "mvThemeCol_ColorPicker4_BgHovered"          , 4308 },
			//{ "mvThemeCol_ColorPicker4_BgActive"           , 4309 },
			//{ "mvThemeCol_ColorPicker4_Border"             , 4305 },
			//{ "mvThemeCol_ColorPicker4_BorderShadow"       , 4306 },
			//{ "mvThemeCol_Tooltip_Bg"                      , 4404 },
			//{ "mvThemeCol_Tooltip_Border"                  , 4405 },
			//{ "mvThemeCol_CollapsingHeader_Text"           , 4500 },
			//{ "mvThemeCol_CollapsingHeader_Bg"             , 4524 },
			//{ "mvThemeCol_CollapsingHeader_BgHovered"      , 4525 },
			//{ "mvThemeCol_CollapsingHeader_BgActive"       , 4526 },
			//{ "mvThemeCol_CollapsingHeader_Border"         , 4505 },
			//{ "mvThemeCol_CollapsingHeader_BorderShadow"   , 4506 },
			//{ "mvThemeCol_Separator"                       , 4627 },
			//{ "mvThemeCol_CheckBox_Text"                   , 4700 },
			//{ "mvThemeCol_CheckBox_Bg"                     , 4707 },
			//{ "mvThemeCol_CheckBox_BgHovered"              , 4708 },
			//{ "mvThemeCol_CheckBox_BgActive"               , 4718 },
			//{ "mvThemeCol_CheckBox_Border"                 , 4705 },
			//{ "mvThemeCol_CheckBox_BorderShadow"           , 4706 },
			//{ "mvThemeCol_Text_Text"                       , 4900 },
			//{ "mvThemeCol_LabelText_Text"                  , 5000 },

			//plot										   
			//{ "mvThemeCol_SimplePlot_Text"                 , 5300 },
			//{ "mvThemeCol_SimplePlot_Border"               , 5305 },
			//{ "mvThemeCol_SimplePlot_BorderShadow"         , 5306 },
			//{ "mvThemeCol_SimplePlot_Bg"                   , 5307 },
			//{ "mvThemeCol_SimplePlot_PopupBg"              , 5304 },
			//{ "mvThemeCol_SimplePlot_Lines"                , 5340 },
			//{ "mvThemeCol_SimplePlot_LinesHovered"         , 5341 },
			//{ "mvThemeCol_SimplePlot_Histogram"            , 5342 },
			//{ "mvThemeCol_SimplePlot_HistogramHovered"     , 5343 },
			//{ "mvThemeCol_Window_TitleText"                , 5700 },
			//{ "mvThemeCol_Window_TitleBg"                  , 5710 },
			//{ "mvThemeCol_Window_TitleBgActive"            , 5711 },
			//{ "mvThemeCol_Window_TitleBgCollapsed"         , 5712 },
			//{ "mvThemeCol_Window_MenuBar"                  , 5713 },
			//{ "mvThemeCol_Window_Bg"                       , 5702 },
			//{ "mvThemeCol_Window_Scrollbar"                , 5714 },
			//{ "mvThemeCol_Window_ScrollbarGrab"            , 5715 },
			//{ "mvThemeCol_Window_ScrollbarGrabHovered"     , 5716 },
			//{ "mvThemeCol_Window_ScrollbarGrabActive"      , 5717 },
			//{ "mvThemeCol_Window_ResizeBorder"             , 5729 },
			//{ "mvThemeCol_Window_ResizeGrip"               , 5730 },
			//{ "mvThemeCol_Window_ResizeGripHovered"        , 5731 },
			//{ "mvThemeCol_Window_ResizeGripActive"         , 5732 },
			//{ "mvThemeCol_Window_Border"                   , 5705 },
			//{ "mvThemeCol_Window_BorderShadow"             , 5706 },
			//{ "mvThemeCol_Popup_TitleText"                 , 5800 },
			//{ "mvThemeCol_Popup_TitleBg"                   , 5811 },
			//{ "mvThemeCol_Popup_Bg"                        , 5804 },
			//{ "mvThemeCol_Popup_ModalDimBg"                , 5849 },
			//{ "mvThemeCol_Popup_Scrollbar"                 , 5814 },
			//{ "mvThemeCol_Popup_ScrollbarGrab"             , 5815 },
			//{ "mvThemeCol_Popup_ScrollbarGrabHovered"      , 5816 },
			//{ "mvThemeCol_Popup_ScrollbarGrabActive"       , 5817 },
			//{ "mvThemeCol_Popup_Border"                    , 5805 },
			//{ "mvThemeCol_Selectable_Text"                 , 5900 },
			//{ "mvThemeCol_Selectable_Bg"                   , 5924 },
			//{ "mvThemeCol_Selectable_BgHovered"            , 5925 },
			//{ "mvThemeCol_Selectable_BgActive"             , 5926 },
			//{ "mvThemeCol_TreeNode_Text"                   , 6000 },
			//{ "mvThemeCol_TreeNode_BgHovered"              , 6025 },
			//{ "mvThemeCol_TreeNode_BgActive"               , 6026 },
			//{ "mvThemeCol_ProgressBar_Text"                , 6100 },
			//{ "mvThemeCol_ProgressBar_Bar"                 , 6142 },
			//{ "mvThemeCol_ProgressBar_Bg"                  , 6107 },
			//{ "mvThemeCol_ProgressBar_Border"              , 6105 },
			//{ "mvThemeCol_ProgressBar_BorderShadow"        , 6106 },
			//{ "mvThemeCol_Table_Text"					   , 6214 },
			//{ "mvThemeCol_Table_Cell"					   , 6224 },
			//{ "mvThemeCol_Table_CellHovered"			   , 6225 },
			//{ "mvThemeCol_Table_CellActive"				   , 6226 },
			//{ "mvThemeCol_Table_Border"				       , 6227 },
			//{ "mvThemeCol_Table_BorderHovered"		       , 6228 },
			//{ "mvThemeCol_Table_BorderActive"				 , 6229 },
			//{ "mvThemeCol_Table_Scrollbar"                 , 6214 },
			//{ "mvThemeCol_Table_ScrollbarGrab"             , 6215 },
			//{ "mvThemeCol_Table_ScrollbarGrabHovered"      , 6216 },
			//{ "mvThemeCol_Table_ScrollbarGrabActive"       , 6217 },
			//{ "mvThemeCol_ImageButton_Bg"                  , 6421 },
			//{ "mvThemeCol_ImageButton_BgHovered"           , 6422 },
			//{ "mvThemeCol_ImageButton_BgActive"            , 6423 },
			//{ "mvThemeCol_ImageButton_Border"              , 6405 },
			//{ "mvThemeCol_ImageButton_BorderShadow"        , 6406 },
			//{ "mvThemeCol_TimePicker_Time"                 , 6500 },
			//{ "mvThemeCol_TimePicker_TimeHovered"          , 6522 },
			//{ "mvThemeCol_TimePicker_DropDown"             , 6524 },
			//{ "mvThemeCol_TimePicker_DropDownHovered"      , 6525 },
			//{ "mvThemeCol_TimePicker_DropDownActive"       , 6526 },
			//{ "mvThemeCol_TimePicker_DropDownBg"           , 6504 },
			//{ "mvThemeCol_TimePicker_Scrollbar"            , 6514 },
			//{ "mvThemeCol_TimePicker_ScrollbarGrab"        , 6515 },
			//{ "mvThemeCol_TimePicker_ScrollbarGrabHovered" , 6516 },
			//{ "mvThemeCol_TimePicker_ScrollbarGrabActive"  , 6517 },
			//{ "mvThemeCol_TimePicker_Border"               , 6505 },
			//{ "mvThemeCol_TimePicker_BorderShadow"         , 6506 },
			//{ "mvThemeCol_DatePicker_Date"				   , 6600 },
			//{ "mvThemeCol_DatePicker_DateDisabled"		   , 6601 },
			//{ "mvThemeCol_DatePicker_DateHovered"          , 6622 },
			//{ "mvThemeCol_DatePicker_DateActive"           , 6623 },
			//{ "mvThemeCol_DatePicker_Border"               , 6605 },
			//{ "mvThemeCol_DatePicker_BorderShadow"         , 6606 },
			//{ "mvThemeCol_ColorButton_Text"                , 6700 },
			//{ "mvThemeCol_ColorButton_Separator"           , 6727 },
			//{ "mvThemeCol_ColorButton_PopupBg"             , 6704 },
			//{ "mvThemeCol_ColorButton_Border"              , 6705 },
			//{ "mvThemeCol_ColorButton_BorderShadow"        , 6706 },
			//{ "mvThemeCol_ManagedColumns_Border"           , 6827 },
			//{ "mvThemeCol_ManagedColumns_BorderHovered"    , 6828 },
			//{ "mvThemeCol_ManagedColumns_BorderActive"     , 6829 },
			//columnset
			//{ "mvThemeCol_NextColumn_Border"               , 7027 },
			//{ "mvThemeCol_NextColumn_BorderHovered"        , 7028 },
			//{ "mvThemeCol_NextColumn_BorderActive"         , 7029 },
			//{ "mvThemeCol_Logger_Text"                     , 7100 },
			//{ "mvThemeCol_Logger_TextHighlight"            , 7144 },
			//{ "mvThemeCol_Logger_OutBg"                    , 7103 },
			//{ "mvThemeCol_Logger_Buttons"                  , 7121 },
			//{ "mvThemeCol_Logger_ButtonsHover"             , 7122 },
			//{ "mvThemeCol_Logger_ButtonsActive"            , 7123 },
			//{ "mvThemeCol_Logger_Settings"                 , 7107 },
			//{ "mvThemeCol_Logger_SettingsHover"            , 7108 },
			//{ "mvThemeCol_Logger_SettingsActive"           , 7109 },
			//{ "mvThemeCol_Logger_SettingSelection"         , 7118 },
			//{ "mvThemeCol_Logger_ScrollBg"                 , 7114 },
			//{ "mvThemeCol_Logger_ScrollGrab"               , 7115 },
			//{ "mvThemeCol_Logger_ScrollGrabHovered"        , 7116 },
			//{ "mvThemeCol_Logger_ScrollGrabActive"         , 7117 },
			//{ "mvThemeCol_Logger_Border"                   , 7105 },
			//{ "mvThemeCol_Logger_BorderShadow"             , 7106 },
			//aboutwindow
			//docwindow
			//debugwindow
			//metricswindow
			//stylewindow
			//filedialog
			//{ "mvThemeCol_TabButton_Text"                  , 7800 },
			//{ "mvThemeCol_TabButton_Bg"                    , 7833 },
			//{ "mvThemeCol_TabButton_BgHovered"             , 7834 },
			//{ "mvThemeCol_TabButton_Border"                , 7805 },
			//{ "mvThemeCol_TabButton_PopupBg"               , 7804 },

			//-----------------------------------------------------------------------------
			// NEW Theme Color IDs
			//-----------------------------------------------------------------------------
			{ "mvThemeStyle_Button_Rounding"                 ,  411 },

			//-----------------------------------------------------------------------------
			// Theme style variable IDs
			//-----------------------------------------------------------------------------
			{ "mvGuiStyleVar_Alpha",               0 },
			{ "mvGuiStyleVar_WindowPadding",       1 },
			{ "mvGuiStyleVar_WindowRounding",      2 },
			{ "mvGuiStyleVar_WindowBorderSize",    3 },
			{ "mvGuiStyleVar_WindowMinSize",       4 },
			{ "mvGuiStyleVar_WindowTitleAlign",    5 },
			{ "mvGuiStyleVar_ChildRounding",       6 },
			{ "mvGuiStyleVar_ChildBorderSize",     7 },
			{ "mvGuiStyleVar_PopupRounding",       8 },
			{ "mvGuiStyleVar_PopupBorderSize",     9 },
			{ "mvGuiStyleVar_FramePadding",        10 },
			{ "mvGuiStyleVar_FrameRounding",       11 },
			{ "mvGuiStyleVar_FrameBorderSize",     12 },
			{ "mvGuiStyleVar_ItemSpacing",         13 },
			{ "mvGuiStyleVar_ItemInnerSpacing",    14 },
			{ "mvGuiStyleVar_IndentSpacing",       15 },
			{ "mvGuiStyleVar_ScrollbarSize",       16 },
			{ "mvGuiStyleVar_ScrollbarRounding",   17 },
			{ "mvGuiStyleVar_GrabMinSize",         18 },
			{ "mvGuiStyleVar_GrabRounding",        19 },
			{ "mvGuiStyleVar_TabRounding",         20 },
			{ "mvGuiStyleVar_ButtonTextAlign",     21 },
			{ "mvGuiStyleVar_SelectableTextAlign", 22 },
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
			{"mvKey_Prior",  266 },
			{"mvKey_Next",  267 },
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
			{ "mvOFF"     , 5 },

			//-----------------------------------------------------------------------------
			// Texture Formats
			//-----------------------------------------------------------------------------
			{ "mvTEX_RGBA_INT", 0 },
			{ "mvTEX_RGBA_FLOAT", 1 },
			{ "mvTEX_RGB_FLOAT", 2 },
			{ "mvTEX_RGB_INT", 3 },

		};

		AddWidgetColorConstants<mvButton>(ModuleConstants);
		AddWidgetColorConstants<mvCombo>(ModuleConstants);
		AddWidgetColorConstants<mvListbox>(ModuleConstants);

		return ModuleConstants;
	}

#ifndef MV_CPP
	static PyMethodDef dearpyguimethods[]
	{
		ADD_PYTHON_FUNCTION(get_item_configuration)
		ADD_PYTHON_FUNCTION(configure_item)

		// app interface
		ADD_PYTHON_FUNCTION(enable_docking)
		ADD_PYTHON_FUNCTION(add_texture)
		ADD_PYTHON_FUNCTION(decrement_texture)
		ADD_PYTHON_FUNCTION(add_character_remap)
		ADD_PYTHON_FUNCTION(set_primary_window)
		ADD_PYTHON_FUNCTION(set_accelerator_callback)
		ADD_PYTHON_FUNCTION(set_main_window_size)
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
		ADD_PYTHON_FUNCTION(open_file_dialog)
		ADD_PYTHON_FUNCTION(set_exit_callback)
		ADD_PYTHON_FUNCTION(set_vsync)
		ADD_PYTHON_FUNCTION(stop_dearpygui)
		ADD_PYTHON_FUNCTION(is_dearpygui_running)
		ADD_PYTHON_FUNCTION(set_main_window_title)
		ADD_PYTHON_FUNCTION(set_main_window_pos)
		ADD_PYTHON_FUNCTION(set_main_window_resizable)
		ADD_PYTHON_FUNCTION(set_start_callback)
		ADD_PYTHON_FUNCTION(close_popup)
		ADD_PYTHON_FUNCTION(get_log_level)
		ADD_PYTHON_FUNCTION(clear_log)
		ADD_PYTHON_FUNCTION(show_logger)
		ADD_PYTHON_FUNCTION(set_logger_window_title)
		ADD_PYTHON_FUNCTION(set_log_level)
		ADD_PYTHON_FUNCTION(log)
		ADD_PYTHON_FUNCTION(log_debug)
		ADD_PYTHON_FUNCTION(log_info)
		ADD_PYTHON_FUNCTION(log_warning)
		ADD_PYTHON_FUNCTION(log_error)

		// app item interface
		ADD_PYTHON_FUNCTION(move_item)
		ADD_PYTHON_FUNCTION(get_managed_column_width)
		ADD_PYTHON_FUNCTION(set_managed_column_width)
		ADD_PYTHON_FUNCTION(get_item_type)
		ADD_PYTHON_FUNCTION(set_item_callback)
		ADD_PYTHON_FUNCTION(set_item_callback_data)
		ADD_PYTHON_FUNCTION(get_value)
		ADD_PYTHON_FUNCTION(set_value)
		ADD_PYTHON_FUNCTION(add_value)
		ADD_PYTHON_FUNCTION(incref_value)
		ADD_PYTHON_FUNCTION(decref_value)
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
		ADD_PYTHON_FUNCTION(get_item_callback_data)
		ADD_PYTHON_FUNCTION(get_item_parent)
		ADD_PYTHON_FUNCTION(delete_item)
		ADD_PYTHON_FUNCTION(does_item_exist)
		ADD_PYTHON_FUNCTION(move_item_down)
		ADD_PYTHON_FUNCTION(move_item_up)
		ADD_PYTHON_FUNCTION(get_windows)
		ADD_PYTHON_FUNCTION(get_all_items)
		ADD_PYTHON_FUNCTION(get_item_children)

		// widget commands
		ADD_PYTHON_FUNCTION(add_logger)
		ADD_PYTHON_FUNCTION(add_next_column)
		ADD_PYTHON_FUNCTION(add_columns)
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
		ADD_PYTHON_FUNCTION(add_color_button)
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
		ADD_PYTHON_FUNCTION(add_tab_button)
		ADD_PYTHON_FUNCTION(add_menu_bar)
		ADD_PYTHON_FUNCTION(add_menu)
		ADD_PYTHON_FUNCTION(add_menu_item)
		ADD_PYTHON_FUNCTION(add_spacing)
		ADD_PYTHON_FUNCTION(add_same_line)
		ADD_PYTHON_FUNCTION(add_tooltip)
		ADD_PYTHON_FUNCTION(add_collapsing_header)
		ADD_PYTHON_FUNCTION(add_dummy)
		ADD_PYTHON_FUNCTION(add_managed_columns)
		ADD_PYTHON_FUNCTION(add_about_window)
		ADD_PYTHON_FUNCTION(add_doc_window)
		ADD_PYTHON_FUNCTION(add_debug_window)
		ADD_PYTHON_FUNCTION(add_style_window)
		ADD_PYTHON_FUNCTION(add_metrics_window)

		// input commands
		ADD_PYTHON_FUNCTION(get_drawing_mouse_pos)
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
		ADD_PYTHON_FUNCTION(set_theme_color)
		ADD_PYTHON_FUNCTION(set_theme_style)
		ADD_PYTHON_FUNCTION(add_additional_font)
		
		// table commands
		ADD_PYTHON_FUNCTION(add_table)
		ADD_PYTHON_FUNCTION(set_table_data)
		ADD_PYTHON_FUNCTION(get_table_data)
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
		ADD_PYTHON_FUNCTION(bring_draw_command_forward)
		ADD_PYTHON_FUNCTION(bring_draw_command_to_front)
		ADD_PYTHON_FUNCTION(send_draw_command_back)
		ADD_PYTHON_FUNCTION(send_draw_command_to_back)
		ADD_PYTHON_FUNCTION(modify_draw_command)
		ADD_PYTHON_FUNCTION(get_draw_command)
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
		ADD_PYTHON_FUNCTION(delete_draw_command)

		// plot commands
		ADD_PYTHON_FUNCTION(add_drag_point)
		ADD_PYTHON_FUNCTION(delete_drag_point)
		ADD_PYTHON_FUNCTION(add_drag_line)
		ADD_PYTHON_FUNCTION(delete_drag_line)
		ADD_PYTHON_FUNCTION(add_annotation)
		ADD_PYTHON_FUNCTION(delete_annotation)
		ADD_PYTHON_FUNCTION(is_plot_queried)
		ADD_PYTHON_FUNCTION(get_plot_query_area)
		ADD_PYTHON_FUNCTION(clear_plot)
		ADD_PYTHON_FUNCTION(reset_xticks)
		ADD_PYTHON_FUNCTION(reset_yticks)
		ADD_PYTHON_FUNCTION(set_xticks)
		ADD_PYTHON_FUNCTION(set_yticks)
		ADD_PYTHON_FUNCTION(set_plot_xlimits_auto)
		ADD_PYTHON_FUNCTION(set_plot_ylimits_auto)
		ADD_PYTHON_FUNCTION(get_plot_xlimits)
		ADD_PYTHON_FUNCTION(set_plot_xlimits)
		ADD_PYTHON_FUNCTION(set_plot_ylimits)
		ADD_PYTHON_FUNCTION(get_plot_ylimits)
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
		ADD_PYTHON_FUNCTION(add_image_series)
		ADD_PYTHON_FUNCTION(add_stair_series)
		ADD_PYTHON_FUNCTION(add_candle_series)
		ADD_PYTHON_FUNCTION(add_vline_series)
		ADD_PYTHON_FUNCTION(add_hline_series)
		ADD_PYTHON_FUNCTION(delete_series)
		ADD_PYTHON_FUNCTION(add_heat_series)
		ADD_PYTHON_FUNCTION(add_text_point)
		{NULL, NULL, 0, NULL}
	};

	static PyModuleDef dearpyguiModule = {
		PyModuleDef_HEAD_INIT, "core", NULL, -1, dearpyguimethods,
		NULL, NULL, NULL, NULL
	};

	PyMODINIT_FUNC PyInit_core(void)
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

#endif

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
