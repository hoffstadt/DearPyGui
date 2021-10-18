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

    void mvPlotLegend::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Integer, "location", mvArgType::KEYWORD_ARG, "5", "location, mvPlot_Location_*" });
        args.push_back({ mvPyDataType::Bool, "horizontal", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "outside", mvArgType::KEYWORD_ARG, "False" });

        mvPythonParserSetup setup;
        setup.about = "Adds a plot legend to a plot.";
        setup.category = { "Plotting", "Widgets" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

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
