#include "mvLoadingIndicator.h"
#include "mvLoadingIndicatorCustom.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

    void mvLoadingIndicator::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Integer, "style", mvArgType::KEYWORD_ARG, "0", "0 is rotating dots style, 1 is rotating bar style." });
        args.push_back({ mvPyDataType::Integer, "circle_count", mvArgType::KEYWORD_ARG, "8", "Number of dots show if dots or size of circle if circle." });
        args.push_back({ mvPyDataType::Float, "speed", mvArgType::KEYWORD_ARG, "1.0", "Speed the anamation will rotate." });
        args.push_back({ mvPyDataType::Float, "radius", mvArgType::KEYWORD_ARG, "3.0", "Radius size of the loading indicator." });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0", "Thickness of the circles or line." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(51, 51, 55, 255)", "Color of the growing center circle." });
        args.push_back({ mvPyDataType::IntList, "secondary_color", mvArgType::KEYWORD_ARG, "(29, 151, 236, 103)", "Background of the dots in dot mode." });

        mvPythonParserSetup setup;
        setup.about = "Adds a rotating animated loading symbol.";
        setup.category = { "Widgets" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    mvLoadingIndicator::mvLoadingIndicator(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvLoadingIndicator::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvLoadingIndicator*>(item);
        _style = titem->_style;
        _circleCount = titem->_circleCount;
        _radius = titem->_radius;
        _speed = titem->_speed;
        _thickness = titem->_thickness;
        _mainColor = titem->_mainColor;
        _optionalColor = titem->_optionalColor;
    }

    void mvLoadingIndicator::draw(ImDrawList* drawlist, float x, float y)
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

            if (_style == 0)
                LoadingIndicatorCircle(_specifiedLabel.c_str(), _radius, _mainColor, _optionalColor, _circleCount, _speed);
            else
                LoadingIndicatorCircle2(_specifiedLabel.c_str(), _radius, _thickness, _mainColor);
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

    void mvLoadingIndicator::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "style")) _style = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "circle_count")) _circleCount = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "radius")) _radius = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) _speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "color")) _mainColor = ToColor(item);
        if (PyObject* item = PyDict_GetItemString(dict, "secondary_color")) _optionalColor = ToColor(item);
    }

    void mvLoadingIndicator::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "style", mvPyObject(ToPyInt(_style)));
        PyDict_SetItemString(dict, "circle_count", mvPyObject(ToPyInt(_circleCount)));
        PyDict_SetItemString(dict, "radius", mvPyObject(ToPyFloat(_radius)));
        PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
        PyDict_SetItemString(dict, "speed", mvPyObject(ToPyFloat(_speed)));
        PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_mainColor)));
        PyDict_SetItemString(dict, "secondary_color", mvPyObject(ToPyColor(_optionalColor)));

    }

}