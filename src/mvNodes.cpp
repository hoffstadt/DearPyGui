#include "mvNodes.h"
#include <imnodes.h>
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPyUtils.h"
#include "mvItemHandlers.h"
#include "mvThemes.h"
#include "mvContainers.h"
#include "mvFontItems.h"

static std::string FindRenderedTextEnd(const char* text, const char* text_end = nullptr)
{
    int size = 0;

    const char* text_display_end = text;
    if (!text_end)
        text_end = (const char*)-1;

    while (text_display_end < text_end && *text_display_end != '\0' && (text_display_end[0] != '#' || text_display_end[1] != '#'))
    {
        text_display_end++;
        size++;
    }

    char* cvalue = new char[size + 1];
    for (int i = 0; i < size; i++)
        cvalue[i] = text[i];

    cvalue[size] = '\0';

    auto result = std::string(cvalue);
    delete[] cvalue;

    return result;
}

mvNodeEditor::mvNodeEditor(mvUUID uuid)
    : mvAppItem(uuid)
{
    _context = ImNodes::EditorContextCreate();
}

mvNodeEditor::~mvNodeEditor()
{
    ImNodes::EditorContextFree(_context);
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

    if (PyObject* item = PyDict_GetItemString(dict, "minimap")) _minimap = ToBool(item);
    if (PyObject* item = PyDict_GetItemString(dict, "minimap_location")) _minimapLocation = ToInt(item);
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
    else
        PyDict_SetItemString(dict, "delink_callback", GetPyNone());

    // helper to check and set bit
    auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
    {
        PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
    };

    // window flags
    checkbitset("menubar", ImGuiWindowFlags_MenuBar, _windowflags);

    PyDict_SetItemString(dict, "minimap", mvPyObject(ToPyBool(_minimap)));
    PyDict_SetItemString(dict, "minimap_location", mvPyObject(ToPyInt(_minimapLocation)));
}

void mvNodeEditor::onChildRemoved(std::shared_ptr<mvAppItem> item)
{
    if (item->type == mvAppItemType::mvNode)
    {

        for (const auto& otherchild : item->childslots[1])
        {
            int attr_id = static_cast<mvNodeAttribute*>(otherchild.get())->getId();

            for (const auto& child : childslots[0])
            {
                if (child->type == mvAppItemType::mvNodeLink)
                {

                    int i1 = static_cast<mvNodeLink*>(child.get())->getId1();
                    int i2 = static_cast<mvNodeLink*>(child.get())->getId2();
                    if (i1 == attr_id || i2 == attr_id)
                    {
                        DeleteItem(*GContext->itemRegistry, child->uuid);
                        CleanUpItem(*GContext->itemRegistry, child->uuid);
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
        for (const auto& child : childslots[1])
        {
            int i1 = item;
            int i2 = static_cast<mvNode*>(child.get())->getId();
            //if (static_cast<mvNode*>(child.get())->getId() == item)
            if (i1 == i2)
                result.push_back(child->uuid);
        }
    }

    return result;
}

std::vector<mvUUID> mvNodeEditor::getSelectedLinks() const
{
    std::vector<mvUUID> result;
    for (const auto& item : _selectedLinks)
    {
        for (const auto& child : childslots[0])
        {
            if (child->type == mvAppItemType::mvNodeLink)
            {
                int i1 = item;
                int i2 = static_cast<mvNodeLink*>(child.get())->getId();
                if (i1 == i2)
                    result.push_back(child->uuid);
            }
        }
    }

    return result;
}

void mvNodeEditor::draw(ImDrawList* drawlist, float x, float y)
{
    ScopedID id(uuid);
    ImNodes::EditorContextSet(_context);

    bool ret = ImGui::BeginChild(info.internalLabel.c_str(), ImVec2((float)config.width, (float)config.height), ImGuiChildFlags_None, _windowflags);

    for (auto& item : childslots[1])
    {
        // skip nodes
        if (item->type != mvAppItemType::mvMenuBar)
            continue;

        // set item width
        if (item->config.width != 0)
            ImGui::SetNextItemWidth((float)item->config.width);

        item->draw(drawlist, x, y);

    }

    //ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_None);

    ImNodesIO& io = ImNodes::GetIO();
    io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;

    ImNodes::BeginNodeEditor();

    if (_clearLinks)
    {
        ImNodes::ClearLinkSelection();
        _clearLinks = false;
    }

    if (_clearNodes)
    {
        ImNodes::ClearNodeSelection();
        _clearNodes = false;
    }

    // build links
    for (auto& item : childslots[0])
        item->draw(drawlist, x, y);

    // draw nodes
    for (auto& item : childslots[1])
    {
        // skip menu bars
        if (item->type != mvAppItemType::mvNode)
            continue;

        // set item width
        if (item->config.width != 0)
            ImGui::SetNextItemWidth((float)item->config.width);

        item->draw(drawlist, x, y);
    }

    state.lastFrameUpdate = GContext->frame;
    state.hovered = ImNodes::IsEditorHovered();
    state.visible = ret;
    state.rectSize = { ImNodes::mvEditorGetSize().Max.x - ImNodes::mvEditorGetSize().Min.x, ImNodes::mvEditorGetSize().Max.y - ImNodes::mvEditorGetSize().Min.y };
    if (_minimap)
    {
        ImNodes::MiniMap(0.2f, _minimapLocation);
    }
    ImNodes::EndNodeEditor();
    ImNodes::PopAttributeFlag();

    // post draw for links
    for (auto& item : childslots[0])
        item->customAction();

    int nodeHovered = -1;
    int linkHovered = -1;
    int pinHovered = -1;
    int attrActive = -1;

    bool anyNodeHovered = ImNodes::IsNodeHovered(&nodeHovered);
    bool anyLinkHovered = ImNodes::IsLinkHovered(&linkHovered);
    bool anyPinHovered = ImNodes::IsPinHovered(&pinHovered);
    bool anyAttrActive = ImNodes::IsAnyAttributeActive(&attrActive);

    for (auto& child : childslots[0])
    {
        child->state.lastFrameUpdate = GContext->frame;
        child->state.hovered = false;

        if (anyLinkHovered && linkHovered == static_cast<mvNodeLink*>(child.get())->getId())
            child->state.hovered = true;

    }

    for (auto& child : childslots[1])
    {
        child->state.lastFrameUpdate = GContext->frame;
        child->state.hovered = false;

        if (child->config.show)
        {
            // We can only refer to visible nodes here
            ImVec2 size = ImNodes::GetNodeDimensions(static_cast<mvNode*>(child.get())->getId());
            child->state.rectSize = { size.x, size.y };
        }
        else
            child->state.rectSize = { 0.0f, 0.0f };

        if (anyNodeHovered && nodeHovered == static_cast<mvNode*>(child.get())->getId())
            child->state.hovered = true;

        for (auto& grandchild : child->childslots[1])
        {
            grandchild->state.lastFrameUpdate = GContext->frame;
            grandchild->state.hovered = false;

            if (anyPinHovered && pinHovered == static_cast<mvNodeAttribute*>(grandchild.get())->getId())
                grandchild->state.hovered = true;
            if (anyAttrActive && attrActive == static_cast<mvNodeAttribute*>(grandchild.get())->getId())
                grandchild->state.active = true;
        }
    }

    _selectedNodes.clear();
    if (ImNodes::NumSelectedNodes() > 0)
    {
        int* selected_nodes = new int[ImNodes::NumSelectedNodes()];
        ImNodes::GetSelectedNodes(selected_nodes);

        for (int i = 0; i < ImNodes::NumSelectedNodes(); i++)
            _selectedNodes.push_back(selected_nodes[i]);
        delete[] selected_nodes;
    }

    _selectedLinks.clear();
    if (ImNodes::NumSelectedLinks() > 0)
    {
        int* selected_links = new int[ImNodes::NumSelectedLinks()];
        ImNodes::GetSelectedLinks(selected_links);

        for (int i = 0; i < ImNodes::NumSelectedLinks(); i++)
            _selectedLinks.push_back(selected_links[i]);
        delete[] selected_links;
    }

    static int start_attr, end_attr;
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
    {
        mvUUID node1, node2;
        for (const auto& child : childslots[1])
        {

            // skip menu bars
            if (child->type != mvAppItemType::mvNode)
                continue;

            for (const auto& grandchild : child->childslots[1])
            {
                if (static_cast<mvNodeAttribute*>(grandchild.get())->getId() == start_attr)
                    node1 = grandchild->uuid;

                if (static_cast<mvNodeAttribute*>(grandchild.get())->getId() == end_attr)
                    node2 = grandchild->uuid;
            }
        }

        if (config.callback)
        {
            if (config.alias.empty())
                mvSubmitCallback([=]() {
                PyObject* link = PyTuple_New(2);
                PyTuple_SetItem(link, 0, ToPyUUID(node1));
                PyTuple_SetItem(link, 1, ToPyUUID(node2));
                mvAddCallback(config.callback, uuid, link, config.user_data);
                    });
            else
                mvSubmitCallback([=]() {
                PyObject* link = PyTuple_New(2);
                PyTuple_SetItem(link, 0, ToPyUUID(node1));
                PyTuple_SetItem(link, 1, ToPyUUID(node2));
                mvAddCallback(config.callback, config.alias, link, config.user_data);
                    });
        }
    }

    static int destroyed_attr;
    if (ImNodes::IsLinkDestroyed(&destroyed_attr))
    {
        mvUUID name = 0;
        for (auto& item : childslots[0])
        {
            if (item->type == mvAppItemType::mvNodeLink)
            {
                if (static_cast<const mvNodeLink*>(item.get())->_id0 == destroyed_attr)
                {
                    name = item->uuid;
                    break;
                }
            }
        }
        if (_delinkCallback)
        {
            if (config.alias.empty())
                mvSubmitCallback([=]() {
                PyObject* link = ToPyUUID(name);
                mvAddCallback(_delinkCallback, uuid, link, config.user_data);
                    });
            else
                mvSubmitCallback([=]() {
                PyObject* link = ToPyUUID(name);
                mvAddCallback(_delinkCallback, config.alias, link, config.user_data);
                    });
        }
    }

    ImGui::EndChild();

    if (handlerRegistry)
        handlerRegistry->checkEvents(&state);
}

mvNode::mvNode(mvUUID uuid)
    : mvAppItem(uuid)
{
    info.internalLabel = FindRenderedTextEnd(info.internalLabel.c_str()) + std::to_string(uuid);
    config.specifiedLabel = info.internalLabel;
    int64_t address = (int64_t)this;
    int64_t reduced_address = address % 2147483648;
    _id = (int)reduced_address;
}

void mvNode::draw(ImDrawList* drawlist, float x, float y)
{

    //-----------------------------------------------------------------------------
    // pre draw
    //-----------------------------------------------------------------------------

    // show/hide
    if (!config.show)
        return;

    // set item width
    if (config.width != 0)
        ImGui::SetNextItemWidth((float)config.width);

    // indent (for children
    if (config.indent > 0.0f)
        ImGui::Indent(config.indent);

    // push font if a font object is attached
    if (font)
    {
        ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
        ImGui::PushFont(fontptr);
    }

    // themes
    apply_local_theming(this);

    //-----------------------------------------------------------------------------
    // draw
    //-----------------------------------------------------------------------------
    {
        ScopedID id(uuid);

        if (info.dirtyPos)
        {
            ImNodes::SetNodeGridSpacePos((int)_id, state.pos);
            info.dirtyPos = false;
        }

        ImNodes::SetNodeDraggable((int)_id, _draggable);

        ImNodes::BeginNode(_id);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(config.specifiedLabel.c_str());
        ImNodes::EndNodeTitleBar();

        state.lastFrameUpdate = GContext->frame;
        state.leftclicked = ImGui::IsItemClicked();
        state.rightclicked = ImGui::IsItemClicked(1);
        state.middleclicked = ImGui::IsItemClicked(2);
        for (int i = 0; i < state.doubleclicked.size(); i++)
        {
            state.doubleclicked[i] = IsItemDoubleClicked(i);
        }
        state.visible = ImGui::IsItemVisible();

        for (auto& item : childslots[1])
        {
            // skip item if it's not shown
            if (!item->config.show)
                continue;

            // set item width
            if (item->config.width != 0)
                ImGui::SetNextItemWidth((float)item->config.width);

            item->draw(drawlist, x, y);

        }

        ImNodes::EndNode();
    }

    //-----------------------------------------------------------------------------
    // post draw
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // update state
    //   * only update if applicable
    //-----------------------------------------------------------------------------
    ImVec2 pos = ImNodes::GetNodeGridSpacePos((int)_id);

    state.pos = { pos.x , pos.y };
    state.rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
    state.rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
    state.rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };

    // undo indents
    if (config.indent > 0.0f)
        ImGui::Unindent(config.indent);

    // pop font off stack
    if (font)
        ImGui::PopFont();

    // handle popping themes
    cleanup_local_theming(this);

    if (handlerRegistry)
        handlerRegistry->checkEvents(&state);

    // handle drag & drop payloads
    apply_drag_drop(this);
}

void mvNode::handleSpecificKeywordArgs(PyObject* dict)
{
    if (dict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(dict, "draggable")) _draggable = ToBool(item);

}

void mvNode::getSpecificConfiguration(PyObject* dict)
{
    if (dict == nullptr)
        return;
    PyDict_SetItemString(dict, "draggable", mvPyObject(ToPyBool(_draggable)));
}

mvNodeAttribute::mvNodeAttribute(mvUUID uuid)
    : mvAppItem(uuid)
{
    int64_t address = (int64_t)this;
    int64_t reduced_address = address % 2147483648;
    _id = (int)reduced_address;
}

void mvNodeAttribute::draw(ImDrawList* drawlist, float x, float y)
{
    //-----------------------------------------------------------------------------
    // pre draw
    //-----------------------------------------------------------------------------

    // show/hide
    if (!config.show)
        return;

    // set item width
    if (config.width != 0)
        ImGui::SetNextItemWidth((float)config.width);

    // indent (for children
    if (config.indent > 0.0f)
        ImGui::Indent(config.indent);

    // push font if a font object is attached
    if (font)
    {
        ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
        ImGui::PushFont(fontptr);
    }

    // themes
    apply_local_theming(this);

    //-----------------------------------------------------------------------------
    // draw
    //-----------------------------------------------------------------------------
    {
        ScopedID id(uuid);

        if (_attrType == mvNodeAttribute::AttributeType::mvAttr_Static)
            ImNodes::BeginStaticAttribute((int)_id);
        else if (_attrType == mvNodeAttribute::AttributeType::mvAttr_Output)
            ImNodes::BeginOutputAttribute((int)_id, _shape);
        else
            ImNodes::BeginInputAttribute((int)_id, _shape);

        for (auto& item : childslots[1])
        {
            // skip item if it's not shown
            if (!item->config.show)
                continue;

            // set item width
            if (item->config.width != 0)
                ImGui::SetNextItemWidth((float)item->config.width);

            if (item->info.focusNextFrame)
            {
                ImGui::SetKeyboardFocusHere();
                item->info.focusNextFrame = false;
            }

            auto oldCursorPos = ImGui::GetCursorPos();
            if (item->info.dirtyPos)
                ImGui::SetCursorPos(item->state.pos);

            item->state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

            item->draw(drawlist, x, y);

            if (item->info.dirtyPos)
                ImGui::SetCursorPos(oldCursorPos);

            UpdateAppItemState(item->state);
        }

        if (_attrType == mvNodeAttribute::AttributeType::mvAttr_Static)
            ImNodes::EndStaticAttribute();
        else if (_attrType == mvNodeAttribute::AttributeType::mvAttr_Output)
            ImNodes::EndOutputAttribute();
        else
            ImNodes::EndInputAttribute();

    }

    // undo indents
    if (config.indent > 0.0f)
        ImGui::Unindent(config.indent);

    // pop font off stack
    if (font)
        ImGui::PopFont();

    // handle popping themes
    cleanup_local_theming(this);
}

void mvNodeAttribute::handleSpecificKeywordArgs(PyObject* dict)
{
    if (dict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(dict, "category")) _category = ToString(item);
    if (PyObject* item = PyDict_GetItemString(dict, "attribute_type")) _attrType = (mvNodeAttribute::AttributeType)ToUUID(item);
    if (PyObject* item = PyDict_GetItemString(dict, "shape"))
    {
        _shape = (ImNodesPinShape)ToInt(item);
    }
}

void mvNodeAttribute::getSpecificConfiguration(PyObject* dict)
{
    if (dict == nullptr)
        return;

    PyDict_SetItemString(dict, "attribute_type", mvPyObject(ToPyLong((long)_attrType)));
    PyDict_SetItemString(dict, "shape", mvPyObject(ToPyInt((int)_shape)));
    PyDict_SetItemString(dict, "category", mvPyObject(ToPyString(_category)));
}

mvNodeLink::mvNodeLink(mvUUID uuid)
    : mvAppItem(uuid)
{
    int64_t address = (int64_t)this;
    int64_t reduced_address = address % 2147483648;
    _id0 = (int)reduced_address;
}

void mvNodeLink::handleSpecificRequiredArgs(PyObject* dict)
{
    if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
        return;

    mvUUID node1 = GetIDFromPyObject(PyTuple_GetItem(dict, 0));
    {
        mvAppItem* node = GetItem(*GContext->itemRegistry, node1);
        if (node->type == mvAppItemType::mvNodeAttribute)
        {
            _id1uuid = node1;
            _id1 = static_cast<const mvNodeAttribute*>(node)->getId();
        }
        else
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, GetEntityCommand(type),
                "Incompatible type. Expected types include: mvNode", node);
            assert(false);
            return;
        }
    }

    mvUUID node2 = GetIDFromPyObject(PyTuple_GetItem(dict, 1));
    {
        mvAppItem* node = GetItem(*GContext->itemRegistry, node2);
        if (node->type == mvAppItemType::mvNodeAttribute)
        {
            _id2uuid = node2;
            _id2 = static_cast<const mvNodeAttribute*>(node)->getId();
        }
        else
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, GetEntityCommand(type),
                "Incompatible type. Expected types include: mvNode", node);
            assert(false);
            return;
        }
    }

}

void mvNodeLink::customAction(void* data)
{
    if (handlerRegistry)
        handlerRegistry->checkEvents(&state);
}

void mvNodeLink::draw(ImDrawList* drawlist, float x, float y)
{

    //-----------------------------------------------------------------------------
    // pre draw
    //-----------------------------------------------------------------------------

    // show/hide
    if (!config.show)
        return;

    // set item width
    if (config.width != 0)
        ImGui::SetNextItemWidth((float)config.width);

    // themes
    apply_local_theming(this);

    //-----------------------------------------------------------------------------
    // draw
    //-----------------------------------------------------------------------------
    ScopedID id(uuid);

    ImNodes::Link(_id0, _id1, _id2);

    //-----------------------------------------------------------------------------
    // post draw
    //-----------------------------------------------------------------------------
    state.lastFrameUpdate = GContext->frame;

    //-----------------------------------------------------------------------------
    // update state
    //   * only update if applicable
    //-----------------------------------------------------------------------------
    //_state.hovered = ImNodes::IsLinkHovered(&_id);
    state.visible = ImGui::IsItemVisible();
    //_state.active = ImNodes::IsLinkStarted(&_id);
    //_state.deactivated = ImNodes::IsLinkDropped(&_id);

    // handle popping themes
    cleanup_local_theming(this);
}

void mvNodeLink::getSpecificConfiguration(PyObject* dict)
{
    if (dict == nullptr)
        return;

    PyDict_SetItemString(dict, "attr_1", mvPyObject(ToPyUUID(_id1uuid)));
    PyDict_SetItemString(dict, "attr_2", mvPyObject(ToPyUUID(_id2uuid)));
}