#include "mvAppEditor.h"
#include "mvCore.h"
#include "mvAppLog.h"
#include <fstream>
#include <streambuf>
#include "Core/StandardWindows/mvDocWindow.h"
#include "Core/StandardWindows/mvAboutWindow.h"
#include "Core/StandardWindows/mvMetricsWindow.h"
#include "Core/StandardWindows/mvSourceWindow.h"
#include "Core/mvUtilities.h"

#include <Windows.h>
#include <filesystem>
#include <misc/cpp/imgui_stdlib.h>

namespace fs = std::filesystem;

static void startup(std::string file, std::string flags)
{
	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	fs::path p = fs::path(file);
	p.replace_extension(" ");
	auto filename = p.filename().string();
	auto pathname = p.parent_path().string();

	size_t lastindex = filename.find_last_of(".");
	std::string rawname = filename.substr(0, lastindex);

	std::string cmd = "MarvelSandbox.exe --app " + rawname + " --path \"" + pathname + "\" " + flags;

	// start the program up
	CreateProcess("MarvelSandbox.exe",   // the path
		const_cast<char*>(cmd.c_str()),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

namespace Marvel {

	mvAppEditor* mvAppEditor::s_instance = nullptr;

	mvAppEditor::mvAppEditor()
	{
		m_editor.SetLanguageDefinition(mvTextEditor::LanguageDefinition::Python());
		m_editor.SetShowWhitespaces(m_showWhiteSpace);
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
		}

		if (ImGui::IsKeyReleased(0x74)) // F5
		{
			saveFile();
			startup(m_file, m_flags);
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
				m_editor.SetText(str);
			}
		}
	}

	void mvAppEditor::closeFile()
	{
		if (!m_saved)
			saveFile();
		setFile("");
		m_saved = true;
		m_editor.ClearUndo();
		m_editor.SetText("");

	}

	void mvAppEditor::openFile()
	{
		if (!m_saved)
			saveFile();

		std::vector<std::pair<std::string, std::string> > extensions = { std::make_pair("Python", "*.py") };
		setFile(OpenFile(extensions));
		m_saved = true;
		m_editor.ClearUndo();


	}

	void mvAppEditor::saveFile()
	{
		if (m_file.empty())
			return;

		auto textToSave = m_editor.GetText();
		/// save text....
		m_saved = true;
		m_editor.ClearUndo();

		{
			std::ofstream t(m_file.c_str());
			if (t.good())
				t << m_editor.GetText();
		}
	}

	void mvAppEditor::saveFileAs()
	{
		std::vector<std::pair<std::string, std::string> > extensions = { std::make_pair("Python", "*.py") };
		m_file = SaveFile(extensions);
		saveFile();
	}

	mvAppEditor* mvAppEditor::GetAppEditor()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvAppEditor();
		return s_instance;
	}

	void mvAppEditor::render()
	{

		// set imgui style to mvstyle
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;

		// standard windows
		if (m_showMetrics)
			mvMetricsWindow::GetWindow()->render(m_showMetrics);
		if (m_showAbout)
			mvAboutWindow::GetWindow()->render(m_showAbout);
		if (m_showDoc)
			mvDocWindow::GetWindow()->render(m_showDoc);

		if (m_editor.CanUndo())
			m_saved = false;

		handleKeyEvents();

		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height));

		auto cpos = m_editor.GetCursorPosition();
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
				bool ro = m_editor.IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					m_editor.SetReadOnly(ro);
				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && m_editor.CanUndo()))
					m_editor.Undo();
				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && m_editor.CanRedo()))
					m_editor.Redo();

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
				if (ImGui::MenuItem("Show Whitespace", nullptr, &m_showWhiteSpace))
					m_editor.SetShowWhitespaces(m_showWhiteSpace);

				if (ImGui::MenuItem("Dark palette"))
				{
					m_editor.SetPalette(mvTextEditor::GetDarkPalette());
					ImGui::StyleColorsDark();
				}
				if (ImGui::MenuItem("Light palette"))
				{
					m_editor.SetPalette(mvTextEditor::GetLightPalette());
					ImGui::StyleColorsLight();
				}
				if (ImGui::MenuItem("Retro blue palette"))
				{
					m_editor.SetPalette(mvTextEditor::GetRetroBluePalette());
					ImGui::StyleColorsClassic();
				}
				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("Run"))
			{
				if (ImGui::MenuItem("Run App", "F5"))
				{
					saveFile();
					startup(m_file, m_flags);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::MenuItem("Documentation"))
					m_showDoc = true;
				if (ImGui::MenuItem("Metrics"))
					m_showMetrics = true;
				if (ImGui::MenuItem("About"))
					m_showAbout = true;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, m_editor.GetTotalLines(),
			m_editor.IsOverwrite() ? "Ovr" : "Ins",
			m_editor.CanUndo() ? "*" : " ",
			m_editor.GetLanguageDefinition().mName.c_str(), getFile().c_str());

		ImGui::SameLine();
		ImGui::SetNextItemWidth(200.0f);
		ImGui::InputText("Flags", &m_flags);

		m_editor.Render("TextEditor");

		ImGui::End();


	}

}