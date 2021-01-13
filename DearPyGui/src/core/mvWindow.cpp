#include "mvWindow.h"
#include <string>
#include <array>
#include "mvPythonExceptions.h"

namespace Marvel {

	mvWindow::mvWindow(unsigned width, unsigned height, bool error) :
		m_error(error), m_width(width), m_height(height)
	{

		m_frontDrawList = CreateRef<mvDrawList>();
		m_backDrawList = CreateRef<mvDrawList>();

		mvEventBus::Subscribe(this, mvEVT_RENDER);

		m_app = mvApp::GetApp();

		if (m_error)
		{
			mvAppLog::ShowMain();
			mvAppLog::setSize(width, height);
		}

	}

	bool mvWindow::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);

		dispatcher.dispatch(BIND_EVENT_METH(mvWindow::onRender), mvEVT_RENDER);

		return event.handled;
	}

	bool mvWindow::onRender(mvEvent& event)
	{
		m_frontDrawList->draw(ImGui::GetForegroundDrawList(), 0.0f, 0.0f);
		m_backDrawList->draw(ImGui::GetBackgroundDrawList(), 0.0f, 0.0f);

		return true;
	}

	void mvWindow::setupFonts()
	{
		if (!mvApp::GetApp()->m_fontFile.empty())
		{
			ImGuiIO& io = ImGui::GetIO();

			ImVector<ImWchar> ranges;
			ImFontGlyphRangesBuilder builder;

			if (mvApp::GetApp()->m_fontGlyphRange.empty())
				builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("korean"))
				builder.AddRanges(io.Fonts->GetGlyphRangesKorean());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("japanese"))
				builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("chinese_full"))
				builder.AddRanges(io.Fonts->GetGlyphRangesChineseFull());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("chinese_simplified_common"))
				builder.AddRanges(io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("cyrillic"))
				builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("thai"))
				builder.AddRanges(io.Fonts->GetGlyphRangesThai());
			else if (mvApp::GetApp()->m_fontGlyphRange == std::string("vietnamese"))
				builder.AddRanges(io.Fonts->GetGlyphRangesVietnamese());
			else
				io.Fonts->AddFontDefault();

			// Add character ranges and merge into the previous font
			// The ranges array is not copied by the AddFont* functions and is used lazily
			// so ensure it is available at the time of building or calling GetTexDataAsRGBA32().
			const ImWchar icons_ranges[] = { 0x0370, 0x03ff, 0 }; // Will not be copied by AddFont* so keep in scope.
			builder.AddRanges(icons_ranges); // Add one of the default ranges
			
			for (const auto& range : mvApp::GetApp()->m_fontGlyphRangeCustom)
				builder.AddRanges(range.data());
			for (const auto& charitem : mvApp::GetApp()->m_fontGlyphChars)
				builder.AddChar(charitem);

			builder.BuildRanges(&ranges);   // Build the final result (ordered ranges with all the unique characters submitted)

			//ImFontConfig config;
			//config.OversampleH = 1;
			//config.OversampleV = 1;
			//config.GlyphExtraSpacing.x = 1.1f;

			ImFont* font = io.Fonts->AddFontFromFileTTF(mvApp::GetApp()->m_fontFile.c_str(), mvApp::GetApp()->m_fontSize, nullptr, ranges.Data);

			// add default proggy
			io.Fonts->AddFontDefault();

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

			for (auto& item : mvApp::GetApp()->m_charRemaps)
				font->AddRemapChar(item.first, item.second);

			
		}
	}

}