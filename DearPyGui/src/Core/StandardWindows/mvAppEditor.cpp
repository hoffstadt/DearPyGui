#include "mvAppEditor.h"
#include "mvCore.h"
#include <fstream>
#include <streambuf>
#include "mvAppLog.h"
#include "Core/StandardWindows/mvDocWindow.h"
#include "Core/StandardWindows/mvAboutWindow.h"
#include "Core/StandardWindows/mvMetricsWindow.h"
#include "Core/mvUtilities.h"
#include <misc/cpp/imgui_stdlib.h>
#include <iostream>
#include "Core/mvTextEditor.h"

static const char* initialText = "from dearpygui import *\n\
\nadd_button('Button 1')\nstart_dearpygui()\n";

namespace Marvel {

	mvAppEditor::mvAppEditor()
	{

		m_editor = new mvTextEditor();

		m_editor->SetLanguageDefinition(mvTextEditor::LanguageDefinition::Python());
		m_editor->SetShowWhitespaces(m_showWhiteSpace);
		m_editor->SetText(initialText);

		addStandardWindow("documentation", mvDocWindow::GetWindow());
		addStandardWindow("about", new mvAboutWindow());
		addStandardWindow("metrics", new mvMetricsWindow());
	}

	mvAppEditor::~mvAppEditor()
	{
		delete m_editor;
		m_editor = nullptr;
	}

	void mvAppEditor::handleKeyEvents()
	{

		// windows specific currently
		if (ImGui::IsKeyDown(0x11)) // control button
		{
			if (ImGui::IsKeyReleased(0x53)) // S
				saveFile();
			if (ImGui::IsKeyReleased(0x4F)) // O
				openFile();
			if (ImGui::IsKeyReleased(0x44)) // D
			{
				std::string line = m_editor->GetCurrentLineText();
				int linepos = m_editor->GetCursorPosition().mLine;
				int linecol = m_editor->GetCursorPosition().mColumn;
				m_editor->MoveEnd();
				m_editor->InsertText("\n");
				m_editor->InsertText(line);
				m_editor->SetCursorPosition({ linepos+1, linecol });
				
			}
		}

		if (ImGui::IsKeyReleased(0x74)) // F5
		{
			saveFile();
			if (m_file.empty())
				return;
			//RunFile(m_file, m_complilerflags);
		}

	}

	void mvAppEditor::setFile(const std::string& file)
	{
		m_file = file;
		
		{
			std::ifstream t(m_file.c_str());
			if (t.good())
			{
				std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
				m_editor->SetText(str);
			}
		}
	}

	void mvAppEditor::closeFile()
	{
		if (!m_saved)
			saveFile();
		setFile("");
		m_saved = true;
		m_editor->ClearUndo();
		m_editor->SetText(initialText);

	}

	void mvAppEditor::openFile()
	{
		if (!m_saved)
			saveFile();

		std::vector<std::pair<std::string, std::string> > extensions = { std::make_pair("Python", "*.py") };
		//setFile(OpenFile(extensions));
		m_saved = true;
		m_editor->ClearUndo();


	}

	void mvAppEditor::saveFile()
	{
		if (m_file.empty())
		{
			saveFileAs();
			return;
		}

		auto textToSave = m_editor->GetText();
		/// save text....
		m_saved = true;
		m_editor->ClearUndo();

		{
			std::ofstream t(m_file.c_str());
			if (t.good())
				t << m_editor->GetText();
		}
	}

	void mvAppEditor::saveFileAs()
	{
		std::vector<std::pair<std::string, std::string> > extensions = { std::make_pair("Python", "*.py") };
		//m_file = SaveFile(extensions);
		if (m_file.empty())
			return;
		saveFile();
	}

	void mvAppEditor::render(bool& show)
	{

		// set imgui style to mvstyle
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;

		for (auto& entry : m_standardWindows)
		{
			if (entry.second.show)
				entry.second.window->render(entry.second.show);
		}

		if (m_editor->CanUndo())
			m_saved = false;

		handleKeyEvents();

		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height));

		auto cpos = m_editor->GetCursorPosition();
		ImGui::Begin("App Source", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar  
			| ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "Ctrl-O"))
					openFile();
				if (ImGui::MenuItem("Close"))
					closeFile();
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl-S")) 
					saveFile();
				if (ImGui::MenuItem("Save As"))
					saveFileAs();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				bool ro = m_editor->IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					m_editor->SetReadOnly(ro);
				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && m_editor->CanUndo()))
					m_editor->Undo();
				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && m_editor->CanRedo()))
					m_editor->Redo();

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, m_editor->HasSelection()))
					m_editor->Copy();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
					m_editor->SetSelection(mvTextEditor::Coordinates(), mvTextEditor::Coordinates(m_editor->GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Show Whitespace", nullptr, &m_showWhiteSpace))
					m_editor->SetShowWhitespaces(m_showWhiteSpace);

				if (ImGui::MenuItem("Dark palette"))
				{
					m_editor->SetPalette(mvTextEditor::GetDarkPalette());
					ImGui::StyleColorsDark();
				}
				if (ImGui::MenuItem("Light palette"))
				{
					m_editor->SetPalette(mvTextEditor::GetLightPalette());
					ImGui::StyleColorsLight();
				}
				if (ImGui::MenuItem("Retro blue palette"))
				{
					m_editor->SetPalette(mvTextEditor::GetRetroBluePalette());
					ImGui::StyleColorsClassic();
				}
				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("Run"))
			{
				if (ImGui::MenuItem("Run App", "F5"))
				{
					saveFile();
					if (m_file.empty())
						return;
					//RunFile(m_file, m_complilerflags);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::MenuItem("Documentation"))
					showStandardWindow("documentation");
				if (ImGui::MenuItem("Metrics"))
					showStandardWindow("metrics");
				if (ImGui::MenuItem("About"))
					showStandardWindow("about");
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, m_editor->GetTotalLines(),
			m_editor->IsOverwrite() ? "Ovr" : "Ins",
			m_editor->CanUndo() ? "*" : " ",
			m_editor->GetLanguageDefinition().mName.c_str(), getFile().c_str());

		ImGui::SameLine();
		ImGui::SetNextItemWidth(-50.0f);
		ImGui::InputText("Flags", &m_complilerflags);

		m_editor->Render("TextEditor");

		ImGui::End();


	}

}