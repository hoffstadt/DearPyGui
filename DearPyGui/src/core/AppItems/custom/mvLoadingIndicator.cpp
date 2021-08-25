#include "mvLoadingIndicator.h"
#include "mvLoadingIndicatorCustom.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

    void mvLoadingIndicator::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::UUID, "Adds a rotating anamated loding symbol.", { "Widgets" });
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_POS)
        );

        parser.addArg<mvPyDataType::Integer>("style", mvArgType::KEYWORD_ARG, "0", "0 is rotating dots style, 1 is rotating bar style.");
        parser.addArg<mvPyDataType::Integer>("circle_count", mvArgType::KEYWORD_ARG, "8", "Number of dots show if dots or size of circle if circle.");
        parser.addArg<mvPyDataType::Float>("speed", mvArgType::KEYWORD_ARG, "1.0", "Speed the anamation will rotate.");
        parser.addArg<mvPyDataType::Float>("radius", mvArgType::KEYWORD_ARG, "3.0", "Radius size of the loading indicator.");
        parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0", "Thickness of the circles or line.");
        parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(51, 51, 55, 255)", "Color of the growing center circle.");
        parser.addArg<mvPyDataType::IntList>("secondary_color", mvArgType::KEYWORD_ARG, "(29, 151, 236, 103)", "Background of the dots in dot mode.");

        parser.finalize();

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
            ImGui::SetCursorPos(_state.getItemPos());

        // update widget's position state
        _state.setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

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

        // handle enabled theming
        if (_enabled)
        {
            // push class theme (if it exists)
            if (auto classTheme = getClassTheme())
                static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

            // push item theme (if it exists)
            if (_theme)
                static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
        }

        // handled disabled theming
        else
        {
            // push class theme (if it exists)
            if (auto classTheme = getClassDisabledTheme())
                static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

            // push item theme (if it exists)
            if (_disabledTheme)
                static_cast<mvTheme*>(_disabledTheme.get())->draw(nullptr, 0.0f, 0.0f);
        }


        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {
            ScopedID id(_uuid);

            if (_style == 0)
                LoadingIndicatorCircle(_specificedlabel.c_str(), _radius, _mainColor, _optionalColor, _circleCount, _speed);
            else
                LoadingIndicatorCircle2(_specificedlabel.c_str(), _radius, _thickness, _mainColor);
        }

        //-----------------------------------------------------------------------------
        // update state
        //   * only update if applicable
        //-----------------------------------------------------------------------------
        _state._lastFrameUpdate = mvApp::s_frame;
        _state._hovered = ImGui::IsItemHovered();
        _state._leftclicked = ImGui::IsItemClicked();
        _state._rightclicked = ImGui::IsItemClicked(1);
        _state._middleclicked = ImGui::IsItemClicked(2);
        _state._visible = ImGui::IsItemVisible();
        _state._rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
        _state._rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
        _state._rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
        _state._contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

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

        // handle popping styles
        if (_enabled)
        {
            if (auto classTheme = getClassTheme())
                static_cast<mvTheme*>(classTheme.get())->customAction();

            if (_theme)
                static_cast<mvTheme*>(_theme.get())->customAction();
        }
        else
        {
            if (auto classTheme = getClassDisabledTheme())
                static_cast<mvTheme*>(classTheme.get())->customAction();

            if (_disabledTheme)
                static_cast<mvTheme*>(_disabledTheme.get())->customAction();
        }

        if (_handlerRegistry)
            _handlerRegistry->customAction(&_state);

        // handle drag & drop payloads
        for (auto& item : _children[3])
        {
            if (!item->preDraw())
                continue;

            item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
        }

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
                        mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
                    else
                        mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _alias, payloadActual->getDragData(), nullptr);
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