#include "mvNodeEditor.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvNode.h"
#include "mvNodeAttribute.h"
#include "mvNodeLink.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvNodeEditor::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::UUID, "Adds a node editor.", { "Node Editor", "Containers", "Widgets" }, true);
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_PARENT |
				MV_PARSER_ARG_BEFORE |
				MV_PARSER_ARG_CALLBACK |
				MV_PARSER_ARG_FILTER |
				MV_PARSER_ARG_DROP_CALLBACK |
				MV_PARSER_ARG_DRAG_CALLBACK |
				MV_PARSER_ARG_PAYLOAD_TYPE |
				MV_PARSER_ARG_SEARCH_DELAY |
				MV_PARSER_ARG_WIDTH |
				MV_PARSER_ARG_HEIGHT |
				MV_PARSER_ARG_TRACKED |
				MV_PARSER_ARG_SHOW)
			);

			parser.addArg<mvPyDataType::Callable>("delink_callback", mvArgType::KEYWORD_ARG, "None", "Callback ran when a link is detached.");
			parser.addArg<mvPyDataType::Bool>("menubar", mvArgType::KEYWORD_ARG, "False", "Shows or hides the menubar.");

			parser.finalize();

			parsers->insert({ s_command, parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUIDList, "Undocumented", { "Node Editor" });
			parser.addArg<mvPyDataType::UUID>("node_editor");
			parser.finalize();
			parsers->insert({ "get_selected_nodes", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::ListStrList, "Undocumented", { "Node Editor" });
			parser.addArg<mvPyDataType::UUID>("node_editor");
			parser.finalize();
			parsers->insert({ "get_selected_links", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Node Editor" });
			parser.addArg<mvPyDataType::UUID>("node_editor");
			parser.finalize();
			parsers->insert({ "clear_selected_links", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Node Editor" });
			parser.addArg<mvPyDataType::UUID>("node_editor");
			parser.finalize();
			parsers->insert({ "clear_selected_nodes", parser });
		}

	}

	mvNodeEditor::mvNodeEditor(mvUUID uuid)
		: mvAppItem(uuid)
	{
		_context = imnodes::EditorContextCreate();
	}

	mvNodeEditor::~mvNodeEditor()
	{
		imnodes::EditorContextFree(_context);
	}

	void mvNodeEditor::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "delink_callback"))
		{

			if (_delinkCallback)
				Py_XDECREF(_delinkCallback);
			item = SanitizeCallback(item);
			if (item)
				Py_XINCREF(item);
			_delinkCallback = item;
		}

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("menubar", ImGuiWindowFlags_MenuBar, _windowflags);
	}

	void mvNodeEditor::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (_delinkCallback)
		{
			Py_XINCREF(_delinkCallback);
			PyDict_SetItemString(dict, "delink_callback", _delinkCallback);
		}


		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
		};

		// window flags
		checkbitset("menubar", ImGuiWindowFlags_MenuBar, _windowflags);
	}

	void mvNodeEditor::onChildRemoved(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvNode)
		{

			for (const auto& otherchild : item->getChildren(1))
			{
				int attr_id = static_cast<mvNodeAttribute*>(otherchild.get())->getId();

				for (const auto& child : _children[0])
				{
					if (child->getType() == mvAppItemType::mvNodeLink)
					{

						int i1 = static_cast<mvNodeLink*>(child.get())->getId1();
						int i2 = static_cast<mvNodeLink*>(child.get())->getId2();
						if (i1 == attr_id || i2 == attr_id)
						{
							mvApp::GetApp()->getItemRegistry().deleteItem(child->getUUID());
							mvApp::GetApp()->getItemRegistry().cleanUpItem(child->getUUID());
						}	
					}
				}
			}
		}
	}

	std::vector<mvUUID> mvNodeEditor::getSelectedNodes() const
	{
		std::vector<mvUUID> result;
		for (const auto& item : _selectedNodes)
		{
			for (const auto& child : _children[1])
			{
			    int i1 = item;
			    int i2 = static_cast<mvNode*>(child.get())->getId();
				//if (static_cast<mvNode*>(child.get())->getId() == item)
				if (i1 == i2)
					result.push_back(child->getUUID());
			}
		}

		return result;
	}

	std::vector<mvUUID> mvNodeEditor::getSelectedLinks() const
	{
		std::vector<mvUUID> result;
		for (const auto& item : _selectedLinks)
		{
			for (const auto& child : _children[0])
			{
				if (child->getType() == mvAppItemType::mvNodeLink)
				{
					int i1 = item;
					int i2 = static_cast<mvNodeLink*>(child.get())->getId();
					if (i1 == i2)
						result.push_back(child->getUUID());
				}
			}
		}

		return result;
	}

	void mvNodeEditor::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);
		imnodes::EditorContextSet(_context);

		ImGui::BeginChild(_internalLabel.c_str(), ImVec2((float)_width, (float)_height), false, _windowflags);

		for (auto& item : _children[1])
		{
			// skip nodes
			if (item->getType() != mvAppItemType::mvMenuBar)
				continue;

			// set item width
			if (item->getWidth() != 0)
				ImGui::SetNextItemWidth((float)item->getWidth());

			item->draw(drawlist, x, y);

		}

		imnodes::PushAttributeFlag(imnodes::AttributeFlags_EnableLinkDetachWithDragClick);

		imnodes::IO& io = imnodes::GetIO();
		io.link_detach_with_modifier_click.modifier = &ImGui::GetIO().KeyCtrl;

		imnodes::BeginNodeEditor();

		if (_clearLinks)
		{
			imnodes::ClearLinkSelection();
			_clearLinks = false;
		}

		if (_clearNodes)
		{
			imnodes::ClearNodeSelection();
			_clearNodes = false;
		}

		// build links
		for (auto& item : _children[0])
			item->draw(drawlist, x, y);

		// draw nodes
		for (auto& item : _children[1])
		{
			// skip menu bars
			if (item->getType() != mvAppItemType::mvNode)
				continue;

			// set item width
			if (item->getWidth() != 0)
				ImGui::SetNextItemWidth((float)item->getWidth());

			item->draw(drawlist, x, y);
		}

		_state._hovered = imnodes::IsEditorHovered();
		imnodes::EndNodeEditor();
		imnodes::PopAttributeFlag();

		// post draw for links
		for (auto& item : _children[0])
			item->customAction();

		for (auto& child : _children[1])
		{
			child->getState()._hovered = false;

			ImVec2 size = imnodes::GetNodeDimensions(static_cast<mvNode*>(child.get())->getId());
			child->getState()._rectSize = { size.x, size.y };
			child->getState()._rectMin = { size.x, size.y };
			child->getState()._rectMax = { size.x, size.y };
		}
		
		_selectedNodes.clear();
		if (imnodes::NumSelectedNodes() > 0)
		{
			int* selected_nodes = new int[imnodes::NumSelectedNodes()];
			imnodes::GetSelectedNodes(selected_nodes);

			for (int i = 0; i < imnodes::NumSelectedNodes(); i++)
				_selectedNodes.push_back(selected_nodes[i]);
			delete[] selected_nodes;
		}

		_selectedLinks.clear();
		if (imnodes::NumSelectedLinks() > 0)
		{
			int* selected_links = new int[imnodes::NumSelectedLinks()];
			imnodes::GetSelectedLinks(selected_links);

			for (int i = 0; i < imnodes::NumSelectedLinks(); i++)
				_selectedLinks.push_back(selected_links[i]);
			delete[] selected_links;
		}

		static int start_attr, end_attr;
		if (imnodes::IsLinkCreated(&start_attr, &end_attr))
		{
			mvUUID node1, node2;
			for (const auto& child : _children[1])
			{

				// skip menu bars
				if (child->getType() != mvAppItemType::mvNode)
					continue;

				for (const auto& grandchild : child->getChildren(1))
				{
					if (static_cast<mvNodeAttribute*>(grandchild.get())->getId() == start_attr)
						node1 = grandchild->getUUID();

					if (static_cast<mvNodeAttribute*>(grandchild.get())->getId() == end_attr)
						node2 = grandchild->getUUID();
				}
			}

			if (_callback)
			{
				if (_alias.empty())
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
						PyObject* link = PyTuple_New(2);
						PyTuple_SetItem(link, 0, ToPyUUID(node1));
						PyTuple_SetItem(link, 1, ToPyUUID(node2));
						mvApp::GetApp()->getCallbackRegistry().addCallback(_callback, _uuid, link, _user_data);
						});
				else
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
						PyObject* link = PyTuple_New(2);
						PyTuple_SetItem(link, 0, ToPyUUID(node1));
						PyTuple_SetItem(link, 1, ToPyUUID(node2));
						mvApp::GetApp()->getCallbackRegistry().addCallback(_callback, _alias, link, _user_data);
						});
			}
		}

		static int destroyed_attr;
		if (imnodes::IsLinkDestroyed(&destroyed_attr))
		{
			mvUUID name = 0;
			for (auto& item : _children[0])
			{
				if (item->getType() == mvAppItemType::mvNodeLink)
				{
					if (static_cast<const mvNodeLink*>(item.get())->_id0 == destroyed_attr)
					{
						name = item->getUUID();
						break;
					}
				}
			}
			if (_delinkCallback)
			{
				if (_alias.empty())
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
						PyObject* link = ToPyUUID(name);
						mvApp::GetApp()->getCallbackRegistry().addCallback(_delinkCallback, _uuid, link, _user_data);
						});
				else
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
						PyObject* link = ToPyUUID(name);
						mvApp::GetApp()->getCallbackRegistry().addCallback(_delinkCallback, _alias, link, _user_data);
							});
			}
		}

		ImGui::EndChild();

		_state._lastFrameUpdate = mvApp::s_frame;

		if (_handlerRegistry)
			_handlerRegistry->customAction(&_state);
	}

	PyObject* mvNodeEditor::get_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvUUID node_editor;


		if (!(mvApp::GetApp()->getParsers())["get_selected_nodes"].parse(args, kwargs, __FUNCTION__, &node_editor))
			return ToPyBool(false);

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_selected_nodes",
				"Item not found: " + std::to_string(node_editor), nullptr);
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_selected_nodes",
				"Incompatible type. Expected types include: mvNodeEditor", anode_editor);
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor);

		auto selected_nodes = editor->getSelectedNodes();

		return ToPyList(selected_nodes);

	}

	PyObject* mvNodeEditor::get_selected_links(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* node_editor_raw;

		if (!(mvApp::GetApp()->getParsers())["get_selected_links"].parse(args, kwargs, __FUNCTION__, &node_editor_raw))
			return ToPyBool(false);

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID node_editor = mvAppItem::GetIDFromPyObject(node_editor_raw);

		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_selected_links",
				"Item not found: " + std::to_string(node_editor), nullptr);
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_selected_links",
				"Incompatible type. Expected types include: mvNodeEditor", anode_editor);
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor);

		auto selected_links = editor->getSelectedLinks();

		return ToPyList(selected_links);
	}

	PyObject* mvNodeEditor::clear_selected_links(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* node_editor_raw;

		if (!(mvApp::GetApp()->getParsers())["clear_selected_links"].parse(args, kwargs, __FUNCTION__, &node_editor_raw))
			return ToPyBool(false);

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID node_editor = mvAppItem::GetIDFromPyObject(node_editor_raw);

		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "clear_selected_links",
				"Item not found: " + std::to_string(node_editor), nullptr);
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "clear_selected_links",
				"Incompatible type. Expected types include: mvNodeEditor", anode_editor);
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor);
		editor->clearLinks();

		return GetPyNone();

	}

	PyObject* mvNodeEditor::clear_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* node_editor_raw;

		if (!(mvApp::GetApp()->getParsers())["clear_selected_nodes"].parse(args, kwargs, __FUNCTION__, &node_editor_raw))
			return ToPyBool(false);

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID node_editor = mvAppItem::GetIDFromPyObject(node_editor_raw);

		auto anode_editor = mvApp::GetApp()->getItemRegistry().getItem(node_editor);
		if (anode_editor == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "clear_selected_nodes",
				"Item not found: " + std::to_string(node_editor), nullptr);
			return GetPyNone();
		}

		if (anode_editor->getType() != mvAppItemType::mvNodeEditor)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "clear_selected_nodes",
				"Incompatible type. Expected types include: mvNodeEditor", anode_editor);
			return GetPyNone();
		}

		mvNodeEditor* editor = static_cast<mvNodeEditor*>(anode_editor);
		editor->clearNodes();

		return GetPyNone();

	}

}
