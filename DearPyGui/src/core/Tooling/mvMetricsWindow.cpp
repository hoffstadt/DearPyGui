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

        static ImPlotAxisFlags rt_axis = ImPlotAxisFlags_NoTickLabels;
        ImPlot::SetNextPlotLimitsX(t - history, t, ImGuiCond_Always);
        if (ImPlot::BeginPlot("##Scrolling", nullptr, nullptr, ImVec2(-1, -1), 0, rt_axis, 0 | ImPlotAxisFlags_LockMin)) {

            for (const auto& item : results)
                ImPlot::PlotLine(item.first.c_str(), &buffers[item.first].Data[0].x, &buffers[item.first].Data[0].y, buffers[item.first].Data.size(), buffers[item.first].Offset, 2 * sizeof(float));
            ImPlot::EndPlot();
        }

    }

}
