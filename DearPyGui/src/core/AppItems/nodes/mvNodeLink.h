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

		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_Link, imnodes::ColorStyle::ColorStyle_Link, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_LinkHovered, imnodes::ColorStyle::ColorStyle_LinkHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_LinkSelected, imnodes::ColorStyle::ColorStyle_LinkSelected, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_Link, mvImGuiCol_Link),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_LinkHovered, mvImGuiCol_LinkHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_LinkSelected, mvImGuiCol_LinkSelected),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvNodeLink(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* args) override;

	private:

		int m_id = 0;
		int m_id1 = 0;
		int m_id2 = 0;
	};

}