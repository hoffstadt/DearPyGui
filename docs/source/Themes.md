**UNDER CONSTRUCTION!**

| [Basics](#basics) - [Categories](#categories) - [Theme Deduction](#how-does-an-app-item-decide-its-color-style) - [Apply theme to specific item](#apply-theme-to-specific-item) - [Apply default theme](#apply-default-theme) -[Apply theme to a container](#apply-theme-to-a-container) - [Apply theme to a type](#apply-theme-to-a-type) - [Plot Markers](#plot-markers) - [Core Colors](#core-colors) - [Plot Colors](#plot-colors) - [Node Colors](#node-colors) - [Core Styles](#core-styles) - [Plot Styles](#plot-styles) - [Node Styles](#node-styles) |
| ---- |
||

## Basics
In _Dear PyGui_, there is an app item container called a "theme". A theme is composed of theme colors and styles which are themselves app items. The theme can either be set as the default theme, attached to an app item type, a item container, or a specific item. 

## Categories
Theme colors and styles fall into the following categories:
* **mvThemeCat_Core**
* **mvThemeCat_Plots**
* **mvthemeCat_Nodes**

## How does an app item decide its color/style?
Every app item requires certain styles/colors to be set. When an app item is drawn, it performs several checks to locate the colors/styles its needs. The search order is:
1. [Locally attached theme item.](#apply-theme-to-specific-item)
2. [Globally attached theme item.](#apply-theme-to-a-type)
3. [Ancestor tree attached theme.](#apply-theme-to-a-container)
4. [User set default theme.](#apply-default-theme)
5. _Dear PyGui_ default theme.

## Apply theme to specific item
Below is an example of attaching a theme to a specific widget:
```python
import dearpygui.dearpygui as dpg

with dpg.window(label="tutorial"):
    dpg.add_button(label="Button 1", id="button1")
    dpg.add_button(label="Button 2", id="button2")

# create a theme
with dpg.theme(id="theme_id"):
    dpg.add_theme_color(dpg.mvThemeCol_Button, (255, 140, 23), category=dpg.mvThemeCat_Core)
    dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)

dpg.set_item_theme("button1", "theme_id")

dpg.start_dearpygui()
```

## Apply theme to a type
By applying a theme to a type, the theme only effects a specific app item type:
```python
import dearpygui.dearpygui as dpg

with dpg.window(label="tutorial"):
    dpg.add_button(label="Button 1")
    dpg.add_button(label="Button 2")

# create a theme
with dpg.theme(id="theme_id"):
    dpg.add_theme_color(dpg.mvThemeCol_Button, (255, 140, 23), category=dpg.mvThemeCat_Core)
    dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)

dpg.set_item_type_theme(dpg.mvButton, "theme_id")

dpg.start_dearpygui()
```

## Apply theme to a container
By applying a theme to a container, the theme is propagated to its children:
```python
import dearpygui.dearpygui as dpg

with dpg.window(label="tutorial", id="window_id"):
    dpg.add_button(label="Button 1")
    dpg.add_button(label="Button 2")

# create a theme
with dpg.theme(id="theme_id"):
    dpg.add_theme_color(dpg.mvThemeCol_Button, (255, 140, 23), category=dpg.mvThemeCat_Core)
    dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)

dpg.set_item_theme("window_id", "theme_id")

dpg.start_dearpygui()
```

## Apply default theme
Default themes will replace the default theme for every new item created.
Below is an example of applying a default theme:
```python
import dearpygui.dearpygui as dpg

# create a theme
with dpg.theme(default_theme=True):
    dpg.add_theme_color(dpg.mvThemeCol_Button, (255, 140, 23), category=dpg.mvThemeCat_Core)
    dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)

with dpg.window(label="tutorial"):
    dpg.add_button(label="Button 1")
    dpg.add_button(label="Button 2")

dpg.start_dearpygui()
```

## Plot Markers
| Plot markers | |
|----|----|
| mvPlotMarker_None | mvPlotMarker_Circle |
| mvPlotMarker_Square | mvPlotMarker_Diamond |
| mvPlotMarker_Up | mvPlotMarker_Down |
| mvPlotMarker_Left | mvPlotMarker_Right |
| mvPlotMarker_Cross | mvPlotMarker_Plus |
| mvPlotMarker_Asterisk |

## Core Colors
| Core Colors | | |
|----|----|----|
| mvThemeCol_Text | mvThemeCol_TabActive | mvThemeCol_SliderGrabActive |
| mvThemeCol_TextDisabled | mvThemeCol_TabUnfocused | mvThemeCol_Button |
| mvThemeCol_WindowBg | mvThemeCol_TabUnfocusedActive | mvThemeCol_ButtonHovered |
| mvThemeCol_ChildBg | mvThemeCol_DockingPreview | mvThemeCol_ButtonActive |
| mvThemeCol_Border | mvThemeCol_DockingEmptyBg | mvThemeCol_Header |
| mvThemeCol_PopupBg | mvThemeCol_PlotLines | mvThemeCol_HeaderHovered |
| mvThemeCol_BorderShadow | mvThemeCol_PlotLinesHovered | mvThemeCol_HeaderActive |
| mvThemeCol_FrameBg | mvThemeCol_PlotHistogram | mvThemeCol_Separator |
| mvThemeCol_FrameBgHovered | mvThemeCol_PlotHistogramHovered | mvThemeCol_SeparatorHovered |
| mvThemeCol_FrameBgActive | mvThemeCol_TableHeaderBg | mvThemeCol_SeparatorActive |
| mvThemeCol_TitleBg | mvThemeCol_TableBorderStrong | mvThemeCol_ResizeGrip |
| mvThemeCol_TitleBgActive | mvThemeCol_TableBorderLight | mvThemeCol_ResizeGripHovered |
| mvThemeCol_TitleBgCollapsed | mvThemeCol_TableRowBg | mvThemeCol_ResizeGripActive |
| mvThemeCol_MenuBarBg | mvThemeCol_TableRowBgAlt | mvThemeCol_Tab |
| mvThemeCol_ScrollbarBg | mvThemeCol_TextSelectedBg | mvThemeCol_TabHovered |
| mvThemeCol_ScrollbarGrab | mvThemeCol_DragDropTarget |
| mvThemeCol_ScrollbarGrabHovered | mvThemeCol_NavHighlight |
| mvThemeCol_ScrollbarGrabActive | mvThemeCol_NavWindowingHighlight |
| mvThemeCol_CheckMark | mvThemeCol_NavWindowingDimBg |
| mvThemeCol_SliderGrab | mvThemeCol_ModalWindowDimBg |

## Plot Colors
| Plot Colors | | |
|----|----|----|
| mvPlotCol_Line | mvPlotCol_LegendBg | mvPlotCol_YAxisGrid |
| mvPlotCol_Fill | mvPlotCol_LegendBorder | mvPlotCol_YAxis2 |
| mvPlotCol_MarkerOutline | mvPlotCol_LegendText | mvPlotCol_YAxisGrid2 |
| mvPlotCol_MarkerFill | mvPlotCol_TitleText | mvPlotCol_YAxis3 |
| mvPlotCol_ErrorBar | mvPlotCol_InlayText | mvPlotCol_YAxisGrid3 |
| mvPlotCol_FrameBg | mvPlotCol_XAxis | mvPlotCol_Selection |
| mvPlotCol_PlotBg | mvPlotCol_XAxisGrid | mvPlotCol_Query |
| mvPlotCol_PlotBorder | mvPlotCol_YAxis | mvPlotCol_Crosshairs |

## Node Colors
| Node Colors | | |
|----|----|----|
| mvNodeCol_NodeBackground | mvNodeCol_TitleBarSelected | mvNodeCol_BoxSelector |
| mvNodeCol_NodeBackgroundHovered | mvNodeCol_Link | mvNodeCol_BoxSelectorOutline |
| mvNodeCol_NodeBackgroundSelected | mvNodeCol_LinkHovered | mvNodeCol_GridBackground |
| mvNodeCol_NodeOutline | mvNodeCol_LinkSelected | mvNodeCol_GridLine |
| mvNodeCol_TitleBar | mvNodeCol_Pin | mvNodeCol_PinHovered |
| mvNodeCol_TitleBarHovered |

## Core Styles
| Constant | Components |
|----------|------------|
| mvStyleVar_Alpha | 1 |
| mvStyleVar_WindowPadding | 2 |
| mvStyleVar_WindowRounding | 1 |
| mvStyleVar_WindowBorderSize | 1 |
| mvStyleVar_WindowMinSize | 2 |
| mvStyleVar_WindowTitleAlign | 2 |
| mvStyleVar_ChildRounding | 1 |
| mvStyleVar_ChildBorderSize | 1 |
| mvStyleVar_PopupRounding | 1 |
| mvStyleVar_PopupBorderSize | 1 |
| mvStyleVar_FramePadding | 2 |
| mvStyleVar_FrameRounding | 1 |
| mvStyleVar_FrameBorderSize | 1 |
| mvStyleVar_ItemSpacing | 2 |
| mvStyleVar_ItemInnerSpacing | 2 |
| mvStyleVar_IndentSpacing | 1 |
| mvStyleVar_CellPadding | 2 |
| mvStyleVar_ScrollbarSize | 1 |
| mvStyleVar_ScrollbarRounding | 1 |
| mvStyleVar_GrabMinSize | 1 |
| mvStyleVar_GrabRounding | 1 |
| mvStyleVar_TabRounding | 1 |
| mvStyleVar_ButtonTextAlign | 2 |
| mvStyleVar_SelectableTextAlign | 2 |

## Plot Styles
| Constant | Components |
|----------|------------|
| mvPlotStyleVar_LineWeight | 1 |
| mvPlotStyleVar_Marker | 1 |
| mvPlotStyleVar_MarkerSize | 1 |
| mvPlotStyleVar_MarkerWeight | 1 |
| mvPlotStyleVar_FillAlpha | 1 |
| mvPlotStyleVar_ErrorBarSize | 1 |
| mvPlotStyleVar_ErrorBarWeight | 1 |
| mvPlotStyleVar_DigitalBitHeight | 1 |
| mvPlotStyleVar_DigitalBitGap | 1 |
| mvPlotStyleVar_PlotBorderSize | 1 |
| mvPlotStyleVar_MinorAlpha | 1 |
| mvPlotStyleVar_MajorTickLen | 2 |
| mvPlotStyleVar_MinorTickLen | 2 |
| mvPlotStyleVar_MajorTickSize | 2 |
| mvPlotStyleVar_MinorTickSize | 2 |
| mvPlotStyleVar_MajorGridSize | 2 |
| mvPlotStyleVar_MinorGridSize | 2 |
| mvPlotStyleVar_PlotPadding | 2 |
| mvPlotStyleVar_LabelPadding | 2 |
| mvPlotStyleVar_LegendPadding | 2 |
| mvPlotStyleVar_LegendInnerPadding | 2 |
| mvPlotStyleVar_LegendSpacing | 2 |
| mvPlotStyleVar_MousePosPadding | 2 |
| mvPlotStyleVar_AnnotationPadding | 2 |
| mvPlotStyleVar_FitPadding | 2 |
| mvPlotStyleVar_PlotDefaultSize | 2 |
| mvPlotStyleVar_PlotMinSize | 2 |

## Node Styles
| Constant | Components |
|----------|------------|
| mvNodeStyleVar_GridSpacing | 1 |
| mvNodeStyleVar_NodeCornerRounding | 1 |
| mvNodeStyleVar_NodePaddingHorizontal | 1 |
| mvNodeStyleVar_NodePaddingVertical | 1 |
| mvNodeStyleVar_NodeBorderThickness | 1 |
| mvNodeStyleVar_LinkThickness | 1 |
| mvNodeStyleVar_LinkLineSegmentsPerLength | 1 |
| mvNodeStyleVar_LinkHoverDistance | 1 |
| mvNodeStyleVar_PinCircleRadius | 1 |
| mvNodeStyleVar_PinQuadSideLength | 1 |
| mvNodeStyleVar_PinTriangleSideLength | 1 |
| mvNodeStyleVar_PinLineThickness | 1 |
| mvNodeStyleVar_PinHoverRadius | 1 |
| mvNodeStyleVar_PinOffset | 1 |