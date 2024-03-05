The list below represents a combination of high-priority work, nice-to-have features, and random ideas. We make no guarantees that all of this work will be completed or even started. If you see something that you need or would like to have, let us know, or better yet consider submitting a PR for the feature.

## API

## Axes

- add flag to remove weekends on Time axis
- pixel space scale (`ImPlotTransform_Display`), normalized space scale (`ImPlotTransform_Axes`), data space scale (`ImPlotTransform_Data`)
- make ImPlotFlags_Equal not a flag -> `SetupEqual(ImPlotAxis x, ImPlotAxis y)`
- allow inverted arguments `SetAxes` to transpose data?
- `SetupAxisColors()`
- `SetupAxisHome()`

## Plot Items

- add `PlotBubbles` (see MATLAB bubble chart)
- add non-zero references for `PlotBars` etc.
- add exploding to `PlotPieChart` (on hover-highlight?)
- fix appearance of `PlotBars` spacing

## Styling

- support gradient and/or colormap sampled fills (e.g. ImPlotFillStyle_)
- API for setting different fonts for plot elements

## Colormaps

- gradient editing tool
- `RemoveColormap`
- `enum ImPlotColorRule_ { Solid, Faded, XValue, YValue, ZValue }`

## Legend

- improve legend icons (e.g. adopt markers, gradients, etc)
- generalize legend rendering for plots and subplots
- add draggable scroll bar if users need it

## Tools / Misc.

- add `IsPlotChanging` to detect change in limits
- add ability to extend plot/axis context menus
- add LTTB downsampling for lines
- add box selection to axes
- first frame render delay might fix "fit pop" effect
- move some code to new `implot_tools.cpp`
- ColormapSlider (see metrics)
- FillAlpha should not affect markers?
- fix mouse text for time axes

## Optimizations

- find faster way to buffer data into ImDrawList (very slow)
- reduce number of calls to `PushClipRect`
- explore SIMD operations for high density plot items

## Plotter Pipeline

Ideally every `PlotX` function should use our faster rendering pipeline when it is applicable.

` User Data > Getter > Fitter > Renderer > RenderPrimitives`

|Plotter|Getter|Fitter|Renderer|RenderPrimitives|
|---|:-:|:-:|:-:|:-:|
|PlotLine|Yes|Yes|Yes|Yes|
|PlotScatter|Yes|Yes|Yes|Yes|
|PlotStairs|Yes|Yes|Yes|Yes|
|PlotShaded|Yes|Yes|Yes|Yes|
|PlotBars|Yes|Yes|Yes|Yes|
|PlotBarGroups|:|:|:|:|
|PlotHistogram|:|:|:|:|
|PlotErrorBars|Yes|Yes|No|No|
|PlotStems|Yes|Yes|Yes|Yes|
|PlotInfLines|Yes|Yes|Yes|Yes|
|PlotPieChart|No|No|No|No|
|PlotHeatmap|Yes|No|Yes|Mixed|
|PlotHistogram2D|:|:|:|:|
|PlotDigital|Yes|No|No|No|
|PlotImage|-|-|-|-|
|PlotText|-|-|-|-|
|PlotDummy|-|-|-|-|

## Completed
- make BeginPlot take fewer args:
- make query a tool -> `DragRect`
- rework DragLine/Point to use ButtonBehavior
- add support for multiple x-axes and don't limit count to 3
- make axis side configurable (top/left, right/bottom) via new flag `ImPlotAxisFlags_Opposite`
- add support for setting tick label strings via callback
- give each axis an ID, remove ad-hoc DND solution
- allow axis to be drag to opposite side (ala ImGui Table headers)
- legend items can be hovered even if plot is not
- fix frame delay on DragX tools
- remove tag from drag line/point -> add `Tag` tool
- add shortcut/legacy overloads for BeginPlot
- `SetupAxisConstraints()`
- `SetupAxisScale()`
- add `ImPlotLineFlags`, `ImPlotBarsFlags`, etc. for each plot type
- add `PlotBarGroups` wrapper that makes rendering groups of bars easier, with stacked bar support
- `PlotBars` restore outlines
- add hover/active color for plot axes
- make legend frame use ButtonBehavior
- `ImPlotLegendFlags_Scroll` (default behavior)
