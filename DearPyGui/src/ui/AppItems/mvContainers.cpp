#include "mvContainers.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"
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


//-----------------------------------------------------------------------------
// [SECTION] template specifics
//-----------------------------------------------------------------------------

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