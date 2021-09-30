#include "mvDebugWindow.h"
#include "mvContext.h"
#include <misc/cpp/imgui_stdlib.h>
#include "mvItemRegistry.h"
#include "mvModule_DearPyGui.h"

namespace Marvel {

    mv_internal void
    DebugItem(const char* label, const char* item)
    {
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", item);
    }

    mv_internal void
    DebugItem(const char* label, float x)
    {
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", std::to_string(x).c_str());
    }

    mv_internal void
    DebugItem(const char* label, float x, float y)
    {
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", (std::to_string(x) + ", " + std::to_string(y)).c_str());
    }

    mvDebugWindow::mvDebugWindow()
    {
        m_windowflags = ImGuiWindowFlags_NoSavedSettings;

        m_width = 700;
        m_height = 500;

        for (const auto& item : GetModuleParsers())
            m_commands.emplace_back(item.first, item.second.documentation);
    }

    void mvDebugWindow::drawWidgets()
    {

        //static char ts[6] = "True";
        //static char fs[6] = "False";
        mv_local_persist std::string commandstring;

        ImGuiIO& io = ImGui::GetIO();

        //static auto app = GContext;

        if (ImGui::BeginTabBar("Main Tabbar"))
        {
            if (ImGui::BeginTabItem("App State"))
            {

                ImGui::PushItemWidth(200);
                ImGui::BeginGroup();
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
                ImGui::Text("%d active allocations", io.MetricsActiveAllocations);
                DebugItem("DearPyGui Version: ", MV_SANDBOX_VERSION);
                DebugItem("ImGui Version: ", IMGUI_VERSION);


                ImGui::EndGroup();
                ImGui::PopItemWidth();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Input Polling"))
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

                ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
                ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
                ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
                ImGui::Text("Mouse dblclick:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
                ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
                ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

                ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("%d (0x%X) (%.02f secs)", i, i, io.KeysDownDuration[i]); }
                ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
                ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
                ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
                ImGui::Text("Chars queue:");    for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); }

                ImGui::EndGroup();
                ImGui::PopItemWidth();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Commands##debug"))
            {

                mv_local_persist size_t commandselection = 0;
                const char* commanddoc = m_commands[commandselection].second.c_str();
                mv_local_persist ImGuiTextFilter filter;
                filter.Draw();

                ImGui::PushItemWidth(-1);
                ImGui::BeginChild("CommandsChild##debug", ImVec2(400.0f, 400.0f), true);
                ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 0.0f, 1.0f });
                for (size_t i = 0; i < m_commands.size(); i++)
                {
                    auto& item = m_commands[i];

                    if (filter.PassFilter(item.first.c_str()))
                    {
                        if (ImGui::Selectable(item.first.c_str(), i == commandselection))
                            commandselection = i;
                    }
                }
                ImGui::PopStyleColor();
                ImGui::EndChild();
                ImGui::SameLine();
                ImGui::BeginChild("CommandsDoc##debug", ImVec2(-1.0f, 400.0f), true);
                ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 1.0f, 1.0f });
                ImGui::PushTextWrapPos(500);
                ImGui::Text("%s", commanddoc);
                ImGui::PopStyleColor();
                ImGui::PopTextWrapPos();
                ImGui::EndChild();

                ImGui::InputTextMultiline("Command##debug", &commandstring, ImVec2(-1, -50));
                ImGui::PopItemWidth();
                if (ImGui::Button("Run##debug"))
                {
                    std::string command = "from dearpygui.dearpygui import *\nfrom dearpygui.demo import *\n" + commandstring;
                    GContext->callbackRegistry->submitCallback([=]() {
                        PyRun_SimpleString(command.c_str());
                        });

                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

    }

}
