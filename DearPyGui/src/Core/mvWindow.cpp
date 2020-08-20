#include "mvWindow.h"
#include <string>
#include "Core/mvPythonExceptions.h"

namespace Marvel {

	mvWindow::mvWindow(unsigned width, unsigned height, bool editor, bool error, bool doc) :
		m_editor(editor), m_error(error), m_doc(doc), m_width(width), m_height(height)
	{
		m_app = mvApp::GetAppStandardWindow();
		m_appEditor = new mvAppEditor();
		m_documentation = mvDocWindow::GetWindow();

		if (m_error)
		{
			mvAppLog::ShowMain();
			mvAppLog::setSize(width, height);
		}

		else if (m_doc)
		{
			m_documentation->setToMainMode();
			m_documentation->setSize(width, height);
		}
	}

	void mvWindow::setupFonts()
	{
		if (!mvApp::GetApp()->m_fontFile.empty())
		{
			ImGuiIO& io = ImGui::GetIO();
			//io.Fonts->AddFontDefault();

			ImFont* font = nullptr;

			if (mvApp::GetApp()->m_fontGlyphRange.empty())
				font = io.Fonts->AddFontFromFileTTF(mvApp::GetApp()->m_fontFile.c_str(), mvApp::GetApp()->m_fontSize,nullptr, io.Fonts->GetGlyphRangesDefault());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("korean"))
				font = io.Fonts->AddFontFromFileTTF(mvApp::GetApp()->m_fontFile.c_str(), mvApp::GetApp()->m_fontSize, nullptr, io.Fonts->GetGlyphRangesKorean());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("japanese"))
				font = io.Fonts->AddFontFromFileTTF(mvApp::GetApp()->m_fontFile.c_str(), mvApp::GetApp()->m_fontSize, nullptr, io.Fonts->GetGlyphRangesJapanese());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("chinese_full"))
				font = io.Fonts->AddFontFromFileTTF(mvApp::GetApp()->m_fontFile.c_str(), mvApp::GetApp()->m_fontSize, nullptr, io.Fonts->GetGlyphRangesChineseFull());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("chinese_simplified_common"))
				font = io.Fonts->AddFontFromFileTTF(mvApp::GetApp()->m_fontFile.c_str(), mvApp::GetApp()->m_fontSize, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("cyrillic"))
				font = io.Fonts->AddFontFromFileTTF(mvApp::GetApp()->m_fontFile.c_str(), mvApp::GetApp()->m_fontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("thai"))
				font = io.Fonts->AddFontFromFileTTF(mvApp::GetApp()->m_fontFile.c_str(), mvApp::GetApp()->m_fontSize, nullptr, io.Fonts->GetGlyphRangesThai());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("vietnamese"))
				font = io.Fonts->AddFontFromFileTTF(mvApp::GetApp()->m_fontFile.c_str(), mvApp::GetApp()->m_fontSize, nullptr, io.Fonts->GetGlyphRangesVietnamese());
			else
				io.Fonts->AddFontDefault();

			// Add character ranges and merge into the previous font
			// The ranges array is not copied by the AddFont* functions and is used lazily
			// so ensure it is available at the time of building or calling GetTexDataAsRGBA32().
			static const ImWchar icons_ranges[] = { 0x0370, 0x03ff, 0 }; // Will not be copied by AddFont* so keep in scope.
			ImFontConfig config;
			config.MergeMode = true;

			ImFont* fontext = io.Fonts->AddFontFromFileTTF(mvApp::GetApp()->m_fontFile.c_str(), mvApp::GetApp()->m_fontSize, &config, icons_ranges);

			if (font == nullptr)
			{
				int line = PyFrame_GetLineNumber(PyEval_GetFrame());
				PyErr_Format(PyExc_Exception,
					"Font file %s could not be found.  %d c", mvApp::GetApp()->m_fontFile.c_str(), line);
				PyErr_Print();
				io.Fonts->Build();
				return;
			}

			io.Fonts->Build();

			ImFont* newfont = io.Fonts->Fonts[0];
			io.FontDefault = newfont;

			
		}
	}

	mvWindow::~mvWindow()
	{
		delete m_appEditor;
		m_appEditor = nullptr;
	}

}