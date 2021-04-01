#include "mvFontManager.h"
#include <imgui.h>
#include <assert.h>
#include <array>
#include "mvApp.h"
#include "mvAppItem.h"
#include "mvCore.h"
#include "mvItemRegistry.h"
#include "mvTextureStorage.h"
#include "mvWindow.h"

#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

// [Internal] Display details for a single font, called by ShowStyleEditor().
static void NodeFont(ImFont* font)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	bool font_details_opened = ImGui::TreeNode(font, "Font: \"%s\"\n%.2f px, %d glyphs, %d file(s)",
		font->ConfigData ? font->ConfigData[0].Name : "", font->FontSize, font->Glyphs.Size, font->ConfigDataCount);
	ImGui::SameLine(); if (ImGui::SmallButton("Set as default")) { io.FontDefault = font; }
	if (!font_details_opened)
		return;

	ImGui::PushFont(font);
	ImGui::Text("The quick brown fox jumps over the lazy dog");
	ImGui::PopFont();
	ImGui::DragFloat("Font scale", &font->Scale, 0.005f, 0.3f, 2.0f, "%.1f");   // Scale only this font
	ImGui::SameLine(); HelpMarker(
		"Note than the default embedded font is NOT meant to be scaled.\n\n"
		"Font are currently rendered into bitmaps at a given size at the time of building the atlas. "
		"You may oversample them to get some flexibility with scaling. "
		"You can also render at multiple sizes and select which one to use at runtime.\n\n"
		"(Glimmer of hope: the atlas system will be rewritten in the future to make scaling more flexible.)");
	//ImGui::InputFloat("Font offset", &font->GlyphOffset.y, 1, 1, "%.0f");
	//ImGui::InputInt("Font offset", &font->ConfigData->GlyphOffset.y, 1, 1, "%.0f");
	ImGui::Text("Ascent: %f, Descent: %f, Height: %f", font->Ascent, font->Descent, font->Ascent - font->Descent);
	ImGui::Text("Fallback character: '%c' (U+%04X)", font->FallbackChar, font->FallbackChar);
	ImGui::Text("Ellipsis character: '%c' (U+%04X)", font->EllipsisChar, font->EllipsisChar);
	const int surface_sqrt = (int)sqrtf((float)font->MetricsTotalSurface);
	ImGui::Text("Texture Area: about %d px ~%dx%d px", font->MetricsTotalSurface, surface_sqrt, surface_sqrt);
	for (int config_i = 0; config_i < font->ConfigDataCount; config_i++)
		if (font->ConfigData)
			if (const ImFontConfig* cfg = &font->ConfigData[config_i])
				ImGui::BulletText("Input %d: \'%s\', Oversample: (%d,%d), PixelSnapH: %d",
					config_i, cfg->Name, cfg->OversampleH, cfg->OversampleV, cfg->PixelSnapH);
	if (ImGui::TreeNode("Glyphs", "Glyphs (%d)", font->Glyphs.Size))
	{
		// Display all glyphs of the fonts in separate pages of 256 characters
		const ImU32 glyph_col = ImGui::GetColorU32(ImGuiCol_Text);
		for (unsigned int base = 0; base <= IM_UNICODE_CODEPOINT_MAX; base += 256)
		{
			// Skip ahead if a large bunch of glyphs are not present in the font (test in chunks of 4k)
			// This is only a small optimization to reduce the number of iterations when IM_UNICODE_MAX_CODEPOINT
			// is large // (if ImWchar==ImWchar32 we will do at least about 272 queries here)
			if (!(base & 4095) && font->IsGlyphRangeUnused(base, base + 4095))
			{
				base += 4096 - 256;
				continue;
			}

			int count = 0;
			for (unsigned int n = 0; n < 256; n++)
				if (font->FindGlyphNoFallback((ImWchar)(base + n)))
					count++;
			if (count <= 0)
				continue;
			if (!ImGui::TreeNode((void*)(intptr_t)base, "U+%04X..U+%04X (%d %s)", base, base + 255, count, count > 1 ? "glyphs" : "glyph"))
				continue;
			float cell_size = font->FontSize * 1;
			float cell_spacing = style.ItemSpacing.y;
			ImVec2 base_pos = ImGui::GetCursorScreenPos();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			for (unsigned int n = 0; n < 256; n++)
			{
				// We use ImFont::RenderChar as a shortcut because we don't have UTF-8 conversion functions
				// available here and thus cannot easily generate a zero-terminated UTF-8 encoded string.
				ImVec2 cell_p1(base_pos.x + (n % 16) * (cell_size + cell_spacing), base_pos.y + (n / 16) * (cell_size + cell_spacing));
				ImVec2 cell_p2(cell_p1.x + cell_size, cell_p1.y + cell_size);
				const ImFontGlyph* glyph = font->FindGlyphNoFallback((ImWchar)(base + n));
				draw_list->AddRect(cell_p1, cell_p2, glyph ? IM_COL32(255, 255, 255, 100) : IM_COL32(255, 255, 255, 50));
				if (glyph)
					font->RenderChar(draw_list, cell_size, cell_p1, glyph_col, (ImWchar)(base + n));
				if (glyph && ImGui::IsMouseHoveringRect(cell_p1, cell_p2))
				{
					ImGui::BeginTooltip();
					ImGui::Text("Codepoint: U+%04X", base + n);
					ImGui::Separator();
					ImGui::Text("Visible: %d", glyph->Visible);
					ImGui::Text("AdvanceX: %.1f", glyph->AdvanceX);
					ImGui::Text("Pos: (%.2f,%.2f)->(%.2f,%.2f)", glyph->X0, glyph->Y0, glyph->X1, glyph->Y1);
					ImGui::Text("UV: (%.3f,%.3f)->(%.3f,%.3f)", glyph->U0, glyph->V0, glyph->U1, glyph->V1);
					ImGui::EndTooltip();
				}
			}
			ImGui::Dummy(ImVec2((cell_size + cell_spacing) * 16, (cell_size + cell_spacing) * 16));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::TreePop();
}

// Demo helper function to select among loaded fonts.
// Here we use the regular BeginCombo()/EndCombo() api which is more the more flexible one.
static void ShowCustomFontSelector(const char* label)
{
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font_current = ImGui::GetFont();
	if (ImGui::BeginCombo(label, font_current->GetDebugName()))
	{
		for (int n = 0; n < io.Fonts->Fonts.Size; n++)
		{
			ImFont* font = io.Fonts->Fonts[n];
			ImGui::PushID((void*)font);
			if (ImGui::Selectable(font->GetDebugName(), font == font_current))
				io.FontDefault = font;
			ImGui::PopID();
		}
		ImGui::EndCombo();
	}
}

namespace Marvel {

	void mvFontManager::InValidateFontTheme()
	{
		auto& frontWindows = mvApp::GetApp()->getItemRegistry().getFrontWindows();
		auto& backWindows = mvApp::GetApp()->getItemRegistry().getBackWindows();

		for (auto& window : frontWindows)
			window->inValidateThemeFontCache();

		for (auto& window : backWindows)
			window->inValidateThemeFontCache();
	}

	mvFontManager::mvFontManager()
	{
		mvEventBus::Subscribe(this, SID("set_font"), mvEVT_CATEGORY_THEMES);
	}

	bool mvFontManager::isInvalid() const
	{
		return m_dirty;
	}

	void mvFontManager::addFont(const std::string& font, const std::string& file, int size, const std::string& rangeHint,
		const std::vector<ImWchar>& chars, const std::vector<std::array<ImWchar, 3>>& fontGlyphRangeCustom,
		const std::vector<std::pair<int, int>>& charRemaps)
	{

		Font newFont = {};
		newFont.key = font + std::to_string(size);
		newFont.name = font;
		newFont.file = file;
		newFont.rangeHint = rangeHint;
		newFont.size = size;
		newFont.chars = chars;
		newFont.fontGlyphRangeCustom = fontGlyphRangeCustom;
		newFont.charRemaps = charRemaps;
		newFont.fontPtr = nullptr;

		m_fonts.push_back(newFont);

		m_dirty = true;
		mvApp::GetApp()->getTextureStorage().scheduleRefresh();
	}

	ImFont* mvFontManager::getFont(const std::string& font, int size)
	{

		if (font.empty())
			return m_font;

		std::string key = font + std::to_string(size);

		for (auto& Font : m_fonts)
		{
			if (Font.key == key)
				return Font.fontPtr;
		}

		assert(false);
		return nullptr;
	}

	void mvFontManager::rebuildAtlas()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->Clear();
		io.FontDefault = io.Fonts->AddFontDefault();

		// Add character ranges and merge into the previous font
		// The ranges array is not copied by the AddFont* functions and is used lazily
		// so ensure it is available at the time of building or calling GetTexDataAsRGBA32().
		const ImWchar icons_ranges[] = { 0x0370, 0x03ff, 0 }; // Will not be copied by AddFont* so keep in scope.

		for (auto& font : m_fonts)
		{
			ImVector<ImWchar> ranges;
			ImFontGlyphRangesBuilder builder;

			if (font.rangeHint.empty())                                          builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
			else if (font.rangeHint == std::string("korean"))                    builder.AddRanges(io.Fonts->GetGlyphRangesKorean());
			else if (font.rangeHint == std::string("japanese"))	                 builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
			else if (font.rangeHint == std::string("chinese_full"))              builder.AddRanges(io.Fonts->GetGlyphRangesChineseFull());
			else if (font.rangeHint == std::string("chinese_simplified_common")) builder.AddRanges(io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
			else if (font.rangeHint == std::string("cyrillic"))                  builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
			else if (font.rangeHint == std::string("thai"))                      builder.AddRanges(io.Fonts->GetGlyphRangesThai());
			else if (font.rangeHint == std::string("vietnamese"))                builder.AddRanges(io.Fonts->GetGlyphRangesVietnamese());
			else io.Fonts->AddFontDefault();

			builder.AddRanges(icons_ranges); // Add one of the default ranges

			for (const auto& range : font.fontGlyphRangeCustom)
				builder.AddRanges(range.data());
			for (const auto& charitem : font.chars)
				builder.AddChar(charitem);

			builder.BuildRanges(&ranges);   // Build the final result (ordered ranges with all the unique characters submitted)

			font.fontPtr = io.Fonts->AddFontFromFileTTF(font.file.c_str(), font.size, nullptr, ranges.Data);

			if (font.fontPtr == nullptr)
			{
				int line = PyFrame_GetLineNumber(PyEval_GetFrame());
				PyErr_Format(PyExc_Exception,
					"Font file %s could not be found.  %d c", font.file.c_str(), line);
				PyErr_Print();
				io.Fonts->Build();
			}

			for (auto& item : font.charRemaps)
				font.fontPtr->AddRemapChar(item.first, item.second);

			io.Fonts->Build();
		}

		InValidateFontTheme();
	}

	void mvFontManager::updateDefaultFont()
	{
		m_font = getFont(m_fontName, m_size);
		m_dirty = false;

		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = m_font;
	}

	bool mvFontManager::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvFontManager::onSetFont), SID("set_font"));
		return event.handled;
	};

	bool mvFontManager::onSetFont(mvEvent& event)
	{
		const std::string& widget = GetEString(event, "WIDGET");
		const std::string& font = GetEString(event, "FONT");
		int size = GetEInt(event, "SIZE");

		if (widget.empty())
		{
			InValidateFontTheme();
			m_font = getFont(font, size);
			m_size = size;
			m_fontName = font;
			return true;
		}

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(widget);
		if (item)
		{
			item->inValidateThemeFontCache();
			item->setFont(getFont(font, size));
		}
		else
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					ThrowPythonException("Item can not be found");
				});
		}
		return true;
	}

	void mvFontManager::show_debugger()
	{
		if (ImGui::Begin("Font Manager"))
		{
			ShowCustomFontSelector("Fonts##Selector");

			ImGuiIO& io = ImGui::GetIO();
			ImFontAtlas* atlas = io.Fonts;
			HelpMarker("Read FAQ and docs/FONTS.md for details on font loading.");
			ImGui::PushItemWidth(120);
			for (int i = 0; i < atlas->Fonts.Size; i++)
			{
				ImFont* font = atlas->Fonts[i];
				ImGui::PushID(font);
				NodeFont(font);
				ImGui::PopID();
			}
			if (ImGui::TreeNode("Atlas texture", "Atlas texture (%dx%d pixels)", atlas->TexWidth, atlas->TexHeight))
			{
				ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
				ImGui::Image(atlas->TexID, ImVec2((float)atlas->TexWidth, (float)atlas->TexHeight), ImVec2(0, 0), ImVec2(1, 1), tint_col, border_col);
				ImGui::TreePop();
			}

			// Post-baking font scaling. Note that this is NOT the nice way of scaling fonts, read below.
			// (we enforce hard clamping manually as by default DragFloat/SliderFloat allows CTRL+Click text to get out of bounds).
			const float MIN_SCALE = 0.3f;
			const float MAX_SCALE = 2.0f;
			HelpMarker(
				"Those are old settings provided for convenience.\n"
				"However, the _correct_ way of scaling your UI is currently to reload your font at the designed size, "
				"rebuild the font atlas, and call style.ScaleAllSizes() on a reference ImGuiStyle structure.\n"
				"Using those settings here will give you poor quality results.");
			if (ImGui::DragFloat("global scale", &getGlobalFontScale(), 0.005f, MIN_SCALE, MAX_SCALE, "%.2f")) // Scale everything
				getGlobalFontScale() = IM_MAX(getGlobalFontScale(), MIN_SCALE);
			ImGui::PopItemWidth();
		}
		ImGui::End();
	}

	void mvFontManager::setGlobalFontScale(float scale)
	{
		m_globalFontScale = scale;
	}

	void mvFontManager::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_font", mvPythonParser({
			{mvPythonDataType::String, "font", "ttf or otf file"},
			{mvPythonDataType::String, "file", "ttf or otf file"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "size", "", "13.0"},
			{mvPythonDataType::String, "glyph_ranges", "options: korean, japanese, chinese_full, chinese_simplified_common, cryillic, thai, vietnamese", "''"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "custom_glyph_chars", "", "()"},
			{mvPythonDataType::Object, "custom_glyph_ranges", "list of ranges", "List[List[int]]"},
			{mvPythonDataType::Object, "char_remaps", "", "List[List[int]]"},
		}, "Adds additional font.", "None", "Themes and Styles") });

		parsers->insert({ "set_font", mvPythonParser({
			{mvPythonDataType::String, "font", "ttf or otf file"},
			{mvPythonDataType::Integer, "size", "ttf or otf file"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "item", "", "''"},
		}, "Adds additional font.", "None", "Themes and Styles") });

		parsers->insert({ "set_global_font_scale", mvPythonParser({
			{mvPythonDataType::Float, "scale", "default is 1.0"}
		}, "Changes the global font scale.") });

		parsers->insert({ "get_global_font_scale", mvPythonParser({
		}, "Returns the global font scale.", "float") });
	}

	PyObject* mvFontManager::add_font(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* font;
		const char* file;
		float size = 13.0f;
		const char* glyph_ranges = "";
		PyObject* custom_glyph_ranges = nullptr;
		PyObject* custom_glyph_chars = nullptr;
		PyObject* char_remaps = nullptr;

		if (!(mvApp::GetApp()->getParsers())["add_font"].parse(args, kwargs, __FUNCTION__,
			&font, &file, &size, &glyph_ranges, &custom_glyph_chars, &custom_glyph_ranges,
			&char_remaps))
			return GetPyNone();

		std::vector<int> custom_chars = ToIntVect(custom_glyph_chars);
		std::vector<std::pair<int, int>> custom_ranges = ToVectInt2(custom_glyph_ranges);
		std::vector<std::pair<int, int>> custom_remaps = ToVectInt2(char_remaps);

		std::vector<std::array<ImWchar, 3>> imgui_custom_ranges;
		std::vector<ImWchar> imgui_custom_chars;

		for (auto& item : custom_ranges)
			imgui_custom_ranges.push_back({ (ImWchar)item.first, (ImWchar)item.second, 0 });
		for (auto& item : custom_chars)
			imgui_custom_chars.push_back((ImWchar)item);

		mvApp::GetApp()->getFontManager().addFont(font, file, size, glyph_ranges, imgui_custom_chars, 
			imgui_custom_ranges, custom_remaps);

		return GetPyNone();
	}

	PyObject* mvFontManager::set_font(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* font = "";
		int size = 0;
		const char* item = "";

		if (!(mvApp::GetApp()->getParsers())["set_font"].parse(args, kwargs, __FUNCTION__, &font, &size, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());

		mvEventBus::Publish
		(
			mvEVT_CATEGORY_THEMES,
			SID("set_font"),
			{
				CreateEventArgument("WIDGET", std::string(item)),
				CreateEventArgument("FONT", std::string(font)),
				CreateEventArgument("SIZE", size)
			}
		);

		return GetPyNone();
	}

	PyObject* mvFontManager::set_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float scale;

		if (!(mvApp::GetApp()->getParsers())["set_global_font_scale"].parse(args, kwargs, __FUNCTION__, &scale))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->getFontManager().setGlobalFontScale(scale);

		return GetPyNone();
	}

	PyObject* mvFontManager::get_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return ToPyFloat(mvApp::GetApp()->getFontManager().getGlobalFontScale());
	}
}