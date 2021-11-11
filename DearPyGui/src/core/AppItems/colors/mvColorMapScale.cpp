#include "mvColorMapScale.h"
#include <utility>
#include "mvContext.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvItemRegistry.h"
#include <implot.h>
#include "mvPythonExceptions.h"
#include "mvAppItemCommons.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

    void mvColorMapScale::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Callable, "drag_callback", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });
        args.push_back({ mvPyDataType::UUID, "colormap", mvArgType::KEYWORD_ARG, "0", "mvPlotColormap_* constants or mvColorMap uuid from a color map registry" });
        args.push_back({ mvPyDataType::Float, "min_scale", mvArgType::KEYWORD_ARG, "0.0", "Sets the min number of the color scale. Typically is the same as the min scale from the heat series." });
        args.push_back({ mvPyDataType::Float, "max_scale", mvArgType::KEYWORD_ARG, "1.0", "Sets the max number of the color scale. Typically is the same as the max scale from the heat series." });

        mvPythonParserSetup setup;
        setup.about = "Adds a legend that pairs values with colors. This is typically used with a heat series. ";
        setup.category = { "Widgets", "Colors" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    mvColorMapScale::mvColorMapScale(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvColorMapScale::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvColorMapScale*>(item);
        _scale_min = titem->_scale_min;
        _scale_max = titem->_scale_max;
        _colormap = titem->_colormap;
    }

    void mvColorMapScale::setColorMap(ImPlotColormap colormap)
    {
        _colormap = colormap;
    }

    void mvColorMapScale::draw(ImDrawList* drawlist, float x, float y)
    {

        //-----------------------------------------------------------------------------
        // pre draw
        //-----------------------------------------------------------------------------

        // show/hide
        if (!_show)
            return;

        // focusing
        if (_focusNextFrame)
        {
            ImGui::SetKeyboardFocusHere();
            _focusNextFrame = false;
        }

        // cache old cursor position
        ImVec2 previousCursorPos = ImGui::GetCursorPos();

        // set cursor position if user set
        if (_dirtyPos)
            ImGui::SetCursorPos(_state.pos);

        // update widget's position state
        _state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

        // set item width
        if (_width != 0)
            ImGui::SetNextItemWidth((float)_width);

        // set indent
        if (_indent > 0.0f)
            ImGui::Indent(_indent);

        // push font if a font object is attached
        if (_font)
        {
            ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
            ImGui::PushFont(fontptr);
        }

        // themes
        if (_enabled)
        {
            if (auto classTheme = getClassThemeComponent())
                static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
        }
        else
        {
            if (auto classTheme = getClassDisabledThemeComponent())
                static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
        }

        if (_theme)
        {
            static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
            static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
            static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
        }

        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {
            ScopedID id(_uuid);

            ImPlot::ColormapScale(_internalLabel.c_str(), _scale_min, _scale_max, ImVec2((float)_width, (float)_height), _colormap);
        }

        //-----------------------------------------------------------------------------
        // update state
        //-----------------------------------------------------------------------------
        UpdateAppItemState(_state);

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------

        // set cursor position to cached position
        if (_dirtyPos)
            ImGui::SetCursorPos(previousCursorPos);

        if (_indent > 0.0f)
            ImGui::Unindent(_indent);

        // pop font off stack
        if (_font)
            ImGui::PopFont();

        // handle popping themes
        if (_enabled)
        {
            if (auto classTheme = getClassThemeComponent())
                static_cast<mvThemeComponent*>(classTheme.get())->customAction();
        }
        else
        {
            if (auto classTheme = getClassDisabledThemeComponent())
                static_cast<mvThemeComponent*>(classTheme.get())->customAction();
        }

        if (_theme)
        {
            static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
            static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
            static_cast<mvTheme*>(_theme.get())->customAction();
        }

        if (_handlerRegistry)
            _handlerRegistry->customAction(&_state);

        // handle drag & drop payloads
        for (auto& item : _children[3])
            item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

        // handle drag & drop if used
        if (_dropCallback)
        {
            ScopedID id(_uuid);
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
                {
                    auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
                    if (_alias.empty())
                        mvAddCallback(_dropCallback,_uuid, payloadActual->getDragData(), nullptr);
                    else
                        mvAddCallback(_dropCallback,_alias, payloadActual->getDragData(), nullptr);
                }

                ImGui::EndDragDropTarget();
            }
        }
    }

    void mvColorMapScale::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "min_scale")) _scale_min = (double)ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_scale")) _scale_max = (double)ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "colormap"))
        {
            _colormap= (ImPlotColormap)GetIDFromPyObject(item);
            if (_colormap > 10)
            {
                auto asource = GetItem(*GContext->itemRegistry, _colormap);
                if (asource == nullptr)
                {
                    mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_colormap",
                        "Source Item not found: " + std::to_string(_colormap), nullptr);
                    _colormap = 0;
                }

                else if (asource->getType() == mvAppItemType::mvColorMap)
                {
                    mvColorMap* colormap = static_cast<mvColorMap*>(asource);
                    _colormap = colormap->getColorMap();
                }
            }
        }
    }

    void mvColorMapScale::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "min_scale", mvPyObject(ToPyFloat((float)_scale_min)));
        PyDict_SetItemString(dict, "max_scale", mvPyObject(ToPyFloat((float)_scale_max)));

    }

}
