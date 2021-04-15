#include "mvMetricsWindow.h"
#include "mvTypeBases.h"
#include "mvAppItem.h"
#include "mvInput.h"
#include "mvModule_Core.h"
#include "mvApp.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvFontScope.h"
#include <implot.h>
#include "mvProfiler.h"

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

    void mvMetricsWindow::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        mvPythonParser parser(mvPyDataType::String);
        mvAppItem::AddCommonArgs(parser);
        parser.removeArg("parent");
        parser.removeArg("before");
        parser.removeArg("source");
        parser.removeArg("callback");
        parser.removeArg("callback_data");
        parser.removeArg("enabled");

        parser.addArg<mvPyDataType::Integer>("x_pos", mvArgType::KEYWORD_ARG, "200");
        parser.addArg<mvPyDataType::Integer>("y_pos", mvArgType::KEYWORD_ARG, "200");

        parser.addArg<mvPyDataType::Bool>("autosize", mvArgType::KEYWORD_ARG, "False", "Autosized the window to fit it's items.");
        parser.addArg<mvPyDataType::Bool>("no_resize", mvArgType::KEYWORD_ARG, "False", "Allows for the window size to be changed or fixed");
        parser.addArg<mvPyDataType::Bool>("no_title_bar", mvArgType::KEYWORD_ARG, "False", "Title name for the title bar of the window");
        parser.addArg<mvPyDataType::Bool>("no_move", mvArgType::KEYWORD_ARG, "False", "Allows for the window's position to be changed or fixed");
        parser.addArg<mvPyDataType::Bool>("no_scrollbar", mvArgType::KEYWORD_ARG, "False", " Disable scrollbars (window can still scroll with mouse or programmatically)");
        parser.addArg<mvPyDataType::Bool>("no_collapse", mvArgType::KEYWORD_ARG, "False", "Disable user collapsing window by double-clicking on it");
        parser.addArg<mvPyDataType::Bool>("horizontal_scrollbar", mvArgType::KEYWORD_ARG, "False", "Allow horizontal scrollbar to appear (off by default).");
        parser.addArg<mvPyDataType::Bool>("no_focus_on_appearing", mvArgType::KEYWORD_ARG, "False", "Disable taking focus when transitioning from hidden to visible state");
        parser.addArg<mvPyDataType::Bool>("no_bring_to_front_on_focus", mvArgType::KEYWORD_ARG, "False", "Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)");
        parser.addArg<mvPyDataType::Bool>("no_close", mvArgType::KEYWORD_ARG, "False");
        parser.addArg<mvPyDataType::Bool>("no_background", mvArgType::KEYWORD_ARG, "False");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvMetricsWindow::mvMetricsWindow(const std::string& name)
        : mvBaseWindowAppitem(name)
    {
    }

    void mvMetricsWindow::draw(ImDrawList* drawlist, float x, float y)
    {
        mvFontScope fscope(this);

        if (!prerender())
            return;

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

        if (ImGui::IsWindowFocused())
        {

            float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

            // update mouse
            ImVec2 mousePos = ImGui::GetMousePos();
            float x = mousePos.x - ImGui::GetWindowPos().x;
            float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
            mvInput::setMousePosition(x, y);

            if (mvApp::GetApp()->getItemRegistry().getActiveWindow() != "metrics##standard")
                mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", std::string("metrics##standard")) });

        }

        ImGui::End();
        
    }

}
