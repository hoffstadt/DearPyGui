#include "mvMetricsWindow.h"
#include "mvProfiler.h"
#include "mvApp.h"

namespace Marvel {

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

        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("Dear PyGui %s", mvApp::GetVersion());
        ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
        ImGui::Text("%d active windows (%d visible)", io.MetricsActiveWindows, io.MetricsRenderWindows);
        ImGui::Text("%d active allocations", io.MetricsActiveAllocations);

        static std::map<std::string, ScrollingBuffer> buffers;
        static float t = 0;
        t += ImGui::GetIO().DeltaTime;

        const auto& results = mvInstrumentor::Get().getResults();

        for (const auto& item : results)
            buffers[item.first].AddPoint(t, (float)item.second.count());

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

        static ImPlotAxisFlags rt_axis = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks;
        ImPlot::SetNextPlotLimitsX(t - history, t, ImGuiCond_Always);
        if (ImPlot::BeginPlot("##Scrolling1", nullptr, nullptr, ImVec2(-1, 200), 0, rt_axis, ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_LockMin))
        {
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

            ImPlot::PlotLine("Frame", &buffers["Frame"].Data[0].x, &buffers["Frame"].Data[0].y, buffers["Frame"].Data.size(), buffers["Frame"].Offset, 2 * sizeof(float));
            ImPlot::PlotLine("Presentation", &buffers["Presentation"].Data[0].x, &buffers["Presentation"].Data[0].y, buffers["Presentation"].Data.size(), buffers["Presentation"].Offset, 2 * sizeof(float));

            ImPlot::EndPlot();
        }
        ImPlot::PopStyleColor(3);

        ImPlot::PushStyleColor(ImPlotCol_PlotBorder, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImPlot::SetNextPlotLimitsX(t - history, t, ImGuiCond_Always);
        if (ImPlot::BeginPlot("##Scrolling2", nullptr, nullptr, ImVec2(-1, -1), 0, rt_axis, 0 | ImPlotAxisFlags_LockMin))
        {

            for (const auto& item : results)
            {
                if (item.first == "Frame" || item.first == "Presentation")
                    continue;
                ImPlot::PlotLine(item.first.c_str(), &buffers[item.first].Data[0].x, &buffers[item.first].Data[0].y, buffers[item.first].Data.size(), buffers[item.first].Offset, 2 * sizeof(float));
            }
            ImPlot::EndPlot();
        }
        ImPlot::PopStyleColor();

    }

}
