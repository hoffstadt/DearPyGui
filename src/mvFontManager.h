#pragma once

#include <string>
#include <unordered_map>
#include <imgui.h>
#include "mvContext.h"
#include "mvToolWindow.h"

struct ImFont;

class mvFontManager : public mvToolWindow
{

public:

	void   rebuildAtlas();
	void   updateAtlas();
	bool   isInvalid() const;
	float& getGlobalFontScale() { return _globalFontScale; }
	void   setGlobalFontScale(float scale);
	void   resetDefault() { _resetDefault = true; }

	mvUUID getUUID() const override { return MV_TOOL_FONT_UUID; }
	const char* getTitle() const override { return "Font Manager"; }

protected:

	void drawWidgets() override;

public:

	// default
	ImFont*           _font = nullptr;
	bool              _dirty = false;
	float             _globalFontScale = 1.0f;
	bool              _resetDefault = false;
	bool              _newDefault = false;

};