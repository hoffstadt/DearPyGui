#include "mvFont.h"
#include <fstream>
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontRangeHint.h"
#include "mvFontRange.h"
#include "mvFontManager.h"
#include "mvFontRegistry.h"
#include "mvFontChars.h"
#include "mvCharRemap.h"

namespace Marvel {

	mvFont::mvFont(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
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

		if(_default)
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
		if(childslots[0].empty())
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

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
			{
				_file = ToString(item);
				std::ifstream ifile;
				ifile.open(_file);
				if (ifile)
					ifile.close();
				else
				{
					state.ok = false;
					mvThrowPythonError(mvErrorCode::mvNone, "Font file could not be found");
				}
				break;
			}

			case 1:
				_size = ToFloat(item);
				break;

			default:
				break;
			}
		}
	}

}