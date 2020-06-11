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


#include "implot.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath> // for 'float' overloads of elementary functions (sin,cos,etc)

#ifdef _MSC_VER
#define sprintf sprintf_s
#endif

namespace {

    float RandomRange(float min, float max) {
        float scale = rand() / (float)RAND_MAX;
        return min + scale * (max - min);
    }

    struct ScrollingData {
        int MaxSize;
        int Offset;
        ImVector<ImVec2> Data;
        ScrollingData() {
            MaxSize = 1000;
            Offset = 0;
            Data.reserve(MaxSize);
        }
        void AddPoint(float x, float y) {
            if (Data.size() < MaxSize)
                Data.push_back(ImVec2(x, y));
            else {
                Data[Offset] = ImVec2(x, y);
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

    struct RollingData {
        float Span;
        ImVector<ImVec2> Data;
        RollingData() {
            Span = 10.0f;
            Data.reserve(1000);
        }
        void AddPoint(float x, float y) {
            float xmod = fmodf(x, Span);
            if (!Data.empty() && xmod < Data.back().x)
                Data.shrink(0);
            Data.push_back(ImVec2(xmod, y));
        }
    };

    struct BenchmarkItem {
        BenchmarkItem() {
            float y = RandomRange(0, 1);
            Data = new ImVec2[1000];
            for (int i = 0; i < 1000; ++i) {
                Data[i].x = i * 0.001f;
                Data[i].y = y + RandomRange(-0.01f, 0.01f);
            }
            Col = ImVec4(RandomRange(0, 1), RandomRange(0, 1), RandomRange(0, 1), 1);
        }
        ~BenchmarkItem() { delete Data; }
        ImVec2* Data;
        ImVec4 Col;
    };

} // private namespace

namespace ImPlot {

    void ShowDemoWindow(bool* p_open) {
        static bool show_app_metrics = false;
        static bool show_app_style_editor = false;
        if (show_app_metrics) { ImGui::ShowMetricsWindow(&show_app_metrics); }
        if (show_app_style_editor) { ImGui::Begin("Style Editor", &show_app_style_editor); ImGui::ShowStyleEditor(); ImGui::End(); }
        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(520, 750), ImGuiCond_FirstUseEver);
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
        ImGui::Text("ImPlot says hello. (0.2 WIP)");
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
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Line Plots")) {
            static float xs1[1001], ys1[1001];
            for (int i = 0; i < 1001; ++i) {
                xs1[i] = i * 0.001f;
                ys1[i] = 0.5f + 0.5f * sin(50 * xs1[i]);
            }
            static float xs2[11], ys2[11];
            for (int i = 0; i < 11; ++i) {
                xs2[i] = i * 0.1f;
                ys2[i] = xs2[i] * xs2[i];
            }
            if (ImPlot::BeginPlot("Line Plot", "x", "f(x)")) {
                ImPlot::PlotLine("sin(50*x)", xs1, ys1, 1001);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Circle);
                ImPlot::PlotLine("x^2", xs2, ys2, 11);
                ImPlot::PopStyleVar();
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Scatter Plots")) {
            srand(0);
            static float xs1[100], ys1[100];
            for (int i = 0; i < 100; ++i) {
                xs1[i] = i * 0.01f;
                ys1[i] = xs1[i] + 0.1f * ((float)rand() / (float)RAND_MAX);
            }
            static float xs2[50], ys2[50];
            for (int i = 0; i < 50; i++) {
                xs2[i] = 0.25f + 0.2f * ((float)rand() / (float)RAND_MAX);
                ys2[i] = 0.75f + 0.2f * ((float)rand() / (float)RAND_MAX);
            }
            if (ImPlot::BeginPlot("Scatter Plot", NULL, NULL)) {
                ImPlot::PlotScatter("Data 1", xs1, ys1, 100);

                ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 6);
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Square);
                ImPlot::PushStyleColor(ImPlotCol_MarkerFill, ImVec4(1, 0, 0, 0.25f));
                ImPlot::PushStyleColor(ImPlotCol_MarkerOutline, ImVec4(0, 0, 0, 0));
                ImPlot::PlotScatter("Data 2", xs2, ys2, 50);
                ImPlot::PopStyleColor(2);
                ImPlot::PopStyleVar(2);

                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Bar Plots")) {
            static bool horz = false;
            ImGui::Checkbox("Horizontal", &horz);
            if (horz)
                ImPlot::SetNextPlotLimits(0, 110, -0.5f, 9.5f, ImGuiCond_Always);
            else
                ImPlot::SetNextPlotLimits(-0.5f, 9.5f, 0, 110, ImGuiCond_Always);
            if (ImPlot::BeginPlot("Bar Plot", horz ? "Score" : "Student", horz ? "Student" : "Score")) {
                static float midtm[10] = { 83, 67, 23, 89, 83, 78, 91, 82, 85, 90 };
                static float final[10] = { 80, 62, 56, 99, 55, 78, 88, 78, 90, 100 };
                static float grade[10] = { 80, 69, 52, 92, 72, 78, 75, 76, 89, 95 };
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
            float xs[5] = { 1,2,3,4,5 };
            float lin[5] = { 8,8,9,7,8 };
            float bar[5] = { 1,2,5,3,4 };
            float err1[5] = { 0.2f, 0.4f, 0.2f, 0.6f, 0.4f };
            float err2[5] = { 0.4f, 0.2f, 0.4f, 0.8f, 0.6f };
            ImPlot::SetNextPlotLimits(0, 6, 0, 10);
            if (ImPlot::BeginPlot("##ErrorBars", NULL, NULL)) {

                ImPlot::PlotBars("Bar", xs, bar, 5, 0.5f);
                ImPlot::PlotErrorBars("Bar", xs, bar, err1, 5);

                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Circle);
                ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 3);
                ImPlot::PushStyleColor(ImPlotCol_ErrorBar, ImVec4(1, 0, 0, 1));
                ImPlot::PlotErrorBars("Line", xs, lin, err1, err2, 5);
                ImPlot::PlotLine("Line", xs, lin, 5);
                ImPlot::PopStyleVar(2);
                ImPlot::PopStyleColor();

                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Pie Charts")) {
            static const char* labels1[] = { "Frogs","Hogs","Dogs","Logs" };
            static float pre_normalized[] = { 0.15f,  0.30f,  0.45f, 0.10f };
            ImVec2 center(0.5f, 0.5f); // in plot units, not pixels
            float radius = 0.4f;      // in plot units, not pixels

            SetNextPlotLimits(0, 1, 0, 1, ImGuiCond_Always);
            if (ImPlot::BeginPlot("##Pie1", NULL, NULL, ImVec2(250, 250), ImPlotFlags_Legend, 0, 0)) {
                ImPlot::PlotPieChart(labels1, pre_normalized, 4, center, radius);
                ImPlot::EndPlot();
            }
            ImGui::SameLine();

            static ImVec4 YlOrRd[5] = {
                ImVec4(1.0000f,    1.0000f,    0.8000f, 1.0f),
                ImVec4(0.9961f,    0.8510f,    0.4627f, 1.0f),
                ImVec4(0.9961f,    0.6314f,    0.2627f, 1.0f),
                ImVec4(0.9882f,    0.3059f,    0.1647f, 1.0f),
                ImVec4(0.7412f,       0.0f,    0.1490f, 1.0f),
            };
            ImPlot::SetPalette(YlOrRd, 5);
            SetNextPlotLimits(0, 1, 0, 1, ImGuiCond_Always);
            static const char* labels2[] = { "One","Two","Three","Four","Five" };
            static float not_normalized[] = { 1,2,3,4,5 };
            if (ImPlot::BeginPlot("##Pie2", NULL, NULL, ImVec2(250, 250), ImPlotFlags_Legend, 0, 0)) {
                ImPlot::PlotPieChart(labels2, not_normalized, 5, center, radius);
                ImPlot::EndPlot();
            }
            ImPlot::RestorePalette();
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Realtime Plots")) {
            ImGui::BulletText("Move your mouse to change the data!");
            ImGui::BulletText("This example assumes 60 FPS. Higher FPS requires larger buffer size.");
            static bool paused = false;
            static ScrollingData sdata1, sdata2;
            static RollingData   rdata1, rdata2;
            ImVec2 mouse = ImGui::GetMousePos();
            static float t = 0;
            if (!paused) {
                t += ImGui::GetIO().DeltaTime;
                sdata1.AddPoint(t, mouse.x * 0.0005f);
                rdata1.AddPoint(t, mouse.x * 0.0005f);
                sdata2.AddPoint(t, mouse.y * 0.0005f);
                rdata2.AddPoint(t, mouse.y * 0.0005f);
            }
            ImPlot::SetNextPlotLimitsX(t - 10, t, paused ? ImGuiCond_Once : ImGuiCond_Always);
            static int rt_axis = ImPlotAxisFlags_Default & ~ImPlotAxisFlags_TickLabels;
            if (ImPlot::BeginPlot("##Scrolling", NULL, NULL, ImVec2(-1, 150), ImPlotFlags_Default, rt_axis, rt_axis)) {
                ImPlot::PlotLine("Data 1", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), sdata1.Offset, 2 * sizeof(float));
                ImPlot::PlotLine("Data 2", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), sdata2.Offset, 2 * sizeof(float));
                ImPlot::EndPlot();
            }
            ImPlot::SetNextPlotLimitsX(0, 10, ImGuiCond_Always);
            if (ImPlot::BeginPlot("##Rolling", NULL, NULL, ImVec2(-1, 150), ImPlotFlags_Default, rt_axis, rt_axis)) {
                ImPlot::PlotLine("Data 1", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 2 * sizeof(float));
                ImPlot::PlotLine("Data 2", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 2 * sizeof(float));
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Markers and Text")) {
            ImPlot::SetNextPlotLimits(0, 10, 0, 12);
            if (ImPlot::BeginPlot("##MarkerStyles", NULL, NULL, ImVec2(-1, 0), 0, 0, 0)) {
                float xs[2] = { 1,4 };
                float ys[2] = { 10,11 };
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
                ImPlot::PopStyleVar(4);
                ImPlot::PopStyleColor(3);

                ImPlot::PlotText("Filled Markers", 1.5, 11.75);
                ImPlot::PlotText("Open Markers", 6.75, 11.75);
                ImPlot::PlotText("Fancy Markers", 4.5, 4.25, true);

                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Log Scale")) {
            ImGui::BulletText("Open the plot context menu (double right click) to change scales.");
            static float xs[1001], ys1[1001], ys2[1001], ys3[1001];
            for (int i = 0; i < 1001; ++i) {
                xs[i] = (float)(i * 0.1f);
                ys1[i] = sin(xs[i]) + 1;
                ys2[i] = log(xs[i]);
                ys3[i] = pow(10.0f, xs[i]);
            }
            ImPlot::SetNextPlotLimits(0.1f, 100, 0, 10);
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

            static float xs[1001], xs2[1001], ys1[1001], ys2[1001], ys3[1001];
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
                xs[i] = (float)(i * 0.1f);
                ys1[i] = sin(xs[i]) * 3 + 1;
                ys2[i] = cos(xs[i]) * 0.2f + 0.5f;
                ys3[i] = sin(xs[i] + 0.5f) * 100 + 200;
                xs2[i] = xs[i] + 10.0f;
            }
            ImPlot::SetNextPlotLimits(0.1f, 100, 0, 10);
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
            static ImVector<ImVec2> data;
            ImPlotLimits range, query;
            if (ImPlot::BeginPlot("##Drawing", NULL, NULL, ImVec2(-1, 0), ImPlotFlags_Default | ImPlotFlags_Query, ImPlotAxisFlags_GridLines, ImPlotAxisFlags_GridLines)) {
                if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyCtrl)
                    data.push_back(ImPlot::GetPlotMousePos());
                ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Diamond);
                if (data.size() > 0)
                    ImPlot::PlotScatter("Points", &data[0].x, &data[0].y, data.size(), 0, 2 * sizeof(float));
                if (ImPlot::IsPlotQueried() && data.size() > 0) {
                    ImPlotLimits range2 = ImPlot::GetPlotQuery();
                    int cnt = 0;
                    ImVec2 avg;
                    for (int i = 0; i < data.size(); ++i) {
                        if (range2.Contains(data[i])) {
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
            static float x_data[512];
            static float y_data1[512];
            static float y_data2[512];
            static float y_data3[512];
            static float sampling_freq = 44100;
            static float freq = 500;
            for (size_t i = 0; i < 512; ++i) {
                const float t = i / sampling_freq;
                x_data[i] = t;
                const float arg = 2 * 3.14f * freq * t;
                y_data1[i] = sin(arg);
                y_data2[i] = y_data1[i] * -0.6f + sin(2 * arg) * 0.4f;
                y_data3[i] = y_data2[i] * -0.6f + sin(3 * arg) * 0.4f;
            }
            ImGui::BulletText("Query the first plot to render a subview in the second plot (see above for controls).");
            ImPlot::SetNextPlotLimits(0, 0.01f, -1, 1);
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
            srand((int)(10000000 * ImGui::GetTime()));
            static bool paused = false;
            static bool init = true;
            static ScrollingData data[10];
            static bool show[10];
            if (init) {
                for (int i = 0; i < 10; ++i) {
                    show[i] = false;
                }
                init = false;
            }
            ImGui::BulletText("Drag data items from the left column onto the plot.");
            ImGui::BeginGroup();
            if (ImGui::Button("Clear", ImVec2(100, 0))) {
                for (int i = 0; i < 10; ++i) {
                    show[i] = false;
                    data[i].Data.shrink(0);
                    data[i].Offset = 0;
                }
            }
            if (ImGui::Button(paused ? "Resume" : "Pause", ImVec2(100, 0)))
                paused = !paused;
            ImGui::Separator();
            for (int i = 0; i < 10; ++i) {
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
            static float t = 0;
            if (!paused) {
                t += ImGui::GetIO().DeltaTime;
                for (int i = 0; i < 10; ++i) {
                    if (show[i])
                        data[i].AddPoint(t, data[i].Data.empty() ?
                            0.25f + 0.5f * (float)rand() / float(RAND_MAX) :
                            data[i].Data.back().y + (0.005f + 0.0002f * (float)rand() / float(RAND_MAX)) * (-1 + 2 * (float)rand() / float(RAND_MAX)));
                }
            }
            ImPlot::SetNextPlotLimitsX(t - 10, t, paused ? ImGuiCond_Once : ImGuiCond_Always);
            if (ImPlot::BeginPlot("##DND")) {
                for (int i = 0; i < 10; ++i) {
                    if (show[i] && data[i].Data.size() > 0) {
                        char label[8];
                        sprintf(label, "data_%d", i);
                        ImPlot::PlotLine(label, &data[i].Data[0].x, &data[i].Data[0].y, data[i].Data.size(), data[i].Offset, 2 * sizeof(float));
                    }
                }
                ImPlot::EndPlot();
            }
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PLOT")) {
                    int i = *(int*)payload->Data;
                    show[i] = true;
                }
                ImGui::EndDragDropTarget();
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
            static float t = 0;
            if (!paused) {
                t += ImGui::GetIO().DeltaTime;
                //digital signal values
                int i = 0;
                if (showDigital[i])
                    dataDigital[i].AddPoint(t, sin(2 * t) > 0.45);
                i++;
                if (showDigital[i])
                    dataDigital[i].AddPoint(t, sin(2 * t) < 0.45);
                i++;
                if (showDigital[i])
                    dataDigital[i].AddPoint(t, fmod(t, 5.0f));
                i++;
                if (showDigital[i])
                    dataDigital[i].AddPoint(t, sin(2 * t) < 0.17);
                //Analog signal values
                i = 0;
                if (showAnalog[i])
                    dataAnalog[i].AddPoint(t, sin(2 * t));
                i++;
                if (showAnalog[i])
                    dataAnalog[i].AddPoint(t, cos(2 * t));
                i++;
                if (showAnalog[i])
                    dataAnalog[i].AddPoint(t, sin(2 * t) * cos(2 * t));
                i++;
                if (showAnalog[i])
                    dataAnalog[i].AddPoint(t, sin(2 * t) - cos(2 * t));
            }
            ImPlot::SetNextPlotLimitsY(-1, 1);
            ImPlot::SetNextPlotLimitsX(t - 10.0f, t, paused ? ImGuiCond_Once : ImGuiCond_Always);
            if (ImPlot::BeginPlot("##Digital")) {
                for (int i = 0; i < K_PLOT_DIGITAL_CH_COUNT; ++i) {
                    if (showDigital[i] && dataDigital[i].Data.size() > 0) {
                        char label[32];
                        sprintf(label, "digital_%d", i);
                        ImPlot::PushStyleVar(ImPlotStyleVar_DigitalBitHeight, bitHeight);
                        ImPlot::PushStyleVar(ImPlotStyleVar_DigitalBitGap, bitGap);
                        ImPlot::PlotDigital(label, &dataDigital[i].Data[0].x, &dataDigital[i].Data[0].y, dataDigital[i].Data.size(), dataDigital[i].Offset, 2 * sizeof(float));
                        ImPlot::PopStyleVar(2);
                    }
                }
                for (int i = 0; i < K_PLOT_ANALOG_CH_COUNT; ++i) {
                    if (showAnalog[i]) {
                        char label[32];
                        sprintf(label, "analog_%d", i);
                        if (dataAnalog[i].Data.size() > 0)
                            ImPlot::PlotLine(label, &dataAnalog[i].Data[0].x, &dataAnalog[i].Data[0].y, dataAnalog[i].Data.size(), dataAnalog[i].Offset, 2 * sizeof(float));
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
        if (ImGui::CollapsingHeader("Offset Data")) {
            float xs[50], ys[50];
            for (int i = 0; i < 50; ++i) {
                xs[i] = 0.5 + 0.4 * cos(i / 50.f * 6.28);
                ys[i] = 0.5 + 0.4 * sin(i / 50.f * 6.28);
            }
            static int offset = 0;
            ImGui::SliderInt("Offset", &offset, -100, 100);
            if (ImPlot::BeginPlot("##offset")) {
                ImPlot::PlotLine("circle", xs, ys, 50, offset);
                ImPlot::EndPlot();
            }
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Custom Styles")) {
            static ImVec4 my_palette[3] = {
                ImVec4(0.000f, 0.980f, 0.604f, 1.0f),
                ImVec4(0.996f, 0.278f, 0.380f, 1.0f),
                ImVec4(0.1176470593f, 0.5647059083f, 1.0f, 1.0f),
            };
            ImPlot::SetPalette(my_palette, 3);
            ImPlot::PushStyleColor(ImPlotCol_FrameBg, IM_COL32(32, 51, 77, 255));
            ImPlot::PushStyleColor(ImPlotCol_PlotBg, ImVec4(0, 0, 0, 0));
            ImPlot::PushStyleColor(ImPlotCol_PlotBorder, ImVec4(0, 0, 0, 0));
            ImPlot::PushStyleColor(ImPlotCol_XAxis, IM_COL32(192, 192, 192, 192));
            ImPlot::PushStyleColor(ImPlotCol_YAxis, IM_COL32(192, 192, 192, 192));
            ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 2);
            ImPlot::SetNextPlotLimits(-0.5f, 9.5f, -0.5f, 9.5f);
            if (ImPlot::BeginPlot("##Custom", NULL, NULL, ImVec2(-1, 0), ImPlotFlags_Default & ~ImPlotFlags_Legend, 0)) {
                float lin[10] = { 8,8,9,7,8,8,8,9,7,8 };
                float bar[10] = { 1,2,5,3,4,1,2,5,3,4 };
                float dot[10] = { 7,6,6,7,8,5,6,5,8,7 };
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
            ImPlot::RestorePalette();
        }
        //-------------------------------------------------------------------------
        if (ImGui::CollapsingHeader("Custom Rendering")) {
            if (ImPlot::BeginPlot("##CustomRend")) {
                ImVec2 cntr = ImPlot::PlotToPixels(ImVec2(0.5f, 0.5f));
                ImVec2 rmin = ImPlot::PlotToPixels(ImVec2(0.25f, 0.75f));
                ImVec2 rmax = ImPlot::PlotToPixels(ImVec2(0.75f, 0.25f));
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
            SetNextPlotLimits(0, 1, 0, 1, ImGuiCond_Always);
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