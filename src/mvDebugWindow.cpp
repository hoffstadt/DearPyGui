#include "mvDebugWindow.h"
#include "mvContext.h"
#include <misc/cpp/imgui_stdlib.h>
#include "mvItemRegistry.h"
#include "dearpygui.h"

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

    static std::string commandstring;

    ImGuiIO& io = ImGui::GetIO();

        static size_t commandselection = 0;
        const char* commanddoc = m_commands[commandselection].second.c_str();
        static ImGuiTextFilter filter;
        filter.Draw();

        ImGui::PushItemWidth(-1);
        ImGui::BeginChild("CommandsChild##debug", ImVec2(400.0f, 400.0f), ImGuiChildFlags_Border);
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
        ImGui::BeginChild("CommandsDoc##debug", ImVec2(-1.0f, 400.0f), ImGuiChildFlags_Border);
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
            mvSubmitCallback([=]() {
                PyRun_SimpleString(command.c_str());
                });

        }

}