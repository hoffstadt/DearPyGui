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

		MV_CREATE_COMMAND(get_selected_nodes);
		MV_CREATE_COMMAND(get_selected_links);
		MV_CREATE_COMMAND(clear_selected_nodes);
		MV_CREATE_COMMAND(clear_selected_links);

		MV_START_COMMANDS
			MV_ADD_COMMAND(get_selected_nodes);
			MV_ADD_COMMAND(get_selected_links);
			MV_ADD_COMMAND(clear_selected_nodes);
			MV_ADD_COMMAND(clear_selected_links);
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

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