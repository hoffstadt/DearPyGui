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
		~mvNodeEditor();

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

		bool canChildBeAdded(mvAppItemType type) override;
		void onChildRemoved(mvRef<mvAppItem> item) override;

		std::vector<mvUUID> getSelectedNodes() const;
		std::vector<mvUUID> getSelectedLinks() const;
		void clearNodes() { _clearNodes = true; }
		void clearLinks() { _clearLinks = true; }

		void draw(ImDrawList* drawlist, float x, float y) override;

	private:

		ImGuiWindowFlags _windowflags = ImGuiWindowFlags_NoSavedSettings;
		std::vector<int> _selectedNodes;
		std::vector<int> _selectedLinks;

		bool _clearNodes = false;
		bool _clearLinks = false;

		PyObject* _delinkCallback = nullptr;
		imnodes::EditorContext* _context = nullptr;
	};

}