#include <algorithm>
#include "mvPlot.h"
#include "mvPlotting.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvPlotAxis.h"
#include "mvThemes.h"
#include "containers/mvDragPayload.h"
#include "mvPyObject.h"
#include "mvFontItems.h"
#include "AppItems/mvItemHandlers.h"

mvPlot::mvPlot(mvUUID uuid)
    : mvAppItem(uuid)
{
    //_label = "Plot###" + std::to_string(uuid);
    config.width = -1;
    config.height = -1;
}

void mvPlot::applySpecificTemplate(mvAppItem* item)
{
    auto titem = static_cast<mvPlot*>(item);
    _flags = titem->_flags;
    _equalAspectRatios = titem->_equalAspectRatios;
    _pan_button = titem->_pan_button;
    _pan_mod = titem->_pan_mod;
    _fit_button = titem->_fit_button;
    _context_menu_button = titem->_context_menu_button;
    _box_select_button = titem->_box_select_button;
    _box_select_mod = titem->_box_select_mod;
    _box_select_cancel_button = titem->_box_select_cancel_button;
    _query_button = titem->_query_button;
    _query_mod = titem->_query_mod;
    _query_toggle_mod = titem->_query_toggle_mod;
    _horizontal_mod = titem->_horizontal_mod;
    _vertical_mod = titem->_vertical_mod;
}

void mvPlot::onChildAdd(mvRef<mvAppItem> item)
{
    if (item->type == mvAppItemType::mvPlotLegend)
        _flags &= ~ImPlotFlags_NoLegend;

    if (item->type == mvAppItemType::mvPlotAxis)
    {
        updateFlags();
        updateAxesNames();
    }
}

void mvPlot::onChildRemoved(mvRef<mvAppItem> item)
{

    if (item->type == mvAppItemType::mvPlotLegend)
        _flags |= ImPlotFlags_NoLegend;

    if (item->type == mvAppItemType::mvPlotAxis)
        updateFlags();
}

void mvPlot::updateFlags()
{
    for (size_t i = 0; i < childslots[1].size(); i++)
    {
        auto child = static_cast<mvPlotAxis*>(childslots[1][i].get());
        switch (i)
        {
        case(0):
            _xflags = child->getFlags();
            break;

        case(1):
            _yflags = child->getFlags();
            break;

        case(2):
            _y1flags = child->getFlags();
            if (child->config.show)
                addFlag(ImPlotFlags_YAxis2);
            else
                removeFlag(ImPlotFlags_YAxis2);
            break;

        case(3):
            _y2flags = child->getFlags();
            if (child->config.show)
                addFlag(ImPlotFlags_YAxis3);
            else
                removeFlag(ImPlotFlags_YAxis3);
            break;

        default:
            _yflags = child->getFlags();
            break;
        }
    }

}

void mvPlot::updateAxesNames()
{
    _xaxisName.clear();
    _y1axisName.clear();
    _y2axisName.clear();
    _y3axisName.clear();

    for (size_t i = 0; i < childslots[1].size(); i++)
    {
        auto axis = childslots[1][i].get();
        switch (i)
        {
        case(0):
            _xaxisName = axis->config.specifiedLabel;
            break;

        case(1):
            _y1axisName = axis->config.specifiedLabel;
            break;

        case(2):
            _y2axisName = axis->config.specifiedLabel;
            break;

        case(3):
            _y3axisName = axis->config.specifiedLabel;
            break;

        default:
            _y1axisName = axis->config.specifiedLabel;
            break;
        }
    }

}

void mvPlot::SetColorMap(ImPlotColormap colormap)
{
    _colormap = colormap;
    _useColorMap = true;
    _newColorMap = true;
}

void mvPlot::draw(ImDrawList* drawlist, float x, float y)
{

    if (!config.show)
        return;

    // cache old cursor position
    ImVec2 previousCursorPos = ImGui::GetCursorPos();

    // set cursor position if user set
    if (info.dirtyPos)
        ImGui::SetCursorPos(state.pos);

    // update widget's position state
    state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

    // push font if a font object is attached
    if (font)
    {
        ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
        ImGui::PushFont(fontptr);
    }

    // themes
    apply_local_theming(this);

    if (_newColorMap)
    {
        ImPlot::BustColorCache(info.internalLabel.c_str());
        _newColorMap = false;
    }

    if (_useColorMap)
        ImPlot::PushColormap(_colormap);

    // custom input mapping
    ImPlot::GetInputMap().PanButton = _pan_button;
    ImPlot::GetInputMap().FitButton = _fit_button;
    ImPlot::GetInputMap().ContextMenuButton = _context_menu_button;
    ImPlot::GetInputMap().BoxSelectButton = _box_select_button;
    ImPlot::GetInputMap().BoxSelectCancelButton = _box_select_cancel_button;
    ImPlot::GetInputMap().QueryButton = _query_button;
    ImPlot::GetInputMap().QueryToggleMod = _query_toggle_mod;
    ImPlot::GetInputMap().HorizontalMod = _horizontal_mod;
    ImPlot::GetInputMap().VerticalMod = _vertical_mod;
    if(_pan_mod != -1) ImPlot::GetInputMap().PanMod = _pan_mod;
    if (_box_select_mod != -1) ImPlot::GetInputMap().BoxSelectMod = _box_select_mod;
    if (_query_mod != -1) ImPlot::GetInputMap().QueryMod = _query_mod;

    // gives axes change to make changes to ticks, limits, etc.
    for (auto& item : childslots[1])
    {
        // skip item if it's not shown
        if (!item->config.show)
            continue;
        item->customAction();
    }

    if (_fitDirty)
    {
        ImPlot::FitNextPlotAxes(_axisfitDirty[0], _axisfitDirty[1], _axisfitDirty[2], _axisfitDirty[3]);
        _fitDirty = false;
        _axisfitDirty[0] = false;
        _axisfitDirty[1] = false;
        _axisfitDirty[2] = false;
        _axisfitDirty[3] = false;
    }

    if (ImPlot::BeginPlot(info.internalLabel.c_str(), 
        _xaxisName.empty() ? nullptr : _xaxisName.c_str(), 
        _y1axisName.empty() ? nullptr : _y1axisName.c_str(),
        ImVec2((float)config.width, (float)config.height),
        _flags, _xflags, _yflags, _y1flags, _y2flags, 
        _y2axisName.empty() ? nullptr : _y2axisName.c_str(), 
        _y3axisName.empty() ? nullptr : _y3axisName.c_str()))
    {

        auto context = ImPlot::GetCurrentContext();
        // legend, drag point and lines
        for (auto& item : childslots[0])
            item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

        // axes
        for (auto& item : childslots[1])
            item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

        ImPlot::PushPlotClipRect();

        ImPlot::SetPlotYAxis(ImPlotYAxis_1); // draw items should use first plot axis
            
        // drawings
        for (auto& item : childslots[2])
        {
            // skip item if it's not shown
            if (!item->config.show)
                continue;
                
            //item->draw(ImPlot::GetPlotDrawList(), ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
            item->draw(ImPlot::GetPlotDrawList(), 0.0f, 0.0f);
                
            UpdateAppItemState(item->state);
        }

        ImPlot::PopPlotClipRect();
            
        if(_useColorMap)
            ImPlot::PopColormap();

        _queried = ImPlot::IsPlotQueried();

        if (_queried)
        {
            ImPlotLimits area = ImPlot::GetPlotQuery();
            _queryArea[0] = area.X.Min;
            _queryArea[1] = area.X.Max;
            _queryArea[2] = area.Y.Min;
            _queryArea[3] = area.Y.Max;
        }

        if (config.callback != nullptr && _queried)
        {

            if (config.alias.empty())
                mvSubmitCallback([=]() {
                    PyObject* area = PyTuple_New(4);
                    PyTuple_SetItem(area, 0, PyFloat_FromDouble(_queryArea[0]));
                    PyTuple_SetItem(area, 1, PyFloat_FromDouble(_queryArea[1]));
                    PyTuple_SetItem(area, 2, PyFloat_FromDouble(_queryArea[2]));
                    PyTuple_SetItem(area, 3, PyFloat_FromDouble(_queryArea[3]));
                    mvAddCallback(config.callback, uuid, area, config.user_data);
                    });
            else
                mvSubmitCallback([=]() {
                PyObject* area = PyTuple_New(4);
                PyTuple_SetItem(area, 0, PyFloat_FromDouble(_queryArea[0]));
                PyTuple_SetItem(area, 1, PyFloat_FromDouble(_queryArea[1]));
                PyTuple_SetItem(area, 2, PyFloat_FromDouble(_queryArea[2]));
                PyTuple_SetItem(area, 3, PyFloat_FromDouble(_queryArea[3]));
                mvAddCallback(config.callback, config.alias, area, config.user_data);
                    });
        }

        if (ImPlot::IsPlotHovered())
        {
            GContext->input.mousePlotPos.x = ImPlot::GetPlotMousePos().x;
            GContext->input.mousePlotPos.y = ImPlot::GetPlotMousePos().y;
        }

        // todo: resolve clipping
        if (config.dropCallback)
        {
            ScopedID id(uuid);
            if (ImPlot::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(config.payloadType.c_str()))
                {
                    auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
                    if (config.alias.empty())
                        mvAddCallback(config.dropCallback, uuid, payloadActual->getDragData(), nullptr);
                    else
                        mvAddCallback(config.dropCallback, config.alias, payloadActual->getDragData(), nullptr);
                }

                ImPlot::EndDragDropTarget();
            }
        }

        // update state

        _flags = context->CurrentPlot->Flags;

        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
        {

            // update mouse
            ImVec2 mousePos = ImGui::GetMousePos();
            float x = mousePos.x - ImGui::GetWindowPos().x;
            float y = mousePos.y - ImGui::GetWindowPos().y;
            GContext->input.mousePos.x = (int)x;
            GContext->input.mousePos.y = (int)y;


            if (GContext->itemRegistry->activeWindow != uuid)
                GContext->itemRegistry->activeWindow = uuid;

        }

        // TODO: find a better way to handle this
        for (auto& item : childslots[0])
        {
            if(item->type == mvAppItemType::mvPlotLegend)
            {
                auto legend = static_cast<mvPlotLegend*>(item.get());
                legend->configData.legendLocation = context->CurrentPlot->Items.Legend.Location;
                legend->configData.horizontal = context->CurrentPlot->Items.Legend.Orientation == ImPlotOrientation_Horizontal;
                legend->configData.outside = context->CurrentPlot->Items.Legend.Outside;
                break;
            }
        }


        ImPlot::EndPlot();
            
    }

    // set cursor position to cached position
    if (info.dirtyPos)
        ImGui::SetCursorPos(previousCursorPos);

    ImPlot::GetInputMap() = _originalMap;

    UpdateAppItemState(state);

    if (font)
    {
        ImGui::PopFont();
    }

    if (theme)
    {
        theme->pop_theme_components();
    }

    if (handlerRegistry)
        handlerRegistry->checkEvents(&state);

    // drag drop
    for (auto& item : childslots[3])
        item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
}

bool mvPlot::isPlotQueried() const
{
    return _queried;
}

double* mvPlot::getPlotQueryArea()
{
    return _queryArea;
}

void mvPlot::addFlag(ImPlotFlags flag)
{
    _flags |= flag;
}

void mvPlot::removeFlag(ImPlotFlags flag)
{
    _flags &= ~flag;
}

void mvPlot::handleSpecificKeywordArgs(PyObject* dict)
{
    if (dict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(dict, "x_axis_name"))_xaxisName = ToString(item);

    // custom input mapping
    if (PyObject* item = PyDict_GetItemString(dict, "pan_button")) _pan_button = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "pad_mod")) _pan_mod = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "fit_button")) _fit_button = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "context_menu_button")) _context_menu_button = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "box_select_button")) _box_select_button = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "box_select_mod")) _box_select_mod = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "box_select_cancel_button")) _box_select_cancel_button = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "query_button")) _query_button = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "query_mod")) _query_mod = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "query_toggle_mod")) _query_toggle_mod = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "horizontal_mod")) _horizontal_mod = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "vertical_mod")) _vertical_mod = ToInt(item);

    // helper for bit flipping
    auto flagop = [dict](const char* keyword, int flag, int& flags)
    {
        if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
    };

    // plot flags
    flagop("no_title",             ImPlotFlags_NoTitle,          _flags);
    flagop("no_menus",             ImPlotFlags_NoMenus,          _flags);
    flagop("no_box_select",        ImPlotFlags_NoBoxSelect,      _flags);
    flagop("no_mouse_pos",         ImPlotFlags_NoMousePos,       _flags);
    flagop("no_highlight",         ImPlotFlags_NoHighlight,      _flags);
    flagop("no_child",             ImPlotFlags_NoChild,          _flags);
    flagop("query",                ImPlotFlags_Query,            _flags);
    flagop("crosshairs",           ImPlotFlags_Crosshairs,       _flags);
    flagop("anti_aliased",         ImPlotFlags_AntiAliased,      _flags);
    flagop("equal_aspects",        ImPlotFlags_Equal,            _flags);

}

void mvPlot::getSpecificConfiguration(PyObject* dict)
{
    if (dict == nullptr)
        return;

    mvPyObject py_x_axis_name = ToPyString(_xaxisName);
    mvPyObject py_pan_button = ToPyInt(_pan_button);
    mvPyObject py_pan_mod = ToPyInt(_pan_mod);
    mvPyObject py_fit_button = ToPyInt(_fit_button);
    mvPyObject py_context_menu_button = ToPyInt(_context_menu_button);
    mvPyObject py_box_select_button = ToPyInt(_box_select_button);
    mvPyObject py_box_select_mod = ToPyInt(_box_select_mod);
    mvPyObject py_box_select_cancel_button = ToPyInt(_box_select_cancel_button);
    mvPyObject py_query_button = ToPyInt(_query_button);
    mvPyObject py_query_mod = ToPyInt(_query_mod);
    mvPyObject py_query_toggle_mod = ToPyInt(_query_toggle_mod);
    mvPyObject py_horizontal_mod = ToPyInt(_horizontal_mod);
    mvPyObject py_vertical_mod = ToPyInt(_vertical_mod);
    
    PyDict_SetItemString(dict, "x_axis_name", py_x_axis_name);
    PyDict_SetItemString(dict, "pan_button", py_pan_button);
    PyDict_SetItemString(dict, "pan_mod", py_pan_mod);
    PyDict_SetItemString(dict, "fit_button", py_fit_button);
    PyDict_SetItemString(dict, "context_menu_button", py_context_menu_button);
    PyDict_SetItemString(dict, "box_select_button", py_box_select_button);
    PyDict_SetItemString(dict, "box_select_mod", py_box_select_mod);
    PyDict_SetItemString(dict, "box_select_cancel_button", py_box_select_cancel_button);
    PyDict_SetItemString(dict, "query_button", py_query_button);
    PyDict_SetItemString(dict, "query_mod", py_query_mod);
    PyDict_SetItemString(dict, "query_toggle_mod", py_query_toggle_mod);
    PyDict_SetItemString(dict, "horizontal_mod", py_horizontal_mod);
    PyDict_SetItemString(dict, "vertical_mod", py_vertical_mod);

    // helper to check and set bit
    auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
    {
        mvPyObject py_result = ToPyBool(flags & flag);
        PyDict_SetItemString(dict, keyword, py_result);
    };

    // plot flags
    checkbitset("no_title",             ImPlotFlags_NoTitle,          _flags);
    checkbitset("no_menus",             ImPlotFlags_NoMenus,          _flags);
    checkbitset("no_box_select",        ImPlotFlags_NoBoxSelect,      _flags);
    checkbitset("no_mouse_pos",         ImPlotFlags_NoMousePos,       _flags);
    checkbitset("no_highlight",         ImPlotFlags_NoHighlight,      _flags);
    checkbitset("no_child",             ImPlotFlags_NoChild,          _flags);
    checkbitset("query",                ImPlotFlags_Query,            _flags);
    checkbitset("crosshairs",           ImPlotFlags_Crosshairs,       _flags);
    checkbitset("anti_aliased",         ImPlotFlags_AntiAliased,      _flags);
    checkbitset("equal_aspects",        ImPlotFlags_Equal,            _flags);
}