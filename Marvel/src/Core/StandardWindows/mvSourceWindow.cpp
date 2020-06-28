#include "mvSourceWindow.h"
#include <imgui.h>
#include <fstream>
#include <streambuf>

namespace Marvel {

	mvSourceWindow* mvSourceWindow::s_instance = nullptr;

	mvSourceWindow* mvSourceWindow::GetWindow()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvSourceWindow();
		return s_instance;
	}

	void mvSourceWindow::setFile(const std::string& file)
	{
		m_file = file;
		m_editor.SetLanguageDefinition(mvTextEditor::LanguageDefinition::Python());
		{
			std::ifstream t(m_file.c_str());
			if (t.good())
			{
				std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
				m_editor.SetText(str);
			}
		}

		m_editor.SetReadOnly(true);
		m_editor.SetShowWhitespaces(false);
	}

	void mvSourceWindow::render(bool& show)
	{
		auto cpos = m_editor.GetCursorPosition();
		ImGui::Begin("App Source", &show, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Edit"))
			{

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, m_editor.HasSelection()))
					m_editor.Copy();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
					m_editor.SetSelection(mvTextEditor::Coordinates(), mvTextEditor::Coordinates(m_editor.GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
					m_editor.SetPalette(mvTextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light palette"))
					m_editor.SetPalette(mvTextEditor::GetLightPalette());
				if (ImGui::MenuItem("Retro blue palette"))
					m_editor.SetPalette(mvTextEditor::GetRetroBluePalette());
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, m_editor.GetTotalLines(),
			m_editor.IsOverwrite() ? "Ovr" : "Ins",
			m_editor.CanUndo() ? "*" : " ",
			m_editor.GetLanguageDefinition().mName.c_str(), m_file.c_str());

		m_editor.Render("TextEditor");
		ImGui::End();
	}

}