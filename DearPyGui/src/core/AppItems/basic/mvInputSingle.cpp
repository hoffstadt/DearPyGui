#include "mvInputSingle.h"
#include <utility>
#include "mvModule_DearPyGui.h"
#include "mvApp.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

    void mvInputInt::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::UUID, "Adds input for an int. Step buttons can be turned on or off.", { "Widgets" });
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_ENABLED |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        parser.addArg<mvPyDataType::Integer>("default_value", mvArgType::KEYWORD_ARG, "0");

        parser.addArg<mvPyDataType::Integer>("min_value", mvArgType::KEYWORD_ARG, "0", "Value for lower limit of input. By default this limits the step buttons. Use clamped to limit manual input.");
        parser.addArg<mvPyDataType::Integer>("max_value", mvArgType::KEYWORD_ARG, "100", "Value for upper limit of input. By default this limits the step buttons. Use clamped to limit manual input.");
        parser.addArg<mvPyDataType::Integer>("step", mvArgType::KEYWORD_ARG, "1", "Increment to change value by when the step buttons are pressed. Setting this to a value of 0 or smaller will turn off step buttons.");
        parser.addArg<mvPyDataType::Integer>("step_fast", mvArgType::KEYWORD_ARG, "100", "After holding the step buttons for extended time the increments will switch to this value.");

        parser.addArg<mvPyDataType::Bool>("min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates min_value.");
        parser.addArg<mvPyDataType::Bool>("max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates max_value.");
        parser.addArg<mvPyDataType::Bool>("on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press.");
        parser.addArg<mvPyDataType::Bool>("readonly", mvArgType::KEYWORD_ARG, "False", "Activates a read only mode for the input.");

        parser.finalize();

        parsers->insert({ s_command, parser });


    }

    void mvInputFloat::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        mvPythonParser parser(mvPyDataType::UUID, "Adds input for floats. Step buttons can be turned on or off.", { "Widgets" });
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_ENABLED |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        parser.addArg<mvPyDataType::Float>("default_value", mvArgType::KEYWORD_ARG, "0.0");

        parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%.3f'");

        parser.addArg<mvPyDataType::Float>("min_value", mvArgType::KEYWORD_ARG, "0.0", "Value for lower limit of input. By default this limits the step buttons. Use clamped to limit manual input.");
        parser.addArg<mvPyDataType::Float>("max_value", mvArgType::KEYWORD_ARG, "100.0", "Value for upper limit of input. By default this limits the step buttons. Use clamped to limit manual input.");
        parser.addArg<mvPyDataType::Float>("step", mvArgType::KEYWORD_ARG, "0.1", "Increment to change value by when the step buttons are pressed. Setting this to a value of 0 or smaller will turn off step buttons.");
        parser.addArg<mvPyDataType::Float>("step_fast", mvArgType::KEYWORD_ARG, "1.0", "After holding the step buttons for extended time the increments will switch to this value.");

        parser.addArg<mvPyDataType::Bool>("min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates min_value.");
        parser.addArg<mvPyDataType::Bool>("max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates max_value.");
        parser.addArg<mvPyDataType::Bool>("on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press.");
        parser.addArg<mvPyDataType::Bool>("readonly", mvArgType::KEYWORD_ARG, "False", "Activates a read only mode for the input.");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvInputInt::mvInputInt(mvUUID uuid)
        : mvAppItem(uuid)
    {
        _last_value = *_value;
    }

    void mvInputInt::setDataSource(mvUUID dataSource)
    {
        if (dataSource == _source) return;
        _source = dataSource;

        mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (item->getValueType() != getValueType())
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
                "Values types do not match: " + std::to_string(dataSource), this);
            return;
        }
        _value = *static_cast<std::shared_ptr<int>*>(item->getValue());
    }

    PyObject* mvInputInt::getPyValue()
    {
        return ToPyInt(*_value);
    }

    void mvInputInt::setPyValue(PyObject* value)
    {
        *_value = ToInt(value);
    }

    void mvInputInt::draw(ImDrawList* drawlist, float x, float y)
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

            if (ImGui::InputInt(_internalLabel.c_str(), _value.get(), _step, _step_fast, _flags))
            {
                // determines clamped cases
                if (_min_clamped && _max_clamped) 
                {
                    if (*_value < _min) *_value = _min;
                    else if (*_value > _max) *_value = _max;
                }
                else if (_min_clamped) 
                {
                    if (*_value < _min) *_value = _min;
                }
                else if (_max_clamped) 
                {
                    if (*_value > _max) *_value = _max;
                }

                // If the widget is edited through ctrl+click mode the active value will be entered every frame.
                // If the value is out of bounds the value will be overwritten with max or min so each frame the value will be switching between the
                // ctrl+click value and the bounds value until the widget is not in ctrl+click mode. To prevent the callback from running every 
                // frame we check if the value was already submitted.
                if (_last_value != *_value)
                {
                    _last_value = *_value;
                    auto value = *_value;
                    mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                        mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyInt(value), _user_data);
                        });
                }
            }

        }

        //-----------------------------------------------------------------------------
        // update state
        //   * only update if applicable
        //-----------------------------------------------------------------------------
        _state._lastFrameUpdate = mvApp::s_frame;
        _state._hovered = ImGui::IsItemHovered();
        _state._active = ImGui::IsItemActive();
        _state._focused = ImGui::IsItemFocused();
        _state._leftclicked = ImGui::IsItemClicked();
        _state._rightclicked = ImGui::IsItemClicked(1);
        _state._middleclicked = ImGui::IsItemClicked(2);
        _state._visible = ImGui::IsItemVisible();
        _state._activated = ImGui::IsItemActivated();
        _state._deactivated = ImGui::IsItemDeactivated();
        _state._deactivatedAfterEdit = ImGui::IsItemDeactivatedAfterEdit();
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

        // handle widget's event handlers
        for (auto& item : _children[3])
        {
            if (!item->preDraw())
                continue;

            item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
        }

        // handle drag & drop payloads
        for (auto& item : _children[4])
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
                    mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
                }

                ImGui::EndDragDropTarget();
            }
        }

    }

    mvInputFloat::mvInputFloat(mvUUID uuid)
        : mvAppItem(uuid)
    {
        _last_value = *_value;
    }

    PyObject* mvInputFloat::getPyValue()
    {
        return ToPyFloat(*_value);
    }

    void mvInputFloat::setPyValue(PyObject* value)
    {
        *_value = ToFloat(value);
    }

    void mvInputFloat::setDataSource(mvUUID dataSource)
    {
        if (dataSource == _source) return;
        _source = dataSource;

        mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (item->getValueType() != getValueType())
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
                "Values types do not match: " + std::to_string(dataSource), this);
            return;
        }
        _value = *static_cast<std::shared_ptr<float>*>(item->getValue());
    }

    void mvInputFloat::draw(ImDrawList* drawlist, float x, float y)
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

            if (ImGui::InputFloat(_internalLabel.c_str(), _value.get(), _step, _step_fast, _format.c_str(), _flags))
            {
                auto inital_value = *_value;
                // determines clamped cases
                if (_min_clamped && _max_clamped)
                {
                    if (*_value < _min) *_value = _min;
                    else if (*_value > _max) *_value = _max;
                }
                else if (_min_clamped)
                {
                    if (*_value < _min) *_value = _min;
                }
                else if (_max_clamped)
                {
                    if (*_value > _max) *_value = _max;
                }

                // If the widget is edited through ctrl+click mode the active value will be entered every frame.
                // If the value is out of bounds the value will be overwritten with max or min so each frame the value will be switching between the
                // ctrl+click value and the bounds value until the widget is not in ctrl+click mode. To prevent the callback from running every 
                // frame we check if the value was already submitted.
                if (_last_value != *_value)
                {
                    _last_value = *_value;
                    auto value = *_value;
                    mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                        mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyFloat(value), _user_data);
                        });
                }
            }
        }

        //-----------------------------------------------------------------------------
        // update state
        //   * only update if applicable
        //-----------------------------------------------------------------------------
        _state._lastFrameUpdate = mvApp::s_frame;
        _state._hovered = ImGui::IsItemHovered();
        _state._active = ImGui::IsItemActive();
        _state._focused = ImGui::IsItemFocused();
        _state._leftclicked = ImGui::IsItemClicked();
        _state._rightclicked = ImGui::IsItemClicked(1);
        _state._middleclicked = ImGui::IsItemClicked(2);
        _state._visible = ImGui::IsItemVisible();
        _state._activated = ImGui::IsItemActivated();
        _state._deactivated = ImGui::IsItemDeactivated();
        _state._deactivatedAfterEdit = ImGui::IsItemDeactivatedAfterEdit();
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

        // handle widget's event handlers
        for (auto& item : _children[3])
        {
            if (!item->preDraw())
                continue;

            item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
        }

        // handle drag & drop payloads
        for (auto& item : _children[4])
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
                    mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
                }

                ImGui::EndDragDropTarget();
            }
        }
    }

    void mvInputInt::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? _flags |= ImGuiInputTextFlags_EnterReturnsTrue : _flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? _stor_flags |= ImGuiInputTextFlags_EnterReturnsTrue : _stor_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        if (PyObject* item = PyDict_GetItemString(dict, "readonly")) ToBool(item) ? _flags |= ImGuiInputTextFlags_ReadOnly : _flags &= ~ImGuiInputTextFlags_ReadOnly;
        if (PyObject* item = PyDict_GetItemString(dict, "readonly")) ToBool(item) ? _stor_flags |= ImGuiInputTextFlags_ReadOnly : _stor_flags &= ~ImGuiInputTextFlags_ReadOnly;
        if (PyObject* item = PyDict_GetItemString(dict, "step")) _step = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "step_fast")) _step_fast = ToInt(item);

        bool minmax_set = false;
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) 
        {
            _min = ToInt(item); 
            _min_clamped = true; 
            minmax_set = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) 
        { 
            _max = ToInt(item); 
            _max_clamped = true; 
            minmax_set = true;
        }

        if (!minmax_set)
        {
            if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) _min_clamped = ToBool(item);
            if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) _max_clamped = ToBool(item);
        }

        if (wasEnabledLastFrameReset())
            _flags = _stor_flags;

        if (wasDisabledLastFrameReset())
        {
            _stor_flags = _flags;
            _flags |= ImGuiInputTextFlags_ReadOnly;
            _flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }
    }

    void mvInputInt::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "on_enter", ToPyBool(_flags & ImGuiInputTextFlags_EnterReturnsTrue));
        PyDict_SetItemString(dict, "readonly", ToPyBool(_flags & ImGuiInputTextFlags_ReadOnly));
        PyDict_SetItemString(dict, "step", ToPyInt(_step));
        PyDict_SetItemString(dict, "step_fast", ToPyInt(_step_fast));
        PyDict_SetItemString(dict, "min_value", ToPyInt(_min));
        PyDict_SetItemString(dict, "max_value", ToPyInt(_max));
        PyDict_SetItemString(dict, "min_clamped", ToPyBool(_min_clamped));
        PyDict_SetItemString(dict, "max_clamped", ToPyBool(_max_clamped));

    }

    void mvInputFloat::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) _format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "step")) _step = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "step_fast")) _step_fast = ToFloat(item);


        bool minmax_set = false;
        if (PyObject* item = PyDict_GetItemString(dict, "min_value"))
        {
            _min = ToFloat(item);
            _min_clamped = true;
            minmax_set = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "max_value"))
        {
            _max = ToFloat(item);
            _max_clamped = true;
            minmax_set = true;
        }

        if (!minmax_set)
        {
            if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) _min_clamped = ToBool(item);
            if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) _max_clamped = ToBool(item);
        }

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, _flags);
        flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, _stor_flags);
        flagop("readonly", ImGuiInputTextFlags_ReadOnly, _flags);
        flagop("readonly", ImGuiInputTextFlags_ReadOnly, _stor_flags);

    }

    void mvInputFloat::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(_format));
        PyDict_SetItemString(dict, "step", ToPyFloat(_step));
        PyDict_SetItemString(dict, "step_fast", ToPyFloat(_step_fast));
        PyDict_SetItemString(dict, "min_value", ToPyFloat(_min));
        PyDict_SetItemString(dict, "max_value", ToPyFloat(_max));
        PyDict_SetItemString(dict, "min_clamped", ToPyBool(_min_clamped));
        PyDict_SetItemString(dict, "max_clamped", ToPyBool(_max_clamped));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, _flags);
        checkbitset("readonly", ImGuiInputTextFlags_ReadOnly, _flags);

    }

}
