#include "mvNodeEditor.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvNode.h"
#include "mvNodeAttribute.h"
#include "mvNodeLink.h"
#include "mvImGuiThemeScope.h"
#include "mvImNodesThemeScope.h"
#include "mvLog.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvNodeEditor::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Node Editor", "Widgets" });
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_PARENT |
				MV_PARSER_ARG_BEFORE |
				MV_PARSER_ARG_CALLBACK |
				MV_PARSER_ARG_FILTER |
				MV_PARSER_ARG_DROP_CALLBACK |
				MV_PARSER_ARG_DRAG_CALLBACK |
				MV_PARSER_ARG_PAYLOAD_TYPE |
				MV_PARSER_ARG_TRACKED |
				MV_PARSER_ARG_SHOW)
			);

			parser.addArg<mvPyDataType::Callable>("delink_callback", mvArgType::KEYWORD_ARG, "None", "Callback ran when a link is detached.");

			parser.finalize();

			parsers->insert({ s_command, parser });
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

	void mvNodeEditor::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "delink_callback"))
		{

			if (m_delinkCallback)
				Py_XDECREF(m_delinkCallback);
			item = SanitizeCallback(item);
			if (item)
				Py_XINCREF(item);
			m_delinkCallback = item;
		}
	}

	bool mvNodeEditor::canChildBeAdded(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if(type ==mvAppItemType::mvNode) return true;
		if(type ==mvAppItemType::mvNodeLink) return true;

		mvThrowPythonError(1006, "Node editor children must be nodes only.");
		MV_ITEM_REGISTRY_ERROR("Node editor children must be nodes only.");
		assert(false);
		return false;
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
		for (auto item : m_children[0])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;

			item->draw(drawlist, x, y);

			item->getState().update();
		}

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

			if (m_callback)
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
				PyObject* link = PyTuple_New(2);
				PyTuple_SetItem(link, 0, ToPyString(node1));
				PyTuple_SetItem(link, 1, ToPyString(node2));
				mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, link, nullptr);
					});
		}

		static int destroyed_attr;
		if (imnodes::IsLinkDestroyed(&destroyed_attr))
		{
			std::string name;
			for (auto& item : m_children[0])
			{
				if (item->getType() == mvAppItemType::mvNodeLink)
				{
					if (static_cast<const mvNodeLink*>(item.get())->m_id == destroyed_attr)
					{
						name = item->m_name;
						break;
					}
				}
			}
			if (m_delinkCallback)
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
				PyObject* link = ToPyString(name);
				mvApp::GetApp()->getCallbackRegistry().addCallback(m_delinkCallback, m_name, link, nullptr);
					});
		}

		m_state.setHovered(imnodes::IsEditorHovered());	
	}

	PyObject* mvNodeEditor::get_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;


		if (!(mvApp::GetApp()->getParsers())["get_selected_nodes"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			mvThrowPythonError(1000, message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			std::string message = node_editor;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor);

		auto selected_nodes = editor->getSelectedNodes();

		return ToPyList(selected_nodes);

	}

	PyObject* mvNodeEditor::get_selected_links(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;

		if (!(mvApp::GetApp()->getParsers())["get_selected_links"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			mvThrowPythonError(1000, message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			std::string message = node_editor;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor);

		auto& selected_links = editor->getSelectedLinks();

		std::vector<std::pair<std::string, std::string>> selections;

		//TODO BROKE

		return ToPyList(selections);

	}

	PyObject* mvNodeEditor::clear_selected_links(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;

		if (!(mvApp::GetApp()->getParsers())["clear_selected_links"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			mvThrowPythonError(1000, message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			std::string message = node_editor;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor);
		editor->clearLinks();

		return GetPyNone();

	}

	PyObject* mvNodeEditor::clear_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* node_editor;

		if (!(mvApp::GetApp()->getParsers())["clear_selected_nodes"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			std::string message = node_editor;
			mvThrowPythonError(1000, message + " node_editor does not exist.");
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			std::string message = node_editor;
			mvThrowPythonError(1000, message + " is not a plot.");
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor);
		editor->clearNodes();

		return GetPyNone();

	}

}
