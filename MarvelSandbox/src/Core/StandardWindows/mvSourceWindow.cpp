#include "mvSourceWindow.h"
#include <imgui.h>
#include "Core/mvApp.h"


namespace Marvel {

	mvSourceWindow* mvSourceWindow::s_instance = nullptr;

	mvSourceWindow* mvSourceWindow::GetWindow()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvSourceWindow();
		return s_instance;
	}

	void mvSourceWindow::render(bool& show)
	{
		auto cpos = mvApp::GetApp()->getEditor().GetCursorPosition();
		ImGui::Begin("App Source", &show, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Edit"))
			{

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, mvApp::GetApp()->getEditor().HasSelection()))
					mvApp::GetApp()->getEditor().Copy();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
					mvApp::GetApp()->getEditor().SetSelection(mvTextEditor::Coordinates(), mvTextEditor::Coordinates(mvApp::GetApp()->getEditor().GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
					mvApp::GetApp()->getEditor().SetPalette(mvTextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light palette"))
					mvApp::GetApp()->getEditor().SetPalette(mvTextEditor::GetLightPalette());
				if (ImGui::MenuItem("Retro blue palette"))
					mvApp::GetApp()->getEditor().SetPalette(mvTextEditor::GetRetroBluePalette());
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, mvApp::GetApp()->getEditor().GetTotalLines(),
			mvApp::GetApp()->getEditor().IsOverwrite() ? "Ovr" : "Ins",
			mvApp::GetApp()->getEditor().CanUndo() ? "*" : " ",
			mvApp::GetApp()->getEditor().GetLanguageDefinition().mName.c_str(), mvApp::GetApp()->getFile().c_str());

		mvApp::GetApp()->getEditor().Render("TextEditor");
		ImGui::End();
	}

}