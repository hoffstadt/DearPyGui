#pragma once
#include "mvAppItem.h"
#include <stdint.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvNodeEditor, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvNodeEditor : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvNodeEditor, add_node_editor)

		MV_CREATE_EXTRA_COMMAND(add_node_link);
		MV_CREATE_EXTRA_COMMAND(delete_node_link);
		MV_CREATE_EXTRA_COMMAND(get_selected_nodes);
		MV_CREATE_EXTRA_COMMAND(get_links);
		MV_CREATE_EXTRA_COMMAND(get_selected_links);
		MV_CREATE_EXTRA_COMMAND(clear_selected_nodes);
		MV_CREATE_EXTRA_COMMAND(clear_selected_links);

		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_Link				, imnodes::ColorStyle::ColorStyle_Link				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_LinkHovered		, imnodes::ColorStyle::ColorStyle_LinkHovered		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_LinkSelected		, imnodes::ColorStyle::ColorStyle_LinkSelected		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_BoxSelector		, imnodes::ColorStyle::ColorStyle_BoxSelector		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_BoxSelectorOutline	, imnodes::ColorStyle::ColorStyle_BoxSelectorOutline, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_GridBackground		, imnodes::ColorStyle::ColorStyle_GridBackground	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_NodeEditor_GridLine			, imnodes::ColorStyle::ColorStyle_GridLine			, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_GridSpacing				, imnodes::StyleVar::StyleVar_GridSpacing				, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_LinkThickness			, imnodes::StyleVar::StyleVar_LinkThickness				, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_LinkLineSegmentsPerLength, imnodes::StyleVar::StyleVar_LinkLineSegmentsPerLength	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_LinkHoverDistance		, imnodes::StyleVar::StyleVar_LinkHoverDistance			, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_PinCircleRadius			, imnodes::StyleVar::StyleVar_PinCircleRadius			, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_PinQuadSideLength		, imnodes::StyleVar::StyleVar_PinQuadSideLength			, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_PinTriangleSideLength	, imnodes::StyleVar::StyleVar_PinTriangleSideLength		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_PinLineThickness			, imnodes::StyleVar::StyleVar_PinLineThickness			, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_PinHoverRadius			, imnodes::StyleVar::StyleVar_PinHoverRadius			, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_NodeEditor_PinOffset				, imnodes::StyleVar::StyleVar_PinOffset					, 0L);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(add_node_link);
			MV_ADD_EXTRA_COMMAND(delete_node_link);
			MV_ADD_EXTRA_COMMAND(get_selected_nodes);
			MV_ADD_EXTRA_COMMAND(get_links);
			MV_ADD_EXTRA_COMMAND(get_selected_links);
			MV_ADD_EXTRA_COMMAND(clear_selected_nodes);
			MV_ADD_EXTRA_COMMAND(clear_selected_links);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_Link,               mvColor( 61, 133, 224, 200)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_LinkHovered,        mvColor( 66, 150, 250, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_LinkSelected,       mvColor( 66, 150, 250, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_BoxSelector,        mvColor( 61, 133, 224,  30)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_BoxSelectorOutline, mvColor( 61, 133, 224, 150)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_GridBackground,     mvColor( 40,  40,  50, 200)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeEditor_GridLine,           mvColor(200, 200, 200,  40)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_NodeEditor_GridSpacing,				20, 20),
			MV_ADD_CONSTANT(mvThemeStyle_NodeEditor_LinkThickness,				3, 12),
			MV_ADD_CONSTANT(mvThemeStyle_NodeEditor_LinkLineSegmentsPerLength,	1,  1),
			MV_ADD_CONSTANT(mvThemeStyle_NodeEditor_LinkHoverDistance,			3, 12),
			MV_ADD_CONSTANT(mvThemeStyle_NodeEditor_PinCircleRadius,			5, 12),
			MV_ADD_CONSTANT(mvThemeStyle_NodeEditor_PinQuadSideLength,			5, 12),
			MV_ADD_CONSTANT(mvThemeStyle_NodeEditor_PinTriangleSideLength,		5, 12),
			MV_ADD_CONSTANT(mvThemeStyle_NodeEditor_PinLineThickness,			3, 12),
			MV_ADD_CONSTANT(mvThemeStyle_NodeEditor_PinHoverRadius,				5, 12),
			MV_ADD_CONSTANT(mvThemeStyle_NodeEditor_PinOffset,					0, 12),
		MV_END_STYLE_CONSTANTS

	public:

		mvNodeEditor(const std::string& name, mvCallable linkCallback, mvCallable delinkCallback);
		~mvNodeEditor();

		bool canChildBeAdded(mvAppItemType type) override;

		// called by python
		void addLink   (const std::string& node1, const std::string& node2);
		void deleteLink(const std::string& node1, const std::string& node2);

		
		void addLink(int node1, int node2);
		void deleteLink(const std::string& node, int id, bool deletion);
		void deleteLink(int node1, int node2);
		std::vector<std::string> getSelectedNodes() const;
		const std::vector<std::pair<std::string, std::string>>& getLinks() const { return m_linksStrings; }
		const std::vector<int>& getSelectedLinks() const { return m_selectedLinks; }
		void clearNodes() { m_clearNodes = true; }
		void clearLinks() { m_clearLinks = true; }

		void draw(ImDrawList* drawlist, float x, float y) override;

	private:

		std::vector<std::pair<int, int>> m_links;
		std::vector<std::pair<std::string, std::string>> m_linksStrings;
		std::vector<int> m_selectedNodes;
		std::vector<int> m_selectedLinks;

		bool m_clearNodes = false;
		bool m_clearLinks = false;

		mvCallable m_linkCallback = nullptr;
		mvCallable m_delinkCallback = nullptr;
	};

}