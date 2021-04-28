#include <map>
#include "mvPythonParser.h"
#include "mvDocWindow.h"
#include "mvModule_Core.h"

namespace Marvel {

	static void ColorText(const char* item)
	{
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotLinesHovered), "%s", item);
	}

	static void CodeColorText(const char* item)
	{
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "%s", item);
	}

	static void WidgetTableEntry(const char* widget, const char* container, const char* callback, const char* source, const char* desc)
	{
		ImGui::Separator();
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_DragDropTarget), widget); ImGui::NextColumn();
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), container); ImGui::NextColumn();
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogramHovered), callback); ImGui::NextColumn();
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotLinesHovered), source); ImGui::NextColumn();
		ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_Text), desc); ImGui::NextColumn();
	}

    mvDocWindow::mvDocWindow()
    {
        m_windowflags = ImGuiWindowFlags_NoSavedSettings;
		m_width = 700;
		m_height = 500;
		setup();
    }

	void mvDocWindow::setup()
	{

		const std::map<std::string, mvPythonParser>& docmap = mvModule_Core::GetModuleParsers();
		const std::vector<std::pair<std::string, long>>& constants = mvModule_Core::GetModuleConstants();

		for (const auto& item : constants)
		{
			m_constantsValues.emplace_back("Constant with a value of " + std::to_string(item.second));
			m_docCategories["Constants"].push_back(m_constantsValues.back().c_str());
			m_commandCategories["Constants"].push_back(item.first.c_str());
		}

		for (const auto& item : docmap)
		{
			m_commands.emplace_back(item.first.c_str(), item.second.getDocumentation());

			const std::vector<std::string>& categories = item.second.getCategory();

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
				docCategory.push_back(item.second.getDocumentation());

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
				ImGui::ListBox("Category", &categorySelection, m_categories.data(), m_categories.size(), 7);

				ImGui::SetNextItemWidth(500);
				ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0, 0, 100));

				static int selection = 0;
				const std::string& category = m_categories[categorySelection];
				std::vector<const char*>& commands = m_commandCategories[category];
				std::vector<const char*>& docs = m_docCategories[category];
				if (ImGui::ListBox("Commands", &selection, commands.data(), commands.size(), 30))
					m_doc = docs[selection];

				//else if (categorySelection == 11)
				//{
				//	static int selection = 0;
				//	if (ImGui::ListBox("Commands", &selection, m_cconstants.data(), (int)m_cconstants.size(), 30))
				//		m_doc = m_constantsValues[selection].c_str();
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

}
