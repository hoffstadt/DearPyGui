#include "mvContainers.h"
#include "mvViewport.h"
#include "mvFontItems.h"
#include "mvThemes.h"
#include "mvContainers.h"
#include "mvPyUtils.h"
#include "mvItemHandlers.h"
#include <misc/cpp/imgui_stdlib.h>
#include "mvTextureItems.h"
#include <iostream>

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
DearPyGui::fill_configuration_dict(const mvTabConfig& inConfig, PyObject* outDict)
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

void
DearPyGui::fill_configuration_dict(const mvChildWindowConfig& inConfig, PyObject* outDict)
{
    if (outDict == nullptr)
        return;

    PyDict_SetItemString(outDict, "autosize_x", mvPyObject(ToPyBool(inConfig.autosize_x)));
    PyDict_SetItemString(outDict, "autosize_y", mvPyObject(ToPyBool(inConfig.autosize_y)));

    // helper for bit flipping
    auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
    {
        PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
    };

    // window flags
    checkbitset("no_scrollbar", ImGuiWindowFlags_NoScrollbar, inConfig.windowflags);
    checkbitset("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, inConfig.windowflags);
    checkbitset("menubar", ImGuiWindowFlags_MenuBar, inConfig.windowflags);
    checkbitset("no_scroll_with_mouse", ImGuiWindowFlags_NoScrollWithMouse, inConfig.windowflags);
    checkbitset("flattened_navigation", ImGuiWindowFlags_NavFlattened, inConfig.windowflags);

    // child flags
    checkbitset("border", ImGuiChildFlags_Border, inConfig.childFlags);
    checkbitset("always_auto_resize", ImGuiChildFlags_AlwaysAutoResize, inConfig.childFlags);
    checkbitset("always_use_window_padding", ImGuiChildFlags_AlwaysUseWindowPadding, inConfig.childFlags);
    checkbitset("auto_resize_x", ImGuiChildFlags_AutoResizeX, inConfig.childFlags);
    checkbitset("auto_resize_y", ImGuiChildFlags_AutoResizeY, inConfig.childFlags);
    checkbitset("frame_style", ImGuiChildFlags_FrameStyle, inConfig.childFlags);
    checkbitset("resize_x", ImGuiChildFlags_ResizeX, inConfig.childFlags);
    checkbitset("resize_y", ImGuiChildFlags_ResizeY, inConfig.childFlags);
}

void
DearPyGui::fill_configuration_dict(const mvGroupConfig& inConfig, PyObject* outDict)
{
    if (outDict == nullptr)
        return;

    PyDict_SetItemString(outDict, "horizontal", mvPyObject(ToPyBool(inConfig.horizontal)));
    PyDict_SetItemString(outDict, "horizontal_spacing", mvPyObject(ToPyFloat(inConfig.hspacing)));
    PyDict_SetItemString(outDict, "xoffset", mvPyObject(ToPyFloat(inConfig.xoffset)));
}

void
DearPyGui::fill_configuration_dict(const mvDragPayloadConfig& inConfig, PyObject* outDict)
{
    if (outDict == nullptr)
        return;

    if (inConfig.dragData)
    {
        Py_XINCREF(inConfig.dragData);
        PyDict_SetItemString(outDict, "drag_data", inConfig.dragData);
    }
    else
        PyDict_SetItemString(outDict, "drag_data", GetPyNone());

    if (inConfig.dropData)
    {
        Py_XINCREF(inConfig.dropData);
        PyDict_SetItemString(outDict, "drop_data", inConfig.dropData);
    }
    else
        PyDict_SetItemString(outDict, "drop_data", GetPyNone());
}

void
DearPyGui::fill_configuration_dict(const mvTreeNodeConfig& inConfig, PyObject* outDict)
{
    if (outDict == nullptr)
        return;

    PyDict_SetItemString(outDict, "selectable", mvPyObject(ToPyBool(inConfig.selectable)));

    // helper to check and set bit
    auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
    {
        PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
    };

    // flags
    checkbitset("default_open", ImGuiTreeNodeFlags_DefaultOpen, inConfig.flags);
    checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, inConfig.flags);
    checkbitset("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, inConfig.flags);
    checkbitset("leaf", ImGuiTreeNodeFlags_Leaf, inConfig.flags);
    checkbitset("bullet", ImGuiTreeNodeFlags_Bullet, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvTabBarConfig& inConfig, PyObject* outDict)
{
    if (outDict == nullptr)
        return;
    // helper to check and set bit
    auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
    {
        PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
    };

    checkbitset("reorderable", ImGuiTabBarFlags_Reorderable, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvCollapsingHeaderConfig& inConfig, PyObject* outDict)
{
    if (outDict == nullptr)
        return;

    PyDict_SetItemString(outDict, "closable", mvPyObject(ToPyBool(*inConfig.value)));
    PyDict_SetItemString(outDict, "closable", mvPyObject(ToPyBool(inConfig.closable)));

    // helper to check and set bit
    auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
    {
        PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
    };

    // flags
    checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, inConfig.flags);
    checkbitset("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, inConfig.flags);
    checkbitset("leaf", ImGuiTreeNodeFlags_Leaf, inConfig.flags);
    checkbitset("bullet", ImGuiTreeNodeFlags_Bullet, inConfig.flags);
}

void
DearPyGui::fill_configuration_dict(const mvWindowAppItemConfig& inConfig, PyObject* outDict)
{
    if (outDict == nullptr)
        return;

    PyDict_SetItemString(outDict, "no_open_over_existing_popup", mvPyObject(ToPyBool(inConfig.no_open_over_existing_popup)));
    PyDict_SetItemString(outDict, "modal", mvPyObject(ToPyBool(inConfig.modal)));
    PyDict_SetItemString(outDict, "popup", mvPyObject(ToPyBool(inConfig.popup)));
    PyDict_SetItemString(outDict, "no_close", mvPyObject(ToPyBool(inConfig.no_close)));
    PyDict_SetItemString(outDict, "collapsed", mvPyObject(ToPyBool(inConfig.collapsed)));
    PyDict_SetItemString(outDict, "min_size", mvPyObject(ToPyPairII(inConfig.min_size.x, inConfig.min_size.y)));
    PyDict_SetItemString(outDict, "max_size", mvPyObject(ToPyPairII(inConfig.max_size.x, inConfig.max_size.y)));
    if (inConfig.on_close)
    {
        Py_XINCREF(inConfig.on_close);
        PyDict_SetItemString(outDict, "on_close", inConfig.on_close);
    }
    else
        PyDict_SetItemString(outDict, "on_close", GetPyNone());

    // helper to check and set bit
    auto checkbitset = [outDict](const char* keyword, int flag, const int& flags)
    {
        PyDict_SetItemString(outDict, keyword, mvPyObject(ToPyBool(flags & flag)));
    };

        // window flags
    checkbitset("autosize", ImGuiWindowFlags_AlwaysAutoResize, inConfig.windowflags);
    checkbitset("no_resize", ImGuiWindowFlags_NoResize, inConfig.windowflags);
    checkbitset("no_title_bar", ImGuiWindowFlags_NoTitleBar, inConfig.windowflags);
    checkbitset("no_move", ImGuiWindowFlags_NoMove, inConfig.windowflags);
    checkbitset("no_scrollbar", ImGuiWindowFlags_NoScrollbar, inConfig.windowflags);
    checkbitset("no_collapse", ImGuiWindowFlags_NoCollapse, inConfig.windowflags);
    checkbitset("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, inConfig.windowflags);
    checkbitset("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, inConfig.windowflags);
    checkbitset("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, inConfig.windowflags);
    checkbitset("menubar", ImGuiWindowFlags_MenuBar, inConfig.windowflags);
    checkbitset("no_background", ImGuiWindowFlags_NoBackground, inConfig.windowflags);
    checkbitset("no_saved_settings", ImGuiWindowFlags_NoSavedSettings, inConfig.windowflags);
    checkbitset("no_scroll_with_mouse", ImGuiWindowFlags_NoScrollWithMouse, inConfig.windowflags);
    checkbitset("unsaved_document", ImGuiWindowFlags_UnsavedDocument, inConfig.windowflags);
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
DearPyGui::set_configuration(PyObject* inDict, mvTabConfig& outConfig)
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

void
DearPyGui::set_configuration(PyObject* inDict, mvChildWindowConfig& outConfig)
{
    if (inDict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(inDict, "autosize_x")) outConfig.autosize_x = ToBool(item);
    if (PyObject* item = PyDict_GetItemString(inDict, "autosize_y")) outConfig.autosize_y = ToBool(item);

    // helper for bit flipping
    auto flagop = [inDict](const char* keyword, int flag, int& flags)
    {
        if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
    };

    // window flags
    flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, outConfig.windowflags);
    flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, outConfig.windowflags);
    flagop("menubar", ImGuiWindowFlags_MenuBar, outConfig.windowflags);
    flagop("no_scroll_with_mouse", ImGuiWindowFlags_NoScrollWithMouse, outConfig.windowflags);
    flagop("flattened_navigation", ImGuiWindowFlags_NavFlattened, outConfig.windowflags);

    // child flags
    flagop("border", ImGuiChildFlags_Border, outConfig.childFlags);
    flagop("always_auto_resize", ImGuiChildFlags_AlwaysAutoResize, outConfig.childFlags);
    flagop("always_use_window_padding", ImGuiChildFlags_AlwaysUseWindowPadding, outConfig.childFlags);
    flagop("auto_resize_x", ImGuiChildFlags_AutoResizeX, outConfig.childFlags);
    flagop("auto_resize_y", ImGuiChildFlags_AutoResizeY, outConfig.childFlags);
    flagop("frame_style", ImGuiChildFlags_FrameStyle, outConfig.childFlags);
    flagop("resize_x", ImGuiChildFlags_ResizeX, outConfig.childFlags);
    flagop("resize_y", ImGuiChildFlags_ResizeY, outConfig.childFlags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvGroupConfig& outConfig)
{
    if (inDict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(inDict, "horizontal")) outConfig.horizontal = ToBool(item);
    if (PyObject* item = PyDict_GetItemString(inDict, "horizontal_spacing")) outConfig.hspacing = ToFloat(item);
    if (PyObject* item = PyDict_GetItemString(inDict, "xoffset")) outConfig.xoffset = ToFloat(item);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvDragPayloadConfig& outConfig)
{
    if (inDict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(inDict, "payload_type")) outConfig.payloadType = ToString(item);

    if (PyObject* item = PyDict_GetItemString(inDict, "drag_data"))
    {
        if (outConfig.dragData)
            Py_XDECREF(outConfig.dragData);

        Py_XINCREF(item);
        outConfig.dragData = item;
    }

    if (PyObject* item = PyDict_GetItemString(inDict, "drop_data"))
    {
        if (outConfig.dropData)
            Py_XDECREF(outConfig.dropData);

        Py_XINCREF(item);
        outConfig.dropData = item;
    }
}

void
DearPyGui::set_configuration(PyObject* inDict, mvTreeNodeConfig& outConfig)
{
    if (inDict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(inDict, "selectable")) outConfig.selectable = ToBool(item);
    if (PyObject* item = PyDict_GetItemString(inDict, "default_open")) *outConfig.value = ToBool(item);

    // helper for bit flipping
    auto flagop = [inDict](const char* keyword, int flag, int& flags)
    {
        if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
    };

    // flags
    flagop("default_open", ImGuiTreeNodeFlags_DefaultOpen, outConfig.flags);
    flagop("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, outConfig.flags);
    flagop("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, outConfig.flags);
    flagop("leaf", ImGuiTreeNodeFlags_Leaf, outConfig.flags);
    flagop("bullet", ImGuiTreeNodeFlags_Bullet, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvTabBarConfig& outConfig)
{
    if (inDict == nullptr)
        return;
    auto flagop = [inDict](const char* keyword, int flag, int& flags)
    {
        if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
    };
    flagop("reorderable", ImGuiTabBarFlags_Reorderable, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvCollapsingHeaderConfig& outConfig)
{
    if (inDict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(inDict, "closable")) outConfig.closable = ToBool(item);
    if (PyObject* item = PyDict_GetItemString(inDict, "default_open")) *outConfig.value = ToBool(item);

    // helper for bit flipping
    auto flagop = [inDict](const char* keyword, int flag, int& flags)
    {
        if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
    };

    // flags
    flagop("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, outConfig.flags);
    flagop("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, outConfig.flags);
    flagop("leaf", ImGuiTreeNodeFlags_Leaf, outConfig.flags);
    flagop("bullet", ImGuiTreeNodeFlags_Bullet, outConfig.flags);
}

void
DearPyGui::set_configuration(PyObject* inDict, mvAppItem& itemc, mvWindowAppItemConfig& outConfig)
{
    if (inDict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(inDict, "modal"))
    {
        outConfig.modal = ToBool(item);
        itemc.info.shownLastFrame = true;
    }

    if (PyObject* item = PyDict_GetItemString(inDict, "popup"))
    {
        outConfig.popup = ToBool(item);
        itemc.info.shownLastFrame = true;
    }

    if (PyObject* item = PyDict_GetItemString(inDict, "label"))
    {
        if (item != Py_None)
        {
            itemc.info.dirtyPos = true;
            itemc.info.dirty_size = true;
        }
    }

    if (PyObject* item = PyDict_GetItemString(inDict, "no_open_over_existing_popup")) outConfig.no_open_over_existing_popup = ToBool(item);
    if (PyObject* item = PyDict_GetItemString(inDict, "no_close")) outConfig.no_close = ToBool(item);
    if (PyObject* item = PyDict_GetItemString(inDict, "collapsed"))
    {
        outConfig._collapsedDirty = true;
        outConfig.collapsed = ToBool(item);
    }

    if (PyObject* item = PyDict_GetItemString(inDict, "min_size"))
    {
        auto min_size = ToIntVect(item);
        outConfig.min_size = { (float)min_size[0], (float)min_size[1] };
    }

    if (PyObject* item = PyDict_GetItemString(inDict, "max_size"))
    {
        auto max_size = ToIntVect(item);
        outConfig.max_size = { (float)max_size[0], (float)max_size[1] };
    }

    if (PyObject* item = PyDict_GetItemString(inDict, "on_close"))
    {
        if (outConfig.on_close)
            Py_XDECREF(outConfig.on_close);
        item = SanitizeCallback(item);
        if (item)
            Py_XINCREF(item);
        outConfig.on_close = item;
    }

    // helper for bit flipping
    auto flagop = [inDict](const char* keyword, int flag, int& flags)
    {
        if (PyObject* item = PyDict_GetItemString(inDict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
    };

    // window flags
    flagop("autosize", ImGuiWindowFlags_AlwaysAutoResize, outConfig.windowflags);
    flagop("no_move", ImGuiWindowFlags_NoMove, outConfig.windowflags);
    flagop("no_resize", ImGuiWindowFlags_NoResize, outConfig.windowflags);
    flagop("no_title_bar", ImGuiWindowFlags_NoTitleBar, outConfig.windowflags);
    flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, outConfig.windowflags);
    flagop("no_collapse", ImGuiWindowFlags_NoCollapse, outConfig.windowflags);
    flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, outConfig.windowflags);
    flagop("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, outConfig.windowflags);
    flagop("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, outConfig.windowflags);
    flagop("menubar", ImGuiWindowFlags_MenuBar, outConfig.windowflags);
    flagop("no_background", ImGuiWindowFlags_NoBackground, outConfig.windowflags);
    flagop("no_saved_settings", ImGuiWindowFlags_NoSavedSettings, outConfig.windowflags);
    flagop("no_scroll_with_mouse", ImGuiWindowFlags_NoScrollWithMouse, outConfig.windowflags);
    flagop("unsaved_document", ImGuiWindowFlags_UnsavedDocument, outConfig.windowflags);


    outConfig._oldxpos = itemc.state.pos.x;
    outConfig._oldypos = itemc.state.pos.y;
    outConfig._oldWidth = itemc.config.width;
    outConfig._oldHeight = itemc.config.height;
    outConfig._oldWindowflags = outConfig.windowflags;
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

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvTreeNodeConfig& outConfig)
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
    if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(mvAppItemType::mvTreeNode))
    {
        mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
            "Values types do not match: " + std::to_string(dataSource), &item);
        return;
    }
    outConfig.value = *static_cast<std::shared_ptr<bool>*>(srcItem->getValue());
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvTabBarConfig& outConfig)
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
    if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(mvAppItemType::mvTabBar))
    {
        mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
            "Values types do not match: " + std::to_string(dataSource), &item);
        return;
    }
    outConfig.value = *static_cast<std::shared_ptr<mvUUID>*>(srcItem->getValue());
}

void
DearPyGui::set_data_source(mvAppItem& item, mvUUID dataSource, mvCollapsingHeaderConfig& outConfig)
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
    if (DearPyGui::GetEntityValueType(srcItem->type) != DearPyGui::GetEntityValueType(mvAppItemType::mvCollapsingHeader))
    {
        mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
            "Values types do not match: " + std::to_string(dataSource), &item);
        return;
    }
    outConfig.value = *static_cast<std::shared_ptr<bool>*>(srcItem->getValue());
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


                GContext->activeWindow = item.uuid;

            }

            ImGui::EndMenu();
        }
        else
        {
            // even if menu popup is not open, we still need to update the state
            item.state.lastFrameUpdate = GContext->frame;
            item.state.active = ImGui::IsItemActive();
            item.state.activated = ImGui::IsItemActivated();
            item.state.deactivated = ImGui::IsItemDeactivated();
            item.state.focused = false;
            item.state.hovered = false;
            item.state.rectSize = { 0.0f, 0.0f };
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
            for (int i = 0; i < item.state.doubleclicked.size(); i++)
            {
                item.state.doubleclicked[i] = IsItemDoubleClicked(i);
            }
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
            for (int i = 0; i < item.state.doubleclicked.size(); i++)
            {
                item.state.doubleclicked[i] = IsItemDoubleClicked(i);
            }
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

void
DearPyGui::draw_child_window(ImDrawList* drawlist, mvAppItem& item, mvChildWindowConfig& config)
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

        // TODO: Do we want to put an if statement to prevent further drawing if not shown?
        ImGui::BeginChild(item.info.internalLabel.c_str(), ImVec2(config.autosize_x ? 0 : (float)item.config.width, config.autosize_y ? 0 : (float)item.config.height), config.childFlags, config.windowflags);
        item.state.lastFrameUpdate = GContext->frame;
        item.state.active = ImGui::IsItemActive();
        item.state.deactivated = ImGui::IsItemDeactivated();
        item.state.focused = ImGui::IsWindowFocused();
        item.state.hovered = ImGui::IsWindowHovered();
        item.state.rectSize = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };
        item.state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        for (auto& child : item.childslots[1])
        {

            child->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
            if (child->config.tracked)
            {
                ImGui::SetScrollHereX(child->config.trackOffset);
                ImGui::SetScrollHereY(child->config.trackOffset);
            }

        }

        if (config._scrollXSet)
        {
            if (config.scrollX < 0.0f)
                ImGui::SetScrollHereX(1.0f);
            else
                ImGui::SetScrollX(config.scrollX);
            config._scrollXSet = false;
        }

        if (config._scrollYSet)
        {
            if (config.scrollY < 0.0f)
                ImGui::SetScrollHereY(1.0f);
            else
                ImGui::SetScrollY(config.scrollY);
            config._scrollYSet = false;
        }

        // allows this item to have a render callback
        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
        {

            // update mouse
            ImVec2 mousePos = ImGui::GetMousePos();
            float x = mousePos.x - ImGui::GetWindowPos().x;
            float y = mousePos.y - ImGui::GetWindowPos().y;
            GContext->input.mousePos.x = (int)x;
            GContext->input.mousePos.y = (int)y;

            GContext->activeWindow = item.uuid;

        }

        config.scrollX = ImGui::GetScrollX();
        config.scrollY = ImGui::GetScrollY();
        config.scrollMaxX = ImGui::GetScrollMaxX();
        config.scrollMaxY = ImGui::GetScrollMaxY();

        ImGui::EndChild();
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
DearPyGui::draw_group(ImDrawList* drawlist, mvAppItem& item, mvGroupConfig& config)
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

        if (item.config.width != 0)
            ImGui::PushItemWidth((float)item.config.width);

        ImGui::BeginGroup();

        for (auto& child : item.childslots[1])
        {
            if (item.config.width != 0)
                child->config.width = item.config.width;

            if (item.config.height != 0)
                child->config.height = item.config.height;

            child->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

            if (config.horizontal && child->config.show)
                ImGui::SameLine((1 + child->info.location) * config.xoffset, config.hspacing);

            if (child->config.tracked)
            {
                ImGui::SetScrollHereX(child->config.trackOffset);
                ImGui::SetScrollHereY(child->config.trackOffset);
            }
        }

        if (item.config.width != 0)
            ImGui::PopItemWidth();

        ImGui::EndGroup();
        UpdateAppItemState(item.state);

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
DearPyGui::draw_drag_payload(ImDrawList* drawlist, mvAppItem& item, mvDragPayloadConfig& config)
{
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
    {
        ImGui::SetDragDropPayload(config.payloadType.c_str(), &item, sizeof(mvDragPayload));

        if (item.info.parentPtr->config.dragCallback)
        {
            if (item.info.parentPtr->config.alias.empty())
                mvAddCallback(item.info.parentPtr->config.dragCallback, item.config.parent, config.dragData, item.config.user_data);
            else
                mvAddCallback(item.info.parentPtr->config.dragCallback, item.info.parentPtr->config.alias, config.dragData, item.config.user_data);
        }

        for (auto& childset : item.childslots)
        {
            for (auto& child : childset)
                child->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
        }

        ImGui::EndDragDropSource();
    }
}

void
DearPyGui::draw_tree_node(ImDrawList* drawlist, mvAppItem& item, mvTreeNodeConfig& config)
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

        ImGui::BeginGroup();

        if (*config.value && config.selectable)
            config.flags |= ImGuiTreeNodeFlags_Selected;
        else
            config.flags &= ~ImGuiTreeNodeFlags_Selected;

        ImGui::SetNextItemOpen(*config.value);

        bool is_open = ImGui::TreeNodeEx(item.info.internalLabel.c_str(), config.flags);

        UpdateAppItemState(item.state);

        // We're only updating the value when we know that the change was triggered
        // by the user.  Note that `is_open` might be reset to `false` while the
        // tree node is actually open, e.g. when the parent window gets out of sight.
        if (item.state.toggledOpen)
            *config.value = is_open;

        if (item.state.toggledOpen && !*config.value)
        {
            item.state.toggledOpen = false;
        }

        if (!is_open)
        {
            ImGui::EndGroup();
        }
        else
        {
            for (auto& child : item.childslots[1])
                child->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

            ImGui::TreePop();
            ImGui::EndGroup();
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
DearPyGui::draw_tab_bar(ImDrawList* drawlist, mvAppItem& item, mvTabBarConfig& config)
{
    if (!item.config.show)
        return;

    ScopedID id(item.uuid);

    ImGui::BeginGroup();

    if (ImGui::BeginTabBar(item.info.internalLabel.c_str(), config.flags))
    {

        item.state.lastFrameUpdate = GContext->frame;
        item.state.visible = true; // BeginTabBar(...) only reaches this if visible

        for (auto& child : item.childslots[1])
        {

            if (*config.value == child->uuid && config._lastValue != *config.value)
                static_cast<mvTab*>(child.get())->configData._flags |= ImGuiTabItemFlags_SetSelected;

            child->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

            if (*config.value == child->uuid)
                static_cast<mvTab*>(child.get())->configData._flags &= ~ImGuiTabItemFlags_SetSelected;
        }

        ImGui::EndTabBar();
    }

    ImGui::EndGroup();

    *config.value = config.uiValue;
    config._lastValue = *config.value;
}

void
DearPyGui::draw_collapsing_header(ImDrawList* drawlist, mvAppItem& item, mvCollapsingHeaderConfig& config)
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

        bool* toggle = nullptr;
        if (config.closable)
            toggle = &item.config.show;

        ImGui::SetNextItemOpen(*config.value);

        bool is_open = ImGui::CollapsingHeader(item.info.internalLabel.c_str(), toggle, config.flags);

        UpdateAppItemState(item.state);

        // We're only updating the value when we know that the change was triggered
        // by the user.  Note that `is_open` might be reset to `false` while the
        // header is actually open, e.g. when the parent window gets out of sight.
        if (item.state.toggledOpen)
            *config.value = is_open;

        if (is_open)
        {
            for (auto& child : item.childslots[1])
                child->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
        }

        if (item.state.toggledOpen && !*config.value)
        {
            item.state.toggledOpen = false;
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
DearPyGui::draw_window(ImDrawList* drawlist, mvAppItem& item, mvWindowAppItemConfig& config)
{
    //-----------------------------------------------------------------------------
    // pre draw
    //-----------------------------------------------------------------------------

    if (!item.config.show)
        return;

    if (GContext->frame == 1 && !GContext->IO.iniFile.empty() && !(config.windowflags & ImGuiWindowFlags_NoSavedSettings))
    {
        item.info.dirtyPos = false;
        item.info.dirty_size = false;
        config._collapsedDirty = false;
    }

    if (item.info.focusNextFrame)
    {
        ImGui::SetNextWindowFocus();
        item.info.focusNextFrame = false;
    }

    // handle fonts
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

    ScopedID id(item.uuid);

    if (config.mainWindow)
    {
        ImGui::SetNextWindowBgAlpha(1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // to prevent main window corners from showing
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2((float)GContext->viewport->clientWidth, (float)GContext->viewport->clientHeight));
    }

    else if (item.info.dirtyPos)
    {
        ImGui::SetNextWindowPos(item.state.pos);
        item.info.dirtyPos = false;
    }

    if (item.info.dirty_size)
    {
        ImGui::SetNextWindowSize(ImVec2((float)item.config.width, (float)item.config.height));
        item.info.dirty_size = false;
    }

    if (config._collapsedDirty)
    {
        ImGui::SetNextWindowCollapsed(config.collapsed);
        config._collapsedDirty = false;
    }

    ImGui::SetNextWindowSizeConstraints(config.min_size, config.max_size);

    if (config.modal)
    {
        if (item.info.shownLastFrame)
        {
            item.info.shownLastFrame = false;
            ImGui::OpenPopup(item.info.internalLabel.c_str(), config.no_open_over_existing_popup ? ImGuiPopupFlags_NoOpenOverExistingPopup : ImGuiPopupFlags_None);
        }

        if (!ImGui::BeginPopupModal(item.info.internalLabel.c_str(), config.no_close ? nullptr : &item.config.show, config.windowflags))
        {
            if (config.mainWindow)
                ImGui::PopStyleVar();

            // shouldn't have to do this but do. Fix later
            item.config.show = false;
            item.state.lastFrameUpdate = GContext->frame;
            item.state.hovered = false;
            item.state.focused = false;
            item.state.toggledOpen = false;
            item.state.visible = false;

            if (item.config.alias.empty())
                mvAddCallback(config.on_close, item.uuid, nullptr, item.config.user_data);
            else
                mvAddCallback(config.on_close, item.config.alias, nullptr, item.config.user_data);

            // handle popping themes
            cleanup_local_theming(&item);

            return;
        }
    }

    else if (config.popup)
    {
        if (item.info.shownLastFrame)
        {
            item.info.shownLastFrame = false;
            ImGui::OpenPopup(item.info.internalLabel.c_str(), config.no_open_over_existing_popup ? ImGuiPopupFlags_NoOpenOverExistingPopup : ImGuiPopupFlags_None);
        }

        if (!ImGui::BeginPopup(item.info.internalLabel.c_str(), config.windowflags))
        {
            if (config.mainWindow)
                ImGui::PopStyleVar();

            // handle popping themes
            cleanup_local_theming(&item);
            return;
        }
    }

    else
    {
        if (!ImGui::Begin(item.info.internalLabel.c_str(), config.no_close ? nullptr : &item.config.show, config.windowflags))
        {
            if (config.mainWindow)
                ImGui::PopStyleVar();

            ImGui::End();

            // handle popping themes
            cleanup_local_theming(&item);

            return;
        }
    }

    ImDrawList* this_drawlist = ImGui::GetWindowDrawList();

    float startx = (float)ImGui::GetCursorScreenPos().x;
    float starty = (float)ImGui::GetCursorScreenPos().y;

    if (config.mainWindow)
        ImGui::PopStyleVar();

    for (auto& child : item.childslots[0])
    {
        // skip item if it's not shown
        if (!child->config.show)
            continue;

        child->draw(this_drawlist, startx, starty);

        UpdateAppItemState(child->state);

    }

    for (auto& child : item.childslots[1])
    {

        child->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
        if (child->config.tracked)
            ImGui::SetScrollHereY(child->config.trackOffset);

    }

    for (auto& child : item.childslots[2])
    {
        // skip item if it's not shown
        if (!child->config.show)
            continue;

        child->draw(this_drawlist, startx, starty);

        UpdateAppItemState(child->state);

    }

    //-----------------------------------------------------------------------------
    // post draw
    //-----------------------------------------------------------------------------

    // pop font from stack
    if (item.font)
        ImGui::PopFont();

    // handle popping themes
    cleanup_local_theming(&item);

    if (config._scrollXSet)
    {
        if (config.scrollX < 0.0f)
            ImGui::SetScrollHereX(1.0f);
        else
            ImGui::SetScrollX(config.scrollX);
        config._scrollXSet = false;
    }

    if (config._scrollYSet)
    {
        if (config.scrollY < 0.0f)
            ImGui::SetScrollHereY(1.0f);
        else
            ImGui::SetScrollY(config.scrollY);
        config._scrollYSet = false;
    }

    config.scrollX = ImGui::GetScrollX();
    config.scrollY = ImGui::GetScrollY();
    config.scrollMaxX = ImGui::GetScrollMaxX();
    config.scrollMaxY = ImGui::GetScrollMaxY();

    //-----------------------------------------------------------------------------
    // update state
    //-----------------------------------------------------------------------------

    item.state.lastFrameUpdate = GContext->frame;
    item.state.visible = true;
    item.state.hovered = ImGui::IsWindowHovered();
    item.state.focused = ImGui::IsWindowFocused();
    item.state.rectSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
    item.state.toggledOpen = ImGui::IsWindowCollapsed();
    if (item.state.mvPrevRectSize.x != item.state.rectSize.x || item.state.mvPrevRectSize.y != item.state.rectSize.y) { item.state.mvRectSizeResized = true; }
    else item.state.mvRectSizeResized = false;
    item.state.mvPrevRectSize = item.state.rectSize;

    if (ImGui::GetWindowWidth() != (float)item.config.width || ImGui::GetWindowHeight() != (float)item.config.height)
    {
        item.config.width = (int)ImGui::GetWindowWidth();
        item.config.height = (int)ImGui::GetWindowHeight();
        config.resized = true;
    }

    item.config.width = (int)ImGui::GetWindowWidth();
    item.config.height = (int)ImGui::GetWindowHeight();

    // update active window
    if (IsItemFocused(item.state))
    {

        float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

        // update mouse
        ImVec2 mousePos = ImGui::GetMousePos();
        float x = mousePos.x - ImGui::GetWindowPos().x;
        float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
        GContext->input.mousePos.x = (int)x;
        GContext->input.mousePos.y = (int)y;

        GContext->activeWindow = item.uuid;

    }

    item.state.pos = { ImGui::GetWindowPos().x , ImGui::GetWindowPos().y };

    if (config.popup || config.modal)
        ImGui::EndPopup();
    else
        ImGui::End();

    config.collapsed = ImGui::IsWindowCollapsed();

    if (!item.config.show)
    {
        item.state.lastFrameUpdate = GContext->frame;
        item.state.hovered = false;
        item.state.focused = false;
        item.state.toggledOpen = false;
        item.state.visible = false;

        if (item.config.alias.empty())
            mvAddCallback(config.on_close, item.uuid, nullptr, item.config.user_data);
        else
            mvAddCallback(config.on_close, item.config.alias, nullptr, item.config.user_data);
    }

    if (item.handlerRegistry)
        item.handlerRegistry->checkEvents(&item.state);
}

void
apply_drag_drop(mvAppItem* item)
{
    for (auto& item : item->childslots[3])
        item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

    if (item->config.dropCallback)
    {
        ScopedID id(item->uuid);
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(item->config.payloadType.c_str()))
            {
                auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
                if (item->config.alias.empty())
                    mvAddCallback(item->config.dropCallback, item->uuid, payloadActual->configData.dragData, nullptr);
                else
                    mvAddCallback(item->config.dropCallback, item->config.alias, payloadActual->configData.dragData, nullptr);
            }

            ImGui::EndDragDropTarget();
        }
    }
}

void
apply_drag_drop_nodraw(mvAppItem* item)
{
    if (item->config.dropCallback)
    {
        ScopedID id(item->uuid);
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(item->config.payloadType.c_str()))
            {
                auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
                if (item->config.alias.empty())
                    mvAddCallback(item->config.dropCallback, item->uuid, payloadActual->configData.dragData, nullptr);
                else
                    mvAddCallback(item->config.dropCallback, item->config.alias, payloadActual->configData.dragData, nullptr);
            }

            ImGui::EndDragDropTarget();
        }
    }
}