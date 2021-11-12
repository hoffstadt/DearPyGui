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

	void mvFont::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			std::vector<mvPythonDataElement> args;

			AddCommonArgs(args,(CommonParserArgs)(
				MV_PARSER_ARG_ID)
			);

			args.push_back({ mvPyDataType::String, "file" });
			args.push_back({ mvPyDataType::Integer, "size" });
			args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_0", "Parent to add this item to. (runtime adding)" });
			args.push_back({ mvPyDataType::Bool, "default_font", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });
			
			mvPythonParserSetup setup;
			setup.about = "Adds font to a font registry.";
			setup.category = { "Fonts", "Containers" };
			setup.returnType = mvPyDataType::UUID;
			setup.createContextManager = true;

			mvPythonParser parser = FinalizeParser(setup, args);

			parsers->insert({ s_command, parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "font" });

			mvPythonParserSetup setup;
			setup.about = "Binds a global font.";
			setup.category = { "Fonts" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "bind_font", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "text" });
			args.push_back({ mvPyDataType::Float, "wrap_width", mvArgType::KEYWORD_ARG, "-1.0", "Wrap width to use (-1.0 turns wrap off)." });
			args.push_back({ mvPyDataType::UUID, "font", mvArgType::KEYWORD_ARG, "0", "Font to use." });

			mvPythonParserSetup setup;
			setup.about = "Returns width/height of text with specified font (must occur after 1st frame).";
			setup.category = { "Fonts" };
			setup.returnType = mvPyDataType::FloatList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_text_size", parser });
		}
	}

	mvFont::mvFont(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvFont::customAction(void* data)
	{
		if (!_state.ok)
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
		for (const auto& range : _children[1])
		{

			if (range->getType() == mvAppItemType::mvCharRemap)
			{
				const auto rangePtr = static_cast<const mvCharRemap*>(range.get());

				_fontPtr->AddRemapChar(rangePtr->getSourceChar(), rangePtr->getTargetChar());
			}

		}
	}

	void mvFont::draw(ImDrawList* drawlist, float x, float y)
	{

		if (!_state.ok)
			return;

		ImFontGlyphRangesBuilder builder;

		static ImFontAtlas atlas;

		// check hints
		if(_children[0].empty())
			builder.AddRanges(atlas.GetGlyphRangesDefault());

		for (const auto& hint : _children[0])
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
		for (const auto& range : _children[1])
		{
			if (range->getType() == mvAppItemType::mvFontRange)
			{
				const auto rangePtr = static_cast<const mvFontRange*>(range.get());
				builder.AddRanges(rangePtr->getRange().data());
			}

			else if (range->getType() == mvAppItemType::mvFontChars)
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
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
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
					_state.ok = false;
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

	PyObject* mvFont::bind_font(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["bind_font"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		if (item == 0)
		{
			for (auto& reg : GContext->itemRegistry->fontRegistryRoots)
				static_cast<mvFontRegistry*>(reg.get())->resetFont();
			return GetPyNone();
		}

		auto aplot = GetItem((*GContext->itemRegistry), item);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_font",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvFont)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "bind_font",
				"Incompatible type. Expected types include: mvFont", aplot);
			return GetPyNone();
		}

		mvFont* graph = static_cast<mvFont*>(aplot);

		graph->_default = true;
		mvToolManager::GetFontManager()._newDefault = true;

		return GetPyNone();
	}

	PyObject* mvFont::get_text_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* text;
		float wrap_width = -1.0f;
		PyObject* fontRaw;

		if (!Parse((GetParsers())["get_text_size"], args, kwargs, __FUNCTION__,
			&text, &wrap_width, &fontRaw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID font = GetIDFromPyObject(fontRaw);

		if (font == 0)
		{
			if(GContext->frame < 1)
				return GetPyNone();

			ImVec2 size = ImGui::CalcTextSize(text, 0, false, wrap_width);
			return ToPyPair(size.x, size.y);
		}

		auto afont = GetItem((*GContext->itemRegistry), font);
		if (afont == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_text_size",
				"Item not found: " + std::to_string(font), nullptr);
			return GetPyNone();
		}

		if (afont->getType() != mvAppItemType::mvFont)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_text_size",
				"Incompatible type. Expected types include: mvFont", afont);
			return GetPyNone();
		}

		mvFont* graph = static_cast<mvFont*>(afont);

		if (graph->_fontPtr)
		{
			ImGuiContext& g = *GImGui;
			ImFont* previousFont = g.Font;
			float previousFontSize = g.FontSize;
			g.Font = graph->_fontPtr;
			g.FontSize = graph->_size;
			ImVec2 size = ImGui::CalcTextSize(text, 0, false, wrap_width);
			g.Font = previousFont;
			g.FontSize = previousFontSize;
			return ToPyPair(size.x, size.y);
		}

		return GetPyNone();
	}
}