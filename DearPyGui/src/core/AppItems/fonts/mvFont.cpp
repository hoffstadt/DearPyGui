#include "mvFont.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontRangeHint.h"
#include "mvFontRange.h"
#include "mvFontManager.h"
#include "mvFontChars.h"
#include "mvCharRemap.h"

namespace Marvel {

	void mvFont::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Fonts", "Containers" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		parser.addArg<mvPyDataType::String>("file");
		parser.addArg<mvPyDataType::Integer>("size");
		parser.addArg<mvPyDataType::Bool>("default_font", mvArgType::KEYWORD_ARG, "False");

		parser.addArg<mvPyDataType::UUID>("parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_0", "Parent to add this item to. (runtime adding)");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvFont::mvFont(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}


	bool mvFont::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvFontRegistry) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: font registry", this);
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvFont::customAction()
	{
		ImGuiIO& io = ImGui::GetIO();

		m_fontPtr = io.Fonts->AddFontFromFileTTF(m_file.c_str(), m_size, 
			nullptr, m_ranges.Data);

		if (m_fontPtr == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "Font file could not be found");
			io.Fonts->Build();
			return;
		}

		// handled by char remaps
		//for (auto& item : font.charRemaps)
		//	m_fontPtr->AddRemapChar(item.first, item.second);

		io.Fonts->Build();

		if(m_default)
			io.FontDefault = m_fontPtr;

		// check ranges
		for (const auto& range : m_children[1])
		{

			if (range->getType() == mvAppItemType::mvCharRemap)
			{
				const auto rangePtr = static_cast<const mvCharRemap*>(range.get());

				m_fontPtr->AddRemapChar(rangePtr->getSourceChar(), rangePtr->getTargetChar());
			}

		}
	}

	void mvFont::draw(ImDrawList* drawlist, float x, float y)
	{

		//ImVector<ImWchar> ranges;
		ImFontGlyphRangesBuilder builder;

		static ImFontAtlas atlas;

		// check hints
		if(m_children[0].empty())
			builder.AddRanges(atlas.GetGlyphRangesDefault());

		for (const auto& hint : m_children[0])
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
		for (const auto& range : m_children[1])
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

		builder.BuildRanges(&m_ranges);   // Build the final result (ordered ranges with all the unique characters submitted)

		//m_dirty = true;
		auto item = mvApp::GetApp()->getItemRegistry().getItem(MV_ATLAS_UUID);
		if (item)
			static_cast<mvStaticTexture*>(item)->markDirty();

		mvToolManager::GetFontManager().m_dirty = true;
	}

	void mvFont::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_file = ToString(item);
				break;

			case 1:
				m_size = ToFloat(item);
				break;

			default:
				break;
			}
		}
	}

	void mvFont::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "default_font")) m_default = ToBool(item);

	}

	void mvFont::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "default_font", ToPyBool(m_default));
	}

}