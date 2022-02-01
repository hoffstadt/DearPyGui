#include "mvTab.h"
#include "mvTabBar.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/mvItemHandlers.h"

void mvTab::applySpecificTemplate(mvAppItem* item)
{
    auto titem = static_cast<mvTab*>(item);
    if (config.source != 0) _value = titem->_value;
    _disabled_value = titem->_disabled_value;
    _closable = titem->_closable;
    _flags = titem->_flags;
}

void mvTab::addFlag(ImGuiTabItemFlags flag)
{
    _flags |= flag;
}

void mvTab::removeFlag(ImGuiTabItemFlags flag)
{
    _flags &= ~flag;
}

PyObject* mvTab::getPyValue()
{
    return ToPyBool(*_value);
}

void mvTab::setPyValue(PyObject* value)
{
    *_value = ToBool(value);
}

void mvTab::setDataSource(mvUUID dataSource)
{
    if (dataSource == config.source) return;
    config.source = dataSource;

    mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
    if (!item)
    {
        mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
            "Source item not found: " + std::to_string(dataSource), this);
        return;
    }
    if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
    {
        mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
            "Values types do not match: " + std::to_string(dataSource), this);
        return;
    }
    _value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
}

void mvTab::draw(ImDrawList* drawlist, float x, float y)
{


    //-----------------------------------------------------------------------------
    // pre draw
    //-----------------------------------------------------------------------------

    // show/hide
    if (!config.show)
        return;

    // focusing
    if (info.focusNextFrame)
    {
        ImGui::SetKeyboardFocusHere();
        info.focusNextFrame = false;
    }

    // cache old cursor position
    ImVec2 previousCursorPos = ImGui::GetCursorPos();

    // set cursor position if user set
    if (info.dirtyPos)
        ImGui::SetCursorPos(state.pos);

    // update widget's position state
    state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

    // set item width
    if (config.width != 0)
        ImGui::SetNextItemWidth((float)config.width);

    // set indent
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

        // cast parent to mvTabBar
        auto parent = (mvTabBar*)info.parentPtr;

        // check if this is first tab
        if (parent->getSpecificValue() == 0)
        {
            // set mvTabBar value to the first tab name
            parent->setValue(uuid);
            *_value = true;
        }
            
        state.lastFrameUpdate = GContext->frame;
        // create tab item and see if it is selected
        if (ImGui::BeginTabItem(info.internalLabel.c_str(), _closable ? &config.show : nullptr, _flags))
        {

            state.hovered = ImGui::IsItemHovered();
            state.active = ImGui::IsItemActive();
            state.leftclicked = ImGui::IsItemClicked();
            state.rightclicked = ImGui::IsItemClicked(1);
            state.middleclicked = ImGui::IsItemClicked(2);
            state.visible = ImGui::IsItemVisible();
            state.activated = ImGui::IsItemActivated();
            state.deactivated = ImGui::IsItemDeactivated();
            state.rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
            state.rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
            state.rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
            state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
        
            // set other tab's value false
            for (auto& child : parent->childslots[1])
            {
                if (child->type == mvAppItemType::mvTab)
                    *((mvTab*)child.get())->_value = false;
            }

            // set current tab value true
            *_value = true;

            // run call back if it exists
            if (parent->getSpecificValue() != uuid)
            {
                mvSubmitCallback([=]() {
                    if(parent->config.alias.empty())
                        mvAddCallback(parent->getCallback(), parent->uuid, ToPyUUID(uuid), parent->config.user_data);
                    else
                        mvAddCallback(parent->getCallback(), parent->config.alias, ToPyUUID(uuid), parent->config.user_data);
                    });
            }

            parent->setValue(uuid);

            for (auto& item : childslots[1])
                item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

            ImGui::EndTabItem();
        }

        else
        {
            state.hovered = ImGui::IsItemHovered();
            state.active = ImGui::IsItemActive();
            state.leftclicked = ImGui::IsItemClicked();
            state.rightclicked = ImGui::IsItemClicked(1);
            state.middleclicked = ImGui::IsItemClicked(2);
            state.visible = ImGui::IsItemVisible();
            state.activated = ImGui::IsItemActivated();
            state.deactivated = ImGui::IsItemDeactivated();
            state.rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
            state.rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
            state.rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
            state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
        }
    }

    //-----------------------------------------------------------------------------
    // post draw
    //-----------------------------------------------------------------------------

    // set cursor position to cached position
    if (info.dirtyPos)
        ImGui::SetCursorPos(previousCursorPos);

    if (config.indent > 0.0f)
        ImGui::Unindent(config.indent);

    // pop font off stack
    if (font)
        ImGui::PopFont();

    // handle popping themes
    cleanup_local_theming(this);

    if (handlerRegistry)
        handlerRegistry->checkEvents(&state);

    // handle drag & drop if used
    apply_drag_drop(this);
}

void mvTab::handleSpecificKeywordArgs(PyObject* dict)
{
    if (dict == nullptr)
        return;
         
    if (PyObject* item = PyDict_GetItemString(dict, "closable")) _closable = ToBool(item);


    if (PyObject* item = PyDict_GetItemString(dict, "order_mode"))
    {
        long order_mode = (long)ToUUID(item);

        if (order_mode == (long)mvTab::TabOrdering::mvTabOrder_Fixed)
            _flags = ImGuiTabItemFlags_NoReorder;
        else if (order_mode == (long)mvTab::TabOrdering::mvTabOrder_Leading)
            _flags = ImGuiTabItemFlags_Leading;
        else if (order_mode == (long)mvTab::TabOrdering::mvTabOrder_Trailing)
            _flags = ImGuiTabItemFlags_Trailing;
        else
            _flags = ImGuiTabItemFlags_None;
    }

    if (PyObject* item = PyDict_GetItemString(dict, "no_tooltip"))
    {
        bool value = ToBool(item);
        if (value)
            _flags |= ImGuiTabItemFlags_NoTooltip;
        else
            _flags &= ~ImGuiTabItemFlags_NoTooltip;
    }

}

void mvTab::getSpecificConfiguration(PyObject* dict)
{
    if (dict == nullptr)
        return;
         
    PyDict_SetItemString(dict, "closable", mvPyObject(ToPyBool(_closable)));

    // helper to check and set bit
    auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
    {
        PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
    };

    checkbitset("no_tooltip", ImGuiTabItemFlags_NoTooltip, _flags);

    if(_flags & ImGuiTabItemFlags_Leading)
        PyDict_SetItemString(dict, "order_mode", mvPyObject(ToPyLong((long)mvTab::TabOrdering::mvTabOrder_Leading)));
    else if (_flags & ImGuiTabItemFlags_Trailing)
        PyDict_SetItemString(dict, "order_mode", mvPyObject(ToPyLong((long)mvTab::TabOrdering::mvTabOrder_Trailing)));
    else if (_flags & ImGuiTabBarFlags_Reorderable)
        PyDict_SetItemString(dict, "order_mode", mvPyObject(ToPyLong((long)mvTab::TabOrdering::mvTabOrder_Reorderable)));
    else
        PyDict_SetItemString(dict, "order_mode", mvPyObject(ToPyLong((long)mvTab::TabOrdering::mvTabOrder_Fixed)));

}
