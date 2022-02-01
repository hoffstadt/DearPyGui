#include <utility>
#include "mvPlotLegend.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvThemes.h"
#include "mvPythonExceptions.h"
#include "mvPlot.h"
#include "mvSubPlots.h"
#include "containers/mvDragPayload.h"

void mvPlotLegend::draw(ImDrawList* drawlist, float x, float y)
{
    if (!config.show)
        return;

    if (_dirty)
    {
        ImPlot::SetLegendLocation(_legendLocation, _horizontal ? ImPlotOrientation_Horizontal : ImPlotOrientation_Vertical, _outside);
        _dirty = false;
    }

    UpdateAppItemState(state);

    if (font)
    {
        ImGui::PopFont();
    }

    if (theme)
    {
        static_cast<mvTheme*>(theme.get())->customAction();
    }

    if (config.dropCallback)
    {
        if (ImPlot::BeginDragDropTargetLegend())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(config.payloadType.c_str()))
            {
                auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
                mvAddCallback(config.dropCallback, uuid, payloadActual->getDragData(), nullptr);
            }

            ImPlot::EndDragDropTarget();
        }
    }
}

void mvPlotLegend::handleSpecificKeywordArgs(PyObject* dict)
{
    if (dict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(dict, "location")) { _legendLocation = ToInt(item); _dirty = true; }
    if (PyObject* item = PyDict_GetItemString(dict, "horizontal")){ _horizontal = ToBool(item); _dirty = true;}
    if (PyObject* item = PyDict_GetItemString(dict, "outside")) {_outside = ToBool(item); _dirty = true;}

    if (info.shownLastFrame)
    {
        info.shownLastFrame = false;
        if (auto plot = static_cast<mvPlot*>(info.parentPtr))
            plot->removeFlag(ImPlotFlags_NoLegend);
        else if (auto plot = static_cast<mvSubPlots*>(info.parentPtr))
            plot->removeFlag(ImPlotSubplotFlags_NoLegend);
        config.show = true;
    }

    if (info.hiddenLastFrame)
    {
        info.hiddenLastFrame = false;
        if (auto plot = static_cast<mvPlot*>(info.parentPtr))
            plot->addFlag(ImPlotFlags_NoLegend);
        else if (auto plot = static_cast<mvSubPlots*>(info.parentPtr))
            plot->addFlag(ImPlotSubplotFlags_NoLegend);
        config.show = false;
    }
}

void mvPlotLegend::applySpecificTemplate(mvAppItem* item)
{
    auto titem = static_cast<mvPlotLegend*>(item);
    _legendLocation = titem->_legendLocation;
    _horizontal = titem->_horizontal;
    _outside = titem->_outside;
}

void mvPlotLegend::getSpecificConfiguration(PyObject* dict)
{
    if (dict == nullptr)
        return;

    PyDict_SetItemString(dict, "location", mvPyObject(ToPyInt(info.location)));
    PyDict_SetItemString(dict, "horizontal", mvPyObject(ToPyBool(_horizontal)));
    PyDict_SetItemString(dict, "outside", mvPyObject(ToPyBool(_outside)));

}