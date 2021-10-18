#include <algorithm>
#include "mvPlot.h"
#include "mvPlotLegend.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvAreaSeries.h"
#include "mvBarSeries.h"
#include "mvCandleSeries.h"
#include "mvErrorSeries.h"
#include "mvHeatSeries.h"
#include "mvImageSeries.h"
#include "mvInfiniteLineSeries.h"
#include "mvLabelSeries.h"
#include "mvPieSeries.h"
#include "mvScatterSeries.h"
#include "mvShadeSeries.h"
#include "mvStairSeries.h"
#include "mvStemSeries.h"
#include "mvPythonExceptions.h"
#include "mvPlotAxis.h"
#include "themes/mvTheme.h"
#include "containers/mvDragPayload.h"
#include "mvPyObject.h"
#include "fonts/mvFont.h"

namespace Marvel {

    void mvPlot::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        {
            std::vector<mvPythonDataElement> args;

            AddCommonArgs(args,(CommonParserArgs)(
                MV_PARSER_ARG_ID |
                MV_PARSER_ARG_WIDTH |
                MV_PARSER_ARG_HEIGHT |
                MV_PARSER_ARG_INDENT |
                MV_PARSER_ARG_PARENT |
                MV_PARSER_ARG_BEFORE |
                MV_PARSER_ARG_SHOW |
                MV_PARSER_ARG_CALLBACK |
                MV_PARSER_ARG_DROP_CALLBACK |
                MV_PARSER_ARG_DRAG_CALLBACK |
                MV_PARSER_ARG_PAYLOAD_TYPE |
                MV_PARSER_ARG_SEARCH_DELAY |
                MV_PARSER_ARG_FILTER |
                MV_PARSER_ARG_TRACKED |
                MV_PARSER_ARG_POS)
            );

            // plot flags
            args.push_back({ mvPyDataType::Bool, "no_title", mvArgType::KEYWORD_ARG, "False"});
            args.push_back({ mvPyDataType::Bool, "no_menus", mvArgType::KEYWORD_ARG, "False"});
            args.push_back({ mvPyDataType::Bool, "no_box_select", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Bool, "no_mouse_pos", mvArgType::KEYWORD_ARG, "False"});
            args.push_back({ mvPyDataType::Bool, "no_highlight", mvArgType::KEYWORD_ARG, "False"});
            args.push_back({ mvPyDataType::Bool, "no_child", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Bool, "query", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Bool, "crosshairs", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Bool, "anti_aliased", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Bool, "equal_aspects", mvArgType::KEYWORD_ARG, "False" });

            // key modifiers
            args.push_back({ mvPyDataType::Integer, "pan_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Left", "enables panning when held" });
            args.push_back({ mvPyDataType::Integer, "pan_mod", mvArgType::KEYWORD_ARG, "-1", "optional modifier that must be held for panning" });
            args.push_back({ mvPyDataType::Integer, "fit_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Left", "fits visible data when double clicked" });
            args.push_back({ mvPyDataType::Integer, "context_menu_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Right", "opens plot context menu (if enabled) when clicked" });
            args.push_back({ mvPyDataType::Integer, "box_select_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Right", "begins box selection when pressed and confirms selection when released" });
            args.push_back({ mvPyDataType::Integer, "box_select_mod", mvArgType::KEYWORD_ARG, "-1", "begins box selection when pressed and confirms selection when released" });
            args.push_back({ mvPyDataType::Integer, "box_select_cancel_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Left", "cancels active box selection when pressed" });
            args.push_back({ mvPyDataType::Integer, "query_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Middle", "begins query selection when pressed and end query selection when released" });
            args.push_back({ mvPyDataType::Integer, "query_mod", mvArgType::KEYWORD_ARG, "-1", "optional modifier that must be held for query selection" });
            args.push_back({ mvPyDataType::Integer, "query_toggle_mod", mvArgType::KEYWORD_ARG, "internal_dpg.mvKey_Control", "when held, active box selections turn into queries" });
            args.push_back({ mvPyDataType::Integer, "horizontal_mod", mvArgType::KEYWORD_ARG, "internal_dpg.mvKey_Alt", "expands active box selection/query horizontally to plot edge when held" });
            args.push_back({ mvPyDataType::Integer, "vertical_mod", mvArgType::KEYWORD_ARG, "internal_dpg.mvKey_Shift", "expands active box selection/query vertically to plot edge when held" });

            mvPythonParserSetup setup;
            setup.about = "Adds a plot which is used to hold series, and can be drawn to with draw commands.";
            setup.category = { "Plotting", "Containers", "Widgets" };
            setup.returnType = mvPyDataType::UUID;
            setup.createContextManager = true;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ s_command, parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID, "plot" });

            mvPythonParserSetup setup;
            setup.about = "Returns true if the plot is currently being queried. (Requires plot 'query' kwarg to be enabled)";
            setup.category = { "Plotting", "App Item Operations" };
            setup.returnType = mvPyDataType::Bool;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "is_plot_queried", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID, "plot" });

            mvPythonParserSetup setup;
            setup.about = "Returns the last/current query area of the plot. (Requires plot 'query' kwarg to be enabled)";
            setup.category = { "Plotting", "App Item Operations" };
            setup.returnType = mvPyDataType::FloatList;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_plot_query_area", parser });
        }

    }

    mvPlot::mvPlot(mvUUID uuid)
        : mvAppItem(uuid)
    {
        //_label = "Plot###" + std::to_string(_uuid);
        _width = -1;
        _height = -1;
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
        if (item->getType() == mvAppItemType::mvPlotLegend)
            _flags &= ~ImPlotFlags_NoLegend;

        if (item->getType() == mvAppItemType::mvPlotAxis)
        {
            updateFlags();
            updateAxesNames();
        }
    }

    void mvPlot::onChildRemoved(mvRef<mvAppItem> item)
    {

        if (item->getType() == mvAppItemType::mvPlotLegend)
            _flags |= ImPlotFlags_NoLegend;

        if (item->getType() == mvAppItemType::mvPlotAxis)
            updateFlags();
    }

    void mvPlot::updateFlags()
    {
        for (size_t i = 0; i < _children[1].size(); i++)
        {
            auto child = static_cast<mvPlotAxis*>(_children[1][i].get());
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
                if (child->_show)
                    addFlag(ImPlotFlags_YAxis2);
                else
                    removeFlag(ImPlotFlags_YAxis2);
                break;

            case(3):
                _y2flags = child->getFlags();
                if (child->_show)
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

        for (size_t i = 0; i < _children[1].size(); i++)
        {
            auto axis = _children[1][i].get();
            switch (i)
            {
            case(0):
                _xaxisName = axis->_specifiedLabel;
                break;

            case(1):
                _y1axisName = axis->_specifiedLabel;
                break;

            case(2):
                _y2axisName = axis->_specifiedLabel;
                break;

            case(3):
                _y3axisName = axis->_specifiedLabel;
                break;

            default:
                _y1axisName = axis->_specifiedLabel;
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

        if (!_show)
            return;

        // cache old cursor position
        ImVec2 previousCursorPos = ImGui::GetCursorPos();

        // set cursor position if user set
        if (_dirtyPos)
            ImGui::SetCursorPos(_state.pos);

        // update widget's position state
        _state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

        // push font if a font object is attached
        if (_font)
        {
            ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
            ImGui::PushFont(fontptr);
        }

        // themes
        if (auto classTheme = getClassThemeComponent())
            static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

        if (_theme)
        {
            static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
            static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
            static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
        }

        if (_newColorMap)
        {
            ImPlot::BustColorCache(_internalLabel.c_str());
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
        for (auto& item : _children[1])
        {
            // skip item if it's not shown
            if (!item->_show)
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

        if (ImPlot::BeginPlot(_internalLabel.c_str(), 
            _xaxisName.empty() ? nullptr : _xaxisName.c_str(), 
            _y1axisName.empty() ? nullptr : _y1axisName.c_str(),
            ImVec2((float)_width, (float)_height), 
            _flags, _xflags, _yflags, _y1flags, _y2flags, 
            _y2axisName.empty() ? nullptr : _y2axisName.c_str(), 
            _y3axisName.empty() ? nullptr : _y3axisName.c_str()))
        {

            auto context = ImPlot::GetCurrentContext();
            // legend, drag point and lines
            for (auto& item : _children[0])
                item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

            // axes
            for (auto& item : _children[1])
                item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);

            ImPlot::PushPlotClipRect();
            
            // drawings
            for (auto& item : _children[2])
            {
                // skip item if it's not shown
                if (!item->_show)
                    continue;
                
                //item->draw(ImPlot::GetPlotDrawList(), ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
                item->draw(ImPlot::GetPlotDrawList(), 0.0f, 0.0f);
                
                UpdateAppItemState(item->_state);
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

            if (_callback != nullptr && _queried)
            {

                if (_alias.empty())
                    mvSubmitCallback([=]() {
                        PyObject* area = PyTuple_New(4);
                        PyTuple_SetItem(area, 0, PyFloat_FromDouble(_queryArea[0]));
                        PyTuple_SetItem(area, 1, PyFloat_FromDouble(_queryArea[1]));
                        PyTuple_SetItem(area, 2, PyFloat_FromDouble(_queryArea[2]));
                        PyTuple_SetItem(area, 3, PyFloat_FromDouble(_queryArea[3]));
                        mvAddCallback(_callback, _uuid, area, _user_data);
                        });
                else
                    mvSubmitCallback([=]() {
                    PyObject* area = PyTuple_New(4);
                    PyTuple_SetItem(area, 0, PyFloat_FromDouble(_queryArea[0]));
                    PyTuple_SetItem(area, 1, PyFloat_FromDouble(_queryArea[1]));
                    PyTuple_SetItem(area, 2, PyFloat_FromDouble(_queryArea[2]));
                    PyTuple_SetItem(area, 3, PyFloat_FromDouble(_queryArea[3]));
                    mvAddCallback(_callback, _alias, area, _user_data);
                        });
            }

            if (ImPlot::IsPlotHovered())
            {
                GContext->input.mousePlotPos.x = ImPlot::GetPlotMousePos().x;
                GContext->input.mousePlotPos.y = ImPlot::GetPlotMousePos().y;
            }

            // todo: resolve clipping
            

            if (_dropCallback)
            {
                ScopedID id(_uuid);
                if (ImPlot::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
                    {
                        auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
                        if (_alias.empty())
                            mvAddCallback(_dropCallback,_uuid, payloadActual->getDragData(), nullptr);
                        else
                            mvAddCallback(_dropCallback,_alias, payloadActual->getDragData(), nullptr);
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


                if (GContext->itemRegistry->activeWindow != _uuid)
                    GContext->itemRegistry->activeWindow = _uuid;

            }

            // TODO: find a better way to handle this
            for (auto& item : _children[0])
            {
                if(item->getType() == mvAppItemType::mvPlotLegend)
                {
                    auto legend = static_cast<mvPlotLegend*>(item.get());
                    legend->_legendLocation = context->CurrentPlot->Items.Legend.Location;
                    legend->_horizontal = context->CurrentPlot->Items.Legend.Orientation == ImPlotOrientation_Horizontal;
                    legend->_outside = context->CurrentPlot->Items.Legend.Outside;
                    break;
                }
            }


            ImPlot::EndPlot();
            
        }

        // set cursor position to cached position
        if (_dirtyPos)
            ImGui::SetCursorPos(previousCursorPos);

        ImPlot::GetInputMap() = _originalMap;

        UpdateAppItemState(_state);

        if (_font)
        {
            ImGui::PopFont();
        }

        if (_theme)
        {
            static_cast<mvTheme*>(_theme.get())->customAction();
        }

        if (_handlerRegistry)
            _handlerRegistry->customAction(&_state);

        // drag drop
        for (auto& item : _children[3])
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

    PyObject* mvPlot::is_plot_queried(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* plotraw;

        if (!Parse((GetParsers())["is_plot_queried"], args, kwargs, __FUNCTION__, &plotraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID plot = GetIDFromPyObject(plotraw);

        auto aplot = GetItem(*GContext->itemRegistry, plot);
        if (aplot == nullptr)
        {
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "is_plot_queried",
                "Item not found: " + std::to_string(plot), nullptr);
            return GetPyNone();
        }

        if (aplot->getType() != mvAppItemType::mvPlot)
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, "is_plot_queried",
                "Incompatible type. Expected types include: mvPlot", aplot);
            return GetPyNone();
        }

        mvPlot* graph = static_cast<mvPlot*>(aplot);

        return ToPyBool(graph->isPlotQueried());
    }

    PyObject* mvPlot::get_plot_query_area(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* plotraw;

        if (!Parse((GetParsers())["get_plot_query_area"], args, kwargs, __FUNCTION__, &plotraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID plot = GetIDFromPyObject(plotraw);

        auto aplot = GetItem(*GContext->itemRegistry, plot);
        if (aplot == nullptr)
        {
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_plot_query_area",
                "Item not found: " + std::to_string(plot), nullptr);
            return GetPyNone();
        }

        if (aplot->getType() != mvAppItemType::mvPlot)
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, "is_plot_queried",
                "Incompatible type. Expected types include: mvPlot", aplot);
            return GetPyNone();
        }

        mvPlot* graph = static_cast<mvPlot*>(aplot);

        double* result = graph->getPlotQueryArea();
        return Py_BuildValue("(dddd)", result[0], result[1], result[2], result[3]);
    }

}
