#include <utility>
#include "mvPlotLegend.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "themes/mvTheme.h"
#include "mvPythonExceptions.h"
#include "mvPlot.h"
#include "mvSubPlots.h"
#include "containers/mvDragPayload.h"

namespace Marvel {

    mvPlotLegend::mvPlotLegend(mvUUID uuid)
        : 
        mvAppItem(uuid)
    {
    }

    void mvPlotLegend::draw(ImDrawList* drawlist, float x, float y)
    {
        if (!_show)
            return;

        if (_dirty)
        {
            ImPlot::SetLegendLocation(_legendLocation, _horizontal ? ImPlotOrientation_Horizontal : ImPlotOrientation_Vertical, _outside);
            _dirty = false;
        }

        UpdateAppItemState(_state);

        if (_font)
        {
            ImGui::PopFont();
        }

        if (_theme)
        {
            static_cast<mvTheme*>(_theme.get())->customAction();
        }

        if (_dropCallback)
        {
            if (ImPlot::BeginDragDropTargetLegend())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
                {
                    auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
                    mvAddCallback(_dropCallback,_uuid, payloadActual->getDragData(), nullptr);
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

        if (_shownLastFrame)
        {
            _shownLastFrame = false;
            if (auto plot = static_cast<mvPlot*>(_parentPtr))
                plot->removeFlag(ImPlotFlags_NoLegend);
            else if (auto plot = static_cast<mvSubPlots*>(_parentPtr))
                plot->removeFlag(ImPlotSubplotFlags_NoLegend);
            _show = true;
        }

        if (_hiddenLastFrame)
        {
            _hiddenLastFrame = false;
            if (auto plot = static_cast<mvPlot*>(_parentPtr))
                plot->addFlag(ImPlotFlags_NoLegend);
            else if (auto plot = static_cast<mvSubPlots*>(_parentPtr))
                plot->addFlag(ImPlotSubplotFlags_NoLegend);
            _show = false;
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

        PyDict_SetItemString(dict, "location", mvPyObject(ToPyInt(_location)));
        PyDict_SetItemString(dict, "horizontal", mvPyObject(ToPyBool(_horizontal)));
        PyDict_SetItemString(dict, "outside", mvPyObject(ToPyBool(_outside)));

    }


}
