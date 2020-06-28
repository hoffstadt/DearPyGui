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
			// Window Flags
			//-----------------------------------------------------------------------------
			//{"mvWindowFlags_None"                     ,       0},
			//{"mvWindowFlags_NoTitleBar"               ,  1 << 0},   // Disable title-bar
			//{"mvWindowFlags_NoResize"                 ,  1 << 1},   // Disable user resizing with the lower-right grip
			//{"mvWindowFlags_NoMove"                   ,  1 << 2},   // Disable user moving the window
			//{"mvWindowFlags_NoScrollbar"              ,  1 << 3},   // Disable scrollbars (window can still scroll with mouse or programmatically)
			//{"mvWindowFlags_NoScrollWithMouse"        ,  1 << 4},   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
			//{"mvWindowFlags_NoCollapse"               ,  1 << 5},   // Disable user collapsing window by double-clicking on it
			//{"mvWindowFlags_AlwaysAutoResize"         ,  1 << 6},   // Resize every window to its content every frame
			//{"mvWindowFlags_NoBackground"             ,  1 << 7},   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
			//{"mvWindowFlags_NoSavedSettings"          ,  1 << 8},   // Never load/save settings in .ini file
			//{"mvWindowFlags_NoMouseInputs"            ,  1 << 9},   // Disable catching mouse, hovering test with pass through.
			//{"mvWindowFlags_MenuBar"                  ,  1 << 10},  // Has a menu-bar
			//{"mvWindowFlags_HorizontalScrollbar"      ,  1 << 11},  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
			//{"mvWindowFlags_NoFocusOnAppearing"       ,  1 << 12},  // Disable taking focus when transitioning from hidden to visible state
			//{"mvWindowFlags_NoBringToFrontOnFocus"    ,  1 << 13},  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
			//{"mvWindowFlags_AlwaysVerticalScrollbar"  ,  1 << 14},  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
			//{"mvWindowFlags_AlwaysHorizontalScrollbar",  1 << 15},  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
			//{"mvWindowFlags_AlwaysUseWindowPadding"   ,  1 << 16},  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
			//{"mvWindowFlags_NoNavInputs"              ,  1 << 18},  // No gamepad/keyboard navigation within the window
			//{"mvWindowFlags_NoNavFocus"               ,  1 << 19},  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
			//{"mvWindowFlags_UnsavedDocument"          ,  1 << 20},  // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. When used in a tab/docking context, tab is selected on closure and closure is deferred by one frame to allow code to cancel the closure (with a confirmation popup, etc.) without flicker.
			//{"mvWindowFlags_NoNav"                    ,   786432},
			//{"mvWindowFlags_NoDecoration"             ,       43},
			//{"mvWindowFlags_NoInputs"                 ,   786944},

			//-----------------------------------------------------------------------------
			// Input Text Flags
			//-----------------------------------------------------------------------------
			{"mvInputTextFlags_None"               ,      0},
			{"mvInputTextFlags_CharsDecimal"       , 1 << 0},   // Allow 0123456789.+-*/
			{"mvInputTextFlags_CharsHexadecimal"   , 1 << 1},   // Allow 0123456789ABCDEFabcdef
			{"mvInputTextFlags_CharsUppercase"     , 1 << 2},   // Turn a..z into A..Z
			{"mvInputTextFlags_CharsNoBlank"       , 1 << 3},   // Filter out spaces, tabs
			{"mvInputTextFlags_AutoSelectAll"      , 1 << 4},   // Select entire text when first taking mouse focus
			{"mvInputTextFlags_EnterReturnsTrue"   , 1 << 5},   // Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.
			{"mvInputTextFlags_CallbackCompletion" , 1 << 6},   // Callback on pressing TAB (for completion handling)
			{"mvInputTextFlags_CallbackHistory"    , 1 << 7},   // Callback on pressing Up/Down arrows (for history handling)
			{"mvInputTextFlags_CallbackAlways"     , 1 << 8},   // Callback on each iteration. User code may query cursor position, modify text buffer.
			{"mvInputTextFlags_CallbackCharFilter" , 1 << 9},   // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
			{"mvInputTextFlags_AllowTabInput"      , 1 << 10},  // Pressing TAB input a '\t' character into the text field
			{"mvInputTextFlags_CtrlEnterForNewLine", 1 << 11},  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
			{"mvInputTextFlags_NoHorizontalScroll" , 1 << 12},  // Disable following the cursor horizontally
			{"mvInputTextFlags_AlwaysInsertMode"   , 1 << 13},  // Insert mode
			{"mvInputTextFlags_ReadOnly"           , 1 << 14},  // Read-only mode
			{"mvInputTextFlags_Password"           , 1 << 15},  // Password mode, display all characters as '*'
			{"mvInputTextFlags_NoUndoRedo"         , 1 << 16},  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
			{"mvInputTextFlags_CharsScientific"    , 1 << 17},  // Allow 0123456789.+-*/eE (Scientific notation input)
			{"mvInputTextFlags_CallbackResize"     , 1 << 18},  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/imgui_stdlib.h for an example of using this)

			//-----------------------------------------------------------------------------
			// tree and collapsing header flags
			//-----------------------------------------------------------------------------
			{"mvTreeNodeFlags_None"                ,      0},
			{"mvTreeNodeFlags_Selected"            , 1 << 0},   // Draw as selected
			{"mvTreeNodeFlags_Framed"              , 1 << 1},   // Full colored frame (e.g. for CollapsingHeader)
			{"mvTreeNodeFlags_AllowItemOverlap"    , 1 << 2},   // Hit testing to allow subsequent widgets to overlap this one
			{"mvTreeNodeFlags_NoTreePushOnOpen"    , 1 << 3},   // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
			{"mvTreeNodeFlags_NoAutoOpenOnLog"     , 1 << 4},   // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
			{"mvTreeNodeFlags_DefaultOpen"         , 1 << 5},   // Default node to be open
			{"mvTreeNodeFlags_OpenOnDoubleClick"   , 1 << 6},   // Need double-click to open node
			{"mvTreeNodeFlags_OpenOnArrow"         , 1 << 7},   // Only open when clicking on the arrow part. If ImGuiTreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.
			{"mvTreeNodeFlags_Leaf"                , 1 << 8},   // No collapsing, no arrow (use as a convenience for leaf nodes).
			{"mvTreeNodeFlags_Bullet"              , 1 << 9},   // Display a bullet instead of arrow
			{"mvTreeNodeFlags_FramePadding"        , 1 << 10},  // Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding().
			{"mvTreeNodeFlags_SpanAvailWidth"      , 1 << 11},  // Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line. In the future we may refactor the hit system to be front-to-back, allowing natural overlaps and then this can become the default.
			{"mvTreeNodeFlags_SpanFullWidth"       , 1 << 12},  // Extend hit box to the left-most and right-most edges (bypass the indented area).
			{"mvTreeNodeFlags_NavLeftJumpsBackHere", 1 << 13},  // (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
			{"mvTreeNodeFlags_CollapsingHeader"    ,      26},

			//-----------------------------------------------------------------------------
			// Selectable flags
			//-----------------------------------------------------------------------------
			//{"mvSelectableFlags_None"            ,      0},
			//{"mvSelectableFlags_DontClosePopups" , 1 << 0},   // Clicking this don't close parent popup window
			//{"mvSelectableFlags_SpanAllColumns"  , 1 << 1},   // Selectable frame can span all columns (text will still fit in current column)
			//{"mvSelectableFlags_AllowDoubleClick", 1 << 2},   // Generate press events on double clicks too
			//{"mvSelectableFlags_Disabled"        , 1 << 3},   // Cannot be selected, display grayed out text
			//{"mvSelectableFlags_AllowItemOverlap", 1 << 4},    // (WIP) Hit testing to allow subsequent widgets to overlap this one

			//-----------------------------------------------------------------------------
			// Combo flags
			//-----------------------------------------------------------------------------
			//{"mvComboFlags_None"          ,      0},
			//{"mvComboFlags_PopupAlignLeft", 1 << 0},   // Align the popup toward the left by default
			//{"mvComboFlags_HeightSmall"   , 1 << 1},   // Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
			//{"mvComboFlags_HeightRegular" , 1 << 2},   // Max ~8 items visible (default)
			//{"mvComboFlags_HeightLarge"   , 1 << 3},   // Max ~20 items visible
			//{"mvComboFlags_HeightLargest" , 1 << 4},   // As many fitting items as possible
			//{"mvComboFlags_NoArrowButton ", 1 << 5},   // Display on the preview box without the square arrow button
			//{"mvComboFlags_NoPreview"     , 1 << 6},   // Display only a square arrow button
			//{"mvComboFlags_HeightMask_"   ,     30},

			//-----------------------------------------------------------------------------
			// Tabbar flags
			//-----------------------------------------------------------------------------
			{"mvTabBarFlags_None"                        ,      0},
			{"mvTabBarFlags_Reorderable"                 , 1 << 0},   // Allow manually dragging tabs to re-order them + New tabs are appended at the end of list
			{"mvTabBarFlags_AutoSelectNewTabs"           , 1 << 1},   // Automatically select new tabs when they appear
			{"mvTabBarFlags_TabListPopupButton"          , 1 << 2},   // Disable buttons to open the tab list popup
			{"mvTabBarFlags_NoCloseWithMiddleMouseButton", 1 << 3},   // Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You can still repro this behavior on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
			{"mvTabBarFlags_NoTabListScrollingButtons"   , 1 << 4},   // Disable scrolling buttons (apply when fitting policy is ImGuiTabBarFlags_FittingPolicyScroll)
			{"mvTabBarFlags_NoTooltip"                   , 1 << 5},   // Disable tooltips when hovering a tab
			{"mvTabBarFlags_FittingPolicyResizeDown"     , 1 << 6},   // Resize tabs when they don't fit
			{"mvTabBarFlags_FittingPolicyScroll"         , 1 << 7},   // Add scroll buttons when tabs don't fit
			{"mvTabBarFlags_FittingPolicyMask_"          ,    192},
			{"mvTabBarFlags_FittingPolicyDefault_"       ,     64},

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
			// mvColorEdit/ColorPicker flags
			//-----------------------------------------------------------------------------
			//{"mvColorEditFlags_None"           , 0},
			//{"mvColorEditFlags_NoAlpha"         , 1 << 1},   //              // ColorEdit, ColorPicker, ColorButton: ignore Alpha component (will only read 3 components from the input pointer).
			//{"mvColorEditFlags_NoPicker"        , 1 << 2},   //              // ColorEdit: disable picker when clicking on colored square.
			//{"mvColorEditFlags_NoOptions"       , 1 << 3},   //              // ColorEdit: disable toggling options menu when right-clicking on inputs/small preview.
			//{"mvColorEditFlags_NoSmallPreview"  , 1 << 4},   //              // ColorEdit, ColorPicker: disable colored square preview next to the inputs. (e.g. to show only the inputs)
			//{"mvColorEditFlags_NoInputs"        , 1 << 5},   //              // ColorEdit, ColorPicker: disable inputs sliders/text widgets (e.g. to show only the small preview colored square).
			//{"mvColorEditFlags_NoTooltip"       , 1 << 6},   //              // ColorEdit, ColorPicker, ColorButton: disable tooltip when hovering the preview.
			//{"mvColorEditFlags_NoLabel"         , 1 << 7},   //              // ColorEdit, ColorPicker: disable display of inline text label (the label is still forwarded to the tooltip and picker).
			//{"mvColorEditFlags_NoSidePreview"   , 1 << 8},   //              // ColorPicker: disable bigger color preview on right side of the picker, use small colored square preview instead.
			//{"mvColorEditFlags_NoDragDrop"      , 1 << 9},   //              // ColorEdit: disable drag and drop target. ColorButton: disable drag and drop source.
			//{"mvColorEditFlags_NoBorder"       , 1 << 10},  //              // ColorButton: disable border (which is enforced by default)
			////User Options (right-click on widget to change some of them).
			//{"mvColorEditFlags_AlphaBar"        , 1 << 16},  //              // ColorEdit, ColorPicker: show vertical alpha bar/gradient in picker.
			//{"mvColorEditFlags_AlphaPreview"    , 1 << 17},  //              // ColorEdit, ColorPicker, ColorButton: display preview as a transparent color over a checkerboard, instead of opaque.
			//{"mvColorEditFlags_AlphaPreviewHalf", 1 << 18},  //              // ColorEdit, ColorPicker, ColorButton: display half opaque / half checkerboard, instead of opaque.
			//{"mvColorEditFlags_HDR"             , 1 << 19},  //              // (WIP) ColorEdit: Currently only disable 0.0f..1.0f limits in RGBA edition (note: you probably want to use ImGuiColorEditFlags_Float flag as well).
			//{"mvColorEditFlags_DisplayRGB"      , 1 << 20},  // [Display]    // ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
			//{"mvColorEditFlags_DisplayHSV"      , 1 << 21},  // [Display]    // "
			//{"mvColorEditFlags_DisplayHex"      , 1 << 22},  // [Display]    // "
			//{"mvColorEditFlags_Uint8"           , 1 << 23},  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0..255.
			//{"mvColorEditFlags_Float"           , 1 << 24},  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0.0f..1.0f floats instead of 0..255 integers. No round-trip of value via integers.
			//{"mvColorEditFlags_PickerHueBar"    , 1 << 25},  // [Picker]     // ColorPicker: bar for Hue, rectangle for Sat/Value.
			//{"mvColorEditFlags_PickerHueWheel"  , 1 << 26},  // [Picker]     // ColorPicker: wheel for Hue, triangle for Sat/Value.
			//{"mvColorEditFlags_InputRGB"        , 1 << 27},  // [Input]      // ColorEdit, ColorPicker: input and output data in RGB format.
			//{"mvColorEditFlags_InputHSV"        , 1 << 28},  // [Input]      // ColorEdit, ColorPicker: input and output data in HSV format.
			//// Defaults Options. You can set application defaults using SetColorEditOptions(). The intent is that you probably don't want to
			//// override them in most of your calls. Let the user choose via the option menu and/or call SetColorEditOptions() once during startup.
			//{"mvColorEditFlags__OptionsDefault", 177209344},

			//-----------------------------------------------------------------------------
			// Mouse Codes
			//-----------------------------------------------------------------------------
			{"mvMouseButton_Left"  , 0},
			{"mvMouseButton_Right" , 1},
			{"mvMouseButton_Middle", 2},

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