#pragma once

#include <string>
#include <unordered_map>
#include <imgui.h>
#include "mvPython.h"
#include "mvEvents.h"

struct ImFont;

namespace Marvel {

	void AddFontCommands(std::map<std::string, mvPythonParser>* parsers);

	PyObject* add_font(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_font(PyObject* self, PyObject* args, PyObject* kwargs);

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

	public:

		mvFontManager();

		void addFont(const std::string& font, const std::string& file, int size, const std::string& rangeHint,
			const std::vector<ImWchar>& chars, const std::vector<std::array<ImWchar, 3>>& fontGlyphRangeCustom,
			const std::vector<std::pair<int, int>>& charRemaps);
		ImFont* getFont(const std::string& font, int size);
		void    rebuildAtlas();
		bool    isInvalid() const;
		void    updateDefaultFont();

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

	};

}