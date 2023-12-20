#include <map>
#include "mvPyUtils.h"
#include "mvDocWindow.h"
#include "dearpygui.h"

mvDocWindow::mvDocWindow()
{
    m_windowflags = ImGuiWindowFlags_NoSavedSettings;
    m_width = 700;
    m_height = 500;
    setup();
}

void mvDocWindow::setup()
{

    const std::map<std::string, mvPythonParser>& docmap = GetModuleParsers();
    const std::vector<std::pair<std::string, long>>& constants = GetModuleConstants();
    const std::vector<std::pair<std::string, long>>& keysConstants = GetKeysConstants();

    for (const auto& item : constants)
    {
        m_constantsValues.emplace_back("Constant with a value of " + std::to_string(item.second));
        m_docCategories["Constants"].push_back(m_constantsValues.back().c_str());
        m_commandCategories["Constants"].push_back(item.first.c_str());
    }

    for (const auto& item : keysConstants)
    {
        m_constantsValues.emplace_back("Key constant with a value of " + std::to_string(item.second));
        m_docCategories["Constants"].push_back(m_constantsValues.back().c_str());
        m_commandCategories["Constants"].push_back(item.first.c_str());
    }

    for (const auto& item : docmap)
    {
        m_commands.emplace_back(item.first.c_str(), item.second.documentation);

        const std::vector<std::string>& categories = item.second.category;

        for (const auto& category : categories)
        {
            bool categoryExists = false;
            for (const auto& cat : m_categories)
            {
                if (cat == category)
                {
                    categoryExists = true;
                    break;
                }
            }

            if (!categoryExists)
                m_categories.push_back(category.c_str());

            auto& docCategory = m_docCategories[category];
            docCategory.push_back(item.second.documentation.c_str());

            auto& commandCategory = m_commandCategories[category];
            commandCategory.push_back(item.first.c_str());
        }

    }

    m_categories.push_back("Constants");

}

void mvDocWindow::drawWidgets()
{

    if (ImGui::BeginTabBar("Main Tabbar##doc"))
    {
        if (ImGui::BeginTabItem("Commands##doc"))
        {
            ImGui::BeginGroup();
            ImGui::SetNextItemWidth(500);
            ImGui::ListBox("Category", &categorySelection, m_categories.data(), (int)m_categories.size(), 7);

            ImGui::SetNextItemWidth(500);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0, 0, 100));

            static int selection = 0;
            const std::string& category = m_categories[categorySelection];
            std::vector<const char*>& commands = m_commandCategories[category];
            std::vector<const char*>& docs = m_docCategories[category];
            if (ImGui::ListBox("Commands", &selection, commands.data(), (int)commands.size(), 30))
                m_doc = docs[selection];

            //else if (categorySelection == 11)
            //{
            //  static int selection = 0;
            //  if (ImGui::ListBox("Commands", &selection, m_cconstants.data(), (int)m_cconstants.size(), 30))
            //      m_doc = m_constantsValues[selection].c_str();
            //}

            ImGui::PopStyleColor();
            ImGui::EndGroup();

            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 255, 100));
            ImGui::BeginChild("DocChild", ImVec2(0, 0), true);
            ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 400);
            ImGui::Text("%s", m_doc);
            ImGui::PopTextWrapPos();
            ImGui::EndChild();
            ImGui::PopStyleColor();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Search Commands##doc"))
        {

            static size_t commandselection = 0;
            const char* commanddoc = m_commands[commandselection].second.c_str();
            static ImGuiTextFilter filter;
            filter.Draw();

            ImGui::PushItemWidth(300);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));
            ImGui::BeginChild("CommandsChild##debug", ImVec2(500.0f, 0), true);

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
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 255, 100));
            ImGui::BeginChild("CommandsDoc##debug", ImVec2(0, 0), true);
            ImGui::PushTextWrapPos(500);
            ImGui::Text("%s", commanddoc);
            ImGui::PopStyleColor();
            ImGui::PopTextWrapPos();
            ImGui::EndChild();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}