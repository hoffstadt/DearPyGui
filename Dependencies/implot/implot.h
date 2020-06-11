// MIT License

// Copyright (c) 2020 Evan Pezent

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// ImPlot v0.2 WIP

#pragma once
#include "imgui.h"

//-----------------------------------------------------------------------------
// Basic types and flags
//-----------------------------------------------------------------------------

typedef int ImPlotFlags;
typedef int ImPlotAxisFlags;
typedef int ImPlotCol;
typedef int ImPlotStyleVar;
typedef int ImPlotMarker;

// Options for plots
enum ImPlotFlags_ {
    ImPlotFlags_MousePos = 1 << 0,  // the mouse position, in plot coordinates, will be displayed in the bottom-right
    ImPlotFlags_Legend = 1 << 1,  // a legend will be displayed in the top-left
    ImPlotFlags_Highlight = 1 << 2,  // plot items will be highlighted when their legend entry is hovered
    ImPlotFlags_BoxSelect = 1 << 3,  // the user will be able to box-select with right-mouse
    ImPlotFlags_Query = 1 << 4,  // the user will be able to draw query rects with middle-mouse
    ImPlotFlags_ContextMenu = 1 << 5,  // the user will be able to open a context menu with double-right click
    ImPlotFlags_Crosshairs = 1 << 6,  // the default mouse cursor will be replaced with a crosshair when hovered
    ImPlotFlags_CullData = 1 << 7,  // plot data outside the plot area will be culled from rendering
    ImPlotFlags_AntiAliased = 1 << 8,  // lines and fills will be anti-aliased (not recommended)
    ImPlotFlags_NoChild = 1 << 9,  // a child window region will not be used to capture mouse scroll (can boost performance for single ImGui window applications)
    ImPlotFlags_YAxis2 = 1 << 10, // enable a 2nd y axis
    ImPlotFlags_YAxis3 = 1 << 11, // enable a 3rd y axis
    ImPlotFlags_Default = ImPlotFlags_MousePos | ImPlotFlags_Legend | ImPlotFlags_Highlight | ImPlotFlags_BoxSelect | ImPlotFlags_ContextMenu | ImPlotFlags_CullData
};

// Options for plot axes (X and Y)
enum ImPlotAxisFlags_ {
    ImPlotAxisFlags_GridLines = 1 << 0, // grid lines will be displayed
    ImPlotAxisFlags_TickMarks = 1 << 1, // tick marks will be displayed for each grid line
    ImPlotAxisFlags_TickLabels = 1 << 2, // text labels will be displayed for each grid line
    ImPlotAxisFlags_Invert = 1 << 3, // the axis will be inverted
    ImPlotAxisFlags_LockMin = 1 << 4, // the axis minimum value will be locked when panning/zooming
    ImPlotAxisFlags_LockMax = 1 << 5, // the axis maximum value will be locked when panning/zooming
    ImPlotAxisFlags_Adaptive = 1 << 6, // grid divisions will adapt to the current pixel size the axis
    ImPlotAxisFlags_LogScale = 1 << 7, // a logartithmic (base 10) axis scale will be used
    ImPlotAxisFlags_Scientific = 1 << 8, // scientific notation will be used for tick labels if displayed (WIP, not very good yet)
    ImPlotAxisFlags_Default = ImPlotAxisFlags_GridLines | ImPlotAxisFlags_TickMarks | ImPlotAxisFlags_TickLabels | ImPlotAxisFlags_Adaptive,
    ImPlotAxisFlags_Auxiliary = ImPlotAxisFlags_Default & ~ImPlotAxisFlags_GridLines,
};

// Plot styling colors
enum ImPlotCol_ {
    ImPlotCol_Line,          // plot line/outline color (defaults to a rotating color set)
    ImPlotCol_Fill,          // plot fill color for bars (defaults to the current line color)
    ImPlotCol_MarkerOutline, // marker outline color (defaults to the current line color)
    ImPlotCol_MarkerFill,    // marker fill color (defaults to the current line color)
    ImPlotCol_ErrorBar,      // error bar color (defaults to ImGuiCol_Text)
    ImPlotCol_FrameBg,       // plot frame background color (defaults to ImGuiCol_FrameBg)
    ImPlotCol_PlotBg,        // plot area background color (defaults to ImGuiCol_WindowBg)
    ImPlotCol_PlotBorder,    // plot area border color (defaults to ImGuiCol_Text)
    ImPlotCol_XAxis,         // x-axis grid/label color (defaults to 25% ImGuiCol_Text)
    ImPlotCol_YAxis,         // y-axis grid/label color (defaults to 25% ImGuiCol_Text)
    ImPlotCol_YAxis2,        // 2nd y-axis grid/label color (defaults to 25% ImGuiCol_Text)
    ImPlotCol_YAxis3,        // 3rd y-axis grid/label color (defaults to 25% ImGuiCol_Text)
    ImPlotCol_Selection,     // box-selection color (defaults to yellow)
    ImPlotCol_Query,         // box-query color (defaults to green)
    ImPlotCol_COUNT
};

// Plot styling variables
enum ImPlotStyleVar_ {
    ImPlotStyleVar_LineWeight,       // float, line weight in pixels
    ImPlotStyleVar_Marker,           // int,   marker specification
    ImPlotStyleVar_MarkerSize,       // float, marker size in pixels (roughly the marker's "radius")
    ImPlotStyleVar_MarkerWeight,     // float, outline weight of markers in pixels
    ImPlotStyleVar_ErrorBarSize,     // float, error bar whisker width in pixels
    ImPlotStyleVar_ErrorBarWeight,   // float, error bar whisker weight in pixels
    ImPlotStyleVar_DigitalBitHeight, // float, digital channels bit height (at 1) in pixels
    ImPlotStyleVar_DigitalBitGap,    // float, digital channels bit padding gap in pixels
    ImPlotStyleVar_COUNT
};

// Marker specification
enum ImPlotMarker_ {
    ImPlotMarker_None = 1 << 0,  // no marker
    ImPlotMarker_Circle = 1 << 1,  // a circle marker will be rendered at each point
    ImPlotMarker_Square = 1 << 2,  // a square maker will be rendered at each point
    ImPlotMarker_Diamond = 1 << 3,  // a diamond marker will be rendered at each point
    ImPlotMarker_Up = 1 << 4,  // an upward-pointing triangle marker will up rendered at each point
    ImPlotMarker_Down = 1 << 5,  // an downward-pointing triangle marker will up rendered at each point
    ImPlotMarker_Left = 1 << 6,  // an leftward-pointing triangle marker will up rendered at each point
    ImPlotMarker_Right = 1 << 7,  // an rightward-pointing triangle marker will up rendered at each point
    ImPlotMarker_Cross = 1 << 8,  // a cross marker will be rendered at each point (not filled)
    ImPlotMarker_Plus = 1 << 9,  // a plus marker will be rendered at each point (not filled)
    ImPlotMarker_Asterisk = 1 << 10, // a asterisk marker will be rendered at each point (not filled)
};

// A range defined by a min/max value. Used for plot axes ranges.
struct ImPlotRange {
    float Min, Max;
    ImPlotRange();
    bool Contains(float value) const;
    float Size() const;
};

// Combination of two ranges for X and Y axes.
struct ImPlotLimits {
    ImPlotRange X, Y;
    ImPlotLimits();
    bool Contains(const ImVec2& p) const;
    ImVec2 Size() const;
};

// Plot style structure
struct ImPlotStyle {
    float    LineWeight;              // = 1, line weight in pixels
    ImPlotMarker Marker;              // = ImPlotMarker_None, marker specification
    float    MarkerSize;              // = 4, marker size in pixels (roughly the marker's "radius")
    float    MarkerWeight;            // = 1, outline weight of markers in pixels
    float    ErrorBarSize;            // = 5, error bar whisker width in pixels
    float    ErrorBarWeight;          // = 1.5, error bar whisker weight in pixels
    float    DigitalBitHeight;        // = 8, digital channels bit height (at y = 1.0f) in pixels
    float    DigitalBitGap;           // = 4, digital channels bit padding gap in pixels
    ImVec4   Colors[ImPlotCol_COUNT]; // array of plot specific colors
    ImPlotStyle();
};

//-----------------------------------------------------------------------------
// Core API
//-----------------------------------------------------------------------------

namespace ImPlot {

    // Starts a 2D plotting context. If this function returns true, EndPlot() must
    // be called, e.g. "if (BeginPlot(...)) { ... EndPlot(); }"". #title_id must
    // be unique. If you need to avoid ID collisions or don't want to display a
    // title in the plot, use double hashes (e.g. "MyPlot##Hidden"). If #x_label
    // and/or #y_label are provided, axes labels will be displayed.
    bool BeginPlot(const char* title_id,
        const char* x_label = NULL,
        const char* y_label = NULL,
        const ImVec2& size = ImVec2(-1, 0),
        ImPlotFlags flags = ImPlotFlags_Default,
        ImPlotAxisFlags x_flags = ImPlotAxisFlags_Default,
        ImPlotAxisFlags y_flags = ImPlotAxisFlags_Default,
        ImPlotAxisFlags y2_flags = ImPlotAxisFlags_Auxiliary,
        ImPlotAxisFlags y3_flags = ImPlotAxisFlags_Auxiliary);
    // Only call EndPlot() if BeginPlot() returns true! Typically called at the end
    // of an if statement conditioned on BeginPlot().
    void EndPlot();

    //-----------------------------------------------------------------------------
    // Plot Items
    //-----------------------------------------------------------------------------

    // Plots a standard 2D line plot.
    void PlotLine(const char* label_id, const float* values, int count, int offset = 0, int stride = sizeof(float));
    void PlotLine(const char* label_id, const float* xs, const float* ys, int count, int offset = 0, int stride = sizeof(float));
    void PlotLine(const char* label_id, const ImVec2* data, int count, int offset = 0);
    void PlotLine(const char* label_id, ImVec2(*getter)(void* data, int idx), void* data, int count, int offset = 0);
    // Plots a standard 2D scatter plot.
    void PlotScatter(const char* label_id, const float* values, int count, int offset = 0, int stride = sizeof(float));
    void PlotScatter(const char* label_id, const float* xs, const float* ys, int count, int offset = 0, int stride = sizeof(float));
    void PlotScatter(const char* label_id, const ImVec2* data, int count, int offset = 0);
    void PlotScatter(const char* label_id, ImVec2(*getter)(void* data, int idx), void* data, int count, int offset = 0);
    // Plots a vertical bar graph.
    void PlotBars(const char* label_id, const float* values, int count, float width = 0.67f, float shift = 0, int offset = 0, int stride = sizeof(float));
    void PlotBars(const char* label_id, const float* xs, const float* ys, int count, float width, int offset = 0, int stride = sizeof(float));
    void PlotBars(const char* label_id, ImVec2(*getter)(void* data, int idx), void* data, int count, float width, int offset = 0);
    // Plots a horizontal bar graph.
    void PlotBarsH(const char* label_id, const float* values, int count, float height = 0.67f, float shift = 0, int offset = 0, int stride = sizeof(float));
    void PlotBarsH(const char* label_id, const float* xs, const float* ys, int count, float height, int offset = 0, int stride = sizeof(float));
    void PlotBarsH(const char* label_id, ImVec2(*getter)(void* data, int idx), void* data, int count, float height, int offset = 0);
    // Plots vertical error bar.
    void PlotErrorBars(const char* label_id, const float* xs, const float* ys, const float* err, int count, int offset = 0, int stride = sizeof(float));
    void PlotErrorBars(const char* label_id, const float* xs, const float* ys, const float* neg, const float* pos, int count, int offset = 0, int stride = sizeof(float));
    void PlotErrorBars(const char* label_id, ImVec4(*getter)(void* data, int idx), void* data, int count, int offset = 0);
    // Plots a pie chart. If the sum of values > 1, each value will be normalized. Center and radius are in plot coordinates.
    void PlotPieChart(const char** label_ids, float* values, int count, const ImVec2& center, float radius, bool show_percents = true, float angle0 = 90);
    // Plots digital data.
    void PlotDigital(const char* label_id, const float* xs, const float* ys, int count, int offset = 0, int stride = sizeof(float));
    void PlotDigital(const char* label_id, ImVec2(*getter)(void* data, int idx), void* data, int count, int offset = 0);
    // Plots a text label at point x,y.
    void PlotText(const char* text, float x, float y, bool vertical = false, const ImVec2& pixel_offset = ImVec2(0, 0));

    //-----------------------------------------------------------------------------
    // Plot Queries
    //-----------------------------------------------------------------------------

    /// Returns true if the plot area in the current or most recent plot is hovered.
    bool IsPlotHovered();
    /// Returns the mouse position in x,y coordinates of the current or most recent plot. A negative y_axis uses the current value of SetPlotYAxis (0 initially).
    ImVec2 GetPlotMousePos(int y_axis = -1);
    /// Returns the current or most recent plot axis range. A negative y_axis uses the current value of SetPlotYAxis (0 initially).
    ImPlotLimits GetPlotLimits(int y_axis = -1);
    /// Returns true if the current or most recent plot is being queried.
    bool IsPlotQueried();
    /// Returns the current or most recent plot query bounds.
    ImPlotLimits GetPlotQuery(int y_axis = -1);

    //-----------------------------------------------------------------------------
    // Plot Styling
    //-----------------------------------------------------------------------------

    // Provides access to plot style structure for permanant modifications to colors, sizes, etc.
    ImPlotStyle& GetStyle();

    // Sets the color palette to be used for plot items.
    void SetPalette(const ImVec4* colors, int num_colors);
    // Restores the default ImPlot color map.
    void RestorePalette();

    // Temporarily modify a plot color.
    void PushStyleColor(ImPlotCol idx, ImU32 col);
    // Temporarily modify a plot color.
    void PushStyleColor(ImPlotCol idx, const ImVec4& col);
    // Undo temporary color modification.
    void PopStyleColor(int count = 1);

    // Temporarily modify a style variable of float type.
    void PushStyleVar(ImPlotStyleVar idx, float val);
    // Temporarily modify a style variable of int type.
    void PushStyleVar(ImPlotStyleVar idx, int val);
    // Undo temporary style modification.
    void PopStyleVar(int count = 1);

    //-----------------------------------------------------------------------------
    // Plot Utils
    //-----------------------------------------------------------------------------

    /// Set the axes range limits of the next plot. Call right before BeginPlot(). If ImGuiCond_Always is used, the axes limits will be locked.
    void SetNextPlotLimits(float x_min, float x_max, float y_min, float y_max, ImGuiCond cond = ImGuiCond_Once);
    /// Set the X axis range limits of the next plot. Call right before BeginPlot(). If ImGuiCond_Always is used, the axis limits will be locked.
    void SetNextPlotLimitsX(float x_min, float x_max, ImGuiCond cond = ImGuiCond_Once);
    /// Set the Y axis range limits of the next plot. Call right before BeginPlot(). If ImGuiCond_Always is used, the axis limits will be locked.
    void SetNextPlotLimitsY(float y_min, float y_max, ImGuiCond cond = ImGuiCond_Once, int y_axis = 0);

    /// Select which Y axis will be used for subsequent plot elements. The default is '0', or the first Y axis.
    void SetPlotYAxis(int y_axis);

    // Get the current Plot position (top-left) in pixels.
    ImVec2 GetPlotPos();
    // Get the curent Plot size in pixels.
    ImVec2 GetPlotSize();

    // Convert pixels to a position in the current plot's coordinate system. A negative y_axis uses the current value of SetPlotYAxis (0 initially).
    ImVec2 PixelsToPlot(const ImVec2& pix, int y_axis = -1);
    // Convert a position in the current plot's coordinate system to pixels. A negative y_axis uses the current value of SetPlotYAxis (0 initially).
    ImVec2 PlotToPixels(const ImVec2& plt, int y_axis = -1);

    // Push clip rect for rendering to current plot area
    void PushPlotClipRect();
    // Pop plot clip rect
    void PopPlotClipRect();

    //-----------------------------------------------------------------------------
    // Demo
    //-----------------------------------------------------------------------------

    // Shows the ImPlot demo. Add implot_demo.cpp to your sources!
    void ShowDemoWindow(bool* p_open = NULL);

}  // namespace ImPlot