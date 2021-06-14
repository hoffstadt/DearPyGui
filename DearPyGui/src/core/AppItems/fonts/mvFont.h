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

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

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

	};

}
