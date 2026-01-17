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
	void setDefaultFont(const std::shared_ptr<mvAppItem>& font)
	{
		_defaultFont = font;
		_updateDefault = true;
	}
	void clearDefaultFont()
	{
		_defaultFont.reset();
		_updateDefault = true;
	}

	mvUUID getUUID() const override { return MV_TOOL_FONT_UUID; }
	const char* getTitle() const override { return "Font Manager"; }

protected:
    ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_HorizontalScrollbar;

	void drawWidgets() override;
	void drawFontNode(ImFont* font);
	void drawFontSelector(const char* label);

public:

	// Must be a mvFont (or empty)
	std::weak_ptr<mvAppItem> _defaultFont;
	// The only reason we need this _updateDefault flag is for "Set as default"
	// buttons work in Font Manager window and in ImGui Style Editor.  If not for those
	// buttons, we could set the default font every frame all the time.
	// Updates are blocked whenever the user clicks one of those "Set as default" buttons.
	bool                     _updateDefault = true;
	float                    _globalFontScale = 1.0f;

};