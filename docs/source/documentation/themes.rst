Themes
======

Themes are containers which are composed of:
    Theme Components:
        containers within a theme that can specify an item type the theme colors/styles target
    Theme Colors:
        items that are added to a theme component and set colors
    Theme Styles:
        items that are added to a theme component and set styles

The theme can be:

* bound as the default theme. This will have a global effect across all windows and propagate.
* bound to a container. This will propagate to its children if applicable theme components are in the theme.
* bound to an item type if applicable theme components are in the theme.

Theme components must have a specified item type. This can either be *mvAll* for all items or a specific item type.

Style and color items have a named constant and will apply that constant to their components named item type. 
Style and color items must have a named category. Constants contain their category in the name.

Theme colors and styles fall into the following categories:

**mvThemeCat_Plots**:
    Items that are associated with plots. Style/color constants identified by *mvPlotCol_\*\*\** or *mvPlotStyle_\*\*\**

**mvThemeCat_Nodes**:
    Items that are associated with Nodes. Style/color constants identified by *mvNodeCol_\*\*\** or *mvNodeStyle_\*\*\**

**mvThemeCat_Core**:
    All other items within dearpygui. Style/color constants identified by *mvThemeCol_\*\*\** or *mvThemeStyle_\*\*\**

Default Theme (global) 
----------------------

Default themes will apply the theme globally across all windows and propagate to children.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial", pos=(20, 50), width=275, height=225) as win1:
        t1 = dpg.add_input_text(default_value="some text")
        t2 = dpg.add_input_text(default_value="some text")
        with dpg.child_window(height=100):
            t3 = dpg.add_input_text(default_value="some text")
            dpg.add_input_int()
        dpg.add_input_text(default_value="some text")

    with dpg.window(label="Tutorial", pos=(320, 50), width=275, height=225) as win2:
        dpg.add_input_text(default_value="some text")
        dpg.add_input_int()

    with dpg.theme() as global_theme:

        with dpg.theme_component(dpg.mvAll):
            dpg.add_theme_color(dpg.mvThemeCol_FrameBg, (255, 140, 23), category=dpg.mvThemeCat_Core)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)

        with dpg.theme_component(dpg.mvInputInt):
            dpg.add_theme_color(dpg.mvThemeCol_FrameBg, (140, 255, 23), category=dpg.mvThemeCat_Core)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)

    dpg.bind_theme(global_theme)

    dpg.show_style_editor()

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Container Propagation
---------------------

Applying a theme to a container will propagate the theme to its children:

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial", pos=(20, 50), width=275, height=225) as win1:
        t1 = dpg.add_input_text(default_value="some text")
        t2 = dpg.add_input_text(default_value="some text")
        with dpg.child_window(height=100):
            t3 = dpg.add_input_text(default_value="some text")
            dpg.add_input_int()
        dpg.add_input_text(default_value="some text")

    with dpg.window(label="Tutorial", pos=(320, 50), width=275, height=225) as win2:
        dpg.add_input_text(default_value="some text")
        dpg.add_input_int()

    with dpg.theme() as container_theme:

        with dpg.theme_component(dpg.mvAll):
            dpg.add_theme_color(dpg.mvThemeCol_FrameBg, (150, 100, 100), category=dpg.mvThemeCat_Core)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)

        with dpg.theme_component(dpg.mvInputInt):
            dpg.add_theme_color(dpg.mvThemeCol_FrameBg, (100, 150, 100), category=dpg.mvThemeCat_Core)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)

    dpg.bind_item_theme(win1, container_theme)

    dpg.show_style_editor()

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Item Specific
-------------

Applying a theme to an item will overide any previous themes on the specified item if the theme contains an applicable component.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial", pos=(20, 50), width=275, height=225) as win1:
        t1 = dpg.add_input_text(default_value="some text")
        t2 = dpg.add_input_text(default_value="some text")
        with dpg.child_window(height=100):
            t3 = dpg.add_input_text(default_value="some text")
            dpg.add_input_int()
        dpg.add_input_text(default_value="some text")

    with dpg.window(label="Tutorial", pos=(320, 50), width=275, height=225) as win2:
        dpg.add_input_text(default_value="some text")
        dpg.add_input_int()

    with dpg.theme() as item_theme:
        with dpg.theme_component(dpg.mvAll):
            dpg.add_theme_color(dpg.mvThemeCol_FrameBg, (200, 200, 100), category=dpg.mvThemeCat_Core)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 0, category=dpg.mvThemeCat_Core)

    dpg.bind_item_theme(t2, item_theme)

    dpg.show_style_editor()

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Priority of Themes
------------------

The theme prioritizes the latest applied theme in the order of 

1. specific item
2. container inherited
3. global

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    with dpg.window(label="Tutorial", pos=(20, 50), width=275, height=225) as win1:
        t1 = dpg.add_input_text(default_value="some text")
        t2 = dpg.add_input_text(default_value="some text")
        with dpg.child_window(height=100):
            t3 = dpg.add_input_text(default_value="some text")
            dpg.add_input_int()
        dpg.add_input_text(default_value="some text")

    with dpg.window(label="Tutorial", pos=(320, 50), width=275, height=225) as win2:
        dpg.add_input_text(default_value="some text")
        dpg.add_input_int()

    with dpg.theme() as global_theme:
        with dpg.theme_component(dpg.mvAll):
            dpg.add_theme_color(dpg.mvThemeCol_FrameBg, (255, 140, 23), category=dpg.mvThemeCat_Core)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)

        with dpg.theme_component(dpg.mvInputInt):
            dpg.add_theme_color(dpg.mvThemeCol_FrameBg, (140, 255, 23), category=dpg.mvThemeCat_Core)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)

    with dpg.theme() as container_theme:
        with dpg.theme_component(dpg.mvAll):
            dpg.add_theme_color(dpg.mvThemeCol_FrameBg, (150, 100, 100), category=dpg.mvThemeCat_Core)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)

        with dpg.theme_component(dpg.mvInputInt):
            dpg.add_theme_color(dpg.mvThemeCol_FrameBg, (100, 150, 100), category=dpg.mvThemeCat_Core)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)

    with dpg.theme() as item_theme:
        with dpg.theme_component(dpg.mvAll):
            dpg.add_theme_color(dpg.mvThemeCol_FrameBg, (200, 200, 100), category=dpg.mvThemeCat_Core)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 0, category=dpg.mvThemeCat_Core)

    dpg.bind_theme(global_theme)
    dpg.bind_item_theme(win1, container_theme)
    dpg.bind_item_theme(t2, item_theme)

    dpg.show_style_editor()

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Theme for Disabled Items
------------------------

Each item has a separate disabled theme that is used when the item is disabled.
The disabled theme follows the same propagation rules as the enabled theme.
When an item's parameter *enabled* is set to *False* the item will use its disabled theme.
If no disabled theme has been set the default theme will be used.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()
    dpg.create_viewport()
    dpg.setup_dearpygui()
    
    with dpg.theme() as disabled_theme:
        with dpg.theme_component(dpg.mvInputFloat, enabled_state=False):
            dpg.add_theme_color(dpg.mvThemeCol_Text, [255, 0, 0])
            dpg.add_theme_color(dpg.mvThemeCol_Button, [255, 0, 0])
    
        with dpg.theme_component(dpg.mvInputInt, enabled_state=False):
            dpg.add_theme_color(dpg.mvThemeCol_Text, [255, 0, 0])
            dpg.add_theme_color(dpg.mvThemeCol_Button, [255, 0, 0])
    
    dpg.bind_theme(disabled_theme)
    
    with dpg.window(label="tutorial"):
        dpg.add_input_float(label="Input float", enabled=False)
        dpg.add_input_int(label="Input int", enabled=False)
    
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Plot Markers
------------

===================== ==============
Plot Markers
===================== ==============
mvPlotMarker_None     mvPlotMarker_Circle
mvPlotMarker_Square   mvPlotMarker_Diamond
mvPlotMarker_Up       mvPlotMarker_Down
mvPlotMarker_Left     mvPlotMarker_Right
mvPlotMarker_Cross    mvPlotMarker_Plus
mvPlotMarker_Asterisk
===================== ==============

Core Colors
-----------

=============================== ================================ ============================
Core Colors
=============================== ================================ ============================
mvThemeCol_Text                 mvThemeCol_TabActive             mvThemeCol_SliderGrabActive
mvThemeCol_TextDisabled         mvThemeCol_TabUnfocused          mvThemeCol_Button
mvThemeCol_WindowBg             mvThemeCol_TabUnfocusedActive    mvThemeCol_ButtonHovered
mvThemeCol_ChildBg              mvThemeCol_ButtonActive
mvThemeCol_Border               mvThemeCol_Header
mvThemeCol_PopupBg              mvThemeCol_PlotLines             mvThemeCol_HeaderHovered
mvThemeCol_BorderShadow         mvThemeCol_PlotLinesHovered      mvThemeCol_HeaderActive
mvThemeCol_FrameBg              mvThemeCol_PlotHistogram         mvThemeCol_Separator
mvThemeCol_FrameBgHovered       mvThemeCol_PlotHistogramHovered  mvThemeCol_SeparatorHovered
mvThemeCol_FrameBgActive        mvThemeCol_TableHeaderBg         mvThemeCol_SeparatorActive
mvThemeCol_TitleBg              mvThemeCol_TableBorderStrong     mvThemeCol_ResizeGrip
mvThemeCol_TitleBgActive        mvThemeCol_TableBorderLight      mvThemeCol_ResizeGripHovered
mvThemeCol_TitleBgCollapsed     mvThemeCol_TableRowBg            mvThemeCol_ResizeGripActive
mvThemeCol_MenuBarBg            mvThemeCol_TableRowBgAlt         mvThemeCol_Tab
mvThemeCol_ScrollbarBg          mvThemeCol_TextSelectedBg        mvThemeCol_TabHovered
mvThemeCol_ScrollbarGrab        mvThemeCol_DragDropTarget
mvThemeCol_ScrollbarGrabHovered mvThemeCol_NavHighlight
mvThemeCol_ScrollbarGrabActive  mvThemeCol_NavWindowingHighlight
mvThemeCol_CheckMark            mvThemeCol_NavWindowingDimBg
mvThemeCol_SliderGrab           mvThemeCol_ModalWindowDimBg
=============================== ================================ ============================

Plot Colors
-----------

======================= ====================== =======================
Plot Colors
======================= ====================== =======================
mvPlotCol_Line          mvPlotCol_LegendBg     mvPlotCol_AxisBgHovered
mvPlotCol_Fill          mvPlotCol_LegendBorder mvPlotCol_AxisGrid
mvPlotCol_MarkerOutline mvPlotCol_LegendText   mvPlotCol_AxisText
mvPlotCol_MarkerFill    mvPlotCol_TitleText    mvPlotCol_Selection
mvPlotCol_ErrorBar      mvPlotCol_InlayText    mvPlotCol_Crosshairs
mvPlotCol_FrameBg       mvPlotCol_AxisBg       
mvPlotCol_PlotBg        mvPlotCol_AxisBgActive 
mvPlotCol_PlotBorder    mvPlotCol_YAxis        
======================= ====================== =======================

Node Colors
-----------

================================ ========================== ============================
Node Colors
================================ ========================== ============================
mvNodeCol_NodeBackground         mvNodeCol_TitleBarSelected mvNodeCol_BoxSelector
mvNodeCol_NodeBackgroundHovered  mvNodeCol_Link             mvNodeCol_BoxSelectorOutline
mvNodeCol_NodeBackgroundSelected mvNodeCol_LinkHovered      mvNodeCol_GridBackground
mvNodeCol_NodeOutline            mvNodeCol_LinkSelected     mvNodeCol_GridLine
mvNodeCol_TitleBar               mvNodeCol_Pin              mvNodeCol_PinHovered
mvNodeCol_TitleBarHovered
================================ ========================== ============================

Core Styles
-----------

============================== ==========
Constant                       Components
============================== ==========
mvStyleVar_Alpha               1
mvStyleVar_DisabledAlpha       1
mvStyleVar_WindowPadding       2
mvStyleVar_WindowRounding      1
mvStyleVar_WindowBorderSize    1
mvStyleVar_WindowMinSize       2
mvStyleVar_WindowTitleAlign    2
mvStyleVar_ChildRounding       1
mvStyleVar_ChildBorderSize     1
mvStyleVar_PopupRounding       1
mvStyleVar_PopupBorderSize     1
mvStyleVar_FramePadding        2
mvStyleVar_FrameRounding       1
mvStyleVar_FrameBorderSize     1
mvStyleVar_ItemSpacing         2
mvStyleVar_ItemInnerSpacing    2
mvStyleVar_IndentSpacing       1
mvStyleVar_CellPadding         2
mvStyleVar_ScrollbarSize       1
mvStyleVar_ScrollbarRounding   1
mvStyleVar_GrabMinSize         1
mvStyleVar_GrabRounding        1
mvStyleVar_TabRounding         1
mvStyleVar_ButtonTextAlign     2
mvStyleVar_SelectableTextAlign 2
============================== ==========

Plot Styles
-----------

================================= ==========
Constant                          Components
================================= ==========
mvPlotStyleVar_LineWeight         1
mvPlotStyleVar_Marker             1
mvPlotStyleVar_MarkerSize         1
mvPlotStyleVar_MarkerWeight       1
mvPlotStyleVar_FillAlpha          1
mvPlotStyleVar_ErrorBarSize       1
mvPlotStyleVar_ErrorBarWeight     1
mvPlotStyleVar_DigitalBitHeight   1
mvPlotStyleVar_DigitalBitGap      1
mvPlotStyleVar_PlotBorderSize     1
mvPlotStyleVar_MinorAlpha         1
mvPlotStyleVar_MajorTickLen       2
mvPlotStyleVar_MinorTickLen       2
mvPlotStyleVar_MajorTickSize      2
mvPlotStyleVar_MinorTickSize      2
mvPlotStyleVar_MajorGridSize      2
mvPlotStyleVar_MinorGridSize      2
mvPlotStyleVar_PlotPadding        2
mvPlotStyleVar_LabelPadding       2
mvPlotStyleVar_LegendPadding      2
mvPlotStyleVar_LegendInnerPadding 2
mvPlotStyleVar_LegendSpacing      2
mvPlotStyleVar_MousePosPadding    2
mvPlotStyleVar_AnnotationPadding  2
mvPlotStyleVar_FitPadding         2
mvPlotStyleVar_PlotDefaultSize    2
mvPlotStyleVar_PlotMinSize        2
================================= ==========

Node Styles
-----------

======================================== ==========
Constant                                 Components
======================================== ==========
mvNodeStyleVar_GridSpacing               1
mvNodeStyleVar_NodeCornerRounding        1
mvNodeStyleVar_NodePaddingHorizontal     1
mvNodeStyleVar_NodePaddingVertical       1
mvNodeStyleVar_NodeBorderThickness       1
mvNodeStyleVar_LinkThickness             1
mvNodeStyleVar_LinkLineSegmentsPerLength 1
mvNodeStyleVar_LinkHoverDistance         1
mvNodeStyleVar_PinCircleRadius           1
mvNodeStyleVar_PinQuadSideLength         1
mvNodeStyleVar_PinTriangleSideLength     1
mvNodeStyleVar_PinLineThickness          1
mvNodeStyleVar_PinHoverRadius            1
mvNodeStyleVar_PinOffset                 1
======================================== ==========