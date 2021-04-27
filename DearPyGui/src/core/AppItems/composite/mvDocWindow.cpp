#include "mvDocWindow.h"
#include <imgui.h>
#include "mvApp.h"
#include "mvModule_Core.h"
#include "mvInput.h"
#include "mvItemRegistry.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvDocWindow::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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

	mvDocWindow::mvDocWindow(const std::string& name)
		: mvBaseWindowAppitem(name)
	{
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

	void mvDocWindow::draw(ImDrawList* drawlist, float x, float y)
	{
		mvFontScope fscope(this);

		if (!prerender())
			return;

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

		if (ImGui::IsWindowFocused())
		{

			float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
			mvInput::setMousePosition(x, y);

			if (mvApp::GetApp()->getItemRegistry().getActiveWindow() != "documentation##standard")
				mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", std::string("documentation##standard")) });

		}

		ImGui::End();
	}

}
