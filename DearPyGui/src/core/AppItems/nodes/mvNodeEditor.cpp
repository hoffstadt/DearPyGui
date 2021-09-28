#include "mvNodeEditor.h"
#include <imnodes.h>
#include "mvContext.h"
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
            std::vector<mvPythonDataElement> args;

            AddCommonArgs(args,(CommonParserArgs)(
                MV_PARSER_ARG_ID |
                MV_PARSER_ARG_PARENT |
                MV_PARSER_ARG_BEFORE |
                MV_PARSER_ARG_CALLBACK |
                MV_PARSER_ARG_FILTER |
                MV_PARSER_ARG_SEARCH_DELAY |
                MV_PARSER_ARG_WIDTH |
                MV_PARSER_ARG_HEIGHT |
                MV_PARSER_ARG_TRACKED |
                MV_PARSER_ARG_SHOW)
            );

            args.push_back({ mvPyDataType::Callable, "delink_callback", mvArgType::KEYWORD_ARG, "None", "Callback ran when a link is detached." });
            args.push_back({ mvPyDataType::Bool, "menubar", mvArgType::KEYWORD_ARG, "False", "Shows or hides the menubar." });

            mvPythonParserSetup setup;
            setup.about = "Adds a node editor.";
            setup.category = { "Node Editor", "Containers", "Widgets" };
            setup.returnType = mvPyDataType::UUID;
            setup.createContextManager = true;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ s_command, parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID, "node_editor" });

            mvPythonParserSetup setup;
            setup.category = { "Node Editor", "App Item Operations" };
            setup.returnType = mvPyDataType::UUIDList;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_selected_nodes", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID, "node_editor" });

            mvPythonParserSetup setup;
            setup.category = { "Node Editor", "App Item Operations" };
            setup.returnType = mvPyDataType::ListStrList;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_selected_links", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID, "node_editor" });

            mvPythonParserSetup setup;
            setup.category = { "Node Editor", "App Item Operations" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "clear_selected_links", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID, "node_editor" });

            mvPythonParserSetup setup;
            setup.category = { "Node Editor", "App Item Operations" };

            mvPythonParser parser = FinalizeParser(setup, args);
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

            for (const auto& otherchild : item->_children[1])
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
                            DeleteItem(*GContext->itemRegistry, child->_uuid);
                            CleanUpItem(*GContext->itemRegistry, child->_uuid);
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
                    result.push_back(child->_uuid);
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
                        result.push_back(child->_uuid);
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
            if (item->_width != 0)
                ImGui::SetNextItemWidth((float)item->_width);

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
            if (item->_width != 0)
                ImGui::SetNextItemWidth((float)item->_width);

            item->draw(drawlist, x, y);
        }

        _state.lastFrameUpdate = GContext->frame;
        _state.hovered = imnodes::IsEditorHovered();
        imnodes::EndNodeEditor();
        imnodes::PopAttributeFlag();

        // post draw for links
        for (auto& item : _children[0])
            item->customAction();

        for (auto& child : _children[1])
        {
            child->_state.hovered = false;

            ImVec2 size = imnodes::GetNodeDimensions(static_cast<mvNode*>(child.get())->getId());
            child->_state.rectSize = { size.x, size.y };
            child->_state.rectMin = { size.x, size.y };
            child->_state.rectMax = { size.x, size.y };
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

                for (const auto& grandchild : child->_children[1])
                {
                    if (static_cast<mvNodeAttribute*>(grandchild.get())->getId() == start_attr)
                        node1 = grandchild->_uuid;

                    if (static_cast<mvNodeAttribute*>(grandchild.get())->getId() == end_attr)
                        node2 = grandchild->_uuid;
                }
            }

            if (_callback)
            {
                if (_alias.empty())
                    GContext->callbackRegistry->submitCallback([=]() {
                        PyObject* link = PyTuple_New(2);
                        PyTuple_SetItem(link, 0, ToPyUUID(node1));
                        PyTuple_SetItem(link, 1, ToPyUUID(node2));
                        GContext->callbackRegistry->addCallback(_callback, _uuid, link, _user_data);
                        });
                else
                    GContext->callbackRegistry->submitCallback([=]() {
                        PyObject* link = PyTuple_New(2);
                        PyTuple_SetItem(link, 0, ToPyUUID(node1));
                        PyTuple_SetItem(link, 1, ToPyUUID(node2));
                        GContext->callbackRegistry->addCallback(_callback, _alias, link, _user_data);
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
                        name = item->_uuid;
                        break;
                    }
                }
            }
            if (_delinkCallback)
            {
                if (_alias.empty())
                    GContext->callbackRegistry->submitCallback([=]() {
                        PyObject* link = ToPyUUID(name);
                        GContext->callbackRegistry->addCallback(_delinkCallback, _uuid, link, _user_data);
                        });
                else
                    GContext->callbackRegistry->submitCallback([=]() {
                        PyObject* link = ToPyUUID(name);
                        GContext->callbackRegistry->addCallback(_delinkCallback, _alias, link, _user_data);
                            });
            }
        }

        ImGui::EndChild();

        _state.lastFrameUpdate = GContext->frame;

        if (_handlerRegistry)
            _handlerRegistry->customAction(&_state);
    }

    PyObject* mvNodeEditor::get_selected_nodes(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        
        PyObject* node_editor_raw;
        
        if (!Parse((GetParsers())["get_selected_nodes"], args, kwargs, __FUNCTION__, &node_editor_raw))
            return ToPyBool(false);

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID node_editor = GetIDFromPyObject(node_editor_raw);
        
        auto anode_editor = GetItem(*GContext->itemRegistry, node_editor);
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

        if (!Parse((GetParsers())["get_selected_links"], args, kwargs, __FUNCTION__, &node_editor_raw))
            return ToPyBool(false);

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID node_editor = GetIDFromPyObject(node_editor_raw);

        auto anode_editor = GetItem(*GContext->itemRegistry, node_editor);
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

        if (!Parse((GetParsers())["clear_selected_links"], args, kwargs, __FUNCTION__, &node_editor_raw))
            return ToPyBool(false);

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID node_editor = GetIDFromPyObject(node_editor_raw);

        auto anode_editor = GetItem(*GContext->itemRegistry, node_editor);
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

        if (!Parse((GetParsers())["clear_selected_nodes"], args, kwargs, __FUNCTION__, &node_editor_raw))
            return ToPyBool(false);

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID node_editor = GetIDFromPyObject(node_editor_raw);

        auto anode_editor = GetItem(*GContext->itemRegistry, node_editor);
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
