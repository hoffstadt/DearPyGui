#include "mvMetricsWindow.h"
#include "mvProfiler.h"
#include "mvContext.h"
#include "imgui_internal.h"

static void
DebugItem(const char* label, const char* item)
{
ImGui::Text("%s", label);
ImGui::SameLine();
ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", item);
}

static void
DebugItem(const char* label, float x)
{
ImGui::Text("%s", label);
ImGui::SameLine();
ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", std::to_string(x).c_str());
}

static void
DebugItem(const char* label, float x, float y)
{
ImGui::Text("%s", label);
ImGui::SameLine();
ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", (std::to_string(x) + ", " + std::to_string(y)).c_str());
}

// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer() {
        MaxSize = 2000;
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
        if (!Data.empty())
        {
            Data.shrink(0);
            Offset = 0;
        }
    }
};


mvMetricsWindow::mvMetricsWindow()
{
    m_windowflags = ImGuiWindowFlags_NoSavedSettings;
}

void mvMetricsWindow::drawWidgets()
{

    if (ImGui::BeginTabBar("Main Tabbar"))
    {
        ImGuiIO& io = ImGui::GetIO();

        if (ImGui::BeginTabItem("General##metricswindow"))
        {
                
            DebugItem("DearPyGui Version: ", MV_SANDBOX_VERSION);
            DebugItem("ImGui Version: ", IMGUI_VERSION);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
            ImGui::Text("%d active windows (%d visible)", io.MetricsActiveWindows, io.MetricsRenderWindows);

            static std::map<std::string, ScrollingBuffer> buffers;
            static float t = 0;
            t += ImGui::GetIO().DeltaTime;

            const auto& results = mvInstrumentor::Get().getResults();

            for (const auto& item : results) {
                buffers[item.first].AddPoint(t, (float)item.second.count());
            }

            static float history = 10.0f;
            ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

            float max_value = 0.0f;

            for (int i = 0; i < buffers["Frame"].Data.Size; i++)
            {
                if (buffers["Frame"].Data[i].y > max_value)
                    max_value = buffers["Frame"].Data[i].y;
            }

            if (ImGui::GetIO().Framerate < 29)
                ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
            else if (ImGui::GetIO().Framerate < 59)
                ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(1.0f, 1.0f, 0.0f, 0.3f));
            else
                ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

            ImPlot::PushStyleColor(ImPlotCol_PlotBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImPlot::PushStyleColor(ImPlotCol_PlotBorder, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

            ImPlot::SetNextAxisToFit(ImAxis_X1);
            static ImPlotAxisFlags rt_axis = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_LockMin;
            if (ImPlot::BeginPlot("##Scrolling1", ImVec2(-1, 200), ImPlotFlags_None))
            {
                ImPlot::SetupAxis(ImAxis_X1, (const char *)nullptr, rt_axis);
                ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
                static float fps_h[2] = { 0.0f, 0.0f };
                static float fps_x[2] = { 0.0f, 10.0f };
                fps_x[0] = t - history;
                fps_x[1] = t;
                static float fps_60[2] = { 16000.0f, 16000.0f };
                static float fps_30[2] = { 32000.0f, 32000.0f };

                ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(0.0f, 1.0f, 0.0f, 0.1f));
                ImPlot::PlotShaded("60+ FPS", fps_x, fps_h, fps_60, 2);
                ImPlot::PopStyleColor();

                ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(1.0f, 1.0f, 0.0f, 0.1f));
                ImPlot::PlotShaded("30+ FPS", fps_x, fps_60, fps_30, 2);
                ImPlot::PopStyleColor();

                ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(1.0f, 0.0f, 0.0f, 0.1f));
                ImPlot::PlotShaded("Low FPS", fps_x, fps_30, 2, INFINITY);
                ImPlot::PopStyleColor();

                if (!buffers["Frame"].Data.empty())
                    ImPlot::PlotLine("Frame", &buffers["Frame"].Data[0].x, &buffers["Frame"].Data[0].y, buffers["Frame"].Data.size(), ImPlotLineFlags_None, buffers["Frame"].Offset, 2 * sizeof(float));
                if (!buffers["Presentation"].Data.empty())
                    ImPlot::PlotLine("Presentation", &buffers["Presentation"].Data[0].x, &buffers["Presentation"].Data[0].y, buffers["Presentation"].Data.size(), ImPlotLineFlags_None, buffers["Presentation"].Offset, 2 * sizeof(float));
                ImPlot::EndPlot();
            }
            ImPlot::PopStyleColor(3);

            ImPlot::PushStyleColor(ImPlotCol_PlotBorder, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            
            ImPlot::SetNextAxisToFit(ImAxis_X1);
            ImPlot::SetNextAxisToFit(ImAxis_Y1); // Is it okay?
            if (ImPlot::BeginPlot("##Scrolling2", ImVec2(-1, -1), ImPlotFlags_None))
            {
                ImPlot::SetupAxis(ImAxis_X1, (const char *)nullptr, rt_axis);
                ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
                for (const auto& item : results)
                {
                    if (item.first == "Frame" || item.first == "Presentation")
                        continue;
                    ImPlot::PlotLine(item.first.c_str(), &buffers[item.first].Data[0].x, &buffers[item.first].Data[0].y, buffers[item.first].Data.size(), ImPlotLineFlags_None, buffers[item.first].Offset, 2 * sizeof(float));
                }
                ImPlot::EndPlot();
            }
            ImPlot::PopStyleColor();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Input##metricswindow"))
        {

            ImGui::PushItemWidth(200);
            ImGui::BeginGroup();

            //DebugItem("Active Window: ", GContext->itemRegistry->getActiveWindow().c_str());
            DebugItem("Local Mouse Position:", (float)GContext->input.mousePos.x, (float)GContext->input.mousePos.y);
            DebugItem("Global Mouse Position:", (float)io.MousePos.x, (float)io.MousePos.y);
            DebugItem("Plot Mouse Position:", (float)GContext->input.mousePlotPos.x, (float)GContext->input.mousePlotPos.y);
            DebugItem("Mouse Drag Delta:", (float)GContext->input.mouseDragDelta.x, (float)GContext->input.mouseDragDelta.y);
            DebugItem("Mouse Drag Threshold:", (float)GContext->input.mouseDragThreshold);

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Text("ImGui State Inputs");

            //TODO: Finish here with migration!
            ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
            ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
            ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse dblclick:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse clicks count:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::GetMouseClickedCount(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

            ImGui::Text("Keys down:");      for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++) if (ImGui::GetKeyData(static_cast<ImGuiKey>(i))->DownDuration >= 0.0f) { ImGui::SameLine(); ImGui::Text("%d (0x%X) (%.02f secs)", i, i, ImGui::GetKeyData(static_cast<ImGuiKey>(i))->DownDuration); }
            ImGui::Text("Keys pressed:");   for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++) if (ImGui::IsKeyPressed(static_cast<ImGuiKey>(i))) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
            ImGui::Text("Keys release:");   for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++) if (ImGui::IsKeyReleased(static_cast<ImGuiKey>(i))) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
            ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
            ImGui::Text("Chars queue:");    for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); }

            ImGui::EndGroup();
            ImGui::PopItemWidth();

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}