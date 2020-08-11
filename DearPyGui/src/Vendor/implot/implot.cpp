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

// ImPlot v0.5 WIP

/*

API BREAKING CHANGES
====================
Occasionally introducing changes that are breaking the API. We try to make the breakage minor and easy to fix.
Below is a change-log of API breaking changes only. If you are using one of the functions listed, expect to have to fix some code.
When you are not sure about a old symbol or function name, try using the Search/Find function of your IDE to look for comments or references in all implot files.
You can read releases logs https://github.com/epezent/implot/releases for more details.

- 2020/06/13 (0.4) - The flags `ImPlotAxisFlag_Adaptive` and `ImPlotFlags_Cull` were removed. Both are now done internally by default.
- 2020/06/03 (0.3) - The signature and behavior of PlotPieChart was changed so that data with sum less than 1 can optionally be normalized. The label format can now be specified as well.
- 2020/06/01 (0.3) - SetPalette was changed to `SetColormap` for consistency with other plotting libraries. `RestorePalette` was removed. Use `SetColormap(ImPlotColormap_Default)`.
- 2020/05/31 (0.3) - Plot functions taking custom ImVec2* getters were removed. Use the ImPlotPoint* getter versions instead.
- 2020/05/29 (0.3) - The signature of ImPlotLimits::Contains was changed to take two doubles instead of ImVec2
- 2020/05/16 (0.2) - All plotting functions were reverted to being prefixed with "Plot" to maintain a consistent VerbNoun style. `Plot` was split into `PlotLine`
                     and `PlotScatter` (however, `PlotLine` can still be used to plot scatter points as `Plot` did before.). `Bar` is not `PlotBars`, to indicate
                     that multiple bars will be plotted.
- 2020/05/13 (0.2) - `ImMarker` was change to `ImPlotMarker` and `ImAxisFlags` was changed to `ImPlotAxisFlags`.
- 2020/05/11 (0.2) - `ImPlotFlags_Selection` was changed to `ImPlotFlags_BoxSelect`
- 2020/05/11 (0.2) - The namespace ImGui:: was replaced with ImPlot::. As a result, the following additional changes were made:
                     - Functions that were prefixed or decorated with the word "Plot" have been truncated. E.g., `ImGui::PlotBars` is now just `ImPlot::Bar`.
                       It should be fairly obvious what was what.
                     - Some functions have been given names that would have otherwise collided with the ImGui namespace. This has been done to maintain a consistent
                       style with ImGui. E.g., 'ImGui::PushPlotStyleVar` is now 'ImPlot::PushStyleVar'.
- 2020/05/10 (0.2) - The following function/struct names were changes:
                    - ImPlotRange       -> ImPlotLimits
                    - GetPlotRange()    -> GetPlotLimits()
                    - SetNextPlotRange  -> SetNextPlotLimits
                    - SetNextPlotRangeX -> SetNextPlotLimitsX
                    - SetNextPlotRangeY -> SetNextPlotLimitsY
- 2020/05/10 (0.2) - Plot queries are pixel based by default. Query rects that maintain relative plot position have been removed. This was done to support multi-y-axis.

*/

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "implot.h"
#include "imgui_internal.h"
#ifdef _MSC_VER
#define sprintf sprintf_s
#endif

#define IM_NORMALIZE2F_OVER_ZERO(VX, VY)                                                           \
    {                                                                                              \
        float d2 = VX * VX + VY * VY;                                                              \
        if (d2 > 0.0f) {                                                                           \
            float inv_len = 1.0f / ImSqrt(d2);                                                     \
            VX *= inv_len;                                                                         \
            VY *= inv_len;                                                                         \
        }                                                                                          \
    }


// The maximum number of support y-axes
#define MAX_Y_AXES 3

// static inline float  ImLog10(float x)  { return log10f(x); }
static inline double ImLog10(double x) { return log10(x); }

ImPlotStyle::ImPlotStyle() {
    LineWeight = 1;
    Marker = ImPlotMarker_None;
    MarkerSize = 4;
    MarkerWeight = 1;
    FillAlpha = 1;
    ErrorBarSize = 5;
    ErrorBarWeight = 1.5;
    DigitalBitHeight = 8;
    DigitalBitGap = 4;

    Colors[ImPlotCol_Line] = IMPLOT_COL_AUTO;
    Colors[ImPlotCol_Fill] = IMPLOT_COL_AUTO;
    Colors[ImPlotCol_MarkerOutline] = IMPLOT_COL_AUTO;
    Colors[ImPlotCol_MarkerFill] = IMPLOT_COL_AUTO;
    Colors[ImPlotCol_ErrorBar] = IMPLOT_COL_AUTO;
    Colors[ImPlotCol_FrameBg] = IMPLOT_COL_AUTO;
    Colors[ImPlotCol_PlotBg] = IMPLOT_COL_AUTO;
    Colors[ImPlotCol_PlotBorder] = IMPLOT_COL_AUTO;
    Colors[ImPlotCol_XAxis] = IMPLOT_COL_AUTO;
    Colors[ImPlotCol_YAxis] = IMPLOT_COL_AUTO;
    Colors[ImPlotCol_YAxis2] = IMPLOT_COL_AUTO;
    Colors[ImPlotCol_YAxis3] = IMPLOT_COL_AUTO;
    Colors[ImPlotCol_Selection] = ImVec4(1, 1, 0, 1);
    Colors[ImPlotCol_Query] = ImVec4(0, 1, 0, 1);
}

ImPlotRange::ImPlotRange() {
    Min = NAN;
    Max = NAN;
}

bool ImPlotRange::Contains(double v) const {
    return v >= Min && v <= Max;
}

double ImPlotRange::Size() const {
    return Max - Min;
}

ImPlotLimits::ImPlotLimits() {}

bool ImPlotLimits::Contains(const ImPlotPoint& p) const {
    return Contains(p.x, p.y);
}

bool ImPlotLimits::Contains(double x, double y) const {
    return X.Contains(x) && Y.Contains(y);
}

ImPlotInputMap::ImPlotInputMap() {
    PanButton = ImGuiMouseButton_Left;
    PanMod = ImGuiKeyModFlags_None;
    FitButton = ImGuiMouseButton_Left;
    ContextMenuButton = ImGuiMouseButton_Right;
    BoxSelectButton = ImGuiMouseButton_Right;
    BoxSelectMod = ImGuiKeyModFlags_None;
    BoxSelectCancelButton = ImGuiMouseButton_Left;
    QueryButton = ImGuiMouseButton_Middle;
    QueryMod = ImGuiKeyModFlags_None;
    QueryToggleMod = ImGuiKeyModFlags_Ctrl;
    HorizontalMod = ImGuiKeyModFlags_Alt;
    VerticalMod = ImGuiKeyModFlags_Shift;
}

namespace ImPlot {

    namespace {

        //-----------------------------------------------------------------------------
        // Private Utils
        //-----------------------------------------------------------------------------

        template <int Count>
        struct OffsetCalculator {
            OffsetCalculator(int* sizes) {
                Offsets[0] = 0;
                for (int i = 1; i < Count; ++i)
                    Offsets[i] = Offsets[i - 1] + sizes[i - 1];
            }
            int Offsets[Count];
        };

        template <typename T>
        void FillRange(ImVector<T>& buffer, int n, T vmin, T vmax) {
            buffer.resize(n);
            T step = (vmax - vmin) / (n - 1);
            for (int i = 0; i < n; ++i) {
                buffer[i] = vmin + i * step;
            }
        }

        // Returns true if a flag is set
        template <typename TSet, typename TFlag>
        inline bool HasFlag(TSet set, TFlag flag) {
            return (set & flag) == flag;
        }

        // Flips a flag in a flagset
        template <typename TSet, typename TFlag>
        inline void FlipFlag(TSet& set, TFlag flag) {
            HasFlag(set, flag) ? set &= ~flag : set |= flag;
        }

        // Linearly remaps x from [x0 x1] to [y0 y1].
        template <typename T>
        inline T Remap(T x, T x0, T x1, T y0, T y1) {
            return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
        }

        // Returns always positive modulo (r != 0)
        inline int PosMod(int l, int r) {
            return (l % r + r) % r;
        }

        // Returns the intersection point of two lines A and B (assumes they are not parallel!)
        inline ImVec2 Intersection(const ImVec2& a1, const ImVec2& a2, const ImVec2& b1, const ImVec2& b2) {
            float v1 = (a1.x * a2.y - a1.y * a2.x);
            float v2 = (b1.x * b2.y - b1.y * b2.x);
            float v3 = ((a1.x - a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x - b2.x));
            return ImVec2((v1 * (b1.x - b2.x) - v2 * (a1.x - a2.x)) / v3,
                (v1 * (b1.y - b2.y) - v2 * (a1.y - a2.y)) / v3);
        }

        // Turns NANs to 0s
        inline double ConstrainNan(double val) {
            return isnan(val) ? 0 : val;
        }

        // Turns infinity to floating point maximums
        inline double ConstrainInf(double val) {
            return val == HUGE_VAL ? DBL_MAX : val == -HUGE_VAL ? -DBL_MAX : val;
        }

        // Turns numbers less than or equal to 0 to 0.001 (sort of arbitrary, is there a better way?)
        inline double ConstrainLog(double val) {
            return val <= 0 ? 0.001f : val;
        }

        // Returns true if val is NAN or INFINITY
        inline bool NanOrInf(double val) {
            return val == HUGE_VAL || val == -HUGE_VAL || isnan(val);
        }

        // Computes order of magnitude of double.
        inline int OrderOfMagnitude(double val) {
            return val == 0 ? 0 : (int)(floor(log10(fabs(val))));
        }

        // Returns the precision required for a order of magnitude.
        inline int OrderToPrecision(int order) {
            return order > 0 ? 0 : 1 - order;
        }

        // Returns a floating point precision to use given a value
        inline int Precision(double val) {
            return OrderToPrecision(OrderOfMagnitude(val));
        }

        // Draws vertical text. The position is the bottom left of the text rect.
        inline void AddTextVertical(ImDrawList* DrawList, const char* text, ImVec2 pos, ImU32 text_color) {
            pos.x = IM_ROUND(pos.x);
            pos.y = IM_ROUND(pos.y);
            ImFont* font = GImGui->Font;
            const ImFontGlyph* glyph;
            for (char c = *text++; c; c = *text++) {
                glyph = font->FindGlyph(c);
                if (!glyph)
                    continue;

                DrawList->PrimReserve(6, 4);
                DrawList->PrimQuadUV(
                    pos + ImVec2(glyph->Y0, -glyph->X0), pos + ImVec2(glyph->Y0, -glyph->X1),
                    pos + ImVec2(glyph->Y1, -glyph->X1), pos + ImVec2(glyph->Y1, -glyph->X0),

                    ImVec2(glyph->U0, glyph->V0), ImVec2(glyph->U1, glyph->V0),
                    ImVec2(glyph->U1, glyph->V1), ImVec2(glyph->U0, glyph->V1), text_color);
                pos.y -= glyph->AdvanceX;
            }
        }

        // Calculates the size of vertical text
        inline ImVec2 CalcTextSizeVertical(const char* text) {
            ImVec2 sz = ImGui::CalcTextSize(text);
            return ImVec2(sz.y, sz.x);
        }

        /// Returns white or black text given background color
        inline ImU32 CalcTextColor(const ImVec4& bg) {
            return (bg.x * 0.299 + bg.y * 0.587 + bg.z * 0.114) > 0.729 ? IM_COL32_BLACK : IM_COL32_WHITE;
        }

    } // private namespace

    //-----------------------------------------------------------------------------
    // Forwards
    //-----------------------------------------------------------------------------

    ImVec4 NextColor();

    //-----------------------------------------------------------------------------
    // Structs
    //-----------------------------------------------------------------------------

    // Tick mark info
    struct ImPlotTick {
        ImPlotTick(double value, bool major, bool render_label = true) {
            PlotPos = value;
            Major = major;
            RenderLabel = render_label;
            Labeled = false;
        }
        double PlotPos;
        float  PixelPos;
        ImVec2 Size;
        int    TextOffset;
        bool   Major;
        bool   RenderLabel;
        bool   Labeled;
    };

    // Axis state information that must persist after EndPlot
    struct ImPlotAxis {
        ImPlotAxis() {
            Dragging = false;
            Hovered = false;
            Range.Min = 0;
            Range.Max = 1;
            Flags = PreviousFlags = ImPlotAxisFlags_Default;
        }
        bool Dragging;
        bool Hovered;
        ImPlotRange Range;
        ImPlotAxisFlags Flags, PreviousFlags;
    };

    // Axis state information only needed between BeginPlot/EndPlot
    struct ImPlotAxisState {
        ImPlotAxis* Axis;
        bool HasRange;
        ImGuiCond RangeCond;
        bool Present;
        int PresentSoFar;
        bool Invert;
        bool LockMin;
        bool LockMax;
        bool Lock;
        ImPlotAxisState(ImPlotAxis& axis, bool has_range, ImGuiCond range_cond, bool present, int previous_present) :
            Axis(&axis),
            HasRange(has_range),
            RangeCond(range_cond),
            Present(present),
            PresentSoFar(previous_present + (Present ? 1 : 0)),
            Invert(HasFlag(Axis->Flags, ImPlotAxisFlags_Invert)),
            LockMin(HasFlag(Axis->Flags, ImPlotAxisFlags_LockMin) || (HasRange && RangeCond == ImGuiCond_Always)),
            LockMax(HasFlag(Axis->Flags, ImPlotAxisFlags_LockMax) || (HasRange && RangeCond == ImGuiCond_Always)),
            Lock(!Present || ((LockMin && LockMax) || (HasRange && RangeCond == ImGuiCond_Always)))
        {}

        ImPlotAxisState() :
            Axis(), HasRange(), RangeCond(), Present(), PresentSoFar(), Invert(), LockMin(), LockMax(), Lock()
        {}
    };

    struct ImPlotAxisColor {
        ImPlotAxisColor() : Major(), Minor(), Txt() {}
        ImU32 Major, Minor, Txt;
    };

    // State information for Plot items
    struct ImPlotItem {
        ImPlotItem() {
            Show = true;
            SeenThisFrame = false;
            Highlight = false;
            Color = NextColor();
            NameOffset = -1;
            ID = 0;
        }
        ~ImPlotItem() { ID = 0; }
        bool Show;
        bool SeenThisFrame;
        bool Highlight;
        ImVec4 Color;
        int NameOffset;
        ImGuiID ID;
    };

    // Holds Plot state information that must persist after EndPlot
    struct ImPlotState {
        ImPlotState() {
            Selecting = Querying = Queried = DraggingQuery = false;
            SelectStart = QueryStart = ImVec2(0, 0);
            Flags = PreviousFlags = ImPlotFlags_Default;
            ColorIdx = 0;
            CurrentYAxis = 0;
        }
        ImPool<ImPlotItem> Items;

        ImRect BB_Legend;
        ImVec2 SelectStart;
        bool Selecting;
        bool Querying;
        bool Queried;
        bool DraggingQuery;
        ImVec2 QueryStart;
        ImRect QueryRect; // relative to BB_Plot!!

        ImPlotAxis XAxis;
        ImPlotAxis YAxis[MAX_Y_AXES];

        ImPlotFlags Flags, PreviousFlags;
        int ColorIdx;
        int CurrentYAxis;
    };

    struct ImPlotNextPlotData {
        ImPlotNextPlotData() {
            HasXRange = false;
            ShowDefaultTicksX = true;
            for (int i = 0; i < MAX_Y_AXES; ++i) {
                HasYRange[i] = false;
                ShowDefaultTicksY[i] = true;
            }
        }
        ImGuiCond XRangeCond;
        ImGuiCond YRangeCond[MAX_Y_AXES];
        bool HasXRange;
        bool HasYRange[MAX_Y_AXES];
        ImPlotRange X;
        ImPlotRange Y[MAX_Y_AXES];
        bool ShowDefaultTicksX;
        bool ShowDefaultTicksY[MAX_Y_AXES];
    };

    // Holds Plot state information that must persist only between calls to BeginPlot()/EndPlot()
    struct ImPlotContext {
        ImPlotContext() : RenderX(), RenderY() {
            ChildWindowMade = false;
            Reset();
            SetColormap(ImPlotColormap_Default);
        }

        void Reset() {
            // end child window if it was made
            if (ChildWindowMade)
                ImGui::EndChild();
            ChildWindowMade = false;
            // reset the next plot data
            NextPlotData = ImPlotNextPlotData();
            // reset items count
            VisibleItemCount = 0;
            // reset legend items
            LegendIndices.shrink(0);
            LegendLabels.Buf.shrink(0);
            // reset ticks/labels
            XTicks.shrink(0);
            XTickLabels.Buf.shrink(0);
            for (int i = 0; i < 3; ++i) {
                YTicks[i].shrink(0);
                YTickLabels[i].Buf.shrink(0);
            }
            // reset extents
            FitX = false;
            ExtentsX.Min = HUGE_VAL;
            ExtentsX.Max = -HUGE_VAL;
            for (int i = 0; i < MAX_Y_AXES; i++) {
                ExtentsY[i].Min = HUGE_VAL;
                ExtentsY[i].Max = -HUGE_VAL;
                FitY[i] = false;
            }
            // reset digital plot items count
            DigitalPlotItemCnt = 0;
            DigitalPlotOffset = 0;
            // nullify plot
            CurrentPlot = NULL;
        }

        // ALl Plots
        ImPool<ImPlotState> Plots;
        // Current Plot
        ImPlotState* CurrentPlot;
        // Legend
        ImVector<int> LegendIndices;
        ImGuiTextBuffer LegendLabels;
        // Bounding regions
        ImRect BB_Frame;
        ImRect BB_Canvas;
        ImRect BB_Plot;
        // Cached Colors
        ImU32 Col_Frame, Col_Bg, Col_Border,
            Col_Txt, Col_TxtDis,
            Col_SlctBg, Col_SlctBd,
            Col_QryBg, Col_QryBd;
        ImPlotAxisColor Col_X;
        ImPlotAxisColor Col_Y[MAX_Y_AXES];
        ImPlotAxisState X;
        ImPlotAxisState Y[MAX_Y_AXES];
        // Tick marks
        ImVector<ImPlotTick> XTicks, YTicks[MAX_Y_AXES];
        ImGuiTextBuffer XTickLabels, YTickLabels[MAX_Y_AXES];
        float AxisLabelReference[MAX_Y_AXES];
        // Transformation cache
        ImRect PixelRange[MAX_Y_AXES];
        // linear scale (slope)
        double Mx;
        double My[MAX_Y_AXES];
        // log scale denominator
        double LogDenX;
        double LogDenY[MAX_Y_AXES];
        // Data extents
        ImPlotRange ExtentsX;
        ImPlotRange ExtentsY[MAX_Y_AXES];
        int VisibleItemCount;
        bool FitThisFrame; bool FitX;
        bool FitY[MAX_Y_AXES];
        // Hover states
        bool Hov_Frame;
        bool Hov_Plot;
        // Render flags
        bool RenderX, RenderY[MAX_Y_AXES];
        // Lock info
        bool LockPlot;
        bool ChildWindowMade;
        // Mouse pos
        ImPlotPoint LastMousePos[MAX_Y_AXES];
        // Style
        ImVec4* Colormap;
        int     ColormapSize;
        ImPlotStyle Style;
        ImPlotInputMap InputMap;
        ImVector<ImGuiColorMod> ColorModifiers;  // Stack for PushStyleColor()/PopStyleColor()
        ImVector<ImGuiStyleMod> StyleModifiers;  // Stack for PushStyleVar()/PopStyleVar()
        ImPlotNextPlotData NextPlotData;
        // Digital plot item count
        int DigitalPlotItemCnt;
        int DigitalPlotOffset;
    };

    // Global plot context
    static ImPlotContext gp;

    //-----------------------------------------------------------------------------
    // Context Utils
    //-----------------------------------------------------------------------------

    ImPlotInputMap& GetInputMap() {
        return gp.InputMap;
    }

    // Returns the next unused default plot color
    ImVec4 NextColor() {
        ImVec4 col = gp.Colormap[gp.CurrentPlot->ColorIdx % gp.ColormapSize];
        gp.CurrentPlot->ColorIdx++;
        return col;
    }

    inline void FitPoint(const ImPlotPoint& p) {
        ImPlotRange* extents_x = &gp.ExtentsX;
        ImPlotRange* extents_y = &gp.ExtentsY[gp.CurrentPlot->CurrentYAxis];
        if (!NanOrInf(p.x)) {
            extents_x->Min = p.x < extents_x->Min ? p.x : extents_x->Min;
            extents_x->Max = p.x > extents_x->Max ? p.x : extents_x->Max;
        }
        if (!NanOrInf(p.y)) {
            extents_y->Min = p.y < extents_y->Min ? p.y : extents_y->Min;
            extents_y->Max = p.y > extents_y->Max ? p.y : extents_y->Max;
        }
    }

    //-----------------------------------------------------------------------------
    // Coordinate Transforms
    //-----------------------------------------------------------------------------

    inline void UpdateTransformCache() {
        // get pixels for transforms
        for (int i = 0; i < MAX_Y_AXES; i++) {
            gp.PixelRange[i] = ImRect(HasFlag(gp.CurrentPlot->XAxis.Flags, ImPlotAxisFlags_Invert) ? gp.BB_Plot.Max.x : gp.BB_Plot.Min.x,
                HasFlag(gp.CurrentPlot->YAxis[i].Flags, ImPlotAxisFlags_Invert) ? gp.BB_Plot.Min.y : gp.BB_Plot.Max.y,
                HasFlag(gp.CurrentPlot->XAxis.Flags, ImPlotAxisFlags_Invert) ? gp.BB_Plot.Min.x : gp.BB_Plot.Max.x,
                HasFlag(gp.CurrentPlot->YAxis[i].Flags, ImPlotAxisFlags_Invert) ? gp.BB_Plot.Max.y : gp.BB_Plot.Min.y);

            gp.My[i] = (gp.PixelRange[i].Max.y - gp.PixelRange[i].Min.y) / gp.CurrentPlot->YAxis[i].Range.Size();
        }
        gp.LogDenX = ImLog10(gp.CurrentPlot->XAxis.Range.Max / gp.CurrentPlot->XAxis.Range.Min);
        for (int i = 0; i < MAX_Y_AXES; i++) {
            gp.LogDenY[i] = ImLog10(gp.CurrentPlot->YAxis[i].Range.Max / gp.CurrentPlot->YAxis[i].Range.Min);
        }
        gp.Mx = (gp.PixelRange[0].Max.x - gp.PixelRange[0].Min.x) / gp.CurrentPlot->XAxis.Range.Size();
    }

    inline ImPlotPoint PixelsToPlot(float x, float y, int y_axis_in = -1) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PixelsToPlot() needs to be called between BeginPlot() and EndPlot()!");
        const int y_axis = y_axis_in >= 0 ? y_axis_in : gp.CurrentPlot->CurrentYAxis;
        ImPlotPoint plt;
        plt.x = (x - gp.PixelRange[y_axis].Min.x) / gp.Mx + gp.CurrentPlot->XAxis.Range.Min;
        plt.y = (y - gp.PixelRange[y_axis].Min.y) / gp.My[y_axis] + gp.CurrentPlot->YAxis[y_axis].Range.Min;
        if (HasFlag(gp.CurrentPlot->XAxis.Flags, ImPlotAxisFlags_LogScale)) {
            double t = (plt.x - gp.CurrentPlot->XAxis.Range.Min) / gp.CurrentPlot->XAxis.Range.Size();
            plt.x = ImPow(10, t * gp.LogDenX) * gp.CurrentPlot->XAxis.Range.Min;
        }
        if (HasFlag(gp.CurrentPlot->YAxis[y_axis].Flags, ImPlotAxisFlags_LogScale)) {
            double t = (plt.y - gp.CurrentPlot->YAxis[y_axis].Range.Min) / gp.CurrentPlot->YAxis[y_axis].Range.Size();
            plt.y = ImPow(10, t * gp.LogDenY[y_axis]) * gp.CurrentPlot->YAxis[y_axis].Range.Min;
        }
        return plt;
    }

    ImPlotPoint PixelsToPlot(const ImVec2& pix, int y_axis) {
        return PixelsToPlot(pix.x, pix.y, y_axis);
    }

    // This function is convenient but should not be used to process a high volume of points. Use the Transformer structs below instead.
    inline ImVec2 PlotToPixels(double x, double y, int y_axis_in = -1) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PlotToPixels() needs to be called between BeginPlot() and EndPlot()!");
        const int y_axis = y_axis_in >= 0 ? y_axis_in : gp.CurrentPlot->CurrentYAxis;
        ImVec2 pix;
        if (HasFlag(gp.CurrentPlot->XAxis.Flags, ImPlotAxisFlags_LogScale)) {
            double t = ImLog10(x / gp.CurrentPlot->XAxis.Range.Min) / gp.LogDenX;
            x = ImLerp(gp.CurrentPlot->XAxis.Range.Min, gp.CurrentPlot->XAxis.Range.Max, (float)t);
        }
        if (HasFlag(gp.CurrentPlot->YAxis[y_axis].Flags, ImPlotAxisFlags_LogScale)) {
            double t = ImLog10(y / gp.CurrentPlot->YAxis[y_axis].Range.Min) / gp.LogDenY[y_axis];
            y = ImLerp(gp.CurrentPlot->YAxis[y_axis].Range.Min, gp.CurrentPlot->YAxis[y_axis].Range.Max, (float)t);
        }
        pix.x = (float)(gp.PixelRange[y_axis].Min.x + gp.Mx * (x - gp.CurrentPlot->XAxis.Range.Min));
        pix.y = (float)(gp.PixelRange[y_axis].Min.y + gp.My[y_axis] * (y - gp.CurrentPlot->YAxis[y_axis].Range.Min));
        return pix;
    }

    // This function is convenient but should not be used to process a high volume of points. Use the Transformer structs below instead.
    ImVec2 PlotToPixels(const ImPlotPoint& plt, int y_axis) {
        return PlotToPixels(plt.x, plt.y, y_axis);
    }

    //-----------------------------------------------------------------------------
    // Legend Utils
    //-----------------------------------------------------------------------------

    ImPlotItem* RegisterItem(const char* label_id) {
        ImGuiID id = ImGui::GetID(label_id);
        ImPlotItem* item = gp.CurrentPlot->Items.GetOrAddByKey(id);
        if (item->SeenThisFrame)
            return item;
        item->SeenThisFrame = true;
        int idx = gp.CurrentPlot->Items.GetIndex(item);
        item->ID = id;
        if (ImGui::FindRenderedTextEnd(label_id, NULL) != label_id) {
            gp.LegendIndices.push_back(idx);
            item->NameOffset = gp.LegendLabels.size();
            gp.LegendLabels.append(label_id, label_id + strlen(label_id) + 1);
        }
        else {
            item->Show = true;
        }
        if (item->Show)
            gp.VisibleItemCount++;
        return item;
    }

    int GetLegendCount() {
        return gp.LegendIndices.size();
    }

    ImPlotItem* GetLegendItem(int i) {
        return gp.CurrentPlot->Items.GetByIndex(gp.LegendIndices[i]);
    }

    ImPlotItem* GetLegendItem(const char* label_id) {
        ImGuiID id = ImGui::GetID(label_id);
        return gp.CurrentPlot->Items.GetByKey(id);
    }

    const char* GetLegendLabel(int i) {
        ImPlotItem* item = gp.CurrentPlot->Items.GetByIndex(gp.LegendIndices[i]);
        IM_ASSERT(item->NameOffset != -1 && item->NameOffset < gp.LegendLabels.Buf.Size);
        return gp.LegendLabels.Buf.Data + item->NameOffset;
    }

    //-----------------------------------------------------------------------------
    // Tick Utils
    //-----------------------------------------------------------------------------

    // Utility function to that rounds x to powers of 2,5 and 10 for generating axis labels
    // Taken from Graphics Gems 1 Chapter 11.2, "Nice Numbers for Graph Labels"
    inline double NiceNum(double x, bool round) {
        double f;  /* fractional part of x */
        double nf; /* nice, rounded fraction */
        int expv = (int)floor(ImLog10(x));
        f = x / ImPow(10.0, (double)expv); /* between 1 and 10 */
        if (round)
            if (f < 1.5)
                nf = 1;
            else if (f < 3)
                nf = 2;
            else if (f < 7)
                nf = 5;
            else
                nf = 10;
        else if (f <= 1)
            nf = 1;
        else if (f <= 2)
            nf = 2;
        else if (f <= 5)
            nf = 5;
        else
            nf = 10;
        return nf * ImPow(10.0, expv);
    }

    inline void AddDefaultTicks(const ImPlotRange& range, int nMajor, int nMinor, bool logscale, ImVector<ImPlotTick>& out) {
        if (logscale) {
            if (range.Min <= 0 || range.Max <= 0)
                return;
            int exp_min = (int)ImLog10(range.Min);
            int exp_max = (int)(ceil(ImLog10(range.Max)));
            for (int e = exp_min - 1; e < exp_max + 1; ++e) {
                double major1 = ImPow(10, (double)(e));
                double major2 = ImPow(10, (double)(e + 1));
                double interval = (major2 - major1) / 9;
                if (major1 >= (range.Min - DBL_EPSILON) && major1 <= (range.Max + DBL_EPSILON))
                    out.push_back(ImPlotTick(major1, true));
                for (int i = 1; i < 9; ++i) {
                    double minor = major1 + i * interval;
                    if (minor >= (range.Min - DBL_EPSILON) && minor <= (range.Max + DBL_EPSILON))
                        out.push_back(ImPlotTick(minor, false, false));
                }
            }
        }
        else {
            const double nice_range = NiceNum(range.Size() * 0.99, 0);
            const double interval = NiceNum(nice_range / (nMajor - 1), 1);
            const double graphmin = floor(range.Min / interval) * interval;
            const double graphmax = ceil(range.Max / interval) * interval;
            for (double major = graphmin; major < graphmax + 0.5 * interval; major += interval) {
                if (major >= range.Min && major <= range.Max)
                    out.push_back(ImPlotTick(major, true));
                for (int i = 1; i < nMinor; ++i) {
                    double minor = major + i * interval / nMinor;
                    if (minor >= range.Min && minor <= range.Max)
                        out.push_back(ImPlotTick(minor, false));
                }
            }
        }
    }

    inline void AddCustomTicks(const double* values, const char** labels, int n, ImVector<ImPlotTick>& ticks, ImGuiTextBuffer& buffer) {
        for (int i = 0; i < n; ++i) {
            ImPlotTick tick(values[i], false);
            tick.TextOffset = buffer.size();
            if (labels != NULL) {
                buffer.append(labels[i], labels[i] + strlen(labels[i]) + 1);
                tick.Size = ImGui::CalcTextSize(labels[i]);
                tick.Labeled = true;
            }
            ticks.push_back(tick);
        }
    }

    inline void LabelTicks(ImVector<ImPlotTick>& ticks, bool scientific, ImGuiTextBuffer& buffer) {
        char temp[32];
        for (int t = 0; t < ticks.Size; t++) {
            ImPlotTick* tk = &ticks[t];
            if (tk->RenderLabel && !tk->Labeled) {
                tk->TextOffset = buffer.size();
                if (scientific)
                    sprintf(temp, "%.0e", tk->PlotPos);
                else
                    sprintf(temp, "%.10g", tk->PlotPos);
                buffer.append(temp, temp + strlen(temp) + 1);
                tk->Size = ImGui::CalcTextSize(buffer.Buf.Data + tk->TextOffset);
                tk->Labeled = true;
            }
        }
    }

    inline float MaxTickLabelWidth(ImVector<ImPlotTick>& ticks) {
        float w = 0;
        for (int i = 0; i < ticks.Size; ++i)
            w = ticks[i].Size.x > w ? ticks[i].Size.x : w;
        return w;
    }

    class YPadCalculator {
    public:
        YPadCalculator(const ImPlotAxisState* axis_states, const float* max_label_widths, float txt_off)
            : ImPlotAxisStates(axis_states), MaxLabelWidths(max_label_widths), TxtOff(txt_off) {}

        float operator()(int y_axis) {
            ImPlotState& plot = *gp.CurrentPlot;
            if (!ImPlotAxisStates[y_axis].Present) { return 0; }
            // If we have more than 1 axis present before us, then we need
            // extra space to account for our tick bar.
            float pad_result = 0;
            if (ImPlotAxisStates[y_axis].PresentSoFar >= 3) {
                pad_result += 6.0f;
            }
            if (!HasFlag(plot.YAxis[y_axis].Flags, ImPlotAxisFlags_TickLabels)) {
                return pad_result;
            }
            pad_result += MaxLabelWidths[y_axis] + TxtOff;
            return pad_result;
        }

    private:
        const ImPlotAxisState* const ImPlotAxisStates;
        const float* const MaxLabelWidths;
        const float TxtOff;
    };

    //-----------------------------------------------------------------------------
    // Axis Utils
    //-----------------------------------------------------------------------------

    void UpdateAxisColor(int axis_flag, ImPlotAxisColor* col) {
        const ImVec4 col_Axis = gp.Style.Colors[axis_flag].w == -1 ? ImGui::GetStyle().Colors[ImGuiCol_Text] * ImVec4(1, 1, 1, 0.25f) : gp.Style.Colors[axis_flag];
        col->Major = ImGui::GetColorU32(col_Axis);
        col->Minor = ImGui::GetColorU32(col_Axis * ImVec4(1, 1, 1, 0.25f));
        col->Txt = ImGui::GetColorU32(ImVec4(col_Axis.x, col_Axis.y, col_Axis.z, 1));
    }

    struct ImPlotAxisScale {
        ImPlotAxisScale(int y_axis, float tx, float ty, float zoom_rate) {
            Min = PixelsToPlot(gp.BB_Plot.Min - gp.BB_Plot.GetSize() * ImVec2(tx * zoom_rate, ty * zoom_rate), y_axis);
            Max = PixelsToPlot(gp.BB_Plot.Max + gp.BB_Plot.GetSize() * ImVec2((1 - tx) * zoom_rate, (1 - ty) * zoom_rate), y_axis);
        }
        ImPlotPoint Min, Max;
    };

    //-----------------------------------------------------------------------------
    // BeginPlot()
    //-----------------------------------------------------------------------------

    bool BeginPlot(const char* title, const char* x_label, const char* y_label, const ImVec2& size, ImPlotFlags flags, ImPlotAxisFlags x_flags, ImPlotAxisFlags y_flags, ImPlotAxisFlags y2_flags, ImPlotAxisFlags y3_flags) {

        IM_ASSERT_USER_ERROR(gp.CurrentPlot == NULL, "Mismatched BeginPlot()/EndPlot()!");

        // FRONT MATTER  -----------------------------------------------------------

        ImGuiContext& G = *GImGui;
        ImGuiWindow* Window = G.CurrentWindow;
        if (Window->SkipItems) {
            gp.Reset();
            return false;
        }

        const ImGuiID     ID = Window->GetID(title);
        const ImGuiStyle& Style = G.Style;
        const ImGuiIO& IO = ImGui::GetIO();

        bool just_created = gp.Plots.GetByKey(ID) == NULL;
        gp.CurrentPlot = gp.Plots.GetOrAddByKey(ID);
        ImPlotState& plot = *gp.CurrentPlot;

        plot.CurrentYAxis = 0;

        if (just_created) {
            plot.Flags = flags;
            plot.XAxis.Flags = x_flags;
            plot.YAxis[0].Flags = y_flags;
            plot.YAxis[1].Flags = y2_flags;
            plot.YAxis[2].Flags = y3_flags;
        }
        else {
            // TODO: Check which individual flags changed, and only reset those!
            // There's probably an easy bit mask trick I'm not aware of.
            if (flags != plot.PreviousFlags)
                plot.Flags = flags;
            if (y_flags != plot.YAxis[0].PreviousFlags)
                plot.YAxis[0].PreviousFlags = y_flags;
            if (y2_flags != plot.YAxis[1].PreviousFlags)
                plot.YAxis[1].PreviousFlags = y2_flags;
            if (y3_flags != plot.YAxis[2].PreviousFlags)
                plot.YAxis[2].PreviousFlags = y3_flags;
        }

        plot.PreviousFlags = flags;
        plot.XAxis.PreviousFlags = x_flags;
        plot.YAxis[0].PreviousFlags = y_flags;
        plot.YAxis[1].PreviousFlags = y2_flags;
        plot.YAxis[2].PreviousFlags = y3_flags;

        // capture scroll with a child region
        const float default_w = 400;
        const float default_h = 300;
        if (!HasFlag(plot.Flags, ImPlotFlags_NoChild)) {
            ImGui::BeginChild(title, ImVec2(size.x == 0 ? default_w : size.x, size.y == 0 ? default_h : size.y));
            Window = ImGui::GetCurrentWindow();
            Window->ScrollMax.y = 1.0f;
            gp.ChildWindowMade = true;
        }
        else {
            gp.ChildWindowMade = false;
        }

        ImDrawList& DrawList = *Window->DrawList;

        // NextPlotData -----------------------------------------------------------

        if (gp.NextPlotData.HasXRange) {
            if (just_created || gp.NextPlotData.XRangeCond == ImGuiCond_Always)
            {
                plot.XAxis.Range = gp.NextPlotData.X;
            }
        }

        for (int i = 0; i < MAX_Y_AXES; i++) {
            if (gp.NextPlotData.HasYRange[i]) {
                if (just_created || gp.NextPlotData.YRangeCond[i] == ImGuiCond_Always)
                {
                    plot.YAxis[i].Range = gp.NextPlotData.Y[i];
                }
            }
        }

        // AXIS STATES ------------------------------------------------------------
        gp.X = ImPlotAxisState(plot.XAxis, gp.NextPlotData.HasXRange, gp.NextPlotData.XRangeCond, true, 0);
        gp.Y[0] = ImPlotAxisState(plot.YAxis[0], gp.NextPlotData.HasYRange[0], gp.NextPlotData.YRangeCond[0], true, 0);
        gp.Y[1] = ImPlotAxisState(plot.YAxis[1], gp.NextPlotData.HasYRange[1], gp.NextPlotData.YRangeCond[1],
            HasFlag(plot.Flags, ImPlotFlags_YAxis2), gp.Y[0].PresentSoFar);
        gp.Y[2] = ImPlotAxisState(plot.YAxis[2], gp.NextPlotData.HasYRange[2], gp.NextPlotData.YRangeCond[2],
            HasFlag(plot.Flags, ImPlotFlags_YAxis3), gp.Y[1].PresentSoFar);

        gp.LockPlot = gp.X.Lock && gp.Y[0].Lock && gp.Y[1].Lock && gp.Y[2].Lock;

        // CONSTRAINTS ------------------------------------------------------------

        plot.XAxis.Range.Min = ConstrainNan(ConstrainInf(plot.XAxis.Range.Min));
        plot.XAxis.Range.Max = ConstrainNan(ConstrainInf(plot.XAxis.Range.Max));
        for (int i = 0; i < MAX_Y_AXES; i++) {
            plot.YAxis[i].Range.Min = ConstrainNan(ConstrainInf(plot.YAxis[i].Range.Min));
            plot.YAxis[i].Range.Max = ConstrainNan(ConstrainInf(plot.YAxis[i].Range.Max));
        }

        if (HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_LogScale))
            plot.XAxis.Range.Min = ConstrainLog(plot.XAxis.Range.Min);
        if (HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_LogScale))
            plot.XAxis.Range.Max = ConstrainLog(plot.XAxis.Range.Max);
        for (int i = 0; i < MAX_Y_AXES; i++) {
            if (HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_LogScale))
                plot.YAxis[i].Range.Min = ConstrainLog(plot.YAxis[i].Range.Min);
            if (HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_LogScale))
                plot.YAxis[i].Range.Max = ConstrainLog(plot.YAxis[i].Range.Max);
        }

        if (plot.XAxis.Range.Max <= plot.XAxis.Range.Min)
            plot.XAxis.Range.Max = plot.XAxis.Range.Min + DBL_EPSILON;
        for (int i = 0; i < MAX_Y_AXES; i++) {
            if (plot.YAxis[i].Range.Max <= plot.YAxis[i].Range.Min)
                plot.YAxis[i].Range.Max = plot.YAxis[i].Range.Min + DBL_EPSILON;
        }

        // COLORS -----------------------------------------------------------------

        gp.Col_Frame = gp.Style.Colors[ImPlotCol_FrameBg].w == -1 ? ImGui::GetColorU32(ImGuiCol_FrameBg) : ImGui::GetColorU32(gp.Style.Colors[ImPlotCol_FrameBg]);
        gp.Col_Bg = gp.Style.Colors[ImPlotCol_PlotBg].w == -1 ? ImGui::GetColorU32(ImGuiCol_WindowBg) : ImGui::GetColorU32(gp.Style.Colors[ImPlotCol_PlotBg]);
        gp.Col_Border = gp.Style.Colors[ImPlotCol_PlotBorder].w == -1 ? ImGui::GetColorU32(ImGuiCol_Text, 0.5f) : ImGui::GetColorU32(gp.Style.Colors[ImPlotCol_PlotBorder]);

        UpdateAxisColor(ImPlotCol_XAxis, &gp.Col_X);
        UpdateAxisColor(ImPlotCol_YAxis, &gp.Col_Y[0]);
        UpdateAxisColor(ImPlotCol_YAxis2, &gp.Col_Y[1]);
        UpdateAxisColor(ImPlotCol_YAxis3, &gp.Col_Y[2]);

        gp.Col_Txt = ImGui::GetColorU32(ImGuiCol_Text);
        gp.Col_TxtDis = ImGui::GetColorU32(ImGuiCol_TextDisabled);
        gp.Col_SlctBg = ImGui::GetColorU32(gp.Style.Colors[ImPlotCol_Selection] * ImVec4(1, 1, 1, 0.25f));
        gp.Col_SlctBd = ImGui::GetColorU32(gp.Style.Colors[ImPlotCol_Selection]);
        gp.Col_QryBg = ImGui::GetColorU32(gp.Style.Colors[ImPlotCol_Query] * ImVec4(1, 1, 1, 0.25f));
        gp.Col_QryBd = ImGui::GetColorU32(gp.Style.Colors[ImPlotCol_Query]);

        // BB AND HOVER -----------------------------------------------------------

        // frame
        const ImVec2 frame_size = ImGui::CalcItemSize(size, default_w, default_h);
        gp.BB_Frame = ImRect(Window->DC.CursorPos, Window->DC.CursorPos + frame_size);
        ImGui::ItemSize(gp.BB_Frame);
        if (!ImGui::ItemAdd(gp.BB_Frame, 0, &gp.BB_Frame)) {
            gp.Reset();
            return false;
        }
        gp.Hov_Frame = ImGui::ItemHoverable(gp.BB_Frame, ID);
        ImGui::RenderFrame(gp.BB_Frame.Min, gp.BB_Frame.Max, gp.Col_Frame, true, Style.FrameRounding);

        // canvas bb
        gp.BB_Canvas = ImRect(gp.BB_Frame.Min + Style.WindowPadding, gp.BB_Frame.Max - Style.WindowPadding);

        // adaptive divisions
        int x_divisions = ImMax(2, (int)IM_ROUND(0.003 * gp.BB_Canvas.GetWidth()));
        int y_divisions[MAX_Y_AXES];
        for (int i = 0; i < MAX_Y_AXES; i++) {
            y_divisions[i] = ImMax(2, (int)IM_ROUND(0.003 * gp.BB_Canvas.GetHeight()));
        }

        gp.RenderX = (HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_GridLines) ||
            HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_TickMarks) ||
            HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_TickLabels)) && x_divisions > 1;
        for (int i = 0; i < MAX_Y_AXES; i++) {
            gp.RenderY[i] =
                gp.Y[i].Present &&
                (HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_GridLines) ||
                    HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_TickMarks) ||
                    HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_TickLabels)) && y_divisions[i] > 1;
        }
        // get ticks
        if (gp.RenderX && gp.NextPlotData.ShowDefaultTicksX)
            AddDefaultTicks(plot.XAxis.Range, x_divisions, 10, HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_LogScale), gp.XTicks);
        for (int i = 0; i < MAX_Y_AXES; i++) {
            if (gp.RenderY[i] && gp.NextPlotData.ShowDefaultTicksY[i]) {
                AddDefaultTicks(plot.YAxis[i].Range, y_divisions[i], 10, HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_LogScale), gp.YTicks[i]);
            }
        }

        // label ticks
        if (HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_TickLabels))
            LabelTicks(gp.XTicks, HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_Scientific), gp.XTickLabels);

        float max_label_width[MAX_Y_AXES] = {};
        for (int i = 0; i < MAX_Y_AXES; i++) {
            if (gp.Y[i].Present && HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_TickLabels)) {
                LabelTicks(gp.YTicks[i], HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_Scientific), gp.YTickLabels[i]);
                max_label_width[i] = MaxTickLabelWidth(gp.YTicks[i]);
            }
        }

        // grid bb
        const ImVec2 title_size = ImGui::CalcTextSize(title, NULL, true);
        const float txt_off = 5;
        const float txt_height = ImGui::GetTextLineHeight();
        const float pad_top = title_size.x > 0.0f ? txt_height + txt_off : 0;
        const float pad_bot = (HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_TickLabels) ? txt_height + txt_off : 0) + (x_label ? txt_height + txt_off : 0);
        YPadCalculator y_axis_pad(gp.Y, max_label_width, txt_off);
        const float pad_left = y_axis_pad(0) + (y_label ? txt_height + txt_off : 0);
        const float pad_right = y_axis_pad(1) + y_axis_pad(2);
        gp.BB_Plot = ImRect(gp.BB_Canvas.Min + ImVec2(pad_left, pad_top), gp.BB_Canvas.Max - ImVec2(pad_right, pad_bot));
        gp.Hov_Plot = gp.BB_Plot.Contains(IO.MousePos);

        // axis region bbs
        const ImRect xAxisRegion_bb(gp.BB_Plot.Min + ImVec2(10, 0), ImVec2(gp.BB_Plot.Max.x, gp.BB_Frame.Max.y) - ImVec2(10, 0));
        plot.XAxis.Hovered = xAxisRegion_bb.Contains(IO.MousePos);

        // The left labels are referenced to the left of the bounding box.
        gp.AxisLabelReference[0] = gp.BB_Plot.Min.x;
        // If Y axis 1 is present, its labels will be referenced to the
        // right of the bounding box.
        gp.AxisLabelReference[1] = gp.BB_Plot.Max.x;
        // The third axis may be either referenced to the right of the
        // bounding box, or 6 pixels further past the end of the 2nd axis.
        gp.AxisLabelReference[2] =
            !gp.Y[1].Present ?
            gp.BB_Plot.Max.x :
            (gp.AxisLabelReference[1] + y_axis_pad(1) + 6);

        ImRect yAxisRegion_bb[MAX_Y_AXES];
        yAxisRegion_bb[0] = ImRect(ImVec2(gp.BB_Frame.Min.x, gp.BB_Plot.Min.y), ImVec2(gp.BB_Plot.Min.x + 6, gp.BB_Plot.Max.y - 10));
        // The auxiliary y axes are off to the right of the BB grid.
        yAxisRegion_bb[1] = ImRect(ImVec2(gp.BB_Plot.Max.x - 6, gp.BB_Plot.Min.y), gp.BB_Plot.Max + ImVec2(y_axis_pad(1), -10));
        yAxisRegion_bb[2] = ImRect(ImVec2(gp.AxisLabelReference[2] - 6, gp.BB_Plot.Min.y), yAxisRegion_bb[1].Max + ImVec2(y_axis_pad(2), -10));

        ImRect centralRegion(ImVec2(gp.BB_Plot.Min.x + 6, gp.BB_Plot.Min.y), ImVec2(gp.BB_Plot.Max.x - 6, gp.BB_Plot.Max.y));

        plot.YAxis[0].Hovered = gp.Y[0].Present && (yAxisRegion_bb[0].Contains(IO.MousePos) || centralRegion.Contains(IO.MousePos));
        plot.YAxis[1].Hovered = gp.Y[1].Present && (yAxisRegion_bb[1].Contains(IO.MousePos) || centralRegion.Contains(IO.MousePos));
        plot.YAxis[2].Hovered = gp.Y[2].Present && (yAxisRegion_bb[2].Contains(IO.MousePos) || centralRegion.Contains(IO.MousePos));

        const bool any_hov_y_axis_region = plot.YAxis[0].Hovered || plot.YAxis[1].Hovered || plot.YAxis[2].Hovered;

        // legend hovered from last frame
        const bool hov_legend = HasFlag(plot.Flags, ImPlotFlags_Legend) ? gp.Hov_Frame && plot.BB_Legend.Contains(IO.MousePos) : false;

        bool hov_query = false;
        if (gp.Hov_Frame && gp.Hov_Plot && plot.Queried && !plot.Querying) {
            ImRect bb_query = plot.QueryRect;

            bb_query.Min += gp.BB_Plot.Min;
            bb_query.Max += gp.BB_Plot.Min;

            hov_query = bb_query.Contains(IO.MousePos);
        }

        // QUERY DRAG -------------------------------------------------------------
        if (plot.DraggingQuery && (IO.MouseReleased[gp.InputMap.PanButton] || !IO.MouseDown[gp.InputMap.PanButton])) {
            plot.DraggingQuery = false;
        }
        if (plot.DraggingQuery) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
            plot.QueryRect.Min += IO.MouseDelta;
            plot.QueryRect.Max += IO.MouseDelta;
        }
        if (gp.Hov_Frame && gp.Hov_Plot && hov_query && !plot.DraggingQuery && !plot.Selecting && !hov_legend) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
            const bool any_y_dragging = plot.YAxis[0].Dragging || plot.YAxis[1].Dragging || plot.YAxis[2].Dragging;
            if (IO.MouseDown[gp.InputMap.PanButton] && !plot.XAxis.Dragging && !any_y_dragging) {
                plot.DraggingQuery = true;
            }
        }

        // DRAG INPUT -------------------------------------------------------------

        // end drags
        if (plot.XAxis.Dragging && (IO.MouseReleased[gp.InputMap.PanButton] || !IO.MouseDown[gp.InputMap.PanButton])) {
            plot.XAxis.Dragging = false;
            G.IO.MouseDragMaxDistanceSqr[0] = 0;
        }
        for (int i = 0; i < MAX_Y_AXES; i++) {
            if (plot.YAxis[i].Dragging && (IO.MouseReleased[gp.InputMap.PanButton] || !IO.MouseDown[gp.InputMap.PanButton])) {
                plot.YAxis[i].Dragging = false;
                G.IO.MouseDragMaxDistanceSqr[0] = 0;
            }
        }
        const bool any_y_dragging = plot.YAxis[0].Dragging || plot.YAxis[1].Dragging || plot.YAxis[2].Dragging;
        bool drag_in_progress = plot.XAxis.Dragging || any_y_dragging;
        // do drag
        if (drag_in_progress) {
            UpdateTransformCache();
            if (!gp.X.Lock && plot.XAxis.Dragging) {
                ImPlotPoint plot_tl = PixelsToPlot(gp.BB_Plot.Min - IO.MouseDelta, 0);
                ImPlotPoint plot_br = PixelsToPlot(gp.BB_Plot.Max - IO.MouseDelta, 0);
                if (!gp.X.LockMin)
                    plot.XAxis.Range.Min = gp.X.Invert ? plot_br.x : plot_tl.x;
                if (!gp.X.LockMax)
                    plot.XAxis.Range.Max = gp.X.Invert ? plot_tl.x : plot_br.x;
            }
            for (int i = 0; i < MAX_Y_AXES; i++) {
                if (!gp.Y[i].Lock && plot.YAxis[i].Dragging) {
                    ImPlotPoint plot_tl = PixelsToPlot(gp.BB_Plot.Min - IO.MouseDelta, i);
                    ImPlotPoint plot_br = PixelsToPlot(gp.BB_Plot.Max - IO.MouseDelta, i);

                    if (!gp.Y[i].LockMin)
                        plot.YAxis[i].Range.Min = gp.Y[i].Invert ? plot_tl.y : plot_br.y;
                    if (!gp.Y[i].LockMax)
                        plot.YAxis[i].Range.Max = gp.Y[i].Invert ? plot_br.y : plot_tl.y;
                }
            }
            // Set the mouse cursor based on which axes are moving.
            int direction = 0;
            if (!gp.X.Lock && plot.XAxis.Dragging) {
                direction |= (1 << 1);
            }
            for (int i = 0; i < MAX_Y_AXES; i++) {
                if (!gp.Y[i].Present) { continue; }
                if (!gp.Y[i].Lock && plot.YAxis[i].Dragging) {
                    direction |= (1 << 2);
                    break;
                }
            }
            if (IO.MouseDragMaxDistanceSqr[0] > 5) {
                if (direction == 0) {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_NotAllowed);
                }
                else if (direction == (1 << 1)) {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
                }
                else if (direction == (1 << 2)) {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
                }
                else {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
                }
            }
        }
        // start drag
        if (!drag_in_progress && gp.Hov_Frame && IO.MouseClicked[gp.InputMap.PanButton] && HasFlag(IO.KeyMods, gp.InputMap.PanMod) && !plot.Selecting && !hov_legend && !hov_query && !plot.DraggingQuery) {
            if (plot.XAxis.Hovered) {
                plot.XAxis.Dragging = true;
            }
            for (int i = 0; i < MAX_Y_AXES; i++) {
                if (plot.YAxis[i].Hovered) {
                    plot.YAxis[i].Dragging = true;
                }
            }
        }

        // SCROLL INPUT -----------------------------------------------------------

        if (gp.Hov_Frame && (plot.XAxis.Hovered || any_hov_y_axis_region) && IO.MouseWheel != 0) {
            UpdateTransformCache();
            float zoom_rate = 0.1f;
            if (IO.MouseWheel > 0)
                zoom_rate = (-zoom_rate) / (1.0f + (2.0f * zoom_rate));
            float tx = Remap(IO.MousePos.x, gp.BB_Plot.Min.x, gp.BB_Plot.Max.x, 0.0f, 1.0f);
            float ty = Remap(IO.MousePos.y, gp.BB_Plot.Min.y, gp.BB_Plot.Max.y, 0.0f, 1.0f);
            if (plot.XAxis.Hovered && !gp.X.Lock) {
                ImPlotAxisScale axis_scale(0, tx, ty, zoom_rate);
                const ImPlotPoint& plot_tl = axis_scale.Min;
                const ImPlotPoint& plot_br = axis_scale.Max;

                if (!gp.X.LockMin)
                    plot.XAxis.Range.Min = gp.X.Invert ? plot_br.x : plot_tl.x;
                if (!gp.X.LockMax)
                    plot.XAxis.Range.Max = gp.X.Invert ? plot_tl.x : plot_br.x;
            }
            for (int i = 0; i < MAX_Y_AXES; i++) {
                if (plot.YAxis[i].Hovered && !gp.Y[i].Lock) {
                    ImPlotAxisScale axis_scale(i, tx, ty, zoom_rate);
                    const ImPlotPoint& plot_tl = axis_scale.Min;
                    const ImPlotPoint& plot_br = axis_scale.Max;
                    if (!gp.Y[i].LockMin)
                        plot.YAxis[i].Range.Min = gp.Y[i].Invert ? plot_tl.y : plot_br.y;
                    if (!gp.Y[i].LockMax)
                        plot.YAxis[i].Range.Max = gp.Y[i].Invert ? plot_br.y : plot_tl.y;
                }
            }
        }

        // BOX-SELECTION AND QUERY ------------------------------------------------

        // confirm selection
        if (plot.Selecting && (IO.MouseReleased[gp.InputMap.BoxSelectButton] || !IO.MouseDown[gp.InputMap.BoxSelectButton])) {
            UpdateTransformCache();
            ImVec2 select_size = plot.SelectStart - IO.MousePos;
            if (HasFlag(plot.Flags, ImPlotFlags_BoxSelect) && ImFabs(select_size.x) > 2 && ImFabs(select_size.y) > 2) {
                ImPlotPoint p1 = PixelsToPlot(plot.SelectStart);
                ImPlotPoint p2 = PixelsToPlot(IO.MousePos);
                if (!gp.X.LockMin && IO.KeyMods != gp.InputMap.HorizontalMod)
                    plot.XAxis.Range.Min = ImMin(p1.x, p2.x);
                if (!gp.X.LockMax && IO.KeyMods != gp.InputMap.HorizontalMod)
                    plot.XAxis.Range.Max = ImMax(p1.x, p2.x);
                for (int i = 0; i < MAX_Y_AXES; i++) {
                    p1 = PixelsToPlot(plot.SelectStart, i);
                    p2 = PixelsToPlot(IO.MousePos, i);
                    if (!gp.Y[i].LockMin && IO.KeyMods != gp.InputMap.VerticalMod)
                        plot.YAxis[i].Range.Min = ImMin(p1.y, p2.y);
                    if (!gp.Y[i].LockMax && IO.KeyMods != gp.InputMap.VerticalMod)
                        plot.YAxis[i].Range.Max = ImMax(p1.y, p2.y);
                }
            }
            plot.Selecting = false;
        }
        // bad selection
        if (plot.Selecting && (!HasFlag(plot.Flags, ImPlotFlags_BoxSelect) || gp.LockPlot) && ImLengthSqr(plot.SelectStart - IO.MousePos) > 4) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_NotAllowed);
        }
        // cancel selection
        if (plot.Selecting && (IO.MouseClicked[gp.InputMap.BoxSelectCancelButton] || IO.MouseDown[gp.InputMap.BoxSelectCancelButton])) {
            plot.Selecting = false;
        }
        // begin selection or query
        if (gp.Hov_Frame && gp.Hov_Plot && IO.MouseClicked[gp.InputMap.BoxSelectButton] && HasFlag(IO.KeyMods, gp.InputMap.BoxSelectMod)) {
            plot.SelectStart = IO.MousePos;
            plot.Selecting = true;
        }
        // update query
        if (plot.Querying) {
            UpdateTransformCache();
            plot.QueryRect.Min.x = HasFlag(IO.KeyMods, gp.InputMap.HorizontalMod) ? gp.BB_Plot.Min.x : ImMin(plot.QueryStart.x, IO.MousePos.x);
            plot.QueryRect.Max.x = HasFlag(IO.KeyMods, gp.InputMap.HorizontalMod) ? gp.BB_Plot.Max.x : ImMax(plot.QueryStart.x, IO.MousePos.x);
            plot.QueryRect.Min.y = HasFlag(IO.KeyMods, gp.InputMap.VerticalMod) ? gp.BB_Plot.Min.y : ImMin(plot.QueryStart.y, IO.MousePos.y);
            plot.QueryRect.Max.y = HasFlag(IO.KeyMods, gp.InputMap.VerticalMod) ? gp.BB_Plot.Max.y : ImMax(plot.QueryStart.y, IO.MousePos.y);

            plot.QueryRect.Min -= gp.BB_Plot.Min;
            plot.QueryRect.Max -= gp.BB_Plot.Min;
        }
        // end query
        if (plot.Querying && (IO.MouseReleased[gp.InputMap.QueryButton] || IO.MouseReleased[gp.InputMap.BoxSelectButton])) {
            plot.Querying = false;
            if (plot.QueryRect.GetWidth() > 2 && plot.QueryRect.GetHeight() > 2) {
                plot.Queried = true;
            }
            else {
                plot.Queried = false;
            }
        }

        // begin query
        if (HasFlag(plot.Flags, ImPlotFlags_Query) && gp.Hov_Frame && gp.Hov_Plot && IO.MouseClicked[gp.InputMap.QueryButton] && HasFlag(IO.KeyMods, gp.InputMap.QueryMod)) {
            plot.QueryRect = ImRect(0, 0, 0, 0);
            plot.Querying = true;
            plot.Queried = true;
            plot.QueryStart = IO.MousePos;
        }
        // toggle between select/query
        if (HasFlag(plot.Flags, ImPlotFlags_Query) && plot.Selecting && HasFlag(IO.KeyMods, gp.InputMap.QueryToggleMod)) {
            plot.Selecting = false;
            plot.QueryRect = ImRect(0, 0, 0, 0);
            plot.Querying = true;
            plot.Queried = true;
            plot.QueryStart = plot.SelectStart;
        }
        if (HasFlag(plot.Flags, ImPlotFlags_BoxSelect) && plot.Querying && !HasFlag(IO.KeyMods, gp.InputMap.QueryToggleMod) && !IO.MouseDown[gp.InputMap.QueryButton]) {
            plot.Selecting = true;
            plot.Querying = false;
            plot.Queried = false;
            plot.QueryRect = ImRect(0, 0, 0, 0);
        }

        // DOUBLE CLICK -----------------------------------------------------------

        if (IO.MouseDoubleClicked[gp.InputMap.FitButton] && gp.Hov_Frame && (plot.XAxis.Hovered || any_hov_y_axis_region) && !hov_legend && !hov_query) {
            gp.FitThisFrame = true;
            gp.FitX = plot.XAxis.Hovered;
            for (int i = 0; i < MAX_Y_AXES; i++) {
                gp.FitY[i] = plot.YAxis[i].Hovered;
            }
        }
        else {
            gp.FitThisFrame = false;
            gp.FitX = false;
            for (int i = 0; i < MAX_Y_AXES; i++) {
                gp.FitY[i] = false;
            }
        }

        // FOCUS ------------------------------------------------------------------

        // focus window
        if ((IO.MouseClicked[0] || IO.MouseClicked[1] || IO.MouseClicked[2]) && gp.Hov_Frame)
            ImGui::FocusWindow(ImGui::GetCurrentWindow());

        UpdateTransformCache();

        // set mouse position
        for (int i = 0; i < MAX_Y_AXES; i++) {
            gp.LastMousePos[i] = PixelsToPlot(IO.MousePos, i);
        }

        // RENDER -----------------------------------------------------------------

        // grid bg
        DrawList.AddRectFilled(gp.BB_Plot.Min, gp.BB_Plot.Max, gp.Col_Bg);

        // render axes
        PushPlotClipRect();

        // transform ticks
        if (gp.RenderX) {
            for (int t = 0; t < gp.XTicks.Size; t++) {
                ImPlotTick* xt = &gp.XTicks[t];
                xt->PixelPos = PlotToPixels(xt->PlotPos, 0, 0).x;
            }
        }
        for (int i = 0; i < MAX_Y_AXES; i++) {
            if (gp.RenderY[i]) {
                for (int t = 0; t < gp.YTicks[i].Size; t++) {
                    ImPlotTick* yt = &gp.YTicks[i][t];
                    yt->PixelPos = PlotToPixels(0, yt->PlotPos, i).y;
                }
            }
        }

        // render grid
        if (HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_GridLines)) {
            for (int t = 0; t < gp.XTicks.Size; t++) {
                ImPlotTick* xt = &gp.XTicks[t];
                DrawList.AddLine(ImVec2(xt->PixelPos, gp.BB_Plot.Min.y), ImVec2(xt->PixelPos, gp.BB_Plot.Max.y), xt->Major ? gp.Col_X.Major : gp.Col_X.Minor, 1);
            }
        }

        for (int i = 0; i < MAX_Y_AXES; i++) {
            if (gp.Y[i].Present && HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_GridLines)) {
                for (int t = 0; t < gp.YTicks[i].Size; t++) {
                    ImPlotTick* yt = &gp.YTicks[i][t];
                    DrawList.AddLine(ImVec2(gp.BB_Plot.Min.x, yt->PixelPos), ImVec2(gp.BB_Plot.Max.x, yt->PixelPos), yt->Major ? gp.Col_Y[i].Major : gp.Col_Y[i].Minor, 1);
                }
            }
        }

        PopPlotClipRect();

        // render title
        if (title_size.x > 0.0f) {
            ImGui::RenderText(ImVec2(gp.BB_Canvas.GetCenter().x - title_size.x * 0.5f, gp.BB_Canvas.Min.y), title, NULL, true);
        }

        // render labels
        if (HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_TickLabels)) {
            ImGui::PushClipRect(gp.BB_Frame.Min, gp.BB_Frame.Max, true);
            for (int t = 0; t < gp.XTicks.Size; t++) {
                ImPlotTick* xt = &gp.XTicks[t];
                if (xt->RenderLabel && xt->PixelPos >= gp.BB_Plot.Min.x - 1 && xt->PixelPos <= gp.BB_Plot.Max.x + 1)
                    DrawList.AddText(ImVec2(xt->PixelPos - xt->Size.x * 0.5f, gp.BB_Plot.Max.y + txt_off), gp.Col_X.Txt, gp.XTickLabels.Buf.Data + xt->TextOffset);
            }
            ImGui::PopClipRect();
        }
        if (x_label) {
            const ImVec2 xLabel_size = ImGui::CalcTextSize(x_label);
            const ImVec2 xLabel_pos(gp.BB_Plot.GetCenter().x - xLabel_size.x * 0.5f,
                gp.BB_Canvas.Max.y - txt_height);
            DrawList.AddText(xLabel_pos, gp.Col_X.Txt, x_label);
        }
        ImGui::PushClipRect(gp.BB_Frame.Min, gp.BB_Frame.Max, true);
        for (int i = 0; i < MAX_Y_AXES; i++) {
            if (gp.Y[i].Present && HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_TickLabels)) {
                for (int t = 0; t < gp.YTicks[i].Size; t++) {
                    const float x_start = gp.AxisLabelReference[i] + (i == 0 ? (-txt_off - gp.YTicks[i][t].Size.x) : txt_off);
                    ImPlotTick* yt = &gp.YTicks[i][t];
                    if (yt->RenderLabel && yt->PixelPos >= gp.BB_Plot.Min.y - 1 && yt->PixelPos <= gp.BB_Plot.Max.y + 1) {
                        ImVec2 start(x_start, yt->PixelPos - 0.5f * yt->Size.y);
                        DrawList.AddText(start, gp.Col_Y[i].Txt, gp.YTickLabels[i].Buf.Data + yt->TextOffset);
                    }
                }
            }
        }
        ImGui::PopClipRect();
        if (y_label) {
            const ImVec2 yLabel_size = CalcTextSizeVertical(y_label);
            const ImVec2 yLabel_pos(gp.BB_Canvas.Min.x, gp.BB_Plot.GetCenter().y + yLabel_size.y * 0.5f);
            AddTextVertical(&DrawList, y_label, yLabel_pos, gp.Col_Y[0].Txt);
        }

        // push plot ID into stack
        ImGui::PushID(ID);
        return true;
    }

    //-----------------------------------------------------------------------------
    // Context Menu
    //-----------------------------------------------------------------------------

    template <typename F>
    bool DragFloat(const char* label, F* v, float v_speed, F v_min, F v_max) {
        return false;
    }

    template <>
    bool DragFloat<double>(const char* label, double* v, float v_speed, double v_min, double v_max) {
        return ImGui::DragScalar(label, ImGuiDataType_Double, v, v_speed, &v_min, &v_max, "%.3f", 1);
    }

    template <>
    bool DragFloat<float>(const char* label, float* v, float v_speed, float v_min, float v_max) {
        return ImGui::DragScalar(label, ImGuiDataType_Float, v, v_speed, &v_min, &v_max, "%.3f", 1);
    }

    inline void BeginDisabledControls(bool cond) {
        if (cond) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
        }
    }

    inline void EndDisabledControls(bool cond) {
        if (cond) {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }
    }

    inline void AxisMenu(ImPlotAxisState& state) {
        ImGui::PushItemWidth(75);
        bool total_lock = state.HasRange && state.RangeCond == ImGuiCond_Always;
        bool logscale = HasFlag(state.Axis->Flags, ImPlotAxisFlags_LogScale);
        bool grid = HasFlag(state.Axis->Flags, ImPlotAxisFlags_GridLines);
        bool ticks = HasFlag(state.Axis->Flags, ImPlotAxisFlags_TickMarks);
        bool labels = HasFlag(state.Axis->Flags, ImPlotAxisFlags_TickLabels);

        BeginDisabledControls(total_lock);
        if (ImGui::Checkbox("##LockMin", &state.LockMin))
            FlipFlag(state.Axis->Flags, ImPlotAxisFlags_LockMin);
        EndDisabledControls(total_lock);

        ImGui::SameLine();
        BeginDisabledControls(state.LockMin);
        DragFloat("Min", &state.Axis->Range.Min, 0.01f * (float)state.Axis->Range.Size(), -HUGE_VAL, state.Axis->Range.Max - DBL_EPSILON);
        EndDisabledControls(state.LockMin);

        BeginDisabledControls(total_lock);
        if (ImGui::Checkbox("##LockMax", &state.LockMax))
            FlipFlag(state.Axis->Flags, ImPlotAxisFlags_LockMax);
        EndDisabledControls(total_lock);

        ImGui::SameLine();
        BeginDisabledControls(state.LockMax);
        DragFloat("Max", &state.Axis->Range.Max, 0.01f * (float)state.Axis->Range.Size(), state.Axis->Range.Min + DBL_EPSILON, HUGE_VAL);
        EndDisabledControls(state.LockMax);

        ImGui::Separator();

        if (ImGui::Checkbox("Invert", &state.Invert))
            FlipFlag(state.Axis->Flags, ImPlotAxisFlags_Invert);
        if (ImGui::Checkbox("Log Scale", &logscale))
            FlipFlag(state.Axis->Flags, ImPlotAxisFlags_LogScale);
        ImGui::Separator();
        if (ImGui::Checkbox("Grid Lines", &grid))
            FlipFlag(state.Axis->Flags, ImPlotAxisFlags_GridLines);
        if (ImGui::Checkbox("Tick Marks", &ticks))
            FlipFlag(state.Axis->Flags, ImPlotAxisFlags_TickMarks);
        if (ImGui::Checkbox("Labels", &labels))
            FlipFlag(state.Axis->Flags, ImPlotAxisFlags_TickLabels);

    }

    void PlotContextMenu(ImPlotState& plot) {
        if (ImGui::BeginMenu("X-Axis")) {
            ImGui::PushID("X");
            AxisMenu(gp.X);
            ImGui::PopID();
            ImGui::EndMenu();
        }
        for (int i = 0; i < MAX_Y_AXES; i++) {
            if (i == 1 && !HasFlag(plot.Flags, ImPlotFlags_YAxis2)) {
                continue;
            }
            if (i == 2 && !HasFlag(plot.Flags, ImPlotFlags_YAxis3)) {
                continue;
            }
            char buf[10] = {};
            if (i == 0) {
                snprintf(buf, sizeof(buf) - 1, "Y-Axis");
            }
            else {
                snprintf(buf, sizeof(buf) - 1, "Y-Axis %d", i + 1);
            }
            if (ImGui::BeginMenu(buf)) {
                ImGui::PushID(i);
                AxisMenu(gp.Y[i]);
                ImGui::PopID();
                ImGui::EndMenu();
            }
        }

        ImGui::Separator();
        if ((ImGui::BeginMenu("Settings"))) {
            if (ImGui::MenuItem("Box Select", NULL, HasFlag(plot.Flags, ImPlotFlags_BoxSelect))) {
                FlipFlag(plot.Flags, ImPlotFlags_BoxSelect);
            }
            if (ImGui::MenuItem("Query", NULL, HasFlag(plot.Flags, ImPlotFlags_Query))) {
                FlipFlag(plot.Flags, ImPlotFlags_Query);
            }
            if (ImGui::MenuItem("Crosshairs", NULL, HasFlag(plot.Flags, ImPlotFlags_Crosshairs))) {
                FlipFlag(plot.Flags, ImPlotFlags_Crosshairs);
            }
            if (ImGui::MenuItem("Mouse Position", NULL, HasFlag(plot.Flags, ImPlotFlags_MousePos))) {
                FlipFlag(plot.Flags, ImPlotFlags_MousePos);
            }
            if (ImGui::MenuItem("Anti-Aliased Lines", NULL, HasFlag(plot.Flags, ImPlotFlags_AntiAliased))) {
                FlipFlag(plot.Flags, ImPlotFlags_AntiAliased);
            }
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Legend", NULL, HasFlag(plot.Flags, ImPlotFlags_Legend))) {
            FlipFlag(plot.Flags, ImPlotFlags_Legend);
        }
#if 0
        if (ImGui::BeginMenu("Metrics")) {
            ImGui::PushItemWidth(75);
            ImGui::LabelText("Plots", "%d", gp.Plots.GetSize());
            ImGui::LabelText("Color Modifiers", "%d", gp.ColorModifiers.size());
            ImGui::LabelText("Style Modifiers", "%d", gp.StyleModifiers.size());
            ImGui::PopItemWidth();
            ImGui::EndMenu();
        }
#endif

    }

    namespace {
        class BufferWriter {
        public:
            BufferWriter(char* buffer, size_t size) : Buffer(buffer), Pos(0), Size(size) {}

            void Write(const char* fmt, ...) IM_FMTARGS(2) {
                va_list argp;
                va_start(argp, fmt);
                VWrite(fmt, argp);
                va_end(argp);
            }

        private:
            void VWrite(const char* fmt, va_list argp) {
                const int written = ::vsnprintf(&Buffer[Pos], Size - Pos - 1, fmt, argp);
                if (written > 0)
                    Pos += ImMin(size_t(written), Size - Pos - 1);
            }

            char* const Buffer;
            size_t Pos;
            const size_t Size;
        };
    }

    //-----------------------------------------------------------------------------
    // EndPlot()
    //-----------------------------------------------------------------------------

    void EndPlot() {

        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "Mismatched BeginPlot()/EndPlot()!");

        ImPlotState& plot = *gp.CurrentPlot;
        ImGuiContext& G = *GImGui;
        ImGuiWindow* Window = G.CurrentWindow;
        ImDrawList& DrawList = *Window->DrawList;
        const ImGuiIO& IO = ImGui::GetIO();

        // AXIS STATES ------------------------------------------------------------

        const bool any_y_locked = gp.Y[0].Lock || gp.Y[1].Present ? gp.Y[1].Lock : false || gp.Y[2].Present ? gp.Y[2].Lock : false;
        const bool any_y_dragging = plot.YAxis[0].Dragging || plot.YAxis[1].Dragging || plot.YAxis[2].Dragging;


        // FINAL RENDER -----------------------------------------------------------

        // render ticks
        PushPlotClipRect();
        if (HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_TickMarks)) {
            for (int t = 0; t < gp.XTicks.Size; t++) {
                ImPlotTick* xt = &gp.XTicks[t];
                DrawList.AddLine(ImVec2(xt->PixelPos, gp.BB_Plot.Max.y), ImVec2(xt->PixelPos, gp.BB_Plot.Max.y - (xt->Major ? 10.0f : 5.0f)), gp.Col_Border, 1);
            }
        }
        PopPlotClipRect();

        ImGui::PushClipRect(gp.BB_Plot.Min, ImVec2(gp.BB_Frame.Max.x, gp.BB_Plot.Max.y), true);
        int axis_count = 0;
        for (int i = 0; i < MAX_Y_AXES; i++) {
            if (!gp.Y[i].Present) { continue; }
            axis_count++;

            float x_start = gp.AxisLabelReference[i];
            if (HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_TickMarks)) {
                float direction = (i == 0) ? 1.0f : -1.0f;
                bool no_major = axis_count >= 3;

                for (int t = 0; t < gp.YTicks[i].Size; t++) {
                    ImPlotTick* yt = &gp.YTicks[i][t];
                    ImVec2 start = ImVec2(x_start, yt->PixelPos);

                    DrawList.AddLine(
                        start,
                        start + ImVec2(direction * ((!no_major && yt->Major) ? 10.0f : 5.0f), 0),
                        gp.Col_Border, 1);
                }
            }

            if (axis_count >= 3) {
                // Draw a bar next to the ticks to act as a visual separator.
                DrawList.AddLine(
                    ImVec2(x_start, gp.BB_Plot.Min.y),
                    ImVec2(x_start, gp.BB_Plot.Max.y),
                    gp.Col_Border, 1);
            }
        }
        ImGui::PopClipRect();

        // render y-axis drag/drop hover 
        if (ImGui::IsDragDropPayloadBeingAccepted()) {
            ImRect bb_plot_pad = gp.BB_Plot;
            bb_plot_pad.Min.x += 5; bb_plot_pad.Max.x -= 5;
            if (!bb_plot_pad.Contains(IO.MousePos)) {
                for (int i = 0; i < MAX_Y_AXES; ++i) {
                    if (plot.YAxis[i].Hovered) {
                        float x_loc = gp.AxisLabelReference[i];
                        ImVec2 p1(x_loc - 5, gp.BB_Plot.Min.y - 5);
                        ImVec2 p2(x_loc + 5, gp.BB_Plot.Max.y + 5);
                        DrawList.AddRect(p1, p2, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, ImDrawCornerFlags_All, 2.0f);
                    }
                }
            }
        }


        PushPlotClipRect();
        // render selection/query
        if (plot.Selecting) {
            ImRect select_bb(ImMin(IO.MousePos, plot.SelectStart), ImMax(IO.MousePos, plot.SelectStart));
            bool select_big_enough = ImLengthSqr(select_bb.GetSize()) > 4;
            if (plot.Selecting && !gp.LockPlot && HasFlag(plot.Flags, ImPlotFlags_BoxSelect) && select_big_enough) {
                if (IO.KeyAlt && IO.KeyShift) {
                    DrawList.AddRectFilled(gp.BB_Plot.Min, gp.BB_Plot.Max, gp.Col_SlctBg);
                    DrawList.AddRect(gp.BB_Plot.Min, gp.BB_Plot.Max, gp.Col_SlctBd);
                }
                else if ((gp.X.Lock || IO.KeyAlt)) {
                    DrawList.AddRectFilled(ImVec2(gp.BB_Plot.Min.x, select_bb.Min.y), ImVec2(gp.BB_Plot.Max.x, select_bb.Max.y), gp.Col_SlctBg);
                    DrawList.AddRect(ImVec2(gp.BB_Plot.Min.x, select_bb.Min.y), ImVec2(gp.BB_Plot.Max.x, select_bb.Max.y), gp.Col_SlctBd);
                }
                else if ((any_y_locked || IO.KeyShift)) {
                    DrawList.AddRectFilled(ImVec2(select_bb.Min.x, gp.BB_Plot.Min.y), ImVec2(select_bb.Max.x, gp.BB_Plot.Max.y), gp.Col_SlctBg);
                    DrawList.AddRect(ImVec2(select_bb.Min.x, gp.BB_Plot.Min.y), ImVec2(select_bb.Max.x, gp.BB_Plot.Max.y), gp.Col_SlctBd);
                }
                else {
                    DrawList.AddRectFilled(select_bb.Min, select_bb.Max, gp.Col_SlctBg);
                    DrawList.AddRect(select_bb.Min, select_bb.Max, gp.Col_SlctBd);
                }
            }
        }

        if (plot.Querying || plot.Queried) {
            if (plot.QueryRect.GetWidth() > 2 && plot.QueryRect.GetHeight() > 2) {
                DrawList.AddRectFilled(plot.QueryRect.Min + gp.BB_Plot.Min, plot.QueryRect.Max + gp.BB_Plot.Min, gp.Col_QryBg);
                DrawList.AddRect(plot.QueryRect.Min + gp.BB_Plot.Min, plot.QueryRect.Max + gp.BB_Plot.Min, gp.Col_QryBd);
            }
        }
        else if (plot.Queried) {
            ImRect bb_query = plot.QueryRect;

            bb_query.Min += gp.BB_Plot.Min;
            bb_query.Max += gp.BB_Plot.Min;

            DrawList.AddRectFilled(bb_query.Min, bb_query.Max, gp.Col_QryBg);
            DrawList.AddRect(bb_query.Min, bb_query.Max, gp.Col_QryBd);
        }

        // render legend
        const float txt_ht = ImGui::GetTextLineHeight();
        const ImVec2 legend_offset(10, 10);
        const ImVec2 legend_padding(5, 5);
        const float  legend_icon_size = txt_ht;
        ImRect legend_content_bb;
        int nItems = GetLegendCount();
        bool hov_legend = false;
        if (HasFlag(plot.Flags, ImPlotFlags_Legend) && nItems > 0) {
            // get max width
            float max_label_width = 0;
            for (int i = 0; i < nItems; ++i) {
                const char* label = GetLegendLabel(i);
                ImVec2 labelWidth = ImGui::CalcTextSize(label, NULL, true);
                max_label_width = labelWidth.x > max_label_width ? labelWidth.x : max_label_width;
            }
            legend_content_bb = ImRect(gp.BB_Plot.Min + legend_offset, gp.BB_Plot.Min + legend_offset + ImVec2(max_label_width, nItems * txt_ht));
            plot.BB_Legend = ImRect(legend_content_bb.Min, legend_content_bb.Max + legend_padding * 2 + ImVec2(legend_icon_size, 0));
            hov_legend = HasFlag(plot.Flags, ImPlotFlags_Legend) ? gp.Hov_Frame && plot.BB_Legend.Contains(IO.MousePos) : false;
            // render legend box
            DrawList.AddRectFilled(plot.BB_Legend.Min, plot.BB_Legend.Max, ImGui::GetColorU32(ImGuiCol_PopupBg));
            DrawList.AddRect(plot.BB_Legend.Min, plot.BB_Legend.Max, gp.Col_Border);
            // render each legend item
            for (int i = 0; i < nItems; ++i) {
                ImPlotItem* item = GetLegendItem(i);
                ImRect icon_bb;
                icon_bb.Min = legend_content_bb.Min + legend_padding + ImVec2(0, i * txt_ht) + ImVec2(2, 2);
                icon_bb.Max = legend_content_bb.Min + legend_padding + ImVec2(0, i * txt_ht) + ImVec2(legend_icon_size - 2, legend_icon_size - 2);
                ImRect label_bb;
                label_bb.Min = legend_content_bb.Min + legend_padding + ImVec2(0, i * txt_ht) + ImVec2(2, 2);
                label_bb.Max = legend_content_bb.Min + legend_padding + ImVec2(0, i * txt_ht) + ImVec2(legend_content_bb.Max.x, legend_icon_size - 2);
                ImU32 col_hl_txt;
                if (HasFlag(plot.Flags, ImPlotFlags_Highlight) && hov_legend && (icon_bb.Contains(IO.MousePos) || label_bb.Contains(IO.MousePos))) {
                    item->Highlight = true;
                    col_hl_txt = ImGui::GetColorU32(ImLerp(G.Style.Colors[ImGuiCol_Text], item->Color, 0.25f));
                }
                else
                {
                    item->Highlight = false;
                    col_hl_txt = gp.Col_Txt;
                }
                ImU32 iconColor;
                if (hov_legend && icon_bb.Contains(IO.MousePos)) {
                    ImVec4 colAlpha = item->Color;
                    colAlpha.w = 0.5f;
                    iconColor = item->Show ? ImGui::GetColorU32(colAlpha)
                        : ImGui::GetColorU32(ImGuiCol_TextDisabled, 0.5f);
                    if (IO.MouseClicked[0])
                        item->Show = !item->Show;
                }
                else {
                    iconColor = item->Show ? ImGui::GetColorU32(item->Color) : gp.Col_TxtDis;
                }
                DrawList.AddRectFilled(icon_bb.Min, icon_bb.Max, iconColor, 1);
                const char* label = GetLegendLabel(i);
                const char* text_display_end = ImGui::FindRenderedTextEnd(label, NULL);
                if (label != text_display_end)
                    DrawList.AddText(legend_content_bb.Min + legend_padding + ImVec2(legend_icon_size, i * txt_ht), item->Show ? col_hl_txt : gp.Col_TxtDis, label, text_display_end);
            }
        }

        // render crosshairs
        if (HasFlag(plot.Flags, ImPlotFlags_Crosshairs) && gp.Hov_Plot && gp.Hov_Frame &&
            !(plot.XAxis.Dragging || any_y_dragging) && !plot.Selecting && !plot.Querying && !hov_legend) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);
            ImVec2 xy = IO.MousePos;
            ImVec2 h1(gp.BB_Plot.Min.x, xy.y);
            ImVec2 h2(xy.x - 5, xy.y);
            ImVec2 h3(xy.x + 5, xy.y);
            ImVec2 h4(gp.BB_Plot.Max.x, xy.y);
            ImVec2 v1(xy.x, gp.BB_Plot.Min.y);
            ImVec2 v2(xy.x, xy.y - 5);
            ImVec2 v3(xy.x, xy.y + 5);
            ImVec2 v4(xy.x, gp.BB_Plot.Max.y);
            DrawList.AddLine(h1, h2, gp.Col_Border);
            DrawList.AddLine(h3, h4, gp.Col_Border);
            DrawList.AddLine(v1, v2, gp.Col_Border);
            DrawList.AddLine(v3, v4, gp.Col_Border);
        }

        // render mouse pos
        if (HasFlag(plot.Flags, ImPlotFlags_MousePos) && gp.Hov_Plot) {
            char buffer[128] = {};
            BufferWriter writer(buffer, sizeof(buffer));

            double range_x = gp.XTicks.Size > 1 ? (gp.XTicks[1].PlotPos - gp.XTicks[0].PlotPos) : plot.XAxis.Range.Size();
            double range_y = gp.YTicks[0].Size > 1 ? (gp.YTicks[0][1].PlotPos - gp.YTicks[0][0].PlotPos) : plot.YAxis[0].Range.Size();

            writer.Write("%.*f,%.*f", Precision(range_x), gp.LastMousePos[0].x, Precision(range_y), gp.LastMousePos[0].y);
            if (HasFlag(plot.Flags, ImPlotFlags_YAxis2)) {
                range_y = gp.YTicks[1].Size > 1 ? (gp.YTicks[1][1].PlotPos - gp.YTicks[1][0].PlotPos) : plot.YAxis[1].Range.Size();
                writer.Write(",(%.*f)", Precision(range_y), gp.LastMousePos[1].y);
            }
            if (HasFlag(plot.Flags, ImPlotFlags_YAxis3)) {
                range_y = gp.YTicks[2].Size > 1 ? (gp.YTicks[2][1].PlotPos - gp.YTicks[2][0].PlotPos) : plot.YAxis[2].Range.Size();
                writer.Write(",(%.*f)", Precision(range_y), gp.LastMousePos[2].y);
            }
            ImVec2 size = ImGui::CalcTextSize(buffer);
            ImVec2 pos = gp.BB_Plot.Max - size - ImVec2(5, 5);
            DrawList.AddText(pos, gp.Col_Txt, buffer);
        }

        PopPlotClipRect();

        // render border
        DrawList.AddRect(gp.BB_Plot.Min, gp.BB_Plot.Max, gp.Col_Border);

        // FIT DATA --------------------------------------------------------------

        if (gp.FitThisFrame && (gp.VisibleItemCount > 0 || plot.Queried)) {
            if (gp.FitX && !HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_LockMin) && !NanOrInf(gp.ExtentsX.Min)) {
                plot.XAxis.Range.Min = gp.ExtentsX.Min;
            }
            if (gp.FitX && !HasFlag(plot.XAxis.Flags, ImPlotAxisFlags_LockMax) && !NanOrInf(gp.ExtentsX.Max)) {
                plot.XAxis.Range.Max = gp.ExtentsX.Max;
            }
            for (int i = 0; i < MAX_Y_AXES; i++) {
                if (gp.FitY[i] && !HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_LockMin) && !NanOrInf(gp.ExtentsY[i].Min)) {
                    plot.YAxis[i].Range.Min = gp.ExtentsY[i].Min;
                }
                if (gp.FitY[i] && !HasFlag(plot.YAxis[i].Flags, ImPlotAxisFlags_LockMax) && !NanOrInf(gp.ExtentsY[i].Max)) {
                    plot.YAxis[i].Range.Max = gp.ExtentsY[i].Max;
                }
            }
        }

        // CONTEXT MENU -----------------------------------------------------------

        if (HasFlag(plot.Flags, ImPlotFlags_ContextMenu) && gp.Hov_Frame && gp.Hov_Plot && IO.MouseDoubleClicked[gp.InputMap.ContextMenuButton] && !hov_legend)
            ImGui::OpenPopup("##Context");
        if (ImGui::BeginPopup("##Context")) {
            PlotContextMenu(plot);
            ImGui::EndPopup();
        }
        // CLEANUP ----------------------------------------------------------------

        // reset the plot items for the next frame
        for (int i = 0; i < gp.CurrentPlot->Items.GetSize(); ++i) {
            gp.CurrentPlot->Items.GetByIndex(i)->SeenThisFrame = false;
        }

        // Pop ImGui::PushID at the end of BeginPlot
        ImGui::PopID();
        // Reset context for next plot
        gp.Reset();
    }

    //-----------------------------------------------------------------------------
    // MISC API
    //-----------------------------------------------------------------------------

    void SetNextPlotLimits(double x_min, double x_max, double y_min, double y_max, ImGuiCond cond) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot == NULL, "SetNextPlotLimits() needs to be called before BeginPlot()!");
        SetNextPlotLimitsX(x_min, x_max, cond);
        SetNextPlotLimitsY(y_min, y_max, cond);
    }

    void SetNextPlotLimitsX(double x_min, double x_max, ImGuiCond cond) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot == NULL, "SetNextPlotLSetNextPlotLimitsXimitsY() needs to be called before BeginPlot()!");
        IM_ASSERT(cond == 0 || ImIsPowerOfTwo(cond)); // Make sure the user doesn't attempt to combine multiple condition flags.
        gp.NextPlotData.HasXRange = true;
        gp.NextPlotData.XRangeCond = cond;
        gp.NextPlotData.X.Min = x_min;
        gp.NextPlotData.X.Max = x_max;
    }

    void SetNextPlotLimitsY(double y_min, double y_max, ImGuiCond cond, int y_axis) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot == NULL, "SetNextPlotLimitsY() needs to be called before BeginPlot()!");
        IM_ASSERT_USER_ERROR(y_axis >= 0 && y_axis < MAX_Y_AXES, "y_axis needs to be between 0 and MAX_Y_AXES");
        IM_ASSERT(cond == 0 || ImIsPowerOfTwo(cond)); // Make sure the user doesn't attempt to combine multiple condition flags.
        gp.NextPlotData.HasYRange[y_axis] = true;
        gp.NextPlotData.YRangeCond[y_axis] = cond;
        gp.NextPlotData.Y[y_axis].Min = y_min;
        gp.NextPlotData.Y[y_axis].Max = y_max;
    }

    void SetNextPlotTicksX(const double* values, int n_ticks, const char** labels, bool show_default) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot == NULL, "SetNextPlotTicksX() needs to be called before BeginPlot()!");
        gp.NextPlotData.ShowDefaultTicksX = show_default;
        AddCustomTicks(values, labels, n_ticks, gp.XTicks, gp.XTickLabels);
    }

    void SetNextPlotTicksX(double x_min, double x_max, int n_ticks, const char** labels, bool show_default) {
        IM_ASSERT_USER_ERROR(n_ticks > 1, "The number of ticks must be greater than 1");
        static ImVector<double> buffer;
        FillRange(buffer, n_ticks, x_min, x_max);
        SetNextPlotTicksX(&buffer[0], n_ticks, labels, show_default);
    }

    void SetNextPlotTicksY(const double* values, int n_ticks, const char** labels, bool show_default, int y_axis) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot == NULL, "SetNextPlotTicksY() needs to be called before BeginPlot()!");
        IM_ASSERT_USER_ERROR(y_axis >= 0 && y_axis < MAX_Y_AXES, "y_axis needs to be between 0 and MAX_Y_AXES");
        gp.NextPlotData.ShowDefaultTicksY[y_axis] = show_default;
        AddCustomTicks(values, labels, n_ticks, gp.YTicks[y_axis], gp.YTickLabels[y_axis]);
    }

    void SetNextPlotTicksY(double y_min, double y_max, int n_ticks, const char** labels, bool show_default, int y_axis) {
        IM_ASSERT_USER_ERROR(n_ticks > 1, "The number of ticks must be greater than 1");
        static ImVector<double> buffer;
        FillRange(buffer, n_ticks, y_min, y_max);
        SetNextPlotTicksY(&buffer[0], n_ticks, labels, show_default, y_axis);
    }

    void SetPlotYAxis(int y_axis) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "SetPlotYAxis() needs to be called between BeginPlot() and EndPlot()!");
        IM_ASSERT_USER_ERROR(y_axis >= 0 && y_axis < MAX_Y_AXES, "y_axis needs to be between 0 and MAX_Y_AXES");
        gp.CurrentPlot->CurrentYAxis = y_axis;
    }

    ImVec2 GetPlotPos() {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "GetPlotPos() needs to be called between BeginPlot() and EndPlot()!");
        return gp.BB_Plot.Min;
    }

    ImVec2 GetPlotSize() {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "GetPlotSize() needs to be called between BeginPlot() and EndPlot()!");
        return gp.BB_Plot.GetSize();
    }

    void PushPlotClipRect() {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PushPlotClipRect() needs to be called between BeginPlot() and EndPlot()!");
        ImGui::PushClipRect(gp.BB_Plot.Min, gp.BB_Plot.Max, true);
    }

    void PopPlotClipRect() {
        ImGui::PopClipRect();
    }

    bool IsPlotHovered() {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "IsPlotHovered() needs to be called between BeginPlot() and EndPlot()!");
        return gp.Hov_Plot;
    }

    bool IsPlotXAxisHovered() {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "IsPlotXAxisHovered() needs to be called between BeginPlot() and EndPlot()!");
        ImRect bb_plot_pad = gp.BB_Plot; bb_plot_pad.Max.y -= 5;
        return gp.CurrentPlot->XAxis.Hovered && !bb_plot_pad.Contains(ImGui::GetMousePos());
    }

    bool IsPlotYAxisHovered(int y_axis_in) {
        IM_ASSERT_USER_ERROR(y_axis_in >= -1 && y_axis_in < MAX_Y_AXES, "y_axis needs to between -1 and MAX_Y_AXES");
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "IsPlotYAxisHovered() needs to be called between BeginPlot() and EndPlot()!");
        const int y_axis = y_axis_in >= 0 ? y_axis_in : gp.CurrentPlot->CurrentYAxis;
        ImRect bb_plot_pad = gp.BB_Plot;
        bb_plot_pad.Min.x += 5; bb_plot_pad.Max.x -= 5;
        return gp.CurrentPlot->YAxis[y_axis].Hovered && !bb_plot_pad.Contains(ImGui::GetMousePos());
    }

    ImPlotPoint GetPlotMousePos(int y_axis_in) {
        IM_ASSERT_USER_ERROR(y_axis_in >= -1 && y_axis_in < MAX_Y_AXES, "y_axis needs to between -1 and MAX_Y_AXES");
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "GetPlotMousePos() needs to be called between BeginPlot() and EndPlot()!");
        const int y_axis = y_axis_in >= 0 ? y_axis_in : gp.CurrentPlot->CurrentYAxis;
        return gp.LastMousePos[y_axis];
    }


    ImPlotLimits GetPlotLimits(int y_axis_in) {
        IM_ASSERT_USER_ERROR(y_axis_in >= -1 && y_axis_in < MAX_Y_AXES, "y_axis needs to between -1 and MAX_Y_AXES");
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "GetPlotLimits() needs to be called between BeginPlot() and EndPlot()!");
        const int y_axis = y_axis_in >= 0 ? y_axis_in : gp.CurrentPlot->CurrentYAxis;

        ImPlotState& plot = *gp.CurrentPlot;
        ImPlotLimits limits;
        limits.X = plot.XAxis.Range;
        limits.Y = plot.YAxis[y_axis].Range;
        return limits;
    }

    bool IsPlotQueried() {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "IsPlotQueried() needs to be called between BeginPlot() and EndPlot()!");
        return gp.CurrentPlot->Queried;
    }

    ImPlotLimits GetPlotQuery(int y_axis_in) {
        IM_ASSERT_USER_ERROR(y_axis_in >= -1 && y_axis_in < MAX_Y_AXES, "y_axis needs to between -1 and MAX_Y_AXES");
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "GetPlotQuery() needs to be called between BeginPlot() and EndPlot()!");
        ImPlotState& plot = *gp.CurrentPlot;
        const int y_axis = y_axis_in >= 0 ? y_axis_in : gp.CurrentPlot->CurrentYAxis;

        UpdateTransformCache();
        ImPlotPoint p1 = PixelsToPlot(plot.QueryRect.Min + gp.BB_Plot.Min, y_axis);
        ImPlotPoint p2 = PixelsToPlot(plot.QueryRect.Max + gp.BB_Plot.Min, y_axis);

        ImPlotLimits result;
        result.X.Min = ImMin(p1.x, p2.x);
        result.X.Max = ImMax(p1.x, p2.x);
        result.Y.Min = ImMin(p1.y, p2.y);
        result.Y.Max = ImMax(p1.y, p2.y);
        return result;
    }

    bool IsLegendEntryHovered(const char* label_id) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "IsPlotItemHighlight() needs to be called between BeginPlot() and EndPlot()!");
        ImGuiID id = ImGui::GetID(label_id);
        ImPlotItem* item = gp.CurrentPlot->Items.GetByKey(id);
        if (item && item->Highlight)
            return true;
        else
            return false;
    }

    //-----------------------------------------------------------------------------
    // STYLING
    //-----------------------------------------------------------------------------

    struct ImPlotStyleVarInfo {
        ImGuiDataType   Type;
        ImU32           Count;
        ImU32           Offset;
        void* GetVarPtr(ImPlotStyle* style) const { return (void*)((unsigned char*)style + Offset); }
    };

    static const ImPlotStyleVarInfo GPlotStyleVarInfo[] =
    {
        { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, LineWeight)         }, // ImPlotStyleVar_LineWeight
        { ImGuiDataType_S32,   1, (ImU32)IM_OFFSETOF(ImPlotStyle, Marker)             }, // ImPlotStyleVar_Marker
        { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, MarkerSize)         }, // ImPlotStyleVar_MarkerSize
        { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, MarkerWeight)       }, // ImPlotStyleVar_MarkerWeight
        { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, FillAlpha)          }, // ImPlotStyleVar_FillAlpha
        { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, ErrorBarSize)       }, // ImPlotStyleVar_ErrorBarSize
        { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, ErrorBarWeight)     }, // ImPlotStyleVar_ErrorBarWeight
        { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, DigitalBitHeight)   }, // ImPlotStyleVar_DigitalBitHeight
        { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImPlotStyle, DigitalBitGap)      }  // ImPlotStyleVar_DigitalBitGap
    };

    static const ImPlotStyleVarInfo* GetPlotStyleVarInfo(ImPlotStyleVar idx)
    {
        IM_ASSERT(idx >= 0 && idx < ImPlotStyleVar_COUNT);
        IM_ASSERT(IM_ARRAYSIZE(GPlotStyleVarInfo) == ImPlotStyleVar_COUNT);
        return &GPlotStyleVarInfo[idx];
    }

    ImPlotStyle& GetStyle() {
        return gp.Style;
    }

    void PushStyleColor(ImPlotCol idx, ImU32 col) {
        ImGuiColorMod backup;
        backup.Col = idx;
        backup.BackupValue = gp.Style.Colors[idx];
        gp.ColorModifiers.push_back(backup);
        gp.Style.Colors[idx] = ImGui::ColorConvertU32ToFloat4(col);
    }

    void PushStyleColor(ImPlotCol idx, const ImVec4& col) {
        ImGuiColorMod backup;
        backup.Col = idx;
        backup.BackupValue = gp.Style.Colors[idx];
        gp.ColorModifiers.push_back(backup);
        gp.Style.Colors[idx] = col;
    }

    void PopStyleColor(int count) {
        while (count > 0)
        {
            ImGuiColorMod& backup = gp.ColorModifiers.back();
            gp.Style.Colors[backup.Col] = backup.BackupValue;
            gp.ColorModifiers.pop_back();
            count--;
        }
    }

    void PushStyleVar(ImPlotStyleVar idx, float val) {
        const ImPlotStyleVarInfo* var_info = GetPlotStyleVarInfo(idx);
        if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1) {
            float* pvar = (float*)var_info->GetVarPtr(&gp.Style);
            gp.StyleModifiers.push_back(ImGuiStyleMod(idx, *pvar));
            *pvar = val;
            return;
        }
        IM_ASSERT(0 && "Called PushStyleVar() float variant but variable is not a float!");
    }

    void PushStyleVar(ImPlotStyleVar idx, int val) {
        const ImPlotStyleVarInfo* var_info = GetPlotStyleVarInfo(idx);
        if (var_info->Type == ImGuiDataType_S32 && var_info->Count == 1) {
            int* pvar = (int*)var_info->GetVarPtr(&gp.Style);
            gp.StyleModifiers.push_back(ImGuiStyleMod(idx, *pvar));
            *pvar = val;
            return;
        }
        else if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1) {
            float* pvar = (float*)var_info->GetVarPtr(&gp.Style);
            gp.StyleModifiers.push_back(ImGuiStyleMod(idx, *pvar));
            *pvar = (float)val;
            return;
        }
        IM_ASSERT(0 && "Called PushStyleVar() int variant but variable is not a int!");
    }

    void PopStyleVar(int count) {
        while (count > 0) {
            ImGuiStyleMod& backup = gp.StyleModifiers.back();
            const ImPlotStyleVarInfo* info = GetPlotStyleVarInfo(backup.VarIdx);
            void* data = info->GetVarPtr(&gp.Style);
            if (info->Type == ImGuiDataType_Float && info->Count == 1) {
                ((float*)data)[0] = backup.BackupFloat[0];
            }
            else if (info->Type == ImGuiDataType_Float && info->Count == 2) {
                ((float*)data)[0] = backup.BackupFloat[0];
                ((float*)data)[1] = backup.BackupFloat[1];
            }
            else if (info->Type == ImGuiDataType_S32 && info->Count == 1) {
                ((int*)data)[0] = backup.BackupInt[0];
            }
            gp.StyleModifiers.pop_back();
            count--;
        }
    }

    //-----------------------------------------------------------------------------
    // GETTERS
    //-----------------------------------------------------------------------------

    template <typename T>
    inline T OffsetAndStride(const T* data, int idx, int count, int offset, int stride) {
        idx = PosMod(offset + idx, count);
        return *(const T*)(const void*)((const unsigned char*)data + (size_t)idx * stride);
    }

    template <typename T>
    struct GetterYs {
        GetterYs(const T* ys, int count, int offset, int stride) {
            Ys = ys;
            Count = count;
            Offset = count ? PosMod(offset, count) : 0;
            Stride = stride;
        }
        const T* Ys;
        int Count;
        int Offset;
        int Stride;
        inline ImPlotPoint operator()(int idx) {
            return ImPlotPoint((T)idx, OffsetAndStride(Ys, idx, Count, Offset, Stride));
        }
    };

    template <typename T>
    struct GetterXsYs {
        GetterXsYs(const T* xs, const T* ys, int count, int offset, int stride) {
            Xs = xs; Ys = ys;
            Count = count;
            Offset = count ? PosMod(offset, count) : 0;
            Stride = stride;
        }
        const T* Xs;
        const T* Ys;
        int Count;
        int Offset;
        int Stride;
        inline ImPlotPoint operator()(int idx) {
            return ImPlotPoint(OffsetAndStride(Xs, idx, Count, Offset, Stride), OffsetAndStride(Ys, idx, Count, Offset, Stride));
        }
    };

    template <typename T>
    struct GetterXsYRef {
        GetterXsYRef(const T* xs, T y_ref, int count, int offset, int stride) {
            Xs = xs;
            YRef = y_ref;
            Count = count;
            Offset = count ? PosMod(offset, count) : 0;
            Stride = stride;
        }
        const T* Xs;
        T YRef;
        int Count;
        int Offset;
        int Stride;
        inline ImPlotPoint operator()(int idx) {
            return ImPlotPoint(OffsetAndStride(Xs, idx, Count, Offset, Stride), YRef);
        }
    };

    struct GetterImVec2 {
        GetterImVec2(const ImVec2* data, int count, int offset) {
            Data = data;
            Count = count;
            Offset = count ? PosMod(offset, count) : 0;
        }
        inline ImPlotPoint operator()(int idx) { return ImPlotPoint(Data[idx].x, Data[idx].y); }
        const ImVec2* Data;
        int Count;
        int Offset;
    };

    struct GetterImPlotPoint {
        GetterImPlotPoint(const ImPlotPoint* data, int count, int offset) {
            Data = data;
            Count = count;
            Offset = count ? PosMod(offset, count) : 0;
        }
        inline ImPlotPoint operator()(int idx) { return Data[idx]; }
        const ImPlotPoint* Data;
        int Count;
        int Offset;
    };

    struct GetterFuncPtrImPlotPoint {
        GetterFuncPtrImPlotPoint(ImPlotPoint(*g)(void* data, int idx), void* d, int count, int offset) {
            getter = g;
            Data = d;
            Count = count;
            Offset = count ? PosMod(offset, count) : 0;
        }
        inline ImPlotPoint operator()(int idx) { return getter(Data, idx); }
        ImPlotPoint(*getter)(void* data, int idx);
        void* Data;
        int Count;
        int Offset;
    };

    template <typename T>
    struct GetterBarV {
        const T* Ys; T XShift; int Count; int Offset; int Stride;
        GetterBarV(const T* ys, T xshift, int count, int offset, int stride) { Ys = ys; XShift = xshift; Count = count; Offset = offset; Stride = stride; }
        inline ImPlotPoint operator()(int idx) { return ImPlotPoint((T)idx + XShift, OffsetAndStride(Ys, idx, Count, Offset, Stride)); }
    };

    template <typename T>
    struct GetterBarH {
        const T* Xs; T YShift; int Count; int Offset; int Stride;
        GetterBarH(const T* xs, T yshift, int count, int offset, int stride) { Xs = xs; YShift = yshift; Count = count; Offset = offset; Stride = stride; }
        inline ImPlotPoint operator()(int idx) { return ImPlotPoint(OffsetAndStride(Xs, idx, Count, Offset, Stride), (T)idx + YShift); }
    };

    struct ImPlotPointError {
        ImPlotPointError(double _x, double _y, double _neg, double _pos) {
            x = _x; y = _y; neg = _neg; pos = _pos;
        }
        double x, y, neg, pos;
    };

    template <typename T>
    struct GetterError {
        const T* Xs; const T* Ys; const T* Neg; const T* Pos; int Count; int Offset; int Stride;
        GetterError(const T* xs, const T* ys, const T* neg, const T* pos, int count, int offset, int stride) {
            Xs = xs; Ys = ys; Neg = neg; Pos = pos; Count = count; Offset = offset; Stride = stride;
        }
        ImPlotPointError operator()(int idx) {
            return ImPlotPointError(OffsetAndStride(Xs, idx, Count, Offset, Stride),
                OffsetAndStride(Ys, idx, Count, Offset, Stride),
                OffsetAndStride(Neg, idx, Count, Offset, Stride),
                OffsetAndStride(Pos, idx, Count, Offset, Stride));
        }
    };

    //-----------------------------------------------------------------------------
    // TRANSFORMERS
    //-----------------------------------------------------------------------------

    struct TransformerLinLin {
        TransformerLinLin(int y_axis) : YAxis(y_axis) {}

        inline ImVec2 operator()(const ImPlotPoint& plt) { return (*this)(plt.x, plt.y); }
        inline ImVec2 operator()(double x, double y) {
            return ImVec2((float)(gp.PixelRange[YAxis].Min.x + gp.Mx * (x - gp.CurrentPlot->XAxis.Range.Min)),
                (float)(gp.PixelRange[YAxis].Min.y + gp.My[YAxis] * (y - gp.CurrentPlot->YAxis[YAxis].Range.Min)));
        }

        int YAxis;
    };

    struct TransformerLogLin {
        TransformerLogLin(int y_axis) : YAxis(y_axis) {}

        inline ImVec2 operator()(const ImPlotPoint& plt) { return (*this)(plt.x, plt.y); }
        inline ImVec2 operator()(double x, double y) {
            double t = ImLog10(x / gp.CurrentPlot->XAxis.Range.Min) / gp.LogDenX;
            x = ImLerp(gp.CurrentPlot->XAxis.Range.Min, gp.CurrentPlot->XAxis.Range.Max, (float)t);
            return ImVec2((float)(gp.PixelRange[YAxis].Min.x + gp.Mx * (x - gp.CurrentPlot->XAxis.Range.Min)),
                (float)(gp.PixelRange[YAxis].Min.y + gp.My[YAxis] * (y - gp.CurrentPlot->YAxis[YAxis].Range.Min)));
        }

        int YAxis;
    };

    struct TransformerLinLog {
        TransformerLinLog(int y_axis) : YAxis(y_axis) {}

        inline ImVec2 operator()(const ImPlotPoint& plt) { return (*this)(plt.x, plt.y); }
        inline ImVec2 operator()(double x, double y) {
            double t = ImLog10(y / gp.CurrentPlot->YAxis[YAxis].Range.Min) / gp.LogDenY[YAxis];
            y = ImLerp(gp.CurrentPlot->YAxis[YAxis].Range.Min, gp.CurrentPlot->YAxis[YAxis].Range.Max, (float)t);
            return ImVec2((float)(gp.PixelRange[YAxis].Min.x + gp.Mx * (x - gp.CurrentPlot->XAxis.Range.Min)),
                (float)(gp.PixelRange[YAxis].Min.y + gp.My[YAxis] * (y - gp.CurrentPlot->YAxis[YAxis].Range.Min)));
        }
        int YAxis;
    };

    struct TransformerLogLog {
        TransformerLogLog(int y_axis) : YAxis(y_axis) {}

        inline ImVec2 operator()(const ImPlotPoint& plt) { return (*this)(plt.x, plt.y); }
        inline ImVec2 operator()(double x, double y) {
            double t = ImLog10(x / gp.CurrentPlot->XAxis.Range.Min) / gp.LogDenX;
            x = ImLerp(gp.CurrentPlot->XAxis.Range.Min, gp.CurrentPlot->XAxis.Range.Max, (float)t);
            t = ImLog10(y / gp.CurrentPlot->YAxis[YAxis].Range.Min) / gp.LogDenY[YAxis];
            y = ImLerp(gp.CurrentPlot->YAxis[YAxis].Range.Min, gp.CurrentPlot->YAxis[YAxis].Range.Max, (float)t);
            return ImVec2((float)(gp.PixelRange[YAxis].Min.x + gp.Mx * (x - gp.CurrentPlot->XAxis.Range.Min)),
                (float)(gp.PixelRange[YAxis].Min.y + gp.My[YAxis] * (y - gp.CurrentPlot->YAxis[YAxis].Range.Min)));
        }

        int YAxis;
    };

    //-----------------------------------------------------------------------------
    // RENDERERS
    //-----------------------------------------------------------------------------

    /// Renders primitive shapes in bulk as efficiently as possible.
    template <typename Renderer>
    inline void RenderPrimitives(Renderer renderer, ImDrawList& DrawList) {
        int prims = renderer.Prims;
        int prims_culled = 0;
        int idx = 0;
        const ImVec2 uv = DrawList._Data->TexUvWhitePixel;
        while (prims) {
            // find how many can be reserved up to end of current draw command's limit
            int cnt = (int)ImMin(size_t(prims), (((size_t(1) << sizeof(ImDrawIdx) * 8) - 1 - DrawList._VtxCurrentIdx) / Renderer::VtxConsumed));
            // make sure at least this many elements can be rendered to avoid situations where at the end of buffer this slow path is not taken all the time
            if (cnt >= ImMin(64, prims)) {
                if (prims_culled >= cnt)
                    prims_culled -= cnt; // reuse previous reservation
                else {
                    DrawList.PrimReserve((cnt - prims_culled) * Renderer::IdxConsumed, (cnt - prims_culled) * Renderer::VtxConsumed); // add more elements to previous reservation
                    prims_culled = 0;
                }
            }
            else
            {
                if (prims_culled > 0) {
                    DrawList.PrimUnreserve(prims_culled * Renderer::IdxConsumed, prims_culled * Renderer::VtxConsumed);
                    prims_culled = 0;
                }
                cnt = (int)ImMin(size_t(prims), (((size_t(1) << sizeof(ImDrawIdx) * 8) - 1 - 0/*DrawList._VtxCurrentIdx*/) / Renderer::VtxConsumed));
                DrawList.PrimReserve(cnt * Renderer::IdxConsumed, cnt * Renderer::VtxConsumed); // reserve new draw command
            }
            prims -= cnt;
            for (int ie = idx + cnt; idx != ie; ++idx) {
                if (!renderer(DrawList, uv, idx))
                    prims_culled++;
            }
        }
        if (prims_culled > 0)
            DrawList.PrimUnreserve(prims_culled * Renderer::IdxConsumed, prims_culled * Renderer::VtxConsumed);
    }

#define SQRT_1_2 0.70710678118f
#define SQRT_3_2 0.86602540378f

    inline void TransformMarker(ImVec2* points, int n, const ImVec2& c, float s) {
        for (int i = 0; i < n; ++i) {
            points[i].x = c.x + points[i].x * s;
            points[i].y = c.y + points[i].y * s;
        }
    }

    inline void MarkerGeneral(ImDrawList& DrawList, ImVec2* points, int n, const ImVec2& c, float s, bool outline, ImU32 col_outline, bool fill, ImU32 col_fill, float weight) {
        TransformMarker(points, n, c, s);
        if (fill)
            DrawList.AddConvexPolyFilled(points, n, col_fill);
        if (outline && !(fill && col_outline == col_fill)) {
            for (int i = 0; i < n; ++i)
                DrawList.AddLine(points[i], points[(i + 1) % n], col_outline, weight);
        }
    }

    inline void MarkerCircle(ImDrawList& DrawList, const ImVec2& c, float s, bool outline, ImU32 col_outline, bool fill, ImU32 col_fill, float weight) {
        ImVec2 marker[10] = { ImVec2(1.0f, 0.0f),
                             ImVec2(0.809017f, 0.58778524f),
                             ImVec2(0.30901697f, 0.95105654f),
                             ImVec2(-0.30901703f, 0.9510565f),
                             ImVec2(-0.80901706f, 0.5877852f),
                             ImVec2(-1.0f, 0.0f),
                             ImVec2(-0.80901694f, -0.58778536f),
                             ImVec2(-0.3090171f, -0.9510565f),
                             ImVec2(0.30901712f, -0.9510565f),
                             ImVec2(0.80901694f, -0.5877853f) };
        MarkerGeneral(DrawList, marker, 10, c, s, outline, col_outline, fill, col_fill, weight);
    }

    inline void MarkerDiamond(ImDrawList& DrawList, const ImVec2& c, float s, bool outline, ImU32 col_outline, bool fill, ImU32 col_fill, float weight) {
        ImVec2 marker[4] = { ImVec2(1, 0), ImVec2(0, -1), ImVec2(-1, 0), ImVec2(0, 1) };
        MarkerGeneral(DrawList, marker, 4, c, s, outline, col_outline, fill, col_fill, weight);
    }

    inline void MarkerSquare(ImDrawList& DrawList, const ImVec2& c, float s, bool outline, ImU32 col_outline, bool fill, ImU32 col_fill, float weight) {
        ImVec2 marker[4] = { ImVec2(SQRT_1_2,SQRT_1_2),ImVec2(SQRT_1_2,-SQRT_1_2),ImVec2(-SQRT_1_2,-SQRT_1_2),ImVec2(-SQRT_1_2,SQRT_1_2) };
        MarkerGeneral(DrawList, marker, 4, c, s, outline, col_outline, fill, col_fill, weight);
    }

    inline void MarkerUp(ImDrawList& DrawList, const ImVec2& c, float s, bool outline, ImU32 col_outline, bool fill, ImU32 col_fill, float weight) {
        ImVec2 marker[3] = { ImVec2(SQRT_3_2,0.5f),ImVec2(0,-1),ImVec2(-SQRT_3_2,0.5f) };
        MarkerGeneral(DrawList, marker, 3, c, s, outline, col_outline, fill, col_fill, weight);
    }

    inline void MarkerDown(ImDrawList& DrawList, const ImVec2& c, float s, bool outline, ImU32 col_outline, bool fill, ImU32 col_fill, float weight) {
        ImVec2 marker[3] = { ImVec2(SQRT_3_2,-0.5f),ImVec2(0,1),ImVec2(-SQRT_3_2,-0.5f) };
        MarkerGeneral(DrawList, marker, 3, c, s, outline, col_outline, fill, col_fill, weight);
    }

    inline void MarkerLeft(ImDrawList& DrawList, const ImVec2& c, float s, bool outline, ImU32 col_outline, bool fill, ImU32 col_fill, float weight) {
        ImVec2 marker[3] = { ImVec2(-1,0), ImVec2(0.5, SQRT_3_2), ImVec2(0.5, -SQRT_3_2) };
        MarkerGeneral(DrawList, marker, 3, c, s, outline, col_outline, fill, col_fill, weight);
    }

    inline void MarkerRight(ImDrawList& DrawList, const ImVec2& c, float s, bool outline, ImU32 col_outline, bool fill, ImU32 col_fill, float weight) {
        ImVec2 marker[3] = { ImVec2(1,0), ImVec2(-0.5, SQRT_3_2), ImVec2(-0.5, -SQRT_3_2) };
        MarkerGeneral(DrawList, marker, 3, c, s, outline, col_outline, fill, col_fill, weight);
    }

    inline void MarkerAsterisk(ImDrawList& DrawList, const ImVec2& c, float s, bool /*outline*/, ImU32 col_outline, bool /*fill*/, ImU32 /*col_fill*/, float weight) {
        ImVec2 marker[6] = { ImVec2(SQRT_3_2, 0.5f), ImVec2(0, -1), ImVec2(-SQRT_3_2, 0.5f), ImVec2(SQRT_3_2, -0.5f), ImVec2(0, 1),  ImVec2(-SQRT_3_2, -0.5f) };
        TransformMarker(marker, 6, c, s);
        DrawList.AddLine(marker[0], marker[5], col_outline, weight);
        DrawList.AddLine(marker[1], marker[4], col_outline, weight);
        DrawList.AddLine(marker[2], marker[3], col_outline, weight);
    }

    inline void MarkerPlus(ImDrawList& DrawList, const ImVec2& c, float s, bool /*outline*/, ImU32 col_outline, bool /*fill*/, ImU32 /*col_fill*/, float weight) {
        ImVec2 marker[4] = { ImVec2(1, 0), ImVec2(0, -1), ImVec2(-1, 0), ImVec2(0, 1) };
        TransformMarker(marker, 4, c, s);
        DrawList.AddLine(marker[0], marker[2], col_outline, weight);
        DrawList.AddLine(marker[1], marker[3], col_outline, weight);
    }

    inline void MarkerCross(ImDrawList& DrawList, const ImVec2& c, float s, bool /*outline*/, ImU32 col_outline, bool /*fill*/, ImU32 /*col_fill*/, float weight) {
        ImVec2 marker[4] = { ImVec2(SQRT_1_2,SQRT_1_2),ImVec2(SQRT_1_2,-SQRT_1_2),ImVec2(-SQRT_1_2,-SQRT_1_2),ImVec2(-SQRT_1_2,SQRT_1_2) };
        TransformMarker(marker, 4, c, s);
        DrawList.AddLine(marker[0], marker[2], col_outline, weight);
        DrawList.AddLine(marker[1], marker[3], col_outline, weight);
    }

    template <typename Transformer, typename Getter>
    inline void RenderMarkers(Getter getter, Transformer transformer, ImDrawList& DrawList, bool rend_mk_line, ImU32 col_mk_line, bool rend_mk_fill, ImU32 col_mk_fill) {
        for (int i = 0; i < getter.Count; ++i) {
            ImVec2 c = transformer(getter(i));
            if (gp.BB_Plot.Contains(c)) {
                // TODO: Optimize the loop and if statements, this is atrocious
                if (HasFlag(gp.Style.Marker, ImPlotMarker_Circle))
                    MarkerCircle(DrawList, c, gp.Style.MarkerSize, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill, gp.Style.MarkerWeight);
                if (HasFlag(gp.Style.Marker, ImPlotMarker_Square))
                    MarkerSquare(DrawList, c, gp.Style.MarkerSize, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill, gp.Style.MarkerWeight);
                if (HasFlag(gp.Style.Marker, ImPlotMarker_Diamond))
                    MarkerDiamond(DrawList, c, gp.Style.MarkerSize, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill, gp.Style.MarkerWeight);
                if (HasFlag(gp.Style.Marker, ImPlotMarker_Up))
                    MarkerUp(DrawList, c, gp.Style.MarkerSize, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill, gp.Style.MarkerWeight);
                if (HasFlag(gp.Style.Marker, ImPlotMarker_Down))
                    MarkerDown(DrawList, c, gp.Style.MarkerSize, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill, gp.Style.MarkerWeight);
                if (HasFlag(gp.Style.Marker, ImPlotMarker_Left))
                    MarkerLeft(DrawList, c, gp.Style.MarkerSize, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill, gp.Style.MarkerWeight);
                if (HasFlag(gp.Style.Marker, ImPlotMarker_Right))
                    MarkerRight(DrawList, c, gp.Style.MarkerSize, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill, gp.Style.MarkerWeight);
                if (HasFlag(gp.Style.Marker, ImPlotMarker_Cross))
                    MarkerCross(DrawList, c, gp.Style.MarkerSize, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill, gp.Style.MarkerWeight);
                if (HasFlag(gp.Style.Marker, ImPlotMarker_Plus))
                    MarkerPlus(DrawList, c, gp.Style.MarkerSize, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill, gp.Style.MarkerWeight);
                if (HasFlag(gp.Style.Marker, ImPlotMarker_Asterisk))
                    MarkerAsterisk(DrawList, c, gp.Style.MarkerSize, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill, gp.Style.MarkerWeight);
            }
        }
    }

    template <typename Getter, typename Transformer>
    struct LineRenderer {
        inline LineRenderer(Getter _getter, Transformer _transformer, ImU32 col, float weight) :
            getter(_getter),
            transformer(_transformer)
        {
            Prims = getter.Count - 1;
            Col = col;
            Weight = weight;
            p1 = transformer(getter(0));
        }
        inline bool operator()(ImDrawList& DrawList, ImVec2 uv, int prim) {
            ImVec2 p2 = transformer(getter(prim + 1));
            if (!gp.BB_Plot.Overlaps(ImRect(ImMin(p1, p2), ImMax(p1, p2)))) {
                p1 = p2;
                return false;
            }
            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            dx *= (Weight * 0.5f);
            dy *= (Weight * 0.5f);
            DrawList._VtxWritePtr[0].pos.x = p1.x + dy;
            DrawList._VtxWritePtr[0].pos.y = p1.y - dx;
            DrawList._VtxWritePtr[0].uv = uv;
            DrawList._VtxWritePtr[0].col = Col;
            DrawList._VtxWritePtr[1].pos.x = p2.x + dy;
            DrawList._VtxWritePtr[1].pos.y = p2.y - dx;
            DrawList._VtxWritePtr[1].uv = uv;
            DrawList._VtxWritePtr[1].col = Col;
            DrawList._VtxWritePtr[2].pos.x = p2.x - dy;
            DrawList._VtxWritePtr[2].pos.y = p2.y + dx;
            DrawList._VtxWritePtr[2].uv = uv;
            DrawList._VtxWritePtr[2].col = Col;
            DrawList._VtxWritePtr[3].pos.x = p1.x - dy;
            DrawList._VtxWritePtr[3].pos.y = p1.y + dx;
            DrawList._VtxWritePtr[3].uv = uv;
            DrawList._VtxWritePtr[3].col = Col;
            DrawList._VtxWritePtr += 4;
            DrawList._IdxWritePtr[0] = (ImDrawIdx)(DrawList._VtxCurrentIdx);
            DrawList._IdxWritePtr[1] = (ImDrawIdx)(DrawList._VtxCurrentIdx + 1);
            DrawList._IdxWritePtr[2] = (ImDrawIdx)(DrawList._VtxCurrentIdx + 2);
            DrawList._IdxWritePtr[3] = (ImDrawIdx)(DrawList._VtxCurrentIdx);
            DrawList._IdxWritePtr[4] = (ImDrawIdx)(DrawList._VtxCurrentIdx + 2);
            DrawList._IdxWritePtr[5] = (ImDrawIdx)(DrawList._VtxCurrentIdx + 3);
            DrawList._IdxWritePtr += 6;
            DrawList._VtxCurrentIdx += 4;
            p1 = p2;
            return true;
        }
        Getter getter;
        Transformer transformer;
        int Prims;
        ImU32 Col;
        float Weight;
        ImVec2 p1;
        static const int IdxConsumed = 6;
        static const int VtxConsumed = 4;
    };

    template <typename Getter, typename Transformer>
    inline void RenderLineStrip(Getter getter, Transformer transformer, ImDrawList& DrawList, float line_weight, ImU32 col) {
        if (HasFlag(gp.CurrentPlot->Flags, ImPlotFlags_AntiAliased)) {
            ImVec2 p1 = transformer(getter(0));
            for (int i = 0; i < getter.Count; ++i) {
                ImVec2 p2 = transformer(getter(i));
                if (gp.BB_Plot.Overlaps(ImRect(ImMin(p1, p2), ImMax(p1, p2))))
                    DrawList.AddLine(p1, p2, col, line_weight);
                p1 = p2;
            }
        }
        else {
            RenderPrimitives(LineRenderer<Getter, Transformer>(getter, transformer, col, line_weight), DrawList);
        }
    }

    template <typename Getter1, typename Getter2, typename Transformer>
    struct ShadedRenderer {
        ShadedRenderer(Getter1 _getter1, Getter2 _getter2, Transformer _transformer, ImU32 col) :
            getter1(_getter1),
            getter2(_getter2),
            transformer(_transformer),
            Col(col)
        {
            Prims = ImMin(getter1.Count, getter2.Count) - 1;
            p11 = transformer(getter1(0));
            p12 = transformer(getter2(0));
        }

        inline bool operator()(ImDrawList& DrawList, ImVec2 uv, int prim) {
            ImVec2 p21 = transformer(getter1(prim + 1));
            ImVec2 p22 = transformer(getter2(prim + 1));
            const int intersect = (p11.y > p12.y && p22.y > p21.y) || (p12.y > p11.y && p21.y > p22.y);
            ImVec2 intersection = Intersection(p11, p21, p12, p22);
            DrawList._VtxWritePtr[0].pos = p11;
            DrawList._VtxWritePtr[0].uv = uv;
            DrawList._VtxWritePtr[0].col = Col;
            DrawList._VtxWritePtr[1].pos = p21;
            DrawList._VtxWritePtr[1].uv = uv;
            DrawList._VtxWritePtr[1].col = Col;
            DrawList._VtxWritePtr[2].pos = intersection;
            DrawList._VtxWritePtr[2].uv = uv;
            DrawList._VtxWritePtr[2].col = Col;
            DrawList._VtxWritePtr[3].pos = p12;
            DrawList._VtxWritePtr[3].uv = uv;
            DrawList._VtxWritePtr[3].col = Col;
            DrawList._VtxWritePtr[4].pos = p22;
            DrawList._VtxWritePtr[4].uv = uv;
            DrawList._VtxWritePtr[4].col = Col;
            DrawList._VtxWritePtr += 5;
            DrawList._IdxWritePtr[0] = (ImDrawIdx)(DrawList._VtxCurrentIdx);
            DrawList._IdxWritePtr[1] = (ImDrawIdx)(DrawList._VtxCurrentIdx + 1 + intersect);
            DrawList._IdxWritePtr[2] = (ImDrawIdx)(DrawList._VtxCurrentIdx + 3);
            DrawList._IdxWritePtr[3] = (ImDrawIdx)(DrawList._VtxCurrentIdx + 1);
            DrawList._IdxWritePtr[4] = (ImDrawIdx)(DrawList._VtxCurrentIdx + 3 - intersect);
            DrawList._IdxWritePtr[5] = (ImDrawIdx)(DrawList._VtxCurrentIdx + 4);
            DrawList._IdxWritePtr += 6;
            DrawList._VtxCurrentIdx += 5;
            p11 = p21;
            p12 = p22;
            return true;
        }
        Getter1 getter1;
        Getter2 getter2;
        Transformer transformer;
        int Prims;
        ImU32 Col;
        ImVec2 p11, p12;
        static const int IdxConsumed = 6;
        static const int VtxConsumed = 5;
    };

    //-----------------------------------------------------------------------------
    // RENDERING UTILS
    //-----------------------------------------------------------------------------

    // Returns true if a style color is set to be automaticaly determined
    inline bool ColorIsAuto(ImPlotCol idx) {
        return gp.Style.Colors[idx].w == -1;
    }
    // Recolors an item from an the current ImPlotCol if it is not automatic (i.e. alpha != -1)
    inline void TryRecolorItem(ImPlotItem* item, ImPlotCol idx) {
        if (gp.Style.Colors[idx].w != -1)
            item->Color = gp.Style.Colors[idx];
    }
    // Returns true if lines will render (e.g. basic lines, bar outlines)
    inline bool WillLineRender() {
        return gp.Style.Colors[ImPlotCol_Line].w != 0 && gp.Style.LineWeight > 0;
    }
    // Returns true if fills will render (e.g. shaded plots, bar fills)
    inline bool WillFillRender() {
        return gp.Style.Colors[ImPlotCol_Fill].w != 0 && gp.Style.FillAlpha > 0;
    }
    // Returns true if marker outlines will render
    inline bool WillMarkerOutlineRender() {
        return gp.Style.Colors[ImPlotCol_MarkerOutline].w != 0 && gp.Style.MarkerWeight > 0;
    }
    // Returns true if mark fill will render
    inline bool WillMarkerFillRender() {
        return gp.Style.Colors[ImPlotCol_MarkerFill].w != 0 && gp.Style.FillAlpha > 0;
    }
    // Gets the line color for an item
    inline ImVec4 GetLineColor(ImPlotItem* item) {
        return ColorIsAuto(ImPlotCol_Line) ? item->Color : gp.Style.Colors[ImPlotCol_Line];
    }
    // Gets the fill color for an item
    inline ImVec4 GetItemFillColor(ImPlotItem* item) {
        ImVec4 col = ColorIsAuto(ImPlotCol_Fill) ? item->Color : gp.Style.Colors[ImPlotCol_Fill];
        col.w *= gp.Style.FillAlpha;
        return col;
    }
    // Gets the marker outline color for an item
    inline ImVec4 GetMarkerOutlineColor(ImPlotItem* item) {
        return ColorIsAuto(ImPlotCol_MarkerOutline) ? GetLineColor(item) : gp.Style.Colors[ImPlotCol_MarkerOutline];
    }
    // Gets the marker fill color for an item
    inline ImVec4 GetMarkerFillColor(ImPlotItem* item) {
        ImVec4 col = ColorIsAuto(ImPlotCol_MarkerFill) ? GetLineColor(item) : gp.Style.Colors[ImPlotCol_MarkerFill];
        col.w *= gp.Style.FillAlpha;
        return col;
    }
    // Gets the error bar color
    inline ImVec4 GetErrorBarColor() {
        return ColorIsAuto(ImPlotCol_ErrorBar) ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : gp.Style.Colors[ImPlotCol_ErrorBar];
    }

    //-----------------------------------------------------------------------------
    // PLOT LINES / MARKERS
    //-----------------------------------------------------------------------------

    template <typename Getter>
    inline void PlotEx(const char* label_id, Getter getter)
    {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PlotEx() needs to be called between BeginPlot() and EndPlot()!");

        ImPlotItem* item = RegisterItem(label_id);
        if (!item->Show)
            return;
        TryRecolorItem(item, ImPlotCol_Line);

        // find data extents
        if (gp.FitThisFrame) {
            for (int i = 0; i < getter.Count; ++i) {
                ImPlotPoint p = getter(i);
                FitPoint(p);
            }
        }

        ImDrawList& DrawList = *ImGui::GetWindowDrawList();
        ImPlotState* plot = gp.CurrentPlot;
        const int y_axis = plot->CurrentYAxis;

        PushPlotClipRect();
        // render line
        if (getter.Count > 1 && WillLineRender()) {
            ImU32 col_line = ImGui::GetColorU32(GetLineColor(item));
            const float line_weight = item->Highlight ? gp.Style.LineWeight * 2 : gp.Style.LineWeight;
            if (HasFlag(plot->XAxis.Flags, ImPlotAxisFlags_LogScale) && HasFlag(plot->YAxis[y_axis].Flags, ImPlotAxisFlags_LogScale))
                RenderLineStrip(getter, TransformerLogLog(y_axis), DrawList, line_weight, col_line);
            else if (HasFlag(plot->XAxis.Flags, ImPlotAxisFlags_LogScale))
                RenderLineStrip(getter, TransformerLogLin(y_axis), DrawList, line_weight, col_line);
            else if (HasFlag(plot->YAxis[y_axis].Flags, ImPlotAxisFlags_LogScale))
                RenderLineStrip(getter, TransformerLinLog(y_axis), DrawList, line_weight, col_line);
            else
                RenderLineStrip(getter, TransformerLinLin(y_axis), DrawList, line_weight, col_line);
        }
        // render markers
        if (gp.Style.Marker != ImPlotMarker_None) {
            const bool rend_mk_line = WillMarkerOutlineRender();
            const bool rend_mk_fill = WillMarkerFillRender();
            const ImU32 col_mk_line = ImGui::GetColorU32(GetMarkerOutlineColor(item));
            const ImU32 col_mk_fill = ImGui::GetColorU32(GetMarkerFillColor(item));
            if (HasFlag(plot->XAxis.Flags, ImPlotAxisFlags_LogScale) && HasFlag(plot->YAxis[y_axis].Flags, ImPlotAxisFlags_LogScale))
                RenderMarkers(getter, TransformerLogLog(y_axis), DrawList, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill);
            else if (HasFlag(plot->XAxis.Flags, ImPlotAxisFlags_LogScale))
                RenderMarkers(getter, TransformerLogLin(y_axis), DrawList, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill);
            else if (HasFlag(plot->YAxis[y_axis].Flags, ImPlotAxisFlags_LogScale))
                RenderMarkers(getter, TransformerLinLog(y_axis), DrawList, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill);
            else
                RenderMarkers(getter, TransformerLinLin(y_axis), DrawList, rend_mk_line, col_mk_line, rend_mk_fill, col_mk_fill);
        }
        PopPlotClipRect();
    }

    //-----------------------------------------------------------------------------
    // float

    void PlotLine(const char* label_id, const float* values, int count, int offset, int stride) {
        GetterYs<float> getter(values, count, offset, stride);
        PlotEx(label_id, getter);
    }

    void PlotLine(const char* label_id, const float* xs, const float* ys, int count, int offset, int stride) {
        GetterXsYs<float> getter(xs, ys, count, offset, stride);
        return PlotEx(label_id, getter);
    }

    void PlotLine(const char* label_id, const ImVec2* data, int count, int offset) {
        GetterImVec2 getter(data, count, offset);
        return PlotEx(label_id, getter);
    }

    //-----------------------------------------------------------------------------
    // double

    void PlotLine(const char* label_id, const double* values, int count, int offset, int stride) {
        GetterYs<double> getter(values, count, offset, stride);
        PlotEx(label_id, getter);
    }

    void PlotLine(const char* label_id, const double* xs, const double* ys, int count, int offset, int stride) {
        GetterXsYs<double> getter(xs, ys, count, offset, stride);
        return PlotEx(label_id, getter);
    }

    void PlotLine(const char* label_id, const ImPlotPoint* data, int count, int offset) {
        GetterImPlotPoint getter(data, count, offset);
        return PlotEx(label_id, getter);
    }

    //-----------------------------------------------------------------------------
    // custom

    void PlotLine(const char* label_id, ImPlotPoint(*getter_func)(void* data, int idx), void* data, int count, int offset) {
        GetterFuncPtrImPlotPoint getter(getter_func, data, count, offset);
        return PlotEx(label_id, getter);
    }

    //-----------------------------------------------------------------------------
    // PLOT SCATTER
    //-----------------------------------------------------------------------------

    inline int PushScatterStyle() {
        int vars = 1;
        PushStyleVar(ImPlotStyleVar_LineWeight, 0);
        if (GetStyle().Marker == ImPlotMarker_None) {
            PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Circle);
            vars++;
        }
        return vars;
    }

    //-----------------------------------------------------------------------------
    // float

    void PlotScatter(const char* label_id, const float* values, int count, int offset, int stride) {
        int vars = PushScatterStyle();
        PlotLine(label_id, values, count, offset, stride);
        PopStyleVar(vars);
    }

    void PlotScatter(const char* label_id, const float* xs, const float* ys, int count, int offset, int stride) {
        int vars = PushScatterStyle();
        PlotLine(label_id, xs, ys, count, offset, stride);
        PopStyleVar(vars);
    }

    void PlotScatter(const char* label_id, const ImVec2* data, int count, int offset) {
        int vars = PushScatterStyle();
        PlotLine(label_id, data, count, offset);
        PopStyleVar(vars);
    }

    //-----------------------------------------------------------------------------
    // double

    void PlotScatter(const char* label_id, const double* values, int count, int offset, int stride) {
        int vars = PushScatterStyle();
        PlotLine(label_id, values, count, offset, stride);
        PopStyleVar(vars);
    }

    void PlotScatter(const char* label_id, const double* xs, const double* ys, int count, int offset, int stride) {
        int vars = PushScatterStyle();
        PlotLine(label_id, xs, ys, count, offset, stride);
        PopStyleVar(vars);
    }

    void PlotScatter(const char* label_id, const ImPlotPoint* data, int count, int offset) {
        int vars = PushScatterStyle();
        PlotLine(label_id, data, count, offset);
        PopStyleVar(vars);
    }

    //-----------------------------------------------------------------------------
    // custom

    void PlotScatter(const char* label_id, ImPlotPoint(*getter)(void* data, int idx), void* data, int count, int offset) {
        int vars = PushScatterStyle();
        PlotLine(label_id, getter, data, count, offset);
        PopStyleVar(vars);
    }

    //-----------------------------------------------------------------------------
    // PLOT SHADED
    //-----------------------------------------------------------------------------

    template <typename Getter1, typename Getter2>
    inline void PlotShadedEx(const char* label_id, Getter1 getter1, Getter2 getter2) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PlotShaded() needs to be called between BeginPlot() and EndPlot()!");

        ImPlotItem* item = RegisterItem(label_id);
        if (!item->Show)
            return;
        TryRecolorItem(item, ImPlotCol_Fill);

        if (!WillFillRender())
            return;

        // find data extents
        if (gp.FitThisFrame) {
            for (int i = 0; i < ImMin(getter1.Count, getter2.Count); ++i) {
                ImPlotPoint p1 = getter1(i);
                ImPlotPoint p2 = getter2(i);
                FitPoint(p1);
                FitPoint(p2);
            }
        }

        ImDrawList& DrawList = *ImGui::GetWindowDrawList();
        ImPlotState* plot = gp.CurrentPlot;
        const int y_axis = plot->CurrentYAxis;

        ImU32 col = ImGui::GetColorU32(GetItemFillColor(item));

        PushPlotClipRect();
        if (HasFlag(plot->XAxis.Flags, ImPlotAxisFlags_LogScale) && HasFlag(plot->YAxis[y_axis].Flags, ImPlotAxisFlags_LogScale))
            RenderPrimitives(ShadedRenderer<Getter1, Getter2, TransformerLogLog>(getter1, getter2, TransformerLogLog(y_axis), col), DrawList);
        else if (HasFlag(plot->XAxis.Flags, ImPlotAxisFlags_LogScale))
            RenderPrimitives(ShadedRenderer<Getter1, Getter2, TransformerLogLin>(getter1, getter2, TransformerLogLin(y_axis), col), DrawList);
        else if (HasFlag(plot->YAxis[y_axis].Flags, ImPlotAxisFlags_LogScale))
            RenderPrimitives(ShadedRenderer<Getter1, Getter2, TransformerLinLog>(getter1, getter2, TransformerLinLog(y_axis), col), DrawList);
        else
            RenderPrimitives(ShadedRenderer<Getter1, Getter2, TransformerLinLin>(getter1, getter2, TransformerLinLin(y_axis), col), DrawList);
        PopPlotClipRect();
    }

    //-----------------------------------------------------------------------------
    // float

    void PlotShaded(const char* label_id, const float* xs, const float* ys1, const float* ys2, int count, int offset, int stride) {
        GetterXsYs<float> getter1(xs, ys1, count, offset, stride);
        GetterXsYs<float> getter2(xs, ys2, count, offset, stride);
        PlotShadedEx(label_id, getter1, getter2);
    }

    void PlotShaded(const char* label_id, const float* xs, const float* ys, int count, float y_ref, int offset, int stride) {
        GetterXsYs<float> getter1(xs, ys, count, offset, stride);
        GetterXsYRef<float> getter2(xs, y_ref, count, offset, stride);
        PlotShadedEx(label_id, getter1, getter2);
    }

    //-----------------------------------------------------------------------------
    // double

    void PlotShaded(const char* label_id, const double* xs, const double* ys1, const double* ys2, int count, int offset, int stride) {
        GetterXsYs<double> getter1(xs, ys1, count, offset, stride);
        GetterXsYs<double> getter2(xs, ys2, count, offset, stride);
        PlotShadedEx(label_id, getter1, getter2);
    }

    void PlotShaded(const char* label_id, const double* xs, const double* ys, int count, double y_ref, int offset, int stride) {
        GetterXsYs<double> getter1(xs, ys, count, offset, stride);
        GetterXsYRef<double> getter2(xs, y_ref, count, offset, stride);
        PlotShadedEx(label_id, getter1, getter2);
    }

    //-----------------------------------------------------------------------------
    // PLOT BAR V
    //-----------------------------------------------------------------------------

    template <typename Getter, typename TWidth>
    void PlotBarsEx(const char* label_id, Getter getter, TWidth width) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PlotBars() needs to be called between BeginPlot() and EndPlot()!");

        ImPlotItem* item = RegisterItem(label_id);
        if (!item->Show)
            return;
        TryRecolorItem(item, ImPlotCol_Fill);

        const TWidth half_width = width / 2;
        // find data extents
        if (gp.FitThisFrame) {
            for (int i = 0; i < getter.Count; ++i) {
                ImPlotPoint p = getter(i);
                FitPoint(ImPlotPoint(p.x - half_width, p.y));
                FitPoint(ImPlotPoint(p.x + half_width, 0));
            }
        }

        ImU32 col_line = ImGui::GetColorU32(GetLineColor(item));
        ImU32 col_fill = ImGui::GetColorU32(GetItemFillColor(item));
        const bool rend_fill = WillFillRender();
        bool rend_line = WillLineRender();
        if (rend_fill && col_line == col_fill)
            rend_line = false;

        ImDrawList& DrawList = *ImGui::GetWindowDrawList();
        PushPlotClipRect();
        for (int i = 0; i < getter.Count; ++i) {
            ImPlotPoint p = getter(i);
            if (p.y == 0)
                continue;
            ImVec2 a = PlotToPixels(p.x - half_width, p.y);
            ImVec2 b = PlotToPixels(p.x + half_width, 0);
            if (rend_fill)
                DrawList.AddRectFilled(a, b, col_fill);
            if (rend_line)
                DrawList.AddRect(a, b, col_line);
        }
        PopPlotClipRect();
    }

    //-----------------------------------------------------------------------------
    // float

    void PlotBars(const char* label_id, const float* values, int count, float width, float shift, int offset, int stride) {
        GetterBarV<float> getter(values, shift, count, offset, stride);
        PlotBarsEx(label_id, getter, width);
    }

    void PlotBars(const char* label_id, const float* xs, const float* ys, int count, float width, int offset, int stride) {
        GetterXsYs<float> getter(xs, ys, count, offset, stride);
        PlotBarsEx(label_id, getter, width);
    }

    //-----------------------------------------------------------------------------
    // double

    void PlotBars(const char* label_id, const double* values, int count, double width, double shift, int offset, int stride) {
        GetterBarV<double> getter(values, shift, count, offset, stride);
        PlotBarsEx(label_id, getter, width);
    }

    void PlotBars(const char* label_id, const double* xs, const double* ys, int count, double width, int offset, int stride) {
        GetterXsYs<double> getter(xs, ys, count, offset, stride);
        PlotBarsEx(label_id, getter, width);
    }

    //-----------------------------------------------------------------------------
    // custom

    void PlotBars(const char* label_id, ImPlotPoint(*getter_func)(void* data, int idx), void* data, int count, double width, int offset) {
        GetterFuncPtrImPlotPoint getter(getter_func, data, count, offset);
        PlotBarsEx(label_id, getter, width);
    }

    //-----------------------------------------------------------------------------
    // PLOT BAR H
    //-----------------------------------------------------------------------------

    // TODO: Migrate to RenderPrimitives

    template <typename Getter, typename THeight>
    void PlotBarsHEx(const char* label_id, Getter getter, THeight height) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PlotBarsH() needs to be called between BeginPlot() and EndPlot()!");

        ImPlotItem* item = RegisterItem(label_id);
        if (!item->Show)
            return;
        TryRecolorItem(item, ImPlotCol_Fill);

        const THeight half_height = height / 2;
        // find data extents
        if (gp.FitThisFrame) {
            for (int i = 0; i < getter.Count; ++i) {
                ImPlotPoint p = getter(i);
                FitPoint(ImPlotPoint(0, p.y - half_height));
                FitPoint(ImPlotPoint(p.x, p.y + half_height));
            }
        }

        ImU32 col_line = ImGui::GetColorU32(GetLineColor(item));
        ImU32 col_fill = ImGui::GetColorU32(GetItemFillColor(item));
        const bool rend_fill = WillFillRender();
        bool rend_line = WillLineRender();
        if (rend_fill && col_line == col_fill)
            rend_line = false;

        PushPlotClipRect();
        ImDrawList& DrawList = *ImGui::GetWindowDrawList();
        for (int i = 0; i < getter.Count; ++i) {
            ImPlotPoint p = getter(i);
            if (p.x == 0)
                continue;
            ImVec2 a = PlotToPixels(0, p.y - half_height);
            ImVec2 b = PlotToPixels(p.x, p.y + half_height);
            if (rend_fill)
                DrawList.AddRectFilled(a, b, col_fill);
            if (rend_line)
                DrawList.AddRect(a, b, col_line);
        }
        PopPlotClipRect();
    }

    //-----------------------------------------------------------------------------
    // float

    void PlotBarsH(const char* label_id, const float* values, int count, float height, float shift, int offset, int stride) {
        GetterBarH<float> getter(values, shift, count, offset, stride);
        PlotBarsHEx(label_id, getter, height);
    }

    void PlotBarsH(const char* label_id, const float* xs, const float* ys, int count, float height, int offset, int stride) {
        GetterXsYs<float> getter(xs, ys, count, offset, stride);
        PlotBarsHEx(label_id, getter, height);
    }

    //-----------------------------------------------------------------------------
    // double

    void PlotBarsH(const char* label_id, const double* values, int count, double height, double shift, int offset, int stride) {
        GetterBarH<double> getter(values, shift, count, offset, stride);
        PlotBarsHEx(label_id, getter, height);
    }

    void PlotBarsH(const char* label_id, const double* xs, const double* ys, int count, double height, int offset, int stride) {
        GetterXsYs<double> getter(xs, ys, count, offset, stride);
        PlotBarsHEx(label_id, getter, height);
    }

    //-----------------------------------------------------------------------------
    // custom

    void PlotBarsH(const char* label_id, ImPlotPoint(*getter_func)(void* data, int idx), void* data, int count, double height, int offset) {
        GetterFuncPtrImPlotPoint getter(getter_func, data, count, offset);
        PlotBarsHEx(label_id, getter, height);
    }

    //-----------------------------------------------------------------------------
    // PLOT ERROR BARS
    //-----------------------------------------------------------------------------

    template <typename Getter>
    void PlotErrorBarsEx(const char* label_id, Getter getter) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PlotErrorBars() needs to be called between BeginPlot() and EndPlot()!");

        ImPlotItem* item = RegisterItem(label_id);
        if (!item->Show)
            return;

        // find data extents
        if (gp.FitThisFrame) {
            for (int i = 0; i < getter.Count; ++i) {
                ImPlotPointError e = getter(i);
                FitPoint(ImPlotPoint(e.x, e.y - e.neg));
                FitPoint(ImPlotPoint(e.x, e.y + e.pos));
            }
        }

        const ImU32 col = ImGui::GetColorU32(GetErrorBarColor());
        const bool rend_whisker = gp.Style.ErrorBarSize > 0;
        const float half_whisker = gp.Style.ErrorBarSize * 0.5f;

        ImDrawList& DrawList = *ImGui::GetWindowDrawList();

        PushPlotClipRect();
        for (int i = 0; i < getter.Count; ++i) {
            ImPlotPointError e = getter(i);
            ImVec2 p1 = PlotToPixels(e.x, e.y - e.neg);
            ImVec2 p2 = PlotToPixels(e.x, e.y + e.pos);
            DrawList.AddLine(p1, p2, col, gp.Style.ErrorBarWeight);
            if (rend_whisker) {
                DrawList.AddLine(p1 - ImVec2(half_whisker, 0), p1 + ImVec2(half_whisker, 0), col, gp.Style.ErrorBarWeight);
                DrawList.AddLine(p2 - ImVec2(half_whisker, 0), p2 + ImVec2(half_whisker, 0), col, gp.Style.ErrorBarWeight);
            }
        }
        PopPlotClipRect();
    }

    //-----------------------------------------------------------------------------
    // float

    void PlotErrorBars(const char* label_id, const float* xs, const float* ys, const float* err, int count, int offset, int stride) {
        GetterError<float> getter(xs, ys, err, err, count, offset, stride);
        PlotErrorBarsEx(label_id, getter);
    }

    void PlotErrorBars(const char* label_id, const float* xs, const float* ys, const float* neg, const float* pos, int count, int offset, int stride) {
        GetterError<float> getter(xs, ys, neg, pos, count, offset, stride);
        PlotErrorBarsEx(label_id, getter);
    }

    //-----------------------------------------------------------------------------
    // double

    void PlotErrorBars(const char* label_id, const double* xs, const double* ys, const double* err, int count, int offset, int stride) {
        GetterError<double> getter(xs, ys, err, err, count, offset, stride);
        PlotErrorBarsEx(label_id, getter);
    }

    void PlotErrorBars(const char* label_id, const double* xs, const double* ys, const double* neg, const double* pos, int count, int offset, int stride) {
        GetterError<double> getter(xs, ys, neg, pos, count, offset, stride);
        PlotErrorBarsEx(label_id, getter);
    }

    //-----------------------------------------------------------------------------
    // PLOT ERROR BARS H
    //-----------------------------------------------------------------------------

    template <typename Getter>
    void PlotErrorBarsHEx(const char* label_id, Getter getter) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PlotErrorBarsH() needs to be called between BeginPlot() and EndPlot()!");

        ImPlotItem* item = RegisterItem(label_id);
        if (!item->Show)
            return;

        // find data extents
        if (gp.FitThisFrame) {
            for (int i = 0; i < getter.Count; ++i) {
                ImPlotPointError e = getter(i);
                FitPoint(ImPlotPoint(e.x - e.neg, e.y));
                FitPoint(ImPlotPoint(e.x + e.pos, e.y));
            }
        }

        const ImU32 col = ImGui::GetColorU32(GetErrorBarColor());
        const bool rend_whisker = gp.Style.ErrorBarSize > 0;
        const float half_whisker = gp.Style.ErrorBarSize * 0.5f;

        ImDrawList& DrawList = *ImGui::GetWindowDrawList();

        PushPlotClipRect();
        for (int i = 0; i < getter.Count; ++i) {
            ImPlotPointError e = getter(i);
            ImVec2 p1 = PlotToPixels(e.x - e.neg, e.y);
            ImVec2 p2 = PlotToPixels(e.x + e.pos, e.y);
            DrawList.AddLine(p1, p2, col, gp.Style.ErrorBarWeight);
            if (rend_whisker) {
                DrawList.AddLine(p1 - ImVec2(0, half_whisker), p1 + ImVec2(0, half_whisker), col, gp.Style.ErrorBarWeight);
                DrawList.AddLine(p2 - ImVec2(0, half_whisker), p2 + ImVec2(0, half_whisker), col, gp.Style.ErrorBarWeight);
            }
        }
        PopPlotClipRect();
    }

    //-----------------------------------------------------------------------------
    // float

    void PlotErrorBarsH(const char* label_id, const float* xs, const float* ys, const float* err, int count, int offset, int stride) {
        GetterError<float> getter(xs, ys, err, err, count, offset, stride);
        PlotErrorBarsHEx(label_id, getter);
    }

    void PlotErrorBarsH(const char* label_id, const float* xs, const float* ys, const float* neg, const float* pos, int count, int offset, int stride) {
        GetterError<float> getter(xs, ys, neg, pos, count, offset, stride);
        PlotErrorBarsHEx(label_id, getter);
    }

    //-----------------------------------------------------------------------------
    // double

    void PlotErrorBarsH(const char* label_id, const double* xs, const double* ys, const double* err, int count, int offset, int stride) {
        GetterError<double> getter(xs, ys, err, err, count, offset, stride);
        PlotErrorBarsHEx(label_id, getter);
    }

    void PlotErrorBarsH(const char* label_id, const double* xs, const double* ys, const double* neg, const double* pos, int count, int offset, int stride) {
        GetterError<double> getter(xs, ys, neg, pos, count, offset, stride);
        PlotErrorBarsHEx(label_id, getter);
    }


    //-----------------------------------------------------------------------------
    // PLOT PIE CHART
    //-----------------------------------------------------------------------------

    inline void RenderPieSlice(ImDrawList& DrawList, const ImPlotPoint& center, double radius, double a0, double a1, ImU32 col) {
        static const float resolution = 50 / (2 * IM_PI);
        static ImVec2 buffer[50];
        buffer[0] = PlotToPixels(center);
        int n = ImMax(3, (int)((a1 - a0) * resolution));
        double da = (a1 - a0) / (n - 1);
        for (int i = 0; i < n; ++i) {
            double a = a0 + i * da;
            buffer[i + 1] = PlotToPixels(center.x + radius * cos(a), center.y + radius * sin(a));
        }
        DrawList.AddConvexPolyFilled(buffer, n + 1, col);
    }

    template <typename T>
    void PlotPieChartEx(const char** label_ids, const T* values, int count, T x, T y, T radius, bool normalize, const char* fmt, T angle0) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PlotPieChart() needs to be called between BeginPlot() and EndPlot()!");
        ImDrawList& DrawList = *ImGui::GetWindowDrawList();

        T sum = 0;
        for (int i = 0; i < count; ++i)
            sum += values[i];

        normalize = normalize || sum > 1.0f;

        ImPlotPoint center(x, y);

        PushPlotClipRect();
        T a0 = angle0 * 2 * IM_PI / 360.0f;
        T a1 = angle0 * 2 * IM_PI / 360.0f;
        for (int i = 0; i < count; ++i) {
            ImPlotItem* item = RegisterItem(label_ids[i]);
            ImU32 col = ImGui::GetColorU32(GetItemFillColor(item));
            T percent = normalize ? values[i] / sum : values[i];
            a1 = a0 + 2 * IM_PI * percent;
            if (item->Show) {
                if (percent < 0.5) {
                    RenderPieSlice(DrawList, center, radius, a0, a1, col);
                }
                else {
                    RenderPieSlice(DrawList, center, radius, a0, a0 + (a1 - a0) * 0.5f, col);
                    RenderPieSlice(DrawList, center, radius, a0 + (a1 - a0) * 0.5f, a1, col);
                }
            }
            a0 = a1;
        }
        if (fmt != NULL) {
            a0 = angle0 * 2 * IM_PI / 360.0f;
            a1 = angle0 * 2 * IM_PI / 360.0f;
            char buffer[32];
            for (int i = 0; i < count; ++i) {
                ImPlotItem* item = GetLegendItem(label_ids[i]);
                T percent = normalize ? values[i] / sum : values[i];
                a1 = a0 + 2 * IM_PI * percent;
                if (item->Show) {
                    sprintf(buffer, fmt, values[i]);
                    ImVec2 size = ImGui::CalcTextSize(buffer);
                    T angle = a0 + (a1 - a0) * 0.5f;
                    ImVec2 pos = PlotToPixels(center.x + 0.5f * radius * cos(angle), center.y + 0.5f * radius * sin(angle));
                    ImU32 col = CalcTextColor(GetItemFillColor(item));
                    DrawList.AddText(pos - size * 0.5f, col, buffer);
                }
                a0 = a1;
            }
        }
        PopPlotClipRect();
    }

    //-----------------------------------------------------------------------------
    // float

    void PlotPieChart(const char** label_ids, const float* values, int count, float x, float y, float radius, bool normalize, const char* fmt, float angle0) {
        return PlotPieChartEx(label_ids, values, count, x, y, radius, normalize, fmt, angle0);
    }

    //-----------------------------------------------------------------------------
    // double

    void PlotPieChart(const char** label_ids, const double* values, int count, double x, double y, double radius, bool normalize, const char* fmt, double angle0) {
        return PlotPieChartEx(label_ids, values, count, x, y, radius, normalize, fmt, angle0);
    }

    //-----------------------------------------------------------------------------
    // PLOT HEATMAP
    //-----------------------------------------------------------------------------

    template <typename T, typename Transformer>
    void RenderHeatmap(Transformer transformer, ImDrawList& DrawList, const T* values, int rows, int cols, T scale_min, T scale_max, const char* fmt, const ImPlotPoint& bounds_min, const ImPlotPoint& bounds_max) {
        const double w = (bounds_max.x - bounds_min.x) / cols;
        const double h = (bounds_max.y - bounds_min.y) / rows;
        const ImPlotPoint half_size(w * 0.5, h * 0.5);
        int i = 0;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                ImPlotPoint p;
                p.x = bounds_min.x + 0.5 * w + c * w;
                p.y = bounds_max.y - (0.5 * h + r * h);
                ImVec2 a = transformer(p.x - half_size.x, p.y - half_size.y);
                ImVec2 b = transformer(p.x + half_size.x, p.y + half_size.y);
                float t = (float)Remap(values[i], scale_min, scale_max, T(0), T(1));
                ImVec4 color = LerpColormap(t);
                color.w *= gp.Style.FillAlpha;
                ImU32 col = ImGui::GetColorU32(color);
                DrawList.AddRectFilled(a, b, col);
                i++;
            }
        }
        if (fmt != NULL) {
            i = 0;
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    ImPlotPoint p;
                    p.x = bounds_min.x + 0.5 * w + c * w;
                    p.y = bounds_min.y + 1 - (0.5 * h + r * h);
                    ImVec2 px = transformer(p);
                    char buff[32];
                    sprintf(buff, fmt, values[i]);
                    ImVec2 size = ImGui::CalcTextSize(buff);
                    float t = (float)Remap(values[i], scale_min, scale_max, T(0), T(1));
                    ImVec4 color = LerpColormap(t);
                    ImU32 col = CalcTextColor(color);
                    DrawList.AddText(px - size * 0.5f, col, buff);
                    i++;
                }
            }
        }
    }

    template <typename T>
    void PlotHeatmapEx(const char* label_id, const T* values, int rows, int cols, T scale_min, T scale_max, const char* fmt, const ImPlotPoint& bounds_min, const ImPlotPoint& bounds_max) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PlotHeatmap() needs to be called between BeginPlot() and EndPlot()!");
        IM_ASSERT_USER_ERROR(scale_min != scale_max, "Scale values must be different!");
        ImPlotItem* item = RegisterItem(label_id);
        if (!item->Show)
            return;
        if (gp.FitThisFrame) {
            FitPoint(bounds_min);
            FitPoint(bounds_max);
        }
        ImDrawList& DrawList = *ImGui::GetWindowDrawList();
        ImGui::PushClipRect(gp.BB_Plot.Min, gp.BB_Plot.Max, true);
        ImPlotState* plot = gp.CurrentPlot;
        int y_axis = plot->CurrentYAxis;
        if (HasFlag(plot->XAxis.Flags, ImPlotAxisFlags_LogScale) && HasFlag(plot->YAxis[y_axis].Flags, ImPlotAxisFlags_LogScale))
            RenderHeatmap(TransformerLogLog(y_axis), DrawList, values, rows, cols, scale_min, scale_max, fmt, bounds_min, bounds_max);
        else if (HasFlag(plot->XAxis.Flags, ImPlotAxisFlags_LogScale))
            RenderHeatmap(TransformerLogLin(y_axis), DrawList, values, rows, cols, scale_min, scale_max, fmt, bounds_min, bounds_max);
        else if (HasFlag(plot->YAxis[y_axis].Flags, ImPlotAxisFlags_LogScale))
            RenderHeatmap(TransformerLinLog(y_axis), DrawList, values, rows, cols, scale_min, scale_max, fmt, bounds_min, bounds_max);
        else
            RenderHeatmap(TransformerLinLin(y_axis), DrawList, values, rows, cols, scale_min, scale_max, fmt, bounds_min, bounds_max);
        ImGui::PopClipRect();
    }

    //-----------------------------------------------------------------------------
    // float

    void PlotHeatmap(const char* label_id, const float* values, int rows, int cols, float scale_min, float scale_max, const char* fmt, const ImPlotPoint& bounds_min, const ImPlotPoint& bounds_max) {
        return PlotHeatmapEx(label_id, values, rows, cols, scale_min, scale_max, fmt, bounds_min, bounds_max);
    }

    //-----------------------------------------------------------------------------
    // double

    void PlotHeatmap(const char* label_id, const double* values, int rows, int cols, double scale_min, double scale_max, const char* fmt, const ImPlotPoint& bounds_min, const ImPlotPoint& bounds_max) {
        return PlotHeatmapEx(label_id, values, rows, cols, scale_min, scale_max, fmt, bounds_min, bounds_max);
    }

    //-----------------------------------------------------------------------------
    // PLOT DIGITAL
    //-----------------------------------------------------------------------------

    template <typename Getter>
    inline void PlotDigitalEx(const char* label_id, Getter getter)
    {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PlotDigital() needs to be called between BeginPlot() and EndPlot()!");

        ImPlotItem* item = RegisterItem(label_id);
        if (!item->Show)
            return;
        TryRecolorItem(item, ImPlotCol_Line);

        // render digital signals as "pixel bases" rectangles
        PushPlotClipRect();
        if (getter.Count > 1 && WillLineRender()) {
            ImDrawList& DrawList = *ImGui::GetWindowDrawList();
            const float line_weight = item->Highlight ? gp.Style.LineWeight * 2 : gp.Style.LineWeight;
            const int y_axis = gp.CurrentPlot->CurrentYAxis;
            int pixYMax = 0;
            ImPlotPoint itemData1 = getter(0);
            for (int i = 0; i < getter.Count; ++i) {
                ImPlotPoint itemData2 = getter(i);
                if (NanOrInf(itemData1.y)) {
                    itemData1 = itemData2;
                    continue;
                }
                if (NanOrInf(itemData2.y)) itemData2.y = ConstrainNan(ConstrainInf(itemData2.y));
                int pixY_0 = (int)(line_weight);
                itemData1.y = ImMax(0.0, itemData1.y);
                float pixY_1_float = gp.Style.DigitalBitHeight * (float)itemData1.y;
                int pixY_1 = (int)(pixY_1_float); //allow only positive values
                int pixY_chPosOffset = (int)(ImMax(gp.Style.DigitalBitHeight, pixY_1_float) + gp.Style.DigitalBitGap);
                pixYMax = ImMax(pixYMax, pixY_chPosOffset);
                ImVec2 pMin = PlotToPixels(itemData1);
                ImVec2 pMax = PlotToPixels(itemData2);
                int pixY_Offset = 20; //20 pixel from bottom due to mouse cursor label
                pMin.y = (gp.PixelRange[y_axis].Min.y) + ((-gp.DigitalPlotOffset) - pixY_Offset);
                pMax.y = (gp.PixelRange[y_axis].Min.y) + ((-gp.DigitalPlotOffset) - pixY_0 - pixY_1 - pixY_Offset);
                //plot only one rectangle for same digital state
                while (((i + 2) < getter.Count) && (itemData1.y == itemData2.y)) {
                    const int in = (i + 1);
                    itemData2 = getter(in);
                    if (NanOrInf(itemData2.y)) break;
                    pMax.x = PlotToPixels(itemData2).x;
                    i++;
                }
                //do not extend plot outside plot range
                if (pMin.x < gp.PixelRange[y_axis].Min.x) pMin.x = gp.PixelRange[y_axis].Min.x;
                if (pMax.x < gp.PixelRange[y_axis].Min.x) pMax.x = gp.PixelRange[y_axis].Min.x;
                if (pMin.x > gp.PixelRange[y_axis].Max.x) pMin.x = gp.PixelRange[y_axis].Max.x;
                if (pMax.x > gp.PixelRange[y_axis].Max.x) pMax.x = gp.PixelRange[y_axis].Max.x;
                //plot a rectangle that extends up to x2 with y1 height
                if ((pMax.x > pMin.x) && (gp.BB_Plot.Contains(pMin) || gp.BB_Plot.Contains(pMax))) {
                    ImVec4 colAlpha = item->Color;
                    colAlpha.w = item->Highlight ? 1.0f : 0.9f;
                    DrawList.AddRectFilled(pMin, pMax, ImGui::GetColorU32(colAlpha));
                }
                itemData1 = itemData2;
            }
            gp.DigitalPlotItemCnt++;
            gp.DigitalPlotOffset += pixYMax;
        }
        PopPlotClipRect();
    }

    //-----------------------------------------------------------------------------
    // float

    void PlotDigital(const char* label_id, const float* xs, const float* ys, int count, int offset, int stride) {
        GetterXsYs<float> getter(xs, ys, count, offset, stride);
        return PlotDigitalEx(label_id, getter);
    }

    //-----------------------------------------------------------------------------
    // double

    void PlotDigital(const char* label_id, const double* xs, const double* ys, int count, int offset, int stride) {
        GetterXsYs<double> getter(xs, ys, count, offset, stride);
        return PlotDigitalEx(label_id, getter);
    }

    //-----------------------------------------------------------------------------
    // custom

    void PlotDigital(const char* label_id, ImPlotPoint(*getter_func)(void* data, int idx), void* data, int count, int offset) {
        GetterFuncPtrImPlotPoint getter(getter_func, data, count, offset);
        return PlotDigitalEx(label_id, getter);
    }

    //-----------------------------------------------------------------------------
    // PLOT TEXT
    //-----------------------------------------------------------------------------
    // float

    void PlotText(const char* text, float x, float y, bool vertical, const ImVec2& pixel_offset) {
        return PlotText(text, (double)x, (double)y, vertical, pixel_offset);
    }

    //-----------------------------------------------------------------------------
    // double
    void PlotText(const char* text, double x, double y, bool vertical, const ImVec2& pixel_offset) {
        IM_ASSERT_USER_ERROR(gp.CurrentPlot != NULL, "PlotText() needs to be called between BeginPlot() and EndPlot()!");
        ImDrawList& DrawList = *ImGui::GetWindowDrawList();
        PushPlotClipRect();
        ImVec2 pos = PlotToPixels(ImPlotPoint(x, y)) + pixel_offset;
        if (vertical)
            AddTextVertical(&DrawList, text, pos, gp.Col_Txt);
        else
            DrawList.AddText(pos, gp.Col_Txt, text);
        PopPlotClipRect();
    }

    //------------------------------------------------------------------------------
    // COLORMAPS
    //------------------------------------------------------------------------------

    void SetColormap(const ImVec4* colors, int num_colors) {
        IM_ASSERT_USER_ERROR(num_colors > 1, "The number of colors must be greater than 1!");
        static ImVector<ImVec4> user_colormap;
        user_colormap.shrink(0);
        user_colormap.reserve(num_colors);
        for (int i = 0; i < num_colors; ++i)
            user_colormap.push_back(colors[i]);
        gp.Colormap = &user_colormap[0];
        gp.ColormapSize = num_colors;
    }

    // Returns the size of the current colormap
    int GetColormapSize() {
        return gp.ColormapSize;
    }

    // Returns a color from the Color map given an index > 0
    ImVec4 GetColormapColor(int index) {
        IM_ASSERT_USER_ERROR(index >= 0, "The Colormap index must be greater than zero!");
        return gp.Colormap[index % gp.ColormapSize];
    }

    ImVec4 LerpColormap(float t) {
        float tc = ImClamp(t, 0.0f, 1.0f);
        int i1 = (int)((gp.ColormapSize - 1) * tc);
        int i2 = i1 + 1;
        if (i2 == gp.ColormapSize)
            return gp.Colormap[i1];
        float t1 = (float)i1 / (float)(gp.ColormapSize - 1);
        float t2 = (float)i2 / (float)(gp.ColormapSize - 1);
        float tr = Remap(t, t1, t2, 0.0f, 1.0f);
        return ImLerp(gp.Colormap[i1], gp.Colormap[i2], tr);
    }

    void ShowColormapScale(double scale_min, double scale_max, float height) {
        static ImVector<ImPlotTick> ticks;
        static ImGuiTextBuffer txt_buff;
        ImPlotRange range;
        range.Min = scale_min;
        range.Max = scale_max;
        ticks.shrink(0);
        txt_buff.Buf.shrink(0);
        AddDefaultTicks(range, 10, 0, false, ticks);
        LabelTicks(ticks, false, txt_buff);
        float max_width = 0;
        for (int i = 0; i < ticks.Size; ++i)
            max_width = ticks[i].Size.x > max_width ? ticks[i].Size.x : max_width;

        ImGuiContext& G = *GImGui;
        ImGuiWindow* Window = G.CurrentWindow;
        if (Window->SkipItems)
            return;
        const ImGuiStyle& Style = G.Style;
        const float txt_off = 5;
        const float bar_w = 20;

        ImDrawList& DrawList = *Window->DrawList;
        ImVec2 size(bar_w + txt_off + max_width + 2 * Style.WindowPadding.x, height);
        ImRect bb_frame = ImRect(Window->DC.CursorPos, Window->DC.CursorPos + size);
        ImGui::ItemSize(bb_frame);
        if (!ImGui::ItemAdd(bb_frame, 0, &bb_frame))
            return;
        ImGui::RenderFrame(bb_frame.Min, bb_frame.Max, ImGui::GetColorU32(ImGuiCol_FrameBg));
        ImRect bb_grad(bb_frame.Min + Style.WindowPadding, bb_frame.Min + ImVec2(bar_w + Style.WindowPadding.x, height - Style.WindowPadding.y));

        int num_cols = GetColormapSize();
        float h_step = (height - 2 * Style.WindowPadding.y) / (num_cols - 1);
        for (int i = 0; i < num_cols - 1; ++i) {
            ImRect rect(bb_grad.Min.x, bb_grad.Min.y + h_step * i, bb_grad.Max.x, bb_grad.Min.y + h_step * (i + 1));
            ImU32 col1 = ImGui::GetColorU32(GetColormapColor(num_cols - 1 - i));
            ImU32 col2 = ImGui::GetColorU32(GetColormapColor(num_cols - 1 - (i + 1)));
            DrawList.AddRectFilledMultiColor(rect.Min, rect.Max, col1, col1, col2, col2);
        }
        ImU32 col_border = gp.Style.Colors[ImPlotCol_PlotBorder].w == -1 ? ImGui::GetColorU32(ImGuiCol_Text, 0.5f) : ImGui::GetColorU32(gp.Style.Colors[ImPlotCol_PlotBorder]);

        ImGui::PushClipRect(bb_frame.Min, bb_frame.Max, true);
        for (int i = 0; i < ticks.Size; ++i) {
            float ypos = Remap((float)ticks[i].PlotPos, (float)range.Max, (float)range.Min, bb_grad.Min.y, bb_grad.Max.y);
            if (ypos < bb_grad.Max.y - 2 && ypos > bb_grad.Min.y + 2)
                DrawList.AddLine(ImVec2(bb_grad.Max.x - 1, ypos), ImVec2(bb_grad.Max.x - (ticks[i].Major ? 10.0f : 5.0f), ypos), col_border, 1.0f);
            DrawList.AddText(ImVec2(bb_grad.Max.x - 1, ypos) + ImVec2(txt_off, -ticks[i].Size.y * 0.5f), ImGui::GetColorU32(ImGuiCol_Text), txt_buff.Buf.Data + ticks[i].TextOffset);
        }
        ImGui::PopClipRect();

        DrawList.AddRect(bb_grad.Min, bb_grad.Max, col_border);

    }

    void SetColormap(ImPlotColormap colormap, int samples) {
        static int csizes[ImPlotColormap_COUNT] = { 10,9,9,12,11,11,11,11,11,11 };
        static OffsetCalculator<ImPlotColormap_COUNT> coffs(csizes);
        static ImVec4 cdata[] = {
            // ImPlotColormap_Default                                  // X11 Named Colors
            ImVec4(0.0f, 0.7490196228f, 1.0f, 1.0f),                   // Blues::DeepSkyBlue,
            ImVec4(1.0f, 0.0f, 0.0f, 1.0f),                            // Reds::Red,
            ImVec4(0.4980392158f, 1.0f, 0.0f, 1.0f),                   // Greens::Chartreuse,
            ImVec4(1.0f, 1.0f, 0.0f, 1.0f),                            // Yellows::Yellow,
            ImVec4(0.0f, 1.0f, 1.0f, 1.0f),                            // Cyans::Cyan,
            ImVec4(1.0f, 0.6470588446f, 0.0f, 1.0f),                   // Oranges::Orange,
            ImVec4(1.0f, 0.0f, 1.0f, 1.0f),                            // Purples::Magenta,
            ImVec4(0.5411764979f, 0.1686274558f, 0.8862745166f, 1.0f), // Purples::BlueViolet,
            ImVec4(0.5f, 0.5f, 0.5f, 1.0f),                            // Grays::Gray50,
            ImVec4(0.8235294223f, 0.7058823705f, 0.5490196347f, 1.0f), // Browns::Tan
            // ImPlotColormap_Dark
            ImVec4(0.894118f, 0.101961f, 0.109804f, 1.0f),
            ImVec4(0.215686f, 0.494118f, 0.721569f, 1.0f),
            ImVec4(0.301961f, 0.686275f, 0.290196f, 1.0f),
            ImVec4(0.596078f, 0.305882f, 0.639216f, 1.0f),
            ImVec4(1.000000f, 0.498039f, 0.000000f, 1.0f),
            ImVec4(1.000000f, 1.000000f, 0.200000f, 1.0f),
            ImVec4(0.650980f, 0.337255f, 0.156863f, 1.0f),
            ImVec4(0.968627f, 0.505882f, 0.749020f, 1.0f),
            ImVec4(0.600000f, 0.600000f, 0.600000f, 1.0f),
            // ImPlotColormap_Pastel
            ImVec4(0.984314f, 0.705882f, 0.682353f, 1.0f),
            ImVec4(0.701961f, 0.803922f, 0.890196f, 1.0f),
            ImVec4(0.800000f, 0.921569f, 0.772549f, 1.0f),
            ImVec4(0.870588f, 0.796078f, 0.894118f, 1.0f),
            ImVec4(0.996078f, 0.850980f, 0.650980f, 1.0f),
            ImVec4(1.000000f, 1.000000f, 0.800000f, 1.0f),
            ImVec4(0.898039f, 0.847059f, 0.741176f, 1.0f),
            ImVec4(0.992157f, 0.854902f, 0.925490f, 1.0f),
            ImVec4(0.949020f, 0.949020f, 0.949020f, 1.0f),
            // ImPlotColormap_Paired
            ImVec4(0.258824f, 0.807843f, 0.890196f, 1.0f),
            ImVec4(0.121569f, 0.470588f, 0.705882f, 1.0f),
            ImVec4(0.698039f, 0.874510f, 0.541176f, 1.0f),
            ImVec4(0.200000f, 0.627451f, 0.172549f, 1.0f),
            ImVec4(0.984314f, 0.603922f, 0.600000f, 1.0f),
            ImVec4(0.890196f, 0.101961f, 0.109804f, 1.0f),
            ImVec4(0.992157f, 0.749020f, 0.435294f, 1.0f),
            ImVec4(1.000000f, 0.498039f, 0.000000f, 1.0f),
            ImVec4(0.792157f, 0.698039f, 0.839216f, 1.0f),
            ImVec4(0.415686f, 0.239216f, 0.603922f, 1.0f),
            ImVec4(1.000000f, 1.000000f, 0.600000f, 1.0f),
            ImVec4(0.694118f, 0.349020f, 0.156863f, 1.0f),
            // ImPlotColormap_Viridis
            ImVec4(0.267004f, 0.004874f, 0.329415f, 1.0f),
            ImVec4(0.282623f, 0.140926f, 0.457517f, 1.0f),
            ImVec4(0.253935f, 0.265254f, 0.529983f, 1.0f),
            ImVec4(0.206756f, 0.371758f, 0.553117f, 1.0f),
            ImVec4(0.163625f, 0.471133f, 0.558148f, 1.0f),
            ImVec4(0.127568f, 0.566949f, 0.550556f, 1.0f),
            ImVec4(0.134692f, 0.658636f, 0.517649f, 1.0f),
            ImVec4(0.266941f, 0.748751f, 0.440573f, 1.0f),
            ImVec4(0.477504f, 0.821444f, 0.318195f, 1.0f),
            ImVec4(0.741388f, 0.873449f, 0.149561f, 1.0f),
            ImVec4(0.993248f, 0.906157f, 0.143936f, 1.0f),
            // ImPlotColormap_Plasma
            ImVec4(5.03830e-02f, 2.98030e-02f, 5.27975e-01f, 1.00000e+00f),
            ImVec4(2.54627e-01f, 1.38820e-02f, 6.15419e-01f, 1.00000e+00f),
            ImVec4(4.17642e-01f, 5.64000e-04f, 6.58390e-01f, 1.00000e+00f),
            ImVec4(5.62738e-01f, 5.15450e-02f, 6.41509e-01f, 1.00000e+00f),
            ImVec4(6.92840e-01f, 1.65141e-01f, 5.64522e-01f, 1.00000e+00f),
            ImVec4(7.98216e-01f, 2.80197e-01f, 4.69538e-01f, 1.00000e+00f),
            ImVec4(8.81443e-01f, 3.92529e-01f, 3.83229e-01f, 1.00000e+00f),
            ImVec4(9.49217e-01f, 5.17763e-01f, 2.95662e-01f, 1.00000e+00f),
            ImVec4(9.88260e-01f, 6.52325e-01f, 2.11364e-01f, 1.00000e+00f),
            ImVec4(9.88648e-01f, 8.09579e-01f, 1.45357e-01f, 1.00000e+00f),
            ImVec4(9.40015e-01f, 9.75158e-01f, 1.31326e-01f, 1.00000e+00f),
            // ImPlotColormap_Hot
            ImVec4(0.2500f,        0.f,        0.f, 1.0f),
            ImVec4(0.5000f,        0.f,        0.f, 1.0f),
            ImVec4(0.7500f,        0.f,        0.f, 1.0f),
            ImVec4(1.0000f,        0.f,        0.f, 1.0f),
            ImVec4(1.0000f,    0.2500f,        0.f, 1.0f),
            ImVec4(1.0000f,    0.5000f,        0.f, 1.0f),
            ImVec4(1.0000f,    0.7500f,        0.f, 1.0f),
            ImVec4(1.0000f,    1.0000f,        0.f, 1.0f),
            ImVec4(1.0000f,    1.0000f,    0.3333f, 1.0f),
            ImVec4(1.0000f,    1.0000f,    0.6667f, 1.0f),
            ImVec4(1.0000f,    1.0000f,    1.0000f, 1.0f),
            // ImPlotColormap_Cool
            ImVec4(0.f,    1.0000f,    1.0000f, 1.0f),
            ImVec4(0.1000f,    0.9000f,    1.0000f, 1.0f),
            ImVec4(0.2000f,    0.8000f,    1.0000f, 1.0f),
            ImVec4(0.3000f,    0.7000f,    1.0000f, 1.0f),
            ImVec4(0.4000f,    0.6000f,    1.0000f, 1.0f),
            ImVec4(0.5000f,    0.5000f,    1.0000f, 1.0f),
            ImVec4(0.6000f,    0.4000f,    1.0000f, 1.0f),
            ImVec4(0.7000f,    0.3000f,    1.0000f, 1.0f),
            ImVec4(0.8000f,    0.2000f,    1.0000f, 1.0f),
            ImVec4(0.9000f,    0.1000f,    1.0000f, 1.0f),
            ImVec4(1.0000f,        0.f,    1.0000f, 1.0f),
            // ImPlotColormap_Pink
            ImVec4(0.2887f,        0.f,        0.f, 1.0f),
            ImVec4(0.4830f,    0.2582f,    0.2582f, 1.0f),
            ImVec4(0.6191f,    0.3651f,    0.3651f, 1.0f),
            ImVec4(0.7303f,    0.4472f,    0.4472f, 1.0f),
            ImVec4(0.7746f,    0.5916f,    0.5164f, 1.0f),
            ImVec4(0.8165f,    0.7071f,    0.5774f, 1.0f),
            ImVec4(0.8563f,    0.8062f,    0.6325f, 1.0f),
            ImVec4(0.8944f,    0.8944f,    0.6831f, 1.0f),
            ImVec4(0.9309f,    0.9309f,    0.8028f, 1.0f),
            ImVec4(0.9661f,    0.9661f,    0.9068f, 1.0f),
            ImVec4(1.0000f,    1.0000f,    1.0000f, 1.0f),
            // ImPlotColormap_Jet
            ImVec4(0.f,        0.f,    0.6667f, 1.0f),
            ImVec4(0.f,        0.f,    1.0000f, 1.0f),
            ImVec4(0.f,    0.3333f,    1.0000f, 1.0f),
            ImVec4(0.f,    0.6667f,    1.0000f, 1.0f),
            ImVec4(0.f,    1.0000f,    1.0000f, 1.0f),
            ImVec4(0.3333f,    1.0000f,    0.6667f, 1.0f),
            ImVec4(0.6667f,    1.0000f,    0.3333f, 1.0f),
            ImVec4(1.0000f,    1.0000f,        0.f, 1.0f),
            ImVec4(1.0000f,    0.6667f,        0.f, 1.0f),
            ImVec4(1.0000f,    0.3333f,        0.f, 1.0f),
            ImVec4(1.0000f,        0.f,        0.f, 1.0f)
        };
        gp.Colormap = &cdata[coffs.Offsets[colormap]];
        gp.ColormapSize = csizes[colormap];
        if (samples > 1) {
            static ImVector<ImVec4> resampled;
            resampled.resize(samples);
            for (int i = 0; i < samples; ++i) {
                float t = i * 1.0f / (samples - 1);
                resampled[i] = LerpColormap(t);
            }
            SetColormap(&resampled[0], samples);
        }
    }

}  // namespace ImPlot