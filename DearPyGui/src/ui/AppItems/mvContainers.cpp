#include "mvContainers.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/containers/mvTabBar.h"
#include "mvPyObject.h"
#include "AppItems/mvItemHandlers.h"
#include "mvPythonExceptions.h"
#include <misc/cpp/imgui_stdlib.h>
#include "mvTextureItems.h"

//-----------------------------------------------------------------------------
// [SECTION] get_item_configuration(...) specifics
//-----------------------------------------------------------------------------

void
DearPyGui::fill_configuration_dict(const mvMenuConfig& inConfig, PyObject* outDict, mvAppItem& item)
{
    if (outDict == nullptr)
        return;

    PyDict_SetItemString(outDict, "enabled", mvPyObject(ToPyBool(item.config.enabled)));
}

void
DearPyGui::fill_configuration_dict(const mvTabConfig& inConfig, PyObject* outDict, mvAppItem& item)
{
    if (outDict == nullptr)
        return;

    PyDict_SetItemString(outDict, "closable", mvPyObject(ToPyBool(inConfig.closable)));

    // helper to check and set bit
    auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
    {
        PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
    };

    checkbitset("no_tooltip", ImGuiTabItemFlags_NoTooltip, inConfig._flags);

    if (inConfig._flags & ImGuiTabItemFlags_Leading)
        PyDict_SetItemString(outDict, "order_mode", mvPyObject(ToPyLong((long)TabOrdering::mvTabOrder_Leading)));
    else if (inConfig._flags & ImGuiTabItemFlags_Trailing)
        PyDict_SetItemString(outDict, "order_mode", mvPyObject(ToPyLong((long)TabOrdering::mvTabOrder_Trailing)));
    else if (inConfig._flags & ImGuiTabBarFlags_Reorderable)
        PyDict_SetItemString(outDict, "order_mode", mvPyObject(ToPyLong((long)TabOrdering::mvTabOrder_Reorderable)));
    else
        PyDict_SetItemString(outDict, "order_mode", mvPyObject(ToPyLong((long)TabOrdering::mvTabOrder_Fixed)));

}

//-----------------------------------------------------------------------------
// [SECTION] configure_item(...) specifics
//-----------------------------------------------------------------------------
void
DearPyGui::set_configuration(PyObject* inDict, mvMenuConfig& outConfig, mvAppItem& itemc)
{
    if (inDict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(inDict, "enabled")) itemc.config.enabled = ToBool(item);

}

void
DearPyGui::set_configuration(PyObject* inDict, mvTabConfig& outConfig, mvAppItem& itemc)
{
    if (inDict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(inDict, "closable")) outConfig.closable = ToBool(item);


    if (PyObject* item = PyDict_GetItemString(inDict, "order_mode"))
    {
        long order_mode = (long)ToUUID(item);

        if (order_mode == (long)TabOrdering::mvTabOrder_Fixed)
            outConfig._flags = ImGuiTabItemFlags_NoReorder;
        else if (order_mode == (long)TabOrdering::mvTabOrder_Leading)
            outConfig._flags = ImGuiTabItemFlags_Leading;
        else if (order_mode == (long)TabOrdering::mvTabOrder_Trailing)
            outConfig._flags = ImGuiTabItemFlags_Trailing;
        else
            outConfig._flags = ImGuiTabItemFlags_None;
    }

    if (PyObject* item = PyDict_GetItemString(inDict, "no_tooltip"))
    {
        bool value = ToBool(item);
        if (value)
            outConfig._flags |= ImGuiTabItemFlags_NoTooltip;
        else
            outConfig._flags &= ~ImGuiTabItemFlags_NoTooltip;
    }
}

//-----------------------------------------------------------------------------
// [SECTION] positional args specifics
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// [SECTION] data sources
//-----------------------------------------------------------------------------
void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvMenuConfig& outConfig)
{
    if (dataSource == item.config.source) return;
    item.config.source = dataSource;

    mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
    if (!srcItem)
    {
        mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                           "Source item not found: " + std::to_string(dataSource), &item);
        return;
    }
    if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
    {
        mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
                           "Values types do not match: " + std::to_string(dataSource), &item);
        return;
    }
    outConfig.value = *static_cast<std::shared_ptr<bool>*>(srcItem->getValue());
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvTabConfig& outConfig)
{
    if (dataSource == item.config.source) return;
    item.config.source = dataSource;

    mvAppItem* srcItem = GetItem((*GContext->itemRegistry), dataSource);
    if (!srcItem)
    {
        mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
            "Source item not found: " + std::to_string(dataSource), &item);
        return;
    }
    if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(item.type))
    {
        mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
            "Values types do not match: " + std::to_string(dataSource), &item);
        return;
    }
    outConfig.value = *static_cast<std::shared_ptr<bool>*>(srcItem->getValue());
}



//-----------------------------------------------------------------------------
// [SECTION] template specifics
//-----------------------------------------------------------------------------

void
DearPyGui::apply_template(const mvTabConfig& sourceConfig, mvTabConfig& dstConfig)
{
    dstConfig.value = sourceConfig.value;
    dstConfig._disabled_value = sourceConfig._disabled_value;
    dstConfig.closable = sourceConfig.closable;
    dstConfig._flags = sourceConfig._flags;
}


//-----------------------------------------------------------------------------
// [SECTION] draw commands
//-----------------------------------------------------------------------------
void
DearPyGui::draw_menu(ImDrawList* drawlist, mvAppItem& item, mvMenuConfig& config)
{
    //-----------------------------------------------------------------------------
    // pre draw
    //-----------------------------------------------------------------------------

    // show/hide
    if (!item.config.show)
    return;

    // focusing
    if (item.info.focusNextFrame)
    {
    ImGui::SetKeyboardFocusHere();
    item.info.focusNextFrame = false;
    }

    // cache old cursor position
    ImVec2 previousCursorPos = ImGui::GetCursorPos();

    // set cursor position if user set
    if (item.info.dirtyPos)
    ImGui::SetCursorPos(item.state.pos);

    // update widget's position state
    item.state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

    // set item width
    if (item.config.width != 0)
    ImGui::SetNextItemWidth((float)item.config.width);

    // set indent
    if (item.config.indent > 0.0f)
    ImGui::Indent(item.config.indent);

    // push font if a font object is attached
    if (item.font)
    {
    ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
    ImGui::PushFont(fontptr);
    }

    // themes
    apply_local_theming(&item);

    //-----------------------------------------------------------------------------
    // draw
    //-----------------------------------------------------------------------------
    {
        ScopedID id(item.uuid);

        // create menu and see if its selected
        if (ImGui::BeginMenu(item.info.internalLabel.c_str(), item.config.enabled))
        {
            item.state.lastFrameUpdate = GContext->frame;
            item.state.active = ImGui::IsItemActive();
            item.state.activated = ImGui::IsItemActivated();
            item.state.deactivated = ImGui::IsItemDeactivated();
            item.state.focused = ImGui::IsWindowFocused();
            item.state.hovered = ImGui::IsWindowHovered();
            item.state.rectSize = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };
            item.state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        // set other menus's value false on same level
        for (auto& sibling : item.info.parentPtr->childslots[1])
        {
        // ensure sibling
        if (sibling->type == mvAppItemType::mvMenu)
        *((mvMenu*)sibling.get())->configData.value = false;
        }

        // set current menu value true
        *config.value = true;

        for (auto& child : item.childslots[1])
            child->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
        {

            // update mouse
            ImVec2 mousePos = ImGui::GetMousePos();
            float x = mousePos.x - ImGui::GetWindowPos().x;
            float y = mousePos.y - ImGui::GetWindowPos().y;
            GContext->input.mousePos.x = (int)x;
            GContext->input.mousePos.y = (int)y;


            if (GContext->itemRegistry->activeWindow != item.uuid)
            GContext->itemRegistry->activeWindow = item.uuid;

        }

        ImGui::EndMenu();
        }
        }

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------

        // set cursor position to cached position
        if (item.info.dirtyPos)
        ImGui::SetCursorPos(previousCursorPos);

        if (item.config.indent > 0.0f)
        ImGui::Unindent(item.config.indent);

        // pop font off stack
        if (item.font)
            ImGui::PopFont();

        // handle popping themes
        cleanup_local_theming(&item);

        if (item.handlerRegistry)
            item.handlerRegistry->checkEvents(&item.state);

        // handle drag & drop if used
        apply_drag_drop(&item);
}

void
DearPyGui::draw_tab(ImDrawList* drawlist, mvAppItem& item, mvTabConfig& config)
{
    //-----------------------------------------------------------------------------
    // pre draw
    //-----------------------------------------------------------------------------

    // show/hide
    if (!item.config.show)
        return;

    // focusing
    if (item.info.focusNextFrame)
    {
        ImGui::SetKeyboardFocusHere();
        item.info.focusNextFrame = false;
    }

    // cache old cursor position
    ImVec2 previousCursorPos = ImGui::GetCursorPos();

    // set cursor position if user set
    if (item.info.dirtyPos)
        ImGui::SetCursorPos(item.state.pos);

    // update widget's position state
    item.state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

    // set item width
    if (item.config.width != 0)
        ImGui::SetNextItemWidth((float)item.config.width);

    // set indent
    if (item.config.indent > 0.0f)
        ImGui::Indent(item.config.indent);

    // push font if a font object is attached
    if (item.font)
    {
        ImFont* fontptr = static_cast<mvFont*>(item.font.get())->getFontPtr();
        ImGui::PushFont(fontptr);
    }

    // themes
    apply_local_theming(&item);

    //-----------------------------------------------------------------------------
    // draw
    //-----------------------------------------------------------------------------
    {

        ScopedID id(item.uuid);

        // cast parent to mvTabBar
        auto parent = (mvTabBar*)item.info.parentPtr;

        // check if this is first tab
        if (parent->getSpecificValue() == 0)
        {
            // set mvTabBar value to the first tab name
            parent->setValue(item.uuid);
            *config.value = true;
        }

        item.state.lastFrameUpdate = GContext->frame;
        // create tab item and see if it is selected
        if (ImGui::BeginTabItem(item.info.internalLabel.c_str(), config.closable ? &item.config.show : nullptr, config._flags))
        {

            item.state.hovered = ImGui::IsItemHovered();
            item.state.active = ImGui::IsItemActive();
            item.state.leftclicked = ImGui::IsItemClicked();
            item.state.rightclicked = ImGui::IsItemClicked(1);
            item.state.middleclicked = ImGui::IsItemClicked(2);
            item.state.visible = ImGui::IsItemVisible();
            item.state.activated = ImGui::IsItemActivated();
            item.state.deactivated = ImGui::IsItemDeactivated();
            item.state.rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
            item.state.rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
            item.state.rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
            item.state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

            // set other tab's value false
            for (auto& child : parent->childslots[1])
            {
                if (child->type == mvAppItemType::mvTab)
                    *((mvTab*)child.get())->configData.value = false;
            }

            // set current tab value true
            *config.value = true;

            // run call back if it exists
            if (parent->getSpecificValue() != item.uuid)
            {
                mvSubmitCallback([=, &item]() {
                    if (parent->config.alias.empty())
                        mvAddCallback(parent->getCallback(), parent->uuid, ToPyUUID(item.uuid), parent->config.user_data);
                    else
                        mvAddCallback(parent->getCallback(), parent->config.alias, ToPyUUID(item.uuid), parent->config.user_data);
                    });
            }

            parent->setValue(item.uuid);

            for (auto& item : item.childslots[1])
                item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

            ImGui::EndTabItem();
        }

        else
        {
            item.state.hovered = ImGui::IsItemHovered();
            item.state.active = ImGui::IsItemActive();
            item.state.leftclicked = ImGui::IsItemClicked();
            item.state.rightclicked = ImGui::IsItemClicked(1);
            item.state.middleclicked = ImGui::IsItemClicked(2);
            item.state.visible = ImGui::IsItemVisible();
            item.state.activated = ImGui::IsItemActivated();
            item.state.deactivated = ImGui::IsItemDeactivated();
            item.state.rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
            item.state.rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
            item.state.rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
            item.state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
        }
    }

    //-----------------------------------------------------------------------------
    // post draw
    //-----------------------------------------------------------------------------

    // set cursor position to cached position
    if (item.info.dirtyPos)
        ImGui::SetCursorPos(previousCursorPos);

    if (item.config.indent > 0.0f)
        ImGui::Unindent(item.config.indent);

    // pop font off stack
    if (item.font)
        ImGui::PopFont();

    // handle popping themes
    cleanup_local_theming(&item);

    if (item.handlerRegistry)
        item.handlerRegistry->checkEvents(&item.state);

    // handle drag & drop if used
    apply_drag_drop(&item);
}