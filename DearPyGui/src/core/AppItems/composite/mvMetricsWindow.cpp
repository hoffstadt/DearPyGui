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
        parsers->insert({ "add_metrics_window", mvPythonParser({
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::Integer, "width", "", "700"},
            {mvPythonDataType::Integer, "height", "", "500"},
            {mvPythonDataType::Integer, "x_pos", "x position the window will start at", "200"},
            {mvPythonDataType::Integer, "y_pos", "y position the window will start at", "200"},
            {mvPythonDataType::Bool, "autosize", "Autosized the window to fit it's items.", "False"},
            {mvPythonDataType::Bool, "no_resize", "Allows for the window size to be changed or fixed", "False"},
            {mvPythonDataType::Bool, "no_title_bar", "Title name for the title bar of the window", "False"},
            {mvPythonDataType::Bool, "no_move", "Allows for the window's position to be changed or fixed", "False"},
            {mvPythonDataType::Bool, "no_scrollbar" ," Disable scrollbars (window can still scroll with mouse or programmatically)", "False"},
            {mvPythonDataType::Bool, "no_collapse" ,"Disable user collapsing window by double-clicking on it", "False"},
            {mvPythonDataType::Bool, "horizontal_scrollbar" ,"Allow horizontal scrollbar to appear (off by default).", "False"},
            {mvPythonDataType::Bool, "no_focus_on_appearing" ,"Disable taking focus when transitioning from hidden to visible state", "False"},
            {mvPythonDataType::Bool, "no_bring_to_front_on_focus" ,"Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)", "False"},
            {mvPythonDataType::Bool, "no_close", "", "False"},
            {mvPythonDataType::Bool, "no_background", "", "False"},
            {mvPythonDataType::String, "label", "", "''"},
            {mvPythonDataType::Bool, "show", "Attempt to render", "True"},
        }, "Creates a metrics window.",
    "None", "Containers") });
    }

    mvMetricsWindow::mvMetricsWindow(const std::string& name)
        : mvBaseWindowAppitem(name)
    {
        m_description.deleteAllowed = false;
    }

    void mvMetricsWindow::draw()
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

#ifdef MV_CPP
#else
    PyObject* add_metrics_window(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* name;
        int width = 700;
        int height = 500;
        int x_pos = 200;
        int y_pos = 200;
        int autosize = false;
        int no_resize = false;
        int no_title_bar = false;
        int no_move = false;
        int no_scrollbar = false;
        int no_collapse = false;
        int horizontal_scrollbar = false;
        int no_focus_on_appearing = false;
        int no_bring_to_front_on_focus = false;
        int noclose = false;
        int no_background = false;

        const char* label = "";
        int show = true;

        if (!(mvApp::GetApp()->getParsers())["add_metrics_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
            &height, &x_pos, &y_pos, &autosize, &no_resize, &no_title_bar, &no_move, &no_scrollbar,
            &no_collapse, &horizontal_scrollbar, &no_focus_on_appearing, &no_bring_to_front_on_focus,
            &noclose, &no_background, &label, &show))
            return ToPyBool(false);

        auto item = CreateRef<mvMetricsWindow>(name);

        item->checkConfigDict(kwargs);
        item->setConfigDict(kwargs);
        item->setExtraConfigDict(kwargs);

        if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, "", ""))
        {
            mvApp::GetApp()->getItemRegistry().pushParent(item);
            if (!show)
                item->hide();

        }

        return GetPyNone();
    }
#endif

}
