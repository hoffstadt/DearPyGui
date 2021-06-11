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

		MV_CREATE_COMMAND(get_global_font_scale);
		MV_CREATE_COMMAND(set_global_font_scale);

		MV_START_COMMANDS
			MV_ADD_COMMAND(get_global_font_scale);
			MV_ADD_COMMAND(set_global_font_scale);
		MV_END_COMMANDS

	public:

		void    rebuildAtlas();
		void    updateAtlas();
		bool    isInvalid() const;
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