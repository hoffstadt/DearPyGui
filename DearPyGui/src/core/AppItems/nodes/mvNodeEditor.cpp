#include "mvNodeEditor.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvNode.h"
#include "mvNodeAttribute.h"

namespace Marvel {

	void mvNodeEditor::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_node_editor", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Callable , "link_callback", "Callback ran when a new link is created.", "None"},
			{mvPythonDataType::Callable , "delink_callback", "Callback ran when a link is detached.", "None"},

		}, "Adds a node editor.",
		"None", "Containers") });

		parsers->insert({ "add_node_link", mvPythonParser({
			{mvPythonDataType::String, "node_editor"},
			{mvPythonDataType::String, "node_1"},
			{mvPythonDataType::String, "node_2"},
		}, "Adds a node link between nodes.",
		"None", "Containers") });

		parsers->insert({ "delete_node_link", mvPythonParser({
			{mvPythonDataType::String, "node_editor"},
			{mvPythonDataType::String, "node_1"},
			{mvPythonDataType::String, "node_2"},
		}, "Deletes a node link if it exist.",
		"None", "Containers") });

		parsers->insert({ "get_selected_nodes", mvPythonParser({
			{mvPythonDataType::String, "node_editor"}
		}, "Returns selected nodes.",
		"List[str]", "Containers") });

		parsers->insert({ "get_selected_links", mvPythonParser({
			{mvPythonDataType::String, "node_editor"}
		}, "Returns selected links.",
		"List[List[str]]", "Containers") });

		parsers->insert({ "get_links", mvPythonParser({
			{mvPythonDataType::String, "node_editor"}
		}, "Returns all links. ",
		"List[List[str]]", "Containers") });

		parsers->insert({ "clear_selected_links", mvPythonParser({
			{mvPythonDataType::String, "node_editor"}
		}, "Clears selected links.",
		"None", "Containers") });

		parsers->insert({ "clear_selected_nodes", mvPythonParser({
			{mvPythonDataType::String, "node_editor"}
		}, "Clears selected nodes.",
		"None", "Containers") });
	}

	mvNodeEditor::mvNodeEditor(const std::string& name, mvCallable linkCallback, mvCallable delinkCallback)
		: mvAppItem(name), m_linkCallback(linkCallback), m_delinkCallback(delinkCallback)
	{
		m_description.container = true;
	}

	void mvNodeEditor::addLink(const std::string& node1, const std::string& node2)
	{
		int64_t node1_id = 0;
        int64_t node2_id = 0;

		for (const auto& node : m_children)
		{
			for (const auto& attr : node->m_children)
			{
				if (attr->getCoreConfig().name == node1)
					node1_id = static_cast<mvNodeAttribute*>(attr.get())->getId();
				if (attr->getCoreConfig().name == node2)
					node2_id = static_cast<mvNodeAttribute*>(attr.get())->getId();
			}
		}

		if (node1_id == 0 || node2_id == 0)
			return;

		addLink(node1_id, node2_id);
		m_linksStrings.emplace_back(node1, node2);

		mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
			PyObject* link = PyTuple_New(2);
			PyTuple_SetItem(link, 0, ToPyString(node1));
			PyTuple_SetItem(link, 1, ToPyString(node2));
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_linkCallback, m_core_config.name, link);
			});

	}

	void mvNodeEditor::deleteLink(const std::string& node1, const std::string& node2)
	{
		int node1_id = 0;
		int node2_id = 0;

		for (const auto& node : m_children)
		{
			for (const auto& attr : node->m_children)
			{
				if (attr->getCoreConfig().name == node1)
					node1_id = static_cast<mvNodeAttribute*>(attr.get())->getId();
				if (attr->getCoreConfig().name == node2)
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

		mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
			PyObject* link = PyTuple_New(2);
			PyTuple_SetItem(link, 0, ToPyString(node1));
			PyTuple_SetItem(link, 1, ToPyString(node2));
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_delinkCallback, m_core_config.name, link);
			});

	}

	void mvNodeEditor::addLink(int node1, int node2)
	{
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
			for (const auto& child : m_children)
			{
			    int i1 = item;
			    int i2 = static_cast<mvNode*>(child.get())->getId();
			    int i3 = i1 + i2;
				//if (static_cast<mvNode*>(child.get())->getId() == item)
				if (i1 == i2)
					result.push_back(child->getCoreConfig().name);
			}
		}

		return result;
	}

	void mvNodeEditor::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;
		mvImGuiThemeScope scope(this);

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
		scope.cleanup();

		for (auto item : m_children)
		{
			// skip item if it's not shown
			if (!item->m_core_config.show)
				continue;

			// set item width
			if (item->m_core_config.width != 0)
				ImGui::SetNextItemWidth((float)item->m_core_config.width);

			item->draw();

			item->getState().update();
		}

		imnodes::EndNodeEditor();
		imnodes::PopAttributeFlag();
		

		static int hovered_node_id;
		for (auto& child : m_children)
		{
			child->getState().setHovered(false);

			ImVec2 size = imnodes::GetNodeDimensions(static_cast<mvNode*>(child.get())->getId());
			child->getState().setRectSize({ size.x, size.y });
			child->getState().setRectMin({ size.x, size.y });
			child->getState().setRectMax({ size.x, size.y });
		}

		if (imnodes::IsNodeHovered(&hovered_node_id))
		{
			for (auto& child : m_children)
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
			for (const auto& child : m_children)
			{
				for (const auto& grandchild : child->m_children)
				{
					if (static_cast<mvNodeAttribute*>(grandchild.get())->getId()== start_attr)
						node1 = grandchild->getCoreConfig().name;

					if (static_cast<mvNodeAttribute*>(grandchild.get())->getId() == end_attr)
						node2 = grandchild->getCoreConfig().name;
				}
			}
			addLink(node1, node2);
		}

		static int destroyed_attr;
		if (imnodes::IsLinkDestroyed(&destroyed_attr))
		{
			auto& item = m_linksStrings[destroyed_attr];
			deleteLink(item.first, item.second);
		}

		m_state.setHovered(imnodes::IsEditorHovered());

		
	}

	PyObject* add_node_editor(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int show = true;
		const char* parent = "";
		const char* before = "";
		PyObject* link_callback = nullptr;
		PyObject* delink_callback = nullptr;


		if (!(*mvApp::GetApp()->getParsers())["add_node_editor"].parse(args, kwargs, __FUNCTION__, &name,
			&show, &parent, &before, &link_callback, &delink_callback))
			return ToPyBool(false);

		if (link_callback)
			Py_XINCREF(link_callback);

		if (delink_callback)
			Py_XINCREF(delink_callback);

		auto item = CreateRef<mvNodeEditor>(name, link_callback, delink_callback);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();

		}

		return GetPyNone();

	}

	PyObject* add_node_link(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;
		const char* node_1;
		const char* node_2;


		if (!(*mvApp::GetApp()->getParsers())["add_node_link"].parse(args, kwargs, __FUNCTION__, &node_editor,
			&node_1, &node_2))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::NodeEditor)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor.get());

		editor->addLink(node_1, node_2);

		return GetPyNone();

	}

	PyObject* delete_node_link(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;
		const char* node_1;
		const char* node_2;


		if (!(*mvApp::GetApp()->getParsers())["delete_node_link"].parse(args, kwargs, __FUNCTION__, &node_editor,
			&node_1, &node_2))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::NodeEditor)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor.get());

		editor->deleteLink(node_1, node_2);

		return GetPyNone();

	}

	PyObject* get_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;


		if (!(*mvApp::GetApp()->getParsers())["get_selected_nodes"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::NodeEditor)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor.get());

		auto selected_nodes = editor->getSelectedNodes();

		return ToPyList(selected_nodes);

	}

	PyObject* get_selected_links(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;

		if (!(*mvApp::GetApp()->getParsers())["get_selected_links"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::NodeEditor)
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

	PyObject* get_links(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;

		if (!(*mvApp::GetApp()->getParsers())["get_links"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::NodeEditor)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor.get());

		auto& links = editor->getLinks();

		return ToPyList(links);

	}

	PyObject* clear_selected_links(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;

		if (!(*mvApp::GetApp()->getParsers())["clear_selected_links"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::NodeEditor)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor.get());
		editor->clearLinks();

		return GetPyNone();

	}

	PyObject* clear_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;

		if (!(*mvApp::GetApp()->getParsers())["clear_selected_nodes"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			ThrowPythonException(message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::NodeEditor)
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