#include "mvFontItems.h"
#include "mvTextureItems.h"
#include "mvPyUtils.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

mvFontRegistry::mvFontRegistry(mvUUID uuid)
	:
	mvAppItem(uuid)
{
	config.show = true;
}

void mvFontRegistry::resetFont()
{
	for (auto& item : childslots[1])
	{
		static_cast<mvFont*>(item.get())->_default = false;
	}

	mvToolManager::GetFontManager()._resetDefault = true;
}

void mvFontRegistry::draw(ImDrawList* drawlist, float x, float y)
{
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->Clear();
	//io.FontDefault = io.Fonts->AddFontDefault();

	for (auto& item : childslots[1])
	{
		item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
	}
	config.show = false;
}

void mvCharRemap::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_source = ToInt(PyTuple_GetItem(dict, 0));
	_target = ToInt(PyTuple_GetItem(dict, 1));
}

void mvFont::customAction(void* data)
{
	if (!state.ok)
		return;

	ImGuiIO& io = ImGui::GetIO();

	_fontPtr = io.Fonts->AddFontFromFileTTF(_file.c_str(), _size,
		nullptr, _ranges.Data);

	if (_fontPtr == nullptr)
	{
		mvThrowPythonError(mvErrorCode::mvNone, "Font file could not be found");
		io.Fonts->Build();
		return;
	}

	// handled by char remaps
	//for (auto& item : font.charRemaps)
	//	_fontPtr->AddRemapChar(item.first, item.second);

	io.Fonts->Build();

	if (_default)
		io.FontDefault = _fontPtr;

	// check ranges
	for (const auto& range : childslots[1])
	{

		if (range->type == mvAppItemType::mvCharRemap)
		{
			const auto rangePtr = static_cast<const mvCharRemap*>(range.get());

			_fontPtr->AddRemapChar(rangePtr->getSourceChar(), rangePtr->getTargetChar());
		}

	}
}

void mvFont::draw(ImDrawList* drawlist, float x, float y)
{

	if (!state.ok)
		return;

	ImFontGlyphRangesBuilder builder;

	static ImFontAtlas atlas;

	// check hints
	if (childslots[0].empty())
		builder.AddRanges(atlas.GetGlyphRangesDefault());

	for (const auto& hint : childslots[0])
	{
		int hintSelection = static_cast<mvFontRangeHint*>(hint.get())->getHint();

		switch (hintSelection)
		{
		case 1:
			builder.AddRanges(atlas.GetGlyphRangesJapanese());
			break;
		case 2:
			builder.AddRanges(atlas.GetGlyphRangesKorean());
			break;
		case 3:
			builder.AddRanges(atlas.GetGlyphRangesChineseFull());
			break;
		case 4:
			builder.AddRanges(atlas.GetGlyphRangesChineseSimplifiedCommon());
			break;
		case 5:
			builder.AddRanges(atlas.GetGlyphRangesCyrillic());
			break;
		case 6:
			builder.AddRanges(atlas.GetGlyphRangesThai());
			break;
		case 7:
			builder.AddRanges(atlas.GetGlyphRangesVietnamese());
			break;
		default:
			builder.AddRanges(atlas.GetGlyphRangesDefault());
		}

	}

	// check ranges and chars
	for (const auto& range : childslots[1])
	{
		if (range->type == mvAppItemType::mvFontRange)
		{
			const auto rangePtr = static_cast<const mvFontRange*>(range.get());
			builder.AddRanges(rangePtr->getRange().data());
		}

		else if (range->type == mvAppItemType::mvFontChars)
		{
			const auto rangePtr = static_cast<const mvFontChars*>(range.get());

			for (const auto& specificChar : rangePtr->getCharacters())
				builder.AddChar(specificChar);
		}
	}

	builder.BuildRanges(&_ranges);   // Build the final result (ordered ranges with all the unique characters submitted)

	//_dirty = true;
	auto item = GetItem(*GContext->itemRegistry, MV_ATLAS_UUID);
	if (item)
		static_cast<mvStaticTexture*>(item)->markDirty();

	mvToolManager::GetFontManager()._dirty = true;
}

void mvFont::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_file = ToString(PyTuple_GetItem(dict, 0));
	std::ifstream ifile;
	ifile.open(_file);
	if (ifile)
		ifile.close();
	else
	{
		state.ok = false;
		mvThrowPythonError(mvErrorCode::mvNone, "Font file could not be found");
	}

	_size = ToFloat(PyTuple_GetItem(dict, 1));

}

void mvFontChars::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	auto charVect = ToIntVect(PyTuple_GetItem(dict, 0));
	for (auto& item : charVect)
		_chars.push_back((ImWchar)item);

}

void mvFontRange::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_min = ToInt(PyTuple_GetItem(dict, 0));
	_range[0] = (ImWchar)_min;
	_max = ToInt(PyTuple_GetItem(dict, 1));
	_range[1] = (ImWchar)_max;
}

void mvFontRangeHint::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_hint = ToInt(PyTuple_GetItem(dict, 0));
}
