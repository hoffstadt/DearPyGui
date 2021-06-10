#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFont, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvFont : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFont, add_font)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvFont(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void customAction() override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		ImFont* getFontPtr() { return m_fontPtr; }

	private:

		// config
		std::string m_file;
		float       m_size = 13.0f;
		bool        m_default = false;

		// finalized
		ImFont* m_fontPtr = nullptr;
		ImVector<ImWchar> m_ranges;

		// widgets

		//std::string                         m_rangeHint;
		//std::vector<ImWchar>                m_chars;
		//std::vector<std::pair<int, int>>    m_charRemaps;

	};

}
