#pragma once

#include <string>
#include <unordered_map>
#include <imgui.h>
#include "mvPython.h"
#include "mvEvents.h"
#include "mvApp.h"
#include "cpp.hint"

struct ImFont;

namespace Marvel {

	class mvFontManager : public mvEventHandler
	{

		struct Font
		{
			std::string                         key;
			std::string                         name;
			ImFont*                             fontPtr;
			std::string                         file;
			std::string                         rangeHint;
			int                                 size;
			std::vector<ImWchar>                chars;
			std::vector<std::array<ImWchar, 3>> fontGlyphRangeCustom;
			std::vector<std::pair<int, int>>    charRemaps;
		};

	public:

		static void InValidateFontTheme();

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_CREATE_EXTRA_COMMAND(add_font);
		MV_CREATE_EXTRA_COMMAND(set_font);
		MV_CREATE_EXTRA_COMMAND(get_global_font_scale);
		MV_CREATE_EXTRA_COMMAND(set_global_font_scale);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(add_font);
			MV_ADD_EXTRA_COMMAND(set_font);
			MV_ADD_EXTRA_COMMAND(get_global_font_scale);
			MV_ADD_EXTRA_COMMAND(set_global_font_scale);
		MV_END_EXTRA_COMMANDS

	public:

		mvFontManager();

		void addFont(const std::string& font, const std::string& file, int size, const std::string& rangeHint,
			const std::vector<ImWchar>& chars, const std::vector<std::array<ImWchar, 3>>& fontGlyphRangeCustom,
			const std::vector<std::pair<int, int>>& charRemaps);
		ImFont* getFont(const std::string& font, int size);
		void    rebuildAtlas();
		bool    isInvalid() const;
		void    updateDefaultFont();
		float&  getGlobalFontScale() { return m_globalFontScale; }
		void     setGlobalFontScale(float scale);

		void show_debugger();

	private:

		bool onEvent(mvEvent& event) override;
		bool onSetFont(mvEvent& event);

	private:

		std::vector<Font> m_fonts;
		bool              m_dirty = false;
		
		// default
		ImFont*           m_font = nullptr;
		std::string       m_fontName;
		int               m_size = 13;
		float             m_globalFontScale = 1.0f;

	};

}