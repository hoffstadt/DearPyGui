#pragma once

#include <string>
#include <unordered_map>
#include <imgui.h>
#include "mvEvents.h"
#include "mvApp.h"
#include "cpp.hint"
#include "mvToolWindow.h"

struct ImFont;

namespace Marvel {

	class mvFontManager : public mvToolWindow
	{
		friend class mvFont;

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		//MV_CREATE_COMMAND(add_font);
		//MV_CREATE_COMMAND(set_font);
		MV_CREATE_COMMAND(get_global_font_scale);
		MV_CREATE_COMMAND(set_global_font_scale);

		MV_START_COMMANDS
			//MV_ADD_COMMAND(add_font);
			//MV_ADD_COMMAND(set_font);
			MV_ADD_COMMAND(get_global_font_scale);
			MV_ADD_COMMAND(set_global_font_scale);
		MV_END_COMMANDS

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

		mvUUID getUUID() const override { return MV_TOOL_FONT_UUID; }
		const char* getTitle() const override { return "Font Manager"; }

	protected:

		void drawWidgets() override;

	private:

		// default
		ImFont*           m_font = nullptr;
		bool              m_dirty = false;
		float             m_globalFontScale = 1.0f;

	};

}