#pragma once
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvNodeEditor, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvNodeEditor : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvNodeEditor, add_node_editor)

		MV_CREATE_EXTRA_COMMAND(get_selected_nodes);
		MV_CREATE_EXTRA_COMMAND(get_selected_links);
		MV_CREATE_EXTRA_COMMAND(clear_selected_nodes);
		MV_CREATE_EXTRA_COMMAND(clear_selected_links);


		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_BoxSelector, imnodes::ColorStyle::ColorStyle_BoxSelector, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_BoxSelectorOutline, imnodes::ColorStyle::ColorStyle_BoxSelectorOutline, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_GridBackground, imnodes::ColorStyle::ColorStyle_GridBackground, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_GridLine, imnodes::ColorStyle::ColorStyle_GridLine, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_GridSpacing, imnodes::StyleVar::StyleVar_GridSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_LinkThickness, imnodes::StyleVar::StyleVar_LinkThickness, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_LinkLineSegmentsPerLength, imnodes::StyleVar::StyleVar_LinkLineSegmentsPerLength, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_LinkHoverDistance, imnodes::StyleVar::StyleVar_LinkHoverDistance, 0L);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(get_selected_nodes);
			MV_ADD_EXTRA_COMMAND(get_selected_links);
			MV_ADD_EXTRA_COMMAND(clear_selected_nodes);
			MV_ADD_EXTRA_COMMAND(clear_selected_links);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_BoxSelector, mvImGuiCol_BoxSelector),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_BoxSelectorOutline, mvImGuiCol_BoxSelectorOutline),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_GridBackground, mvImGuiCol_GridBackground),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_GridLine, mvImGuiCol_GridLine),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_NodeEditor_GridSpacing, 20, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_NodeEditor_LinkThickness, 1, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_NodeEditor_LinkLineSegmentsPerLength, 1, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_NodeEditor_LinkHoverDistance, 3, 12),
		MV_END_STYLE_CONSTANTS

	public:

		mvNodeEditor(mvUUID uuid);

		void handleSpecificKeywordArgs(PyObject* dict) override;

		bool canChildBeAdded(mvAppItemType type) override;

		std::vector<mvUUID> getSelectedNodes() const;
		const std::vector<int>& getSelectedLinks() const { return m_selectedLinks; }
		void clearNodes() { m_clearNodes = true; }
		void clearLinks() { m_clearLinks = true; }

		void draw(ImDrawList* drawlist, float x, float y) override;

	private:

		std::vector<int> m_selectedNodes;
		std::vector<int> m_selectedLinks;

		bool m_clearNodes = false;
		bool m_clearLinks = false;

		PyObject* m_delinkCallback = nullptr;
	};

}