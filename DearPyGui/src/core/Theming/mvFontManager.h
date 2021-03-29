#pragma once

#include <string>
#include <unordered_map>
#include "mvPython.h"
#include "mvEvents.h"

struct ImFont;

namespace Marvel {

	void AddFontCommands(std::map<std::string, mvPythonParser>* parsers);

	PyObject* add_font(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_font(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvFontManager : public mvEventHandler
	{

	public:

		mvFontManager();

		void    addFont(const std::string& font, const std::string& file, int size);
		ImFont* getFont(const std::string& font, int size);
		void    rebuildAtlas();

		bool rebuild = false;

	private:

		bool onEvent(mvEvent& event) override;
		bool onSetFont(mvEvent& event);

	private:

		std::unordered_map<std::string, ImFont*> m_fonts;
		std::unordered_map<std::string, int> m_fontSize;
		std::unordered_map<std::string, std::string> m_fontFile;

	};

}