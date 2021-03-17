#pragma once
#include "mvAppItem.h"
#include <stdint.h>

namespace Marvel {

	PyObject* add_node_editor(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_node_link(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_node_link(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_links(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_selected_links(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* clear_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* clear_selected_links(PyObject* self, PyObject* args, PyObject* kwargs);

	MV_REGISTER_WIDGET(mvNodeEditor);
	class mvNodeEditor : public mvAppItem
	{

		MV_APPITEM_TYPE(mvAppItemType::mvNodeEditor, "add_node_editor")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_NodeEditor_Link,                7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_NodeEditor_LinkHovered,         8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_NodeEditor_LinkSelected,        9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_NodeEditor_BoxSelector,        12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_NodeEditor_BoxSelectorOutline, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_NodeEditor_GridBackground,     14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_NodeEditor_GridLine,           15L, 0L);

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
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvNodeEditor(const std::string& name, mvCallable linkCallback, mvCallable delinkCallback);
		~mvNodeEditor();

		void addLink(const std::string& node1, const std::string& node2);
		void addLink(int node1, int node2);
		void deleteLink(const std::string& node, int id, bool deletion);
		void deleteLink(const std::string& node1, const std::string& node2);
		void deleteLink(int node1, int node2);
		std::vector<std::string> getSelectedNodes() const;
		const std::vector<std::pair<std::string, std::string>>& getLinks() const { return m_linksStrings; }
		const std::vector<int>& getSelectedLinks() const { return m_selectedLinks; }
		void clearNodes() { m_clearNodes = true; }
		void clearLinks() { m_clearLinks = true; }

		void draw() override;

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