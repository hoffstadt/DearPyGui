#include <utility>
#include "mvPlotAxis.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "themes/mvTheme.h"
#include "mvPythonExceptions.h"
#include "mvPlot.h"
#include "containers/mvDragPayload.h"

namespace Marvel {

    mvPlotAxis::mvPlotAxis(mvUUID uuid)
        : 
        mvAppItem(uuid)
    {
    }

    void mvPlotAxis::customAction(void* data)
    {
        if (_setLimits || _dirty)
        {
            switch (info.location)
            {
            case(0):
                ImPlot::SetNextPlotLimitsX(_limits.x, _limits.y, ImGuiCond_Always);
                break;

            case(1):
                ImPlot::SetNextPlotLimitsY(_limits.x, _limits.y, ImGuiCond_Always);
                break;

            case(2):
                ImPlot::SetNextPlotLimitsY(_limits.x, _limits.y, ImGuiCond_Always, ImPlotYAxis_2);
                break;

            case(3):
                ImPlot::SetNextPlotLimitsY(_limits.x, _limits.y, ImGuiCond_Always, ImPlotYAxis_3);
                break;

            default:
                ImPlot::SetNextPlotLimitsY(_limits.x, _limits.y, ImGuiCond_Always);
                break;
            }

            _dirty = false;
            
        }

        if (!_labels.empty())
        {
            // TODO: Checks
            if(info.location == 0)
                ImPlot::SetNextPlotTicksX(_labelLocations.data(), (int)_labels.size(), _clabels.data());
            else
                ImPlot::SetNextPlotTicksY(_labelLocations.data(), (int)_labels.size(), _clabels.data());
        }
    }

    void mvPlotAxis::draw(ImDrawList* drawlist, float x, float y)
    {

        if (!config.show)
            return;

        // todo: add check
        if(_axis != 0)
            ImPlot::SetPlotYAxis(info.location - 1);

        for (auto& item : childslots[1])
            item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

        // x axis
        if (_axis == 0)
        {
            _limits_actual.x = (float)ImPlot::GetPlotLimits(info.location).X.Min;
            _limits_actual.y = (float)ImPlot::GetPlotLimits(info.location).X.Max;
            auto context = ImPlot::GetCurrentContext();
            _flags = context->CurrentPlot->XAxis.Flags;

        }

        // y axis
        else
        {
            _limits_actual.x = (float)ImPlot::GetPlotLimits(info.location -1).Y.Min;
            _limits_actual.y = (float)ImPlot::GetPlotLimits(info.location -1).Y.Max;
            auto context = ImPlot::GetCurrentContext();
            _flags = context->CurrentPlot->YAxis[info.location-1].Flags;
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
            ScopedID id(uuid);
            if (info.location == 0 && ImPlot::BeginDragDropTargetX())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(config.payloadType.c_str()))
                {
                    auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
                    mvAddCallback(config.dropCallback, uuid, payloadActual->getDragData(), nullptr);
                }

                ImPlot::EndDragDropTarget();
            }
            else if (ImPlot::BeginDragDropTargetY(info.location - 1))
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

    void mvPlotAxis::fitAxisData()
    {
        static_cast<mvPlot*>(info.parentPtr)->_fitDirty = true;
        static_cast<mvPlot*>(info.parentPtr)->_axisfitDirty[info.location] = true;
    }

    void mvPlotAxis::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // axis flags
        flagop("no_gridlines", ImPlotAxisFlags_NoGridLines, _flags);
        flagop("no_tick_marks", ImPlotAxisFlags_NoTickMarks, _flags);
        flagop("no_tick_labels", ImPlotAxisFlags_NoTickLabels, _flags);
        flagop("log_scale", ImPlotAxisFlags_LogScale, _flags);
        flagop("invert", ImPlotAxisFlags_Invert, _flags);
        flagop("lock_min", ImPlotAxisFlags_LockMin, _flags);
        flagop("lock_max", ImPlotAxisFlags_LockMax, _flags);
        flagop("time", ImPlotAxisFlags_Time, _flags);
        
        if (info.parentPtr)
        {
            static_cast<mvPlot*>(info.parentPtr)->updateFlags();
            static_cast<mvPlot*>(info.parentPtr)->updateAxesNames();
        }

        if (info.shownLastFrame)
        {
            info.shownLastFrame = false;
            if (auto plot = static_cast<mvPlot*>(info.parentPtr))
                plot->removeFlag(ImPlotFlags_NoLegend);
            config.show = true;
        }

        if (info.hiddenLastFrame)
        {
            info.hiddenLastFrame = false;
            if (auto plot = static_cast<mvPlot*>(info.parentPtr))
                plot->addFlag(ImPlotFlags_NoLegend);
            config.show = false;
        }
    }

    void mvPlotAxis::handleSpecificRequiredArgs(PyObject* dict)
    {
        if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
            return;

        for (int i = 0; i < PyTuple_Size(dict); i++)
        {
            PyObject* item = PyTuple_GetItem(dict, i);
            switch (i)
            {
            case 0:
                _axis = ToInt(item);
                if (_axis > 1)
                    _axis = 1;
                break;

            default:
                break;
            }
        }
    }

    void mvPlotAxis::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

                // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            mvPyObject py_result = ToPyBool(flags & flag);
            PyDict_SetItemString(dict, keyword, py_result);
        };

        // plot flags
        checkbitset("no_gridlines", ImPlotAxisFlags_NoGridLines, _flags);
        checkbitset("no_tick_marks", ImPlotAxisFlags_NoTickMarks, _flags);
        checkbitset("no_tick_labels", ImPlotAxisFlags_NoTickLabels, _flags);
        checkbitset("log_scale", ImPlotAxisFlags_LogScale, _flags);
        checkbitset("invert", ImPlotAxisFlags_Invert, _flags);
        checkbitset("lock_min", ImPlotAxisFlags_LockMin, _flags);
        checkbitset("lock_max", ImPlotAxisFlags_LockMax, _flags);
        checkbitset("time", ImPlotAxisFlags_Time, _flags);
    }

    void mvPlotAxis::setLimits(float y_min, float y_max)
    {
        _setLimits = true;
        _limits = ImVec2(y_min, y_max);
    }

    void mvPlotAxis::setLimitsAuto()
    {
        _setLimits = false;
    }

    void mvPlotAxis::onChildAdd(mvRef<mvAppItem> item)
    {
    }

    void mvPlotAxis::onChildRemoved(mvRef<mvAppItem> item)
    {
    }

    void mvPlotAxis::resetYTicks()
    {
        _labels.clear();
        _clabels.clear();
        _labelLocations.clear();
    }

    void mvPlotAxis::setYTicks(const std::vector<std::string>& labels, const std::vector<double>& locations)
    {
        _labels = labels;
        _labelLocations = locations;

        for (const auto& item : _labels)
            _clabels.push_back(item.data());
    }

    void mvPlotAxis::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvPlotAxis*>(item);
        _flags = titem->_flags;
        _axis = titem->_axis;
        _setLimits = titem->_setLimits;
        _limits = titem->_limits;
        _limits_actual = titem->_limits_actual;
        _labels = titem->_labels;
        _labelLocations = titem->_labelLocations;
        _clabels = titem->_clabels;
    }

}
