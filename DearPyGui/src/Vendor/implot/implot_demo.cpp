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

#include "implot.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
#define sprintf sprintf_s
#endif

namespace ImPlot {

    /// Choose whether the demo uses double or float versions of the ImPlot API.
    /// NB: You don't ever need to typdef of define values for ImPlot. This
    /// is only being done here for the sake of demoing both precision types.

    // #define IMPLOT_DEMO_USE_DOUBLE
#ifdef IMPLOT_DEMO_USE_DOUBLE
    typedef double t_float;
    typedef ImPlotPoint t_float2;
#define Sin sin
#define Cos cos
#define Pow pow
#define Log log
#define Fmod fmod
#else
    typedef float t_float;
    typedef ImVec2 t_float2;
#define Sin sinf
#define Cos cosf
#define Pow powf
#define Log logf
#define Fmod fmodf
#endif

    t_float RandomRange(t_float min, t_float max) {
        t_float scale = rand() / (t_float)RAND_MAX;
        return min + scale * (max - min);
    }

    // utility structure for realtime plot
    struct ScrollingData {
        int MaxSize;
        int Offset;
        ImVector<t_float2> Data;
        ScrollingData() {
            MaxSize = 2000;
            Offset = 0;
            Data.reserve(MaxSize);
        }
        void AddPoint(t_float x, t_float y) {
            if (Data.size() < MaxSize)
                Data.push_back(t_float2(x, y));
            else {
                Data[Offset] = t_float2(x, y);
                Offset = (Offset + 1) % MaxSize;
            }
        }
        void Erase() {
            if (Data.size() > 0) {
                Data.shrink(0);
                Offset = 0;
            }
        }
    };

    // utility structure for realtime plot
    struct RollingData {
        t_float Span;
        ImVector<t_float2> Data;
        RollingData() {
            Span = 10.0f;
            Data.reserve(2000);
        }
        void AddPoint(t_float x, t_float y) {
            t_float xmod = Fmod(x, Span);
            if (!Data.empty() && xmod < Data.back().x)
                Data.shrink(0);
            Data.push_back(t_float2(xmod, y));
        }
    };

    // utility structure for benchmark data
    struct BenchmarkItem {
        BenchmarkItem() {
            t_float y = RandomRange(0, 1);
            Data = new t_float2[1000];
            for (int i = 0; i < 1000; ++i) {
                Data[i].x = i * 0.001f;
                Data[i].y = y + RandomRange(-0.01f, 0.01f);
            }
            Col = ImVec4((float)RandomRange(0, 1), (float)RandomRange(0, 1), (float)RandomRange(0, 1), 1);
        }
        ~BenchmarkItem() { delete[] Data; }
        t_float2* Data;
        ImVec4 Col;
    };

    void ShowDemoWindow(bool* p_open) {
        static const char* cmap_names[] = { "Default","Dark","Pastel","Paired","Viridis","Plasma","Hot","Cool","Pink","Jet" };
        static bool show_app_metrics = false;
        static bool show_app_style_editor = false;
        if (show_app_metrics) { ImGui::ShowMetricsWindow(&show_app_metrics); }
        if (show_app_style_editor) { ImGui::Begin("Style Editor", &show_app_style_editor); ImGui::ShowStyleEditor(); ImGui::End(); }
        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(530, 750), ImGuiCond_FirstUseEver);
        ImGui::Begin("ImPlot Demo", p_open, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Tools")) {
                ImGui::MenuItem("Metrics", NULL, &show_app_metrics);
                ImGui::MenuItem("Style Editor (ImGui)", NULL, &show_app_style_editor);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        //-------------------------------------------------------------------------
        ImGui::Text("ImPlot says hello. (0.5 WIP)");
        if (ImGui::CollapsingHeader("Help")) {
            ImGui::Text("USER GUIDE:");
            ImGui::BulletText("Left click and drag within the plot area to pan X and Y axes.");
            ImGui::Indent();
            ImGui::BulletText("Left click and drag on an axis to pan an individual axis.");
            ImGui::Unindent();
            ImGui::BulletText("Scroll in the plot area to zoom both X any Y axes.");
            ImGui::Indent();
            ImGui::BulletText("Scroll on an axis to zoom an individual axis.");
            ImGui::Unindent();
            ImGui::BulletText("Right click and drag to box select data.");
            ImGui::Indent();
            ImGui::BulletText("Hold Alt to expand box selection horizontally.");
            ImGui::BulletText("Hold Shift to expand box selection vertically.");
            ImGui::BulletText("Left click while box selecting to cancel the selection.");
            ImGui::Unindent();
            ImGui::BulletText("Double left click to fit all visible data.");
            ImGui::Indent();
            ImGui::BulletText("Double left click on an axis to fit the individual axis.");
            ImGui::Unindent();
            ImGui::BulletText("Double right click to open the plot context menu.");
            ImGui::BulletText("Click legend label icons to show/hide plot items.");
            ImGui::BulletText("IMPORTANT: By default, anti-aliased lines are turned OFF.");
            ImGui::Indent();
            ImGui::BulletText("Software AA can be enabled per plot with ImPlotFlags_AntiAliased.");
            ImGui::BulletText("AA for demo plots can be enabled from the plot's context menu.");
            ImGui::BulletText("If permitable, you are better off using hardware AA (e.g. MSAA).");
            ImGui::Unindent();
#ifdef IMPLOT_DEMO_USE_DOUBLE
            ImGui::BulletText("The demo data precision is: double");
#else
            ImGui::BulletText("The demo data precision is: float");
#endif
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Line Plots")) {
            static t_float xs1[1001], ys1[1001];
            for (int i = 0; i < 1001; ++i) {
                xs1[i] = i * 0.001f;
                ys1[i] = 0.5f + 0.5f * Sin(50 * xs1[i]);
            }
            static t_float xs2[11], ys2[11];
            for (int i = 0; i < 11; ++i) {
                xs2[i] = i * 0.1f;
                ys2[i] = xs2[i] * xs2[i];
            }
            static float weight = ImPlot::GetStyle().LineWeight;
            ImGui::BulletText("Anti-aliasing can be enabled from the plot's context menu (see Help).");
            ImGui::DragFloat("Line Weight", &weight, 0.05f, 1.0f, 5.0f, "%.2f px");
            if (ImPlot::BeginPlot("Line Plot", "x", "f(x)")) {
                ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, weight);
                ImPlot::PlotLine("0.5 + 0.5*sin(50*x)", xs1, ys1, 1001);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Circle);
                ImPlot::PlotLine("x^2", xs2, ys2, 11);
                ImPlot::PopStyleVar(2);
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Filled Line Plots")) {
            static t_float xs1[101], ys1[101], ys2[101], ys3[101];
            srand(0);
            for (int i = 0; i < 101; ++i) {
                xs1[i] = (float)i;
                ys1[i] = RandomRange(400, 450);
                ys2[i] = RandomRange(275, 350);
                ys3[i] = RandomRange(150, 225);
            }
            static bool show_lines = true;
            static bool show_fills = true;
            static float fill_ref = 0;
            ImGui::Checkbox("Lines", &show_lines); ImGui::SameLine();
            ImGui::Checkbox("Fills", &show_fills);
            ImGui::DragFloat("Reference", &fill_ref, 1, -100, 500);
            ImPlot::SetNextPlotLimits(0, 100, 0, 500);
            if (ImPlot::BeginPlot("Stock Prices", "Days", "Price")) {
                if (show_fills) {
                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                    ImPlot::PlotShaded("Stock 1", xs1, ys1, 101, fill_ref);
                    ImPlot::PlotShaded("Stock 2", xs1, ys2, 101, fill_ref);
                    ImPlot::PlotShaded("Stock 3", xs1, ys3, 101, fill_ref);
                    ImPlot::PopStyleVar();
                }
                if (show_lines) {
                    ImPlot::PlotLine("Stock 1", xs1, ys1, 101);
                    ImPlot::PlotLine("Stock 2", xs1, ys2, 101);
                    ImPlot::PlotLine("Stock 3", xs1, ys3, 101);
                }
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Shaded Plots")) {
            static t_float xs[1001], ys[1001], ys1[1001], ys2[1001], ys3[1001], ys4[1001];
            srand(0);
            for (int i = 0; i < 1001; ++i) {
                xs[i] = i * 0.001f;
                ys[i] = 0.25f + 0.25f * Sin(25 * xs[i]) * Sin(5 * xs[i]) + RandomRange(-0.01f, 0.01f);
                ys1[i] = ys[i] + RandomRange(0.1f, 0.12f);
                ys2[i] = ys[i] - RandomRange(0.1f, 0.12f);
                ys3[i] = 0.75f + 0.2f * Sin(25 * xs[i]);
                ys4[i] = 0.75f + 0.1f * Cos(25 * xs[i]);
            }
            static float alpha = 0.25f;
            ImGui::DragFloat("Alpha", &alpha, 0.01f, 0, 1);
            if (ImPlot::BeginPlot("Shaded Plots")) {
                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, alpha);
                ImPlot::PlotShaded("Uncertain Data", xs, ys1, ys2, 1001);
                ImPlot::PlotLine("Uncertain Data", xs, ys, 1001);
                ImPlot::PlotShaded("Overlapping", xs, ys3, ys4, 1001);
                ImPlot::PlotLine("Overlapping", xs, ys3, 1001);
                ImPlot::PlotLine("Overlapping", xs, ys4, 1001);
                ImPlot::PopStyleVar();
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Scatter Plots")) {
            srand(0);
            static t_float xs1[100], ys1[100];
            for (int i = 0; i < 100; ++i) {
                xs1[i] = i * 0.01f;
                ys1[i] = xs1[i] + 0.1f * ((t_float)rand() / (t_float)RAND_MAX);
            }
            static t_float xs2[50], ys2[50];
            for (int i = 0; i < 50; i++) {
                xs2[i] = 0.25f + 0.2f * ((t_float)rand() / (t_float)RAND_MAX);
                ys2[i] = 0.75f + 0.2f * ((t_float)rand() / (t_float)RAND_MAX);
            }
            if (ImPlot::BeginPlot("Scatter Plot", NULL, NULL)) {
                ImPlot::PlotScatter("Data 1", xs1, ys1, 100);
                ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 6);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Square);
                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                ImPlot::PlotScatter("Data 2", xs2, ys2, 50);
                ImPlot::PopStyleVar(3);
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Bar Plots")) {
            static const char* labels[] = { "S1","S2","S3","S4","S5","S6","S7","S8","S9","S10" };
            static const double positions[] = { 0,1,2,3,4,5,6,7,8,9 };
            static bool horz = false;
            ImGui::Checkbox("Horizontal", &horz);
            if (horz) {
                ImPlot::SetNextPlotLimits(0, 110, -0.5, 9.5, ImGuiCond_Always);
                ImPlot::SetNextPlotTicksY(positions, 10, labels);
            }
            else {
                ImPlot::SetNextPlotLimits(-0.5, 9.5, 0, 110, ImGuiCond_Always);
                ImPlot::SetNextPlotTicksX(positions, 10, labels);
            }
            if (ImPlot::BeginPlot("Bar Plot", horz ? "Score" : "Student", horz ? "Student" : "Score")) {
                static t_float midtm[10] = { 83, 67, 23, 89, 83, 78, 91, 82, 85, 90 };
                static t_float final[10] = { 80, 62, 56, 99, 55, 78, 88, 78, 90, 100 };
                static t_float grade[10] = { 80, 69, 52, 92, 72, 78, 75, 76, 89, 95 };
                if (horz) {
                    ImPlot::PlotBarsH("Midterm Exam", midtm, 10, 0.2f, -0.2f);
                    ImPlot::PlotBarsH("Final Exam", final, 10, 0.2f, 0);
                    ImPlot::PlotBarsH("Course Grade", grade, 10, 0.2f, 0.2f);
                }
                else {
                    ImPlot::PlotBars("Midterm Exam", midtm, 10, 0.2f, -0.2f);
                    ImPlot::PlotBars("Final Exam", final, 10, 0.2f, 0);
                    ImPlot::PlotBars("Course Grade", grade, 10, 0.2f, 0.2f);
                }
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Error Bars")) {
            t_float xs[5] = { 1,2,3,4,5 };
            t_float bar[5] = { 1,2,5,3,4 };
            t_float lin1[5] = { 8,8,9,7,8 };
            t_float lin2[5] = { 6,7,6,9,6 };
            t_float err1[5] = { 0.2f, 0.4f, 0.2f, 0.6f, 0.4f };
            t_float err2[5] = { 0.4f, 0.2f, 0.4f, 0.8f, 0.6f };
            t_float err3[5] = { 0.09f, 0.14f, 0.09f, 0.12f, 0.16f };
            t_float err4[5] = { 0.02f, 0.08f, 0.15f, 0.05f, 0.2f };
            static float size = ImPlot::GetStyle().ErrorBarSize;
            static float weight = ImPlot::GetStyle().ErrorBarWeight;
            ImGui::DragFloat("Error Bar Size", &size, 0.1f, 0, 10, "%.2f px");
            ImGui::DragFloat("Error Bar Weight", &weight, 0.01f, 1, 3, "%.2f px");
            ImPlot::SetNextPlotLimits(0, 6, 0, 10);
            if (ImPlot::BeginPlot("##ErrorBars", NULL, NULL)) {
                ImPlot::PushStyleVar(ImPlotStyleVar_ErrorBarSize, size);
                ImPlot::PushStyleVar(ImPlotStyleVar_ErrorBarWeight, weight);
                ImPlot::PlotBars("Bar", xs, bar, 5, 0.5f);
                // error bars can be grouped with the associated item by using the same label ID
                ImPlot::PlotErrorBars("Bar", xs, bar, err1, 5);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Circle);
                ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 3);
                ImPlot::PushStyleColor(ImPlotCol_ErrorBar, ImPlot::GetColormapColor(1));
                ImPlot::PlotErrorBars("Line1", xs, lin1, err1, err2, 5);
                ImPlot::PlotLine("Line1", xs, lin1, 5);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Square);
                ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 3);
                ImPlot::PushStyleColor(ImPlotCol_ErrorBar, ImPlot::GetColormapColor(2));
                ImPlot::PlotErrorBars("Line2", xs, lin2, err2, 5);
                ImPlot::PlotErrorBarsH("Line2", xs, lin2, err3, err4, 5);
                ImPlot::PlotLine("Line2", xs, lin2, 5);
                ImPlot::PopStyleVar(6);
                ImPlot::PopStyleColor(2);
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Pie Charts")) {
            static const char* labels1[] = { "Frogs","Hogs","Dogs","Logs" };
            static float data1[] = { 0.15f,  0.30f,  0.2f, 0.05f };
            static bool normalize = false;
            ImGui::SetNextItemWidth(250);
            ImGui::DragFloat4("Values", data1, 0.01f, 0, 1);
            if ((data1[0] + data1[1] + data1[2] + data1[3]) < 1) {
                ImGui::SameLine();
                ImGui::Checkbox("Normalize", &normalize);
            }
            SetNextPlotLimits(0, 1, 0, 1, ImGuiCond_Always);
            if (ImPlot::BeginPlot("##Pie1", NULL, NULL, ImVec2(250, 250), ImPlotFlags_Legend, 0, 0)) {
                ImPlot::PlotPieChart(labels1, data1, 4, 0.5f, 0.5f, 0.4f, normalize, "%.2f");
                ImPlot::EndPlot();
            }
            ImGui::SameLine();
            ImPlot::SetColormap(ImPlotColormap_Cool, 5);
            SetNextPlotLimits(0, 1, 0, 1, ImGuiCond_Always);
            static const char* labels2[] = { "A","B","C","D","E" };
            static t_float data2[] = { 1,1,2,3,5 };
            if (ImPlot::BeginPlot("##Pie2", NULL, NULL, ImVec2(250, 250), ImPlotFlags_Legend, 0, 0)) {
                ImPlot::PlotPieChart(labels2, data2, 5, 0.5f, 0.5f, 0.4f, true, "%.0f", 180);
                ImPlot::EndPlot();
            }
            ImPlot::SetColormap(ImPlotColormap_Default);
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Heatmaps")) {
            static t_float values1[7][7] = { {0.8f, 2.4f, 2.5f, 3.9f, 0.0f, 4.0f, 0.0f},
                                            {2.4f, 0.0f, 4.0f, 1.0f, 2.7f, 0.0f, 0.0f},
                                            {1.1f, 2.4f, 0.8f, 4.3f, 1.9f, 4.4f, 0.0f},
                                            {0.6f, 0.0f, 0.3f, 0.0f, 3.1f, 0.0f, 0.0f},
                                            {0.7f, 1.7f, 0.6f, 2.6f, 2.2f, 6.2f, 0.0f},
                                            {1.3f, 1.2f, 0.0f, 0.0f, 0.0f, 3.2f, 5.1f},
                                            {0.1f, 2.0f, 0.0f, 1.4f, 0.0f, 1.9f, 6.3f} };
            static float scale_min = 0;
            static float scale_max = 6.3f;
            static t_float values2[100 * 100];
            for (int i = 0; i < 100 * 100; ++i) {
                values2[i] = RandomRange(0, 1);
            }
            static ImPlotColormap map = ImPlotColormap_Viridis;
            if (ImGui::Button("Change Colormap", ImVec2(225, 0)))
                map = (map + 1) % ImPlotColormap_COUNT;
            ImPlot::SetColormap(map);
            ImGui::SameLine();
            ImGui::LabelText("##Colormap Index", "%s", cmap_names[map]);
            ImGui::SetNextItemWidth(225);
            ImGui::DragFloat("Max", &scale_max, 0.01f, 0.1f, 20);
            static ImPlotAxisFlags axes_flags = ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax | ImPlotAxisFlags_TickLabels;
            static const char* xlabels[] = { "C1","C2","C3","C4","C5","C6","C7" };
            static const char* ylabels[] = { "R1","R2","R3","R4","R5","R6","R7" };

            SetNextPlotTicksX(0 + 1.0 / 14.0, 1 - 1.0 / 14.0, 7, xlabels);
            SetNextPlotTicksY(1 - 1.0 / 14.0, 0 + 1.0 / 14.0, 7, ylabels);

            if (ImPlot::BeginPlot("##Heatmap1", NULL, NULL, ImVec2(225, 225), 0, axes_flags, axes_flags)) {
                ImPlot::PlotHeatmap("heat", values1[0], 7, 7, scale_min, scale_max);
                ImPlot::EndPlot();
            }
            ImGui::SameLine();
            ImPlot::ShowColormapScale(scale_min, scale_max, 225);
            ImPlot::SetColormap(ImPlotColormap_Default);
            ImGui::SameLine();
            static ImVec4 gray[2] = { ImVec4(0,0,0,1), ImVec4(1,1,1,1) };
            ImPlot::SetColormap(&gray[0], 2);
            ImPlot::SetNextPlotLimits(-1, 1, -1, 1);
            if (ImPlot::BeginPlot("##Heatmap2", NULL, NULL, ImVec2(225, 225), ImPlotFlags_ContextMenu, 0, 0)) {
                ImPlot::PlotHeatmap("heat1", values2, 100, 100, 0, 1, NULL);
                ImPlot::PlotHeatmap("heat2", values2, 100, 100, 0, 1, NULL, ImPlotPoint(-1, -1), ImPlotPoint(0, 0));
                ImPlot::EndPlot();
            }
            ImPlot::SetColormap(ImPlotColormap_Default);
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Realtime Plots")) {
            ImGui::BulletText("Move your mouse to change the data!");
            ImGui::BulletText("This example assumes 60 FPS. Higher FPS requires larger buffer size.");
            static bool paused = false;
            static ScrollingData sdata1, sdata2;
            static RollingData   rdata1, rdata2;
            ImVec2 mouse = ImGui::GetMousePos();
            static t_float t = 0;
            if (!paused) {
                t += ImGui::GetIO().DeltaTime;
                sdata1.AddPoint(t, mouse.x * 0.0005f);
                rdata1.AddPoint(t, mouse.x * 0.0005f);
                sdata2.AddPoint(t, mouse.y * 0.0005f);
                rdata2.AddPoint(t, mouse.y * 0.0005f);
            }
            static float history = 10.0f;
            ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
            rdata1.Span = history;
            rdata2.Span = history;
            ImPlot::SetNextPlotLimitsX(t - history, t, paused ? ImGuiCond_Once : ImGuiCond_Always);
            static int rt_axis = ImPlotAxisFlags_Default & ~ImPlotAxisFlags_TickLabels;
            if (ImPlot::BeginPlot("##Scrolling", NULL, NULL, ImVec2(-1, 150), ImPlotFlags_Default, rt_axis, rt_axis | ImPlotAxisFlags_LockMin)) {
                ImPlot::PlotLine("Data 1", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), sdata1.Offset, 2 * sizeof(t_float));
                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                ImPlot::PlotShaded("Data 2", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(t_float));
                ImPlot::PopStyleVar();
                ImPlot::EndPlot();
            }
            ImPlot::SetNextPlotLimitsX(0, history, ImGuiCond_Always);
            if (ImPlot::BeginPlot("##Rolling", NULL, NULL, ImVec2(-1, 150), ImPlotFlags_Default, rt_axis, rt_axis)) {
                ImPlot::PlotLine("Data 1", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 2 * sizeof(t_float));
                ImPlot::PlotLine("Data 2", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 2 * sizeof(t_float));
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Colormaps, Markers, and Text")) {
            static ImPlotColormap map = ImPlotColormap_Default;
            if (ImGui::Button("Change Colormap##2"))
                map = (map + 1) % ImPlotColormap_COUNT;
            ImGui::SameLine();
            ImGui::LabelText("##Colormap Index", "%s", cmap_names[map]);
            static float mk_size = ImPlot::GetStyle().MarkerSize;
            static float mk_weight = ImPlot::GetStyle().MarkerWeight;
            ImGui::DragFloat("Marker Size", &mk_size, 0.1f, 2.0f, 10.0f, "%.2f px");
            ImGui::DragFloat("Marker Weight", &mk_weight, 0.05f, 0.5f, 3.0f, "%.2f px");
            ImGui::PushID(map); // NB: This is merely a workaround so that the demo can cycle color maps. You wouldn't need to do this in your own code!
            ImPlot::SetNextPlotLimits(0, 10, 0, 12);
            if (ImPlot::BeginPlot("##MarkerStyles", NULL, NULL, ImVec2(-1, 0), 0, 0, 0)) {
                ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, mk_size);
                ImPlot::PushStyleVar(ImPlotStyleVar_MarkerWeight, mk_weight);
                ImPlot::SetColormap(map);
                t_float xs[2] = { 1,4 };
                t_float ys[2] = { 10,11 };
                // filled
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Circle);
                ImPlot::PlotLine("Circle##Fill", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Square);   ys[0]--; ys[1]--;
                ImPlot::PlotLine("Square##Fill", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Diamond);  ys[0]--; ys[1]--;
                ImPlot::PlotLine("Diamond##Fill", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Up);       ys[0]--; ys[1]--;
                ImPlot::PlotLine("Up##Fill", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Down);     ys[0]--; ys[1]--;
                ImPlot::PlotLine("Down##Fill", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Left);     ys[0]--; ys[1]--;
                ImPlot::PlotLine("Left##Fill", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Right);    ys[0]--; ys[1]--;
                ImPlot::PlotLine("Right##Fill", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Cross);    ys[0]--; ys[1]--;
                ImPlot::PlotLine("Cross##Fill", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Plus);     ys[0]--; ys[1]--;
                ImPlot::PlotLine("Plus##Fill", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Asterisk); ys[0]--; ys[1]--;
                ImPlot::PlotLine("Asterisk##Fill", xs, ys, 2);
                ImPlot::PopStyleVar(10);

                xs[0] = 6; xs[1] = 9;
                ys[0] = 10; ys[1] = 11;
                ImPlot::PushStyleColor(ImPlotCol_MarkerFill, ImVec4(0, 0, 0, 0));
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Circle);
                ImPlot::PlotLine("Circle", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Square);   ys[0]--; ys[1]--;
                ImPlot::PlotLine("Square", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Diamond);  ys[0]--; ys[1]--;
                ImPlot::PlotLine("Diamond", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Up);       ys[0]--; ys[1]--;
                ImPlot::PlotLine("Up", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Down);     ys[0]--; ys[1]--;
                ImPlot::PlotLine("Down", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Left);     ys[0]--; ys[1]--;
                ImPlot::PlotLine("Left", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Right);    ys[0]--; ys[1]--;
                ImPlot::PlotLine("Right", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Cross);    ys[0]--; ys[1]--;
                ImPlot::PlotLine("Cross", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Plus);     ys[0]--; ys[1]--;
                ImPlot::PlotLine("Plus", xs, ys, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Asterisk); ys[0]--; ys[1]--;
                ImPlot::PlotLine("Asterisk", xs, ys, 2);
                ImPlot::PopStyleColor();
                ImPlot::PopStyleVar(10);

                xs[0] = 5; xs[1] = 5;
                ys[0] = 1; ys[1] = 11;

                ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 8);
                ImPlot::PushStyleVar(ImPlotStyleVar_MarkerWeight, 2);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Circle | ImPlotMarker_Cross);
                ImPlot::PushStyleColor(ImPlotCol_MarkerOutline, ImVec4(0, 0, 0, 1));
                ImPlot::PushStyleColor(ImPlotCol_MarkerFill, ImVec4(1, 1, 1, 1));
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0, 0, 0, 1));
                ImPlot::PlotLine("Circle|Cross", xs, ys, 2);
                ImPlot::PopStyleVar(6);
                ImPlot::PopStyleColor(3);

                ImPlot::PlotText("Filled Markers", 1.5, 11.75);
                ImPlot::PlotText("Open Markers", 6.75, 11.75);
                ImPlot::PlotText("Fancy Markers", 4.5, 4.25, true);

                ImPlot::SetColormap(ImPlotColormap_Default);

                ImPlot::EndPlot();
            }
            ImGui::PopID();
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Log Scale")) {
            ImGui::BulletText("Open the plot context menu (double right click) to change scales.");
            static t_float xs[1001], ys1[1001], ys2[1001], ys3[1001];
            for (int i = 0; i < 1001; ++i) {
                xs[i] = i * 0.1f;
                ys1[i] = Sin(xs[i]) + 1;
                ys2[i] = Log(xs[i]);
                ys3[i] = Pow(10.0f, xs[i]);
            }
            ImPlot::SetNextPlotLimits(0.1, 100, 0, 10);
            if (ImPlot::BeginPlot("Log Plot", NULL, NULL, ImVec2(-1, 0), ImPlotFlags_Default, ImPlotAxisFlags_Default | ImPlotAxisFlags_LogScale)) {
                ImPlot::PlotLine("f(x) = x", xs, xs, 1001);
                ImPlot::PlotLine("f(x) = sin(x)+1", xs, ys1, 1001);
                ImPlot::PlotLine("f(x) = log(x)", xs, ys2, 1001);
                ImPlot::PlotLine("f(x) = 10^x", xs, ys3, 21);
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Multiple Y-Axes")) {
            static ImVec4 txt_col = ImGui::GetStyle().Colors[ImGuiCol_Text];
            txt_col.w = 0.25f;
            static ImVec4 y1_col = txt_col;
            static ImVec4 y2_col = txt_col;
            static ImVec4 y3_col = txt_col;

            static t_float xs[1001], xs2[1001], ys1[1001], ys2[1001], ys3[1001];
            static bool y2_axis = true;
            static bool y3_axis = false;
            ImGui::Checkbox("Y-Axis 2", &y2_axis);
            ImGui::SameLine();
            ImGui::Checkbox("Y-Axis 3", &y3_axis);
            ImGui::SameLine();
            ImGui::ColorEdit4("##Col1", &y1_col.x, ImGuiColorEditFlags_NoInputs);
            ImGui::SameLine();
            ImGui::ColorEdit4("##Col2", &y2_col.x, ImGuiColorEditFlags_NoInputs);
            ImGui::SameLine();
            ImGui::ColorEdit4("##Col3", &y3_col.x, ImGuiColorEditFlags_NoInputs);
            for (int i = 0; i < 1001; ++i) {
                xs[i] = (i * 0.1f);
                ys1[i] = Sin(xs[i]) * 3 + 1;
                ys2[i] = Cos(xs[i]) * 0.2f + 0.5f;
                ys3[i] = Sin(xs[i] + 0.5f) * 100 + 200;
                xs2[i] = xs[i] + 10.0f;
            }
            ImPlot::SetNextPlotLimits(0.1, 100, 0, 10);
            ImPlot::SetNextPlotLimitsY(0, 1, ImGuiCond_Once, 1);
            ImPlot::SetNextPlotLimitsY(0, 300, ImGuiCond_Once, 2);
            ImPlot::PushStyleColor(ImPlotCol_YAxis, y1_col);
            ImPlot::PushStyleColor(ImPlotCol_YAxis2, y2_col);
            ImPlot::PushStyleColor(ImPlotCol_YAxis3, y3_col);

            if (ImPlot::BeginPlot("Multi-Axis Plot", NULL, NULL, ImVec2(-1, 0),
                ImPlotFlags_Default |
                (y2_axis ? ImPlotFlags_YAxis2 : 0) |
                (y3_axis ? ImPlotFlags_YAxis3 : 0))) {
                ImPlot::PlotLine("f(x) = x", xs, xs, 1001);
                ImPlot::PlotLine("f(x) = sin(x)*3+1", xs, ys1, 1001);

                if (y2_axis) {
                    ImPlot::SetPlotYAxis(1);
                    ImPlot::PlotLine("f(x) = cos(x)*.2+.5 (Y2)", xs, ys2, 1001);
                }

                if (y3_axis) {
                    ImPlot::SetPlotYAxis(2);
                    ImPlot::PlotLine("f(x) = sin(x+.5)*100+200 (Y3)", xs2, ys3, 1001);
                }

                ImPlot::EndPlot();
            }
            ImPlot::PopStyleColor(3);
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Querying")) {
            ImGui::BulletText("Ctrl + click in the plot area to draw points.");
            ImGui::BulletText("Middle click (or Ctrl + right click) and drag to create a query rect.");
            ImGui::Indent();
            ImGui::BulletText("Hold Alt to expand query horizontally.");
            ImGui::BulletText("Hold Shift to expand query vertically.");
            ImGui::BulletText("The query rect can be dragged after it's created.");
            ImGui::Unindent();
            static ImVector<t_float2> data;
            ImPlotLimits range, query;
            if (ImPlot::BeginPlot("##Drawing", NULL, NULL, ImVec2(-1, 0), ImPlotFlags_Default | ImPlotFlags_Query, ImPlotAxisFlags_GridLines, ImPlotAxisFlags_GridLines)) {
                if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyCtrl) {
                    ImPlotPoint pt = ImPlot::GetPlotMousePos();

                    data.push_back(t_float2((t_float)pt.x, (t_float)pt.y));
                }
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Diamond);
                if (data.size() > 0)
                    ImPlot::PlotScatter("Points", &data[0].x, &data[0].y, data.size(), 0, 2 * sizeof(t_float));
                if (ImPlot::IsPlotQueried() && data.size() > 0) {
                    ImPlotLimits range2 = ImPlot::GetPlotQuery();
                    int cnt = 0;
                    t_float2 avg;
                    for (int i = 0; i < data.size(); ++i) {
                        if (range2.Contains(data[i].x, data[i].y)) {
                            avg.x += data[i].x;
                            avg.y += data[i].y;
                            cnt++;
                        }
                    }
                    if (cnt > 0) {
                        avg.x = avg.x / cnt;
                        avg.y = avg.y / cnt;
                        ImPlot::PlotScatter("Average", &avg.x, &avg.y, 1);
                    }
                }
                ImPlot::PopStyleVar();
                range = ImPlot::GetPlotLimits();
                query = ImPlot::GetPlotQuery();
                ImPlot::EndPlot();
            }
            ImGui::Text("The current plot limits are:  [%g,%g,%g,%g]", range.X.Min, range.X.Max, range.Y.Min, range.Y.Max);
            ImGui::Text("The current query limits are: [%g,%g,%g,%g]", query.X.Min, query.X.Max, query.Y.Min, query.Y.Max);
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Views")) {
            // mimic's soulthread's imgui_plot demo
            static t_float x_data[512];
            static t_float y_data1[512];
            static t_float y_data2[512];
            static t_float y_data3[512];
            static t_float sampling_freq = 44100;
            static t_float freq = 500;
            for (size_t i = 0; i < 512; ++i) {
                const t_float t = i / sampling_freq;
                x_data[i] = t;
                const t_float arg = 2 * 3.14f * freq * t;
                y_data1[i] = Sin(arg);
                y_data2[i] = y_data1[i] * -0.6f + Sin(2 * arg) * 0.4f;
                y_data3[i] = y_data2[i] * -0.6f + Sin(3 * arg) * 0.4f;
            }
            ImGui::BulletText("Query the first plot to render a subview in the second plot (see above for controls).");
            ImPlot::SetNextPlotLimits(0, 0.01, -1, 1);
            ImPlotAxisFlags flgs = ImPlotAxisFlags_Default & ~ImPlotAxisFlags_TickLabels;
            ImPlotLimits query;
            if (ImPlot::BeginPlot("##View1", NULL, NULL, ImVec2(-1, 150), ImPlotFlags_Default | ImPlotFlags_Query, flgs, flgs)) {
                ImPlot::PlotLine("Signal 1", x_data, y_data1, 512);
                ImPlot::PlotLine("Signal 2", x_data, y_data2, 512);
                ImPlot::PlotLine("Signal 3", x_data, y_data3, 512);
                query = ImPlot::GetPlotQuery();
                ImPlot::EndPlot();
            }
            ImPlot::SetNextPlotLimits(query.X.Min, query.X.Max, query.Y.Min, query.Y.Max, ImGuiCond_Always);
            if (ImPlot::BeginPlot("##View2", NULL, NULL, ImVec2(-1, 150), 0, 0, 0)) {
                ImPlot::PlotLine("Signal 1", x_data, y_data1, 512);
                ImPlot::PlotLine("Signal 2", x_data, y_data2, 512);
                ImPlot::PlotLine("Signal 3", x_data, y_data3, 512);
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Drag and Drop")) {
            const int K_CHANNELS = 9;
            srand((int)(10000000 * ImGui::GetTime()));
            static bool paused = false;
            static bool init = true;
            static ScrollingData data[K_CHANNELS];
            static bool show[K_CHANNELS];
            static int yAxis[K_CHANNELS];
            if (init) {
                for (int i = 0; i < K_CHANNELS; ++i) {
                    show[i] = false;
                    yAxis[i] = 0;
                }
                init = false;
            }
            ImGui::BulletText("Drag data items from the left column onto the plot or onto a specific y-axis.");
            ImGui::BeginGroup();
            if (ImGui::Button("Clear", ImVec2(100, 0))) {
                for (int i = 0; i < K_CHANNELS; ++i) {
                    show[i] = false;
                    data[i].Data.shrink(0);
                    data[i].Offset = 0;
                }
            }
            if (ImGui::Button(paused ? "Resume" : "Pause", ImVec2(100, 0)))
                paused = !paused;
            ImGui::Separator();
            for (int i = 0; i < K_CHANNELS; ++i) {
                char label[8];
                sprintf(label, show[i] ? "data_%d*" : "data_%d", i);
                ImGui::Selectable(label, false, 0, ImVec2(100, 0));
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                    ImGui::SetDragDropPayload("DND_PLOT", &i, sizeof(int));
                    ImGui::TextUnformatted(label);
                    ImGui::EndDragDropSource();
                }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            srand((unsigned int)ImGui::GetTime() * 10000000);
            static t_float t = 0;
            if (!paused) {
                t += ImGui::GetIO().DeltaTime;
                for (int i = 0; i < K_CHANNELS; ++i) {
                    if (show[i])
                        data[i].AddPoint(t, (i + 1) * 0.1f + RandomRange(-0.01f, 0.01f));
                }
            }
            ImPlot::SetNextPlotLimitsX((double)t - 10, t, paused ? ImGuiCond_Once : ImGuiCond_Always);
            if (ImPlot::BeginPlot("##DND", NULL, NULL, ImVec2(-1, 0), ImPlotFlags_Legend | ImPlotFlags_Highlight | ImPlotFlags_BoxSelect | ImPlotFlags_ContextMenu | ImPlotFlags_NoChild | ImPlotFlags_YAxis2 | ImPlotFlags_YAxis3)) {
                for (int i = 0; i < K_CHANNELS; ++i) {
                    if (show[i] && data[i].Data.size() > 0) {
                        char label[K_CHANNELS];
                        sprintf(label, "data_%d", i);
                        ImPlot::SetPlotYAxis(yAxis[i]);
                        ImPlot::PlotLine(label, &data[i].Data[0].x, &data[i].Data[0].y, data[i].Data.size(), data[i].Offset, 2 * sizeof(t_float));
                    }
                }
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PLOT")) {
                        int i = *(int*)payload->Data;
                        show[i] = true;
                        for (int y = 0; y < 3; y++) {
                            if (ImPlot::IsPlotYAxisHovered(y))
                                yAxis[i] = y;
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Digital and Analog Signals")) {
            static bool paused = false;
#define K_PLOT_DIGITAL_CH_COUNT 4
#define K_PLOT_ANALOG_CH_COUNT  4
            static ScrollingData dataDigital[K_PLOT_DIGITAL_CH_COUNT];
            static ScrollingData dataAnalog[K_PLOT_ANALOG_CH_COUNT];
            static bool showDigital[K_PLOT_DIGITAL_CH_COUNT];
            static bool showAnalog[K_PLOT_ANALOG_CH_COUNT];

            ImGui::BulletText("You can plot digital and analog signals on the same plot.");
            ImGui::BulletText("Digital signals do not respond to Y drag and zoom, so that");
            ImGui::Indent();
            ImGui::Text("you can drag analog signals over the rising/falling digital edge.");
            ImGui::Unindent();
            ImGui::BeginGroup();
            if (ImGui::Button("Clear", ImVec2(100, 0))) {
                for (int i = 0; i < K_PLOT_DIGITAL_CH_COUNT; ++i)
                    showDigital[i] = false;
                for (int i = 0; i < K_PLOT_ANALOG_CH_COUNT; ++i)
                    showAnalog[i] = false;
            }
            if (ImGui::Button(paused ? "Resume" : "Pause", ImVec2(100, 0)))
                paused = !paused;
            ImGui::SetNextItemWidth(100);
            static float bitHeight = 8;
            ImGui::DragFloat("##Bit Height", &bitHeight, 1, 5, 25, "%.0f px");
            ImGui::SetNextItemWidth(100);
            static float bitGap = 4;
            ImGui::DragFloat("##Bit Gap", &bitGap, 1, 2, 20, "%.0f px");
            ImGui::Separator();
            for (int i = 0; i < K_PLOT_DIGITAL_CH_COUNT; ++i) {
                char label[32];
                sprintf(label, "digital_%d", i);
                ImGui::Checkbox(label, &showDigital[i]);
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                    ImGui::SetDragDropPayload("DND_DIGITAL_PLOT", &i, sizeof(int));
                    ImGui::TextUnformatted(label);
                    ImGui::EndDragDropSource();
                }
            }
            for (int i = 0; i < K_PLOT_ANALOG_CH_COUNT; ++i) {
                char label[32];
                sprintf(label, "analog_%d", i);
                ImGui::Checkbox(label, &showAnalog[i]);
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                    ImGui::SetDragDropPayload("DND_ANALOG_PLOT", &i, sizeof(int));
                    ImGui::TextUnformatted(label);
                    ImGui::EndDragDropSource();
                }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            static t_float t = 0;
            if (!paused) {
                t += ImGui::GetIO().DeltaTime;
                //digital signal values
                int i = 0;
                if (showDigital[i])
                    dataDigital[i].AddPoint(t, Sin(2 * t) > 0.45);
                i++;
                if (showDigital[i])
                    dataDigital[i].AddPoint(t, Sin(2 * t) < 0.45);
                i++;
                if (showDigital[i])
                    dataDigital[i].AddPoint(t, Fmod(t, 5.0f));
                i++;
                if (showDigital[i])
                    dataDigital[i].AddPoint(t, Sin(2 * t) < 0.17);
                //Analog signal values
                i = 0;
                if (showAnalog[i])
                    dataAnalog[i].AddPoint(t, Sin(2 * t));
                i++;
                if (showAnalog[i])
                    dataAnalog[i].AddPoint(t, Cos(2 * t));
                i++;
                if (showAnalog[i])
                    dataAnalog[i].AddPoint(t, Sin(2 * t) * Cos(2 * t));
                i++;
                if (showAnalog[i])
                    dataAnalog[i].AddPoint(t, Sin(2 * t) - Cos(2 * t));
            }
            ImPlot::SetNextPlotLimitsY(-1, 1);
            ImPlot::SetNextPlotLimitsX(t - 10.0, t, paused ? ImGuiCond_Once : ImGuiCond_Always);
            if (ImPlot::BeginPlot("##Digital")) {
                for (int i = 0; i < K_PLOT_DIGITAL_CH_COUNT; ++i) {
                    if (showDigital[i] && dataDigital[i].Data.size() > 0) {
                        char label[32];
                        sprintf(label, "digital_%d", i);
                        ImPlot::PushStyleVar(ImPlotStyleVar_DigitalBitHeight, bitHeight);
                        ImPlot::PushStyleVar(ImPlotStyleVar_DigitalBitGap, bitGap);
                        ImPlot::PlotDigital(label, &dataDigital[i].Data[0].x, &dataDigital[i].Data[0].y, dataDigital[i].Data.size(), dataDigital[i].Offset, 2 * sizeof(t_float));
                        ImPlot::PopStyleVar(2);
                    }
                }
                for (int i = 0; i < K_PLOT_ANALOG_CH_COUNT; ++i) {
                    if (showAnalog[i]) {
                        char label[32];
                        sprintf(label, "analog_%d", i);
                        if (dataAnalog[i].Data.size() > 0)
                            ImPlot::PlotLine(label, &dataAnalog[i].Data[0].x, &dataAnalog[i].Data[0].y, dataAnalog[i].Data.size(), dataAnalog[i].Offset, 2 * sizeof(t_float));
                    }
                }
                ImPlot::EndPlot();
            }
            if (ImGui::BeginDragDropTarget()) {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DIGITAL_PLOT");
                if (payload) {
                    int i = *(int*)payload->Data;
                    showDigital[i] = true;
                }
                else
                {
                    payload = ImGui::AcceptDragDropPayload("DND_ANALOG_PLOT");
                    if (payload) {
                        int i = *(int*)payload->Data;
                        showAnalog[i] = true;
                    }
                }
                ImGui::EndDragDropTarget();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Offset and Stride")) {
            static const int k_circles = 11;
            static const int k_points_per = 50;
            static const int k_size = 2 * k_points_per * k_circles;
            static t_float interleaved_data[k_size];
            for (int p = 0; p < k_points_per; ++p) {
                for (int c = 0; c < k_circles; ++c) {
                    t_float r = (t_float)c / (k_circles - 1) * 0.2f + 0.2f;
                    interleaved_data[p * 2 * k_circles + 2 * c + 0] = 0.5f + r * Cos((t_float)p / k_points_per * 6.28f);
                    interleaved_data[p * 2 * k_circles + 2 * c + 1] = 0.5f + r * Sin((t_float)p / k_points_per * 6.28f);
                }
            }
            static int offset = 0;
            ImGui::BulletText("Offsetting is useful for realtime plots (see above) and circular buffers.");
            ImGui::BulletText("Striding is useful for interleaved data (e.g. audio) or plotting structs.");
            ImGui::BulletText("Here, all circle data is stored in a single interleaved buffer:");
            ImGui::BulletText("[c0.x0 c0.y0 ... cn.x0 cn.y0 c0.x1 c0.y1 ... cn.x1 cn.y1 ... cn.xm cn.ym]");
            ImGui::BulletText("The offset value indicates which circle point index is considered the first.");
            ImGui::BulletText("Offsets can be negative and/or larger than the actual data count.");
            ImGui::SliderInt("Offset", &offset, -2 * k_points_per, 2 * k_points_per);
            if (ImPlot::BeginPlot("##strideoffset")) {
                ImPlot::SetColormap(ImPlotColormap_Jet);
                char buff[16];
                for (int c = 0; c < k_circles; ++c) {
                    sprintf(buff, "Circle %d", c);
                    ImPlot::PlotLine(buff, &interleaved_data[c * 2 + 0], &interleaved_data[c * 2 + 1], k_points_per, offset, 2 * k_circles * sizeof(t_float));
                }
                ImPlot::EndPlot();
                ImPlot::SetColormap(ImPlotColormap_Default);
            }
            // offset++; uncomment for animation!
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Custom Ticks")) {
            static bool custom_ticks = true;
            static bool custom_labels = true;
            ImGui::Checkbox("Show Custom Ticks", &custom_ticks);
            if (custom_ticks) {
                ImGui::SameLine();
                ImGui::Checkbox("Show Custom Labels", &custom_labels);
            }
            double pi = 3.14;
            const char* pi_str[] = { "PI" };
            static double yticks[] = { 1,3,7,9 };
            static const char* ylabels[] = { "One","Three","Seven","Nine" };
            static double yticks_aux[] = { 0.2,0.4,0.6 };
            static const char* ylabels_aux[] = { "A","B","C","D","E","F" };
            if (custom_ticks) {
                ImPlot::SetNextPlotTicksX(&pi, 1, custom_labels ? pi_str : NULL, true);
                ImPlot::SetNextPlotTicksY(yticks, 4, custom_labels ? ylabels : NULL);
                ImPlot::SetNextPlotTicksY(yticks_aux, 3, custom_labels ? ylabels_aux : NULL, false, 1);
                ImPlot::SetNextPlotTicksY(0, 1, 6, custom_labels ? ylabels_aux : NULL, false, 2);
            }
            ImPlot::SetNextPlotLimits(2.5, 5, 0, 10);
            if (ImPlot::BeginPlot("Custom Ticks", NULL, NULL, ImVec2(-1, 0), ImPlotFlags_Default | ImPlotFlags_YAxis2 | ImPlotFlags_YAxis3)) {

                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Custom Styles")) {
            static ImVec4 my_map[3] = {
                ImVec4(0.000f, 0.980f, 0.604f, 1.0f),
                ImVec4(0.996f, 0.278f, 0.380f, 1.0f),
                ImVec4(0.1176470593f, 0.5647059083f, 1.0f, 1.0f),
            };
            ImPlot::SetColormap(my_map, 3);
            ImPlot::PushStyleColor(ImPlotCol_FrameBg, IM_COL32(32, 51, 77, 255));
            ImPlot::PushStyleColor(ImPlotCol_PlotBg, ImVec4(0, 0, 0, 0));
            ImPlot::PushStyleColor(ImPlotCol_PlotBorder, ImVec4(0, 0, 0, 0));
            ImPlot::PushStyleColor(ImPlotCol_XAxis, IM_COL32(192, 192, 192, 192));
            ImPlot::PushStyleColor(ImPlotCol_YAxis, IM_COL32(192, 192, 192, 192));
            ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 2);
            ImPlot::SetNextPlotLimits(-0.5f, 9.5f, -0.5f, 9.5f);
            if (ImPlot::BeginPlot("##Custom", NULL, NULL, ImVec2(-1, 0), ImPlotFlags_Default & ~ImPlotFlags_Legend, 0)) {
                t_float lin[10] = { 8,8,9,7,8,8,8,9,7,8 };
                t_float bar[10] = { 1,2,5,3,4,1,2,5,3,4 };
                t_float dot[10] = { 7,6,6,7,8,5,6,5,8,7 };
                ImPlot::PlotBars("Bar", bar, 10, 0.5f);
                ImPlot::PlotLine("Line", lin, 10);
                ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 0);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Square);
                ImPlot::PlotLine("Dot", dot, 10);
                ImPlot::PopStyleVar(2);
                ImPlot::EndPlot();
            }
            ImPlot::PopStyleColor(5);
            ImPlot::PopStyleVar();
            ImPlot::SetColormap(ImPlotColormap_Default);
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Custom Rendering")) {
            if (ImPlot::BeginPlot("##CustomRend")) {
                ImVec2 cntr = ImPlot::PlotToPixels(ImPlotPoint(0.5f, 0.5f));
                ImVec2 rmin = ImPlot::PlotToPixels(ImPlotPoint(0.25f, 0.75f));
                ImVec2 rmax = ImPlot::PlotToPixels(ImPlotPoint(0.75f, 0.25f));
                ImPlot::PushPlotClipRect();
                ImGui::GetWindowDrawList()->AddCircleFilled(cntr, 20, IM_COL32(255, 255, 0, 255), 20);
                ImGui::GetWindowDrawList()->AddRect(rmin, rmax, IM_COL32(128, 0, 255, 255));
                ImPlot::PopPlotClipRect();
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Benchmark")) {
            static const int n_items = 100;
            static BenchmarkItem items[n_items];
            ImGui::BulletText("Make sure VSync is disabled.");
            ImGui::BulletText("%d lines with %d points each @ %.3f FPS.", n_items, 1000, ImGui::GetIO().Framerate);
            ImPlot::SetNextPlotLimits(0, 1, 0, 1, ImGuiCond_Always);
            if (ImPlot::BeginPlot("##Bench", NULL, NULL, ImVec2(-1, 0), ImPlotFlags_Default | ImPlotFlags_NoChild)) {
                char buff[16];
                for (int i = 0; i < 100; ++i) {
                    sprintf(buff, "item_%d", i);
                    ImPlot::PushStyleColor(ImPlotCol_Line, items[i].Col);
                    ImPlot::PlotLine(buff, items[i].Data, 1000);
                    ImPlot::PopStyleColor();
                }
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        ImGui::End();
    }

} // namespace ImPlot