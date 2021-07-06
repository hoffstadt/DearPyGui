#pragma once
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvNodeLink, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 0);
	class mvNodeLink : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		friend class mvNodeEditor;

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvNodeLink, add_node_link)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvNodeLink(mvUUID uuid);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void customAction() override;
		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* args) override;

	private:

		int m_id = 0;
		int m_id1 = 0;
		int m_id2 = 0;
	};

}