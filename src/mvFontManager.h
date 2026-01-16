#pragma once

#include <string>
#include <unordered_map>
#include <imgui.h>
#include "mvContext.h"
#include "mvToolWindow.h"

class mvFontManager : public mvToolWindow
{

public:

	void   updateAtlas();
	float& getGlobalFontScale() { return _globalFontScale; }
	void   setGlobalFontScale(float scale);
	bool   isDefaultFont(const std::shared_ptr<mvAppItem>& item)
	{
		return (_defaultFont.lock() == item);
	}

	mvUUID getUUID() const override { return MV_TOOL_FONT_UUID; }
	const char* getTitle() const override { return "Font Manager"; }

protected:
    ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_HorizontalScrollbar;

	void drawWidgets() override;

public:

	// Must be a mvFont (or empty)
	std::weak_ptr<mvAppItem> _defaultFont;
	float             _globalFontScale = 1.0f;

};