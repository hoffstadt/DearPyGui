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

	class mvNodeEditor : public mvAppItem
	{

		MV_APPITEM_TYPE(mvAppItemType::NodeEditor, "add_node_editor")

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvNodeEditor(const std::string& name, PyObject* linkCallback, PyObject* delinkCallback);

		void addLink(const std::string& node1, const std::string& node2);
		void addLink(int node1, int node2);
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

		PyObject* m_linkCallback;
		PyObject* m_delinkCallback;
	};

}