#include "mvMetricsWindow.h"
#include "mvTypeBases.h"
#include "mvAppItem.h"
#include "mvInput.h"
#include "mvModule_Core.h"
#include "mvApp.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvFontScope.h"

namespace Marvel {

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

        parser.addArg<mvPyDataType::Integer>("x_pos", mvArgType::KEYWORD, "200");
        parser.addArg<mvPyDataType::Integer>("y_pos", mvArgType::KEYWORD, "200");

        parser.addArg<mvPyDataType::Bool>("autosize", mvArgType::KEYWORD, "False", "Autosized the window to fit it's items.");
        parser.addArg<mvPyDataType::Bool>("no_resize", mvArgType::KEYWORD, "False", "Allows for the window size to be changed or fixed");
        parser.addArg<mvPyDataType::Bool>("no_title_bar", mvArgType::KEYWORD, "False", "Title name for the title bar of the window");
        parser.addArg<mvPyDataType::Bool>("no_move", mvArgType::KEYWORD, "False", "Allows for the window's position to be changed or fixed");
        parser.addArg<mvPyDataType::Bool>("no_scrollbar", mvArgType::KEYWORD, "False", " Disable scrollbars (window can still scroll with mouse or programmatically)");
        parser.addArg<mvPyDataType::Bool>("no_collapse", mvArgType::KEYWORD, "False", "Disable user collapsing window by double-clicking on it");
        parser.addArg<mvPyDataType::Bool>("horizontal_scrollbar", mvArgType::KEYWORD, "False", "Allow horizontal scrollbar to appear (off by default).");
        parser.addArg<mvPyDataType::Bool>("no_focus_on_appearing", mvArgType::KEYWORD, "False", "Disable taking focus when transitioning from hidden to visible state");
        parser.addArg<mvPyDataType::Bool>("no_bring_to_front_on_focus", mvArgType::KEYWORD, "False", "Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)");
        parser.addArg<mvPyDataType::Bool>("no_close", mvArgType::KEYWORD, "False");
        parser.addArg<mvPyDataType::Bool>("no_background", mvArgType::KEYWORD, "False");

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
