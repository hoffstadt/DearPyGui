#include "mvNodeEditor.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvNode.h"
#include "mvNodeAttribute.h"
#include "mvImGuiThemeScope.h"
#include "mvImNodesThemeScope.h"
#include "mvLog.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvNodeEditor::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::String);
			mvAppItem::AddCommonArgs(parser);
			parser.removeArg("source");
			parser.removeArg("label");
			parser.removeArg("width");
			parser.removeArg("height");
			parser.removeArg("callback");
			parser.removeArg("callback_data");
			parser.removeArg("enabled");

			parser.addArg<mvPyDataType::Callable>("link_callback", mvArgType::KEYWORD_ARG, "None", "Callback ran when a new link is created.");
			parser.addArg<mvPyDataType::Callable>("delink_callback", mvArgType::KEYWORD_ARG, "None", "Callback ran when a link is detached.");

			parser.finalize();

			parsers->insert({ s_command, parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);

			parser.addArg<mvPyDataType::String>("node_editor");
			parser.addArg<mvPyDataType::String>("node_1");
			parser.addArg<mvPyDataType::String>("node_2");

			parser.finalize();

			parsers->insert({ "add_node_link", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);

			parser.addArg<mvPyDataType::String>("node_editor");
			parser.addArg<mvPyDataType::String>("node_1");
			parser.addArg<mvPyDataType::String>("node_2");

			parser.finalize();

			parsers->insert({ "delete_node_link", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::StringList);
			parser.addArg<mvPyDataType::String>("node_editor");
			parser.finalize();
			parsers->insert({ "get_selected_nodes", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::ListStrList);
			parser.addArg<mvPyDataType::String>("node_editor");
			parser.finalize();
			parsers->insert({ "get_selected_links", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::ListStrList);
			parser.addArg<mvPyDataType::String>("node_editor");
			parser.finalize();
			parsers->insert({ "get_links", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("node_editor");
			parser.finalize();
			parsers->insert({ "clear_selected_links", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("node_editor");
			parser.finalize();
			parsers->insert({ "clear_selected_nodes", parser });
		}

	}

	mvNodeEditor::mvNodeEditor(const std::string& name)
		: mvAppItem(name)
	{
	}

	mvNodeEditor::~mvNodeEditor()
	{
		m_delinkCallback = nullptr;
		for (auto& child : m_children[1])
		{
			for (auto& grandchild : child->m_children[1])
			{
				((mvNodeAttribute*)grandchild.get())->markForDeletion();
				deleteLink(grandchild->m_name, ((mvNodeAttribute*)grandchild.get())->getId(), true);
			}

		}
	}

	void mvNodeEditor::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "link_callback"))
		{
			if (m_linkCallback)
				Py_XDECREF(m_linkCallback);
			
			Py_XINCREF(item);
			m_linkCallback = item;
		}

		if (PyObject* item = PyDict_GetItemString(dict, "delink_callback"))
		{
			if (m_delinkCallback)
				Py_XDECREF(m_delinkCallback);

			Py_XINCREF(item);
			m_delinkCallback = item;
		}
	}

	bool mvNodeEditor::canChildBeAdded(mvAppItemType type)
	{
		if(type ==mvAppItemType::mvNode)
			return true;

		mvThrowPythonError(1006, "Node editor children must be nodes only.");
		MV_ITEM_REGISTRY_ERROR("Node editor children must be nodes only.");
		assert(false);
		return false;
	}

	void mvNodeEditor::addLink(const std::string& node1, const std::string& node2)
	{
		int64_t node1_id = 0;
        int64_t node2_id = 0;

		for (const auto& node : m_children[1])
		{
			for (const auto& attr : node->m_children[1])
			{
				if (attr->m_name == node1)
					node1_id = static_cast<mvNodeAttribute*>(attr.get())->getId();
				if (attr->m_name == node2)
					node2_id = static_cast<mvNodeAttribute*>(attr.get())->getId();
			}
		}

		if (node1_id == 0 || node2_id == 0)
			return;

		addLink((int)node1_id, (int)node2_id);

		bool found = false;
		for (const auto& link : m_linksStrings)
		{
			if (link.first == node1 && link.second == node2)
				found = true;
		}
		if (!found)
			m_linksStrings.push_back(std::make_pair(node1, node2));

		if(m_linkCallback)
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
				PyObject* link = PyTuple_New(2);
				PyTuple_SetItem(link, 0, ToPyString(node1));
				PyTuple_SetItem(link, 1, ToPyString(node2));
				mvApp::GetApp()->getCallbackRegistry().addCallback(m_linkCallback, m_name, link);
				});

	}

	void mvNodeEditor::deleteLink(const std::string& node, int id, bool deletion)
	{
		int nodeid = id;

		std::vector<std::pair<int, int>> oldLinks = m_links;
		m_links.clear();
		for (auto& link : oldLinks)
		{
			if (link.first == nodeid || link.second == nodeid)
				continue;
			m_links.push_back(link);
		}

		std::vector<std::pair<std::string, std::string>> oldLinkStrings = m_linksStrings;
		m_linksStrings.clear();
		for (auto& link_string : oldLinkStrings)
		{
			if (link_string.first == node || link_string.second == node)
				continue;
			m_linksStrings.push_back(link_string);
		}

		if(m_delinkCallback && !deletion)
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
				PyObject* link = PyTuple_New(1);
				PyTuple_SetItem(link, 0, ToPyString(node));
				mvApp::GetApp()->getCallbackRegistry().addCallback(m_delinkCallback, m_name, link);
				});

	}

	void mvNodeEditor::deleteLink(const std::string& node1, const std::string& node2)
	{
		int node1_id = 0;
		int node2_id = 0;

		for (const auto& node : m_children[1])
		{
			for (const auto& attr : node->m_children[1])
			{
				if (attr->m_name == node1)
					node1_id = static_cast<mvNodeAttribute*>(attr.get())->getId();
				if (attr->m_name == node2)
					node2_id = static_cast<mvNodeAttribute*>(attr.get())->getId();
			}
		}

		if (node1_id == 0 || node2_id == 0)
			return;

		deleteLink(node1_id, node2_id);
		std::vector<std::pair<std::string, std::string>> oldLinkStrings = m_linksStrings;
		m_linksStrings.clear();
		for (auto& link_string : oldLinkStrings)
		{
			if (link_string.first == node1 && link_string.second == node2)
				continue;
			m_linksStrings.push_back(link_string);
		}

		if (m_delinkCallback)
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
			PyObject* link = PyTuple_New(2);
			PyTuple_SetItem(link, 0, ToPyString(node1));
			PyTuple_SetItem(link, 1, ToPyString(node2));
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_delinkCallback, m_name, link);
				});

	}

	void mvNodeEditor::addLink(int node1, int node2)
	{
		bool found = false;
		for (const auto& link : m_links)
		{
			if (link.first == node1 && link.second == node2)
				found = true;
		}
		if (!found)
			m_links.push_back(std::make_pair(node1, node2));
	}

	void mvNodeEditor::deleteLink(int node1, int node2)
	{
		std::vector<std::pair<int, int>> oldLinks = m_links;
		m_links.clear();
		for (auto& link : oldLinks)
		{
			if (link.first == node1 && link.second == node2)
				continue;
			m_links.push_back(link);
		}
	}

	std::vector<std::string> mvNodeEditor::getSelectedNodes() const
	{
		std::vector<std::string> result;
		for (const auto& item : m_selectedNodes)
		{
			for (const auto& child : m_children[1])
			{
			    int i1 = item;
			    int i2 = static_cast<mvNode*>(child.get())->getId();
			    int i3 = i1 + i2;
				//if (static_cast<mvNode*>(child.get())->getId() == item)
				if (i1 == i2)
					result.push_back(child->m_name);
			}
		}

		return result;
	}

	void mvNodeEditor::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		imnodes::StyleColorsClassic();
		mvImNodesThemeScope scope(this);
		mvFontScope fscope(this);

		imnodes::PushAttributeFlag(imnodes::AttributeFlags_EnableLinkDetachWithDragClick);

		imnodes::IO& io = imnodes::GetIO();
		io.link_detach_with_modifier_click.modifier = &ImGui::GetIO().KeyCtrl;

		imnodes::BeginNodeEditor();

		if (m_clearLinks)
		{
			imnodes::ClearLinkSelection();
			m_clearLinks = false;
		}

		if (m_clearNodes)
		{
			imnodes::ClearNodeSelection();
			m_clearNodes = false;
		}

		// build links
		for(int i = 0; i<m_links.size(); i++)
			imnodes::Link(i, m_links[i].first, m_links[i].second);

		//we do this so that the children dont get the theme
		//scope.cleanup();

		for (auto item : m_children[1])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;

			// set item width
			if (item->m_width != 0)
				ImGui::SetNextItemWidth((float)item->m_width);

			item->draw(drawlist, x, y);

			item->getState().update();
		}

		imnodes::EndNodeEditor();
		imnodes::PopAttributeFlag();
		

		static int hovered_node_id;
		for (auto& child : m_children[1])
		{
			child->getState().setHovered(false);

			ImVec2 size = imnodes::GetNodeDimensions(static_cast<mvNode*>(child.get())->getId());
			child->getState().setRectSize({ size.x, size.y });
			child->getState().setRectMin({ size.x, size.y });
			child->getState().setRectMax({ size.x, size.y });
		}

		if (imnodes::IsNodeHovered(&hovered_node_id))
		{
			for (auto& child : m_children[1])
			{
				if (static_cast<mvNode*>(child.get())->getId() == hovered_node_id)
					child->getState().setHovered(true);
			}
		}
		
		m_selectedNodes.clear();
		if (imnodes::NumSelectedNodes() > 0)
		{
			int* selected_nodes = new int[imnodes::NumSelectedNodes()];
			imnodes::GetSelectedNodes(selected_nodes);

			for (int i = 0; i < imnodes::NumSelectedNodes(); i++)
				m_selectedNodes.push_back(selected_nodes[i]);
			delete[] selected_nodes;
		}

		m_selectedLinks.clear();
		if (imnodes::NumSelectedLinks() > 0)
		{
			int* selected_links = new int[imnodes::NumSelectedLinks()];
			imnodes::GetSelectedLinks(selected_links);

			for (int i = 0; i < imnodes::NumSelectedLinks(); i++)
				m_selectedLinks.push_back(selected_links[i]);
			delete[] selected_links;
		}

		static int start_attr, end_attr;
		if (imnodes::IsLinkCreated(&start_attr, &end_attr))
		{
			std::string node1, node2;
			for (const auto& child : m_children[1])
			{
				for (const auto& grandchild : child->m_children[1])
				{
					if (static_cast<mvNodeAttribute*>(grandchild.get())->getId()== start_attr)
						node1 = grandchild->m_name;

					if (static_cast<mvNodeAttribute*>(grandchild.get())->getId() == end_attr)
						node2 = grandchild->m_name;
				}
			}
			addLink(node1, node2);
		}

		static int destroyed_attr;
		if (imnodes::IsLinkDestroyed(&destroyed_attr))
		{
			auto item = m_linksStrings[destroyed_attr];
			deleteLink(item.first, item.second);
		}

		m_state.setHovered(imnodes::IsEditorHovered());

		
	}

	PyObject* mvNodeEditor::add_node_link(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;
		const char* node_1;
		const char* node_2;


		if (!(mvApp::GetApp()->getParsers())["add_node_link"].parse(args, kwargs, __FUNCTION__, &node_editor,
			&node_1, &node_2))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor.get());

		editor->addLink(node_1, node_2);

		return GetPyNone();

	}

	PyObject* mvNodeEditor::delete_node_link(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;
		const char* node_1;
		const char* node_2;


		if (!(mvApp::GetApp()->getParsers())["delete_node_link"].parse(args, kwargs, __FUNCTION__, &node_editor,
			&node_1, &node_2))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor.get());

		editor->deleteLink(node_1, node_2);

		return GetPyNone();

	}

	PyObject* mvNodeEditor::get_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;


		if (!(mvApp::GetApp()->getParsers())["get_selected_nodes"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor.get());

		auto selected_nodes = editor->getSelectedNodes();

		return ToPyList(selected_nodes);

	}

	PyObject* mvNodeEditor::get_selected_links(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;

		if (!(mvApp::GetApp()->getParsers())["get_selected_links"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor.get());

		auto& selected_links = editor->getSelectedLinks();
		auto& links = editor->getLinks();

		std::vector<std::pair<std::string, std::string>> selections;
		for (auto& link : selected_links)
			selections.push_back(links[link]);

		return ToPyList(selections);

	}

	PyObject* mvNodeEditor::get_links(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;

		if (!(mvApp::GetApp()->getParsers())["get_links"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor.get());

		auto& links = editor->getLinks();

		return ToPyList(links);

	}

	PyObject* mvNodeEditor::clear_selected_links(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;

		if (!(mvApp::GetApp()->getParsers())["clear_selected_links"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor.get());
		editor->clearLinks();

		return GetPyNone();

	}

	PyObject* mvNodeEditor::clear_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;

		if (!(mvApp::GetApp()->getParsers())["clear_selected_nodes"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor.get());
		editor->clearNodes();

		return GetPyNone();

	}

}
