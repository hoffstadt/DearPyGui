#include "mvInput.h"
#include <misc/cpp/imgui_stdlib.h>
#include <utility>
#include "mvModule_DearPyGui.h"
#include "mvContext.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

    void mvInputText::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
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

        args.push_back({ mvPyDataType::String, "default_value", mvArgType::KEYWORD_ARG, "''" });
        args.push_back({ mvPyDataType::String, "hint", mvArgType::KEYWORD_ARG, "''", "Displayed only when value is an empty string. Will reappear if input value is set to empty string. Will not show if default value is anything other than default empty string." });
        args.push_back({ mvPyDataType::Bool, "multiline", mvArgType::KEYWORD_ARG, "False", "Allows for multiline text input." });
        args.push_back({ mvPyDataType::Bool, "no_spaces", mvArgType::KEYWORD_ARG, "False", "Filter out spaces and tabs." });
        args.push_back({ mvPyDataType::Bool, "uppercase", mvArgType::KEYWORD_ARG, "False", "Automatically make all inputs uppercase." });
        args.push_back({ mvPyDataType::Bool, "tab_input", mvArgType::KEYWORD_ARG, "False", "Allows tabs to be input into the string value instead of changing item focus." });
        args.push_back({ mvPyDataType::Bool, "decimal", mvArgType::KEYWORD_ARG, "False", "Only allow characters 0123456789.+-*/" });
        args.push_back({ mvPyDataType::Bool, "hexadecimal", mvArgType::KEYWORD_ARG, "False", "Only allow characters 0123456789ABCDEFabcdef" });
        args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode where no text can be input but text can still be highlighted." });
        args.push_back({ mvPyDataType::Bool, "password", mvArgType::KEYWORD_ARG, "False", "Display all input characters as '*'." });
        args.push_back({ mvPyDataType::Bool, "scientific", mvArgType::KEYWORD_ARG, "False", "Only allow characters 0123456789.+-*/eE (Scientific notation input)" });
        args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press." });

        mvPythonParserSetup setup;
        setup.about = "Adds input for text.";
        setup.category = { "Widgets" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    void mvInputIntMulti::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
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

        args.push_back({ mvPyDataType::IntList, "default_value", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)" });
        args.push_back({ mvPyDataType::Integer, "min_value", mvArgType::KEYWORD_ARG, "0", "Value for lower limit of input for each cell. Use min_clamped to turn on." });
        args.push_back({ mvPyDataType::Integer, "max_value", mvArgType::KEYWORD_ARG, "100", "Value for upper limit of input for each cell. Use max_clamped to turn on." });
        args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4", "Number of components displayed for input." });
        args.push_back({ mvPyDataType::Bool, "min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of min_value." });
        args.push_back({ mvPyDataType::Bool, "max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of max_value." });
        args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter." });
        args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode where no text can be input but text can still be highlighted." });

        mvPythonParserSetup setup;
        setup.about = "Adds multi int input for up to 4 integer values.";
        setup.category = { "Widgets" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    void mvInputFloatMulti::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;
        AddCommonArgs(args,(CommonParserArgs)(
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

        args.push_back({ mvPyDataType::FloatList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)" });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%.3f'", "Determines the format the float will be displayed as use python string formatting." });
        args.push_back({ mvPyDataType::Float, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Value for lower limit of input for each cell. Use min_clamped to turn on." });
        args.push_back({ mvPyDataType::Float, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Value for upper limit of input for each cell. Use max_clamped to turn on." });
        args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4", "Number of components displayed for input." });
        args.push_back({ mvPyDataType::Bool, "min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of min_value." });
        args.push_back({ mvPyDataType::Bool, "max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of max_value."});
        args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press." });
        args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode where no text can be input but text can still be highlighted." });

        mvPythonParserSetup setup;
        setup.about = "Adds multi float input for up to 4 float values.";
        setup.category = { "Widgets" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    void mvInputInt::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::Integer, "default_value", mvArgType::KEYWORD_ARG, "0" });
        args.push_back({ mvPyDataType::Integer, "min_value", mvArgType::KEYWORD_ARG, "0", "Value for lower limit of input. By default this limits the step buttons. Use min_clamped to limit manual input." });
        args.push_back({ mvPyDataType::Integer, "max_value", mvArgType::KEYWORD_ARG, "100", "Value for upper limit of input. By default this limits the step buttons. Use max_clamped to limit manual input." });
        args.push_back({ mvPyDataType::Integer, "step", mvArgType::KEYWORD_ARG, "1", "Increment to change value by when the step buttons are pressed. Setting this to a value of 0 or smaller will turn off step buttons." });
        args.push_back({ mvPyDataType::Integer, "step_fast", mvArgType::KEYWORD_ARG, "100", "After holding the step buttons for extended time the increments will switch to this value." });
        args.push_back({ mvPyDataType::Bool, "min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of min_value." });
        args.push_back({ mvPyDataType::Bool, "max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of max_value." });
        args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press." });
        args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode where no text can be input but text can still be highlighted." });

        mvPythonParserSetup setup;
        setup.about = "Adds input for an int. +/- buttons can be activated by setting the value of step.";
        setup.category = { "Widgets" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });

    }

    void mvInputFloat::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::Float, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%.3f'", "Determines the format the float will be displayed as use python string formatting." });
        args.push_back({ mvPyDataType::Float, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Value for lower limit of input. By default this limits the step buttons. Use min_clamped to limit manual input." });
        args.push_back({ mvPyDataType::Float, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Value for upper limit of input. By default this limits the step buttons. Use max_clamped to limit manual input." });
        args.push_back({ mvPyDataType::Float, "step", mvArgType::KEYWORD_ARG, "0.1", "Increment to change value by when the step buttons are pressed. Setting this to a value of 0 or smaller will turn off step buttons." });
        args.push_back({ mvPyDataType::Float, "step_fast", mvArgType::KEYWORD_ARG, "1.0", "After holding the step buttons for extended time the increments will switch to this value." });
        args.push_back({ mvPyDataType::Bool, "min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of min_value." });
        args.push_back({ mvPyDataType::Bool, "max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of max_value." });
        args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press." });
        args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode where no text can be input but text can still be highlighted." });

        mvPythonParserSetup setup;
        setup.about = "Adds input for an float. +/- buttons can be activated by setting the value of step.";
        setup.category = { "Widgets" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    mvInputText::mvInputText(mvUUID uuid)
        :
        mvAppItem(uuid)
    {
    }

    void mvInputText::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvInputText*>(item);
        if (_source != 0) _value = titem->_value;
        _disabled_value = titem->_disabled_value;
        _hint = titem->_hint;
        _multiline = titem->_multiline;
        _flags = titem->_flags;
        _stor_flags = titem->_stor_flags;
    }

    PyObject* mvInputText::getPyValue()
    {
        return ToPyString(*_value);
    }

    void mvInputText::setPyValue(PyObject* value)
    {
        *_value = ToString(value);
    }

    void mvInputText::setDataSource(mvUUID dataSource)
    {
        if (dataSource == _source) return;
        _source = dataSource;

        mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (GetEntityValueType(item->getType()) != GetEntityValueType(getType()))
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
                "Values types do not match: " + std::to_string(dataSource), this);
            return;
        }
        _value = *static_cast<std::shared_ptr<std::string>*>(item->getValue());
    }

    void mvInputText::draw(ImDrawList* drawlist, float x, float y)
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

            bool activated = false;

            if (_multiline)
                _hint.clear();

            if (_hint.empty())
            {
                if (_multiline)
                    activated = ImGui::InputTextMultiline(_internalLabel.c_str(), _value.get(), ImVec2((float)_width, (float)_height), _flags);
                else
                    activated = ImGui::InputText(_internalLabel.c_str(), _value.get(), _flags);
            }
            else
                activated = ImGui::InputTextWithHint(_internalLabel.c_str(), _hint.c_str(), _value.get(), _flags);

            if (activated)
            {
                auto value = *_value;
                if (_alias.empty())
                    mvSubmitCallback([=]() {
                    mvAddCallback(getCallback(false), _uuid, ToPyString(value), _user_data);
                        });
                else
                    mvSubmitCallback([=]() {
                    mvAddCallback(getCallback(false), _alias, ToPyString(value), _user_data);
                        });
            }

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
                        mvAddCallback(_dropCallback, _uuid, payloadActual->getDragData(), nullptr);
                    else
                        mvAddCallback(_dropCallback, _alias, payloadActual->getDragData(), nullptr);
                }

                ImGui::EndDragDropTarget();
            }
        }
    }

    void mvInputText::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "hint")) _hint = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "multiline")) _multiline = ToBool(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("no_spaces", ImGuiInputTextFlags_CharsNoBlank, _flags);
        flagop("uppercase", ImGuiInputTextFlags_CharsUppercase, _flags);
        flagop("decimal", ImGuiInputTextFlags_CharsDecimal, _flags);
        flagop("hexadecimal", ImGuiInputTextFlags_CharsHexadecimal, _flags);
        flagop("readonly", ImGuiInputTextFlags_ReadOnly, _flags);
        flagop("password", ImGuiInputTextFlags_Password, _flags);
        flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, _flags);
        flagop("scientific", ImGuiInputTextFlags_CharsScientific, _flags);
        flagop("tab_input", ImGuiInputTextFlags_AllowTabInput, _flags);

        if (_enabledLastFrame)
        {
            _enabledLastFrame = false;
            _flags = _stor_flags;
        }

        if (_disabledLastFrame)
        {
            _disabledLastFrame = false;
            _stor_flags = _flags;
            _flags |= ImGuiInputTextFlags_ReadOnly;
            _flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }
    }

    void mvInputText::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "hint", mvPyObject(ToPyString(_hint)));
        PyDict_SetItemString(dict, "multline", mvPyObject(ToPyBool(_multiline)));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
        };

        // window flags
        checkbitset("no_spaces", ImGuiInputTextFlags_CharsNoBlank, _flags);
        checkbitset("uppercase", ImGuiInputTextFlags_CharsUppercase, _flags);
        checkbitset("decimal", ImGuiInputTextFlags_CharsDecimal, _flags);
        checkbitset("hexadecimal", ImGuiInputTextFlags_CharsHexadecimal, _flags);
        checkbitset("readonly", ImGuiInputTextFlags_ReadOnly, _flags);
        checkbitset("password", ImGuiInputTextFlags_Password, _flags);
        checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, _flags);
        checkbitset("scientific", ImGuiInputTextFlags_CharsScientific, _flags);
        checkbitset("tab_input", ImGuiInputTextFlags_AllowTabInput, _flags);
    }

    mvInputIntMulti::mvInputIntMulti(mvUUID uuid)
        : mvAppItem(uuid)
    {
        _last_value = *_value;
    }

    void mvInputIntMulti::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvInputIntMulti*>(item);
        if (_source != 0) _value = titem->_value;
        _disabled_value[0] = titem->_disabled_value[0];
        _disabled_value[1] = titem->_disabled_value[1];
        _disabled_value[2] = titem->_disabled_value[2];
        _disabled_value[3] = titem->_disabled_value[3];
        _min = titem->_min;
        _max = titem->_max;
        _min_clamped = titem->_min_clamped;
        _max_clamped = titem->_max_clamped;
        _flags = titem->_flags;
        _stor_flags = titem->_stor_flags;
        _last_value = titem->_last_value;
        _size = titem->_size;
    }

    PyObject* mvInputIntMulti::getPyValue()
    {
        return ToPyIntList(_value->data(), 4);
    }

    void mvInputIntMulti::setPyValue(PyObject* value)
    {
        std::vector<int> temp = ToIntVect(value);
        while (temp.size() < 4)
            temp.push_back(0);
        std::array<int, 4> temp_array;
        for (size_t i = 0; i < temp_array.size(); i++)
            temp_array[i] = temp[i];
        if (_value)
            *_value = temp_array;
        else
            _value = std::make_shared<std::array<int, 4>>(temp_array);
    }

    void mvInputIntMulti::setDataSource(mvUUID dataSource)
    {
        if (dataSource == _source) return;
        _source = dataSource;

        mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (GetEntityValueType(item->getType()) != GetEntityValueType(getType()))
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
                "Values types do not match: " + std::to_string(dataSource), this);
            return;
        }
        _value = *static_cast<std::shared_ptr<std::array<int, 4>>*>(item->getValue());
    }

    void mvInputIntMulti::draw(ImDrawList* drawlist, float x, float y)
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

            bool res = false;

            switch (_size)
            {
            case 2:
                res = ImGui::InputInt2(_internalLabel.c_str(), _value->data(), _flags);
                break;
            case 3:
                res = ImGui::InputInt3(_internalLabel.c_str(), _value->data(), _flags);
                break;
            case 4:
                res = ImGui::InputInt4(_internalLabel.c_str(), _value->data(), _flags);
                break;
            default:
                break;
            }

            if (res)
            {
                auto inital_value = *_value;
                // determines clamped cases
                if (_min_clamped && _max_clamped)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (_value->data()[i] < _min) _value->data()[i] = _min;
                        else if (_value->data()[i] > _max) _value->data()[i] = _max;
                    }
                }
                else if (_min_clamped)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (_value->data()[i] < _min) _value->data()[i] = _min;
                    }
                }
                else if (_max_clamped)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (_value->data()[i] > _max) _value->data()[i] = _max;
                    }
                }

                // If the widget is edited through ctrl+click mode the active value will be entered every frame.
                // If the value is out of bounds the value will be overwritten with max or min so each frame the value will be switching between the
                // ctrl+click value and the bounds value until the widget is not in ctrl+click mode. To prevent the callback from running every 
                // frame we check if the value was already submitted.
                if (_last_value != *_value)
                {
                    _last_value = *_value;

                    auto value = *_value;

                    if(_alias.empty())
                        mvSubmitCallback([=]() {
                            mvAddCallback(getCallback(false), _uuid, ToPyIntList(value.data(), (int)value.size()), _user_data);
                            });
                    else
                        mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), _alias, ToPyIntList(value.data(), (int)value.size()), _user_data);
                            });
                }
            }
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

    mvInputFloatMulti::mvInputFloatMulti(mvUUID uuid)
        : mvAppItem(uuid)
    {
        _last_value = *_value;
    }

    void mvInputFloatMulti::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvInputFloatMulti*>(item);
        if (_source != 0) _value = titem->_value;
        _disabled_value[0] = titem->_disabled_value[0];
        _disabled_value[1] = titem->_disabled_value[1];
        _disabled_value[2] = titem->_disabled_value[2];
        _disabled_value[3] = titem->_disabled_value[3];
        _min = titem->_min;
        _max = titem->_max;
        _min_clamped = titem->_min_clamped;
        _max_clamped = titem->_max_clamped;
        _format = titem->_format;
        _flags = titem->_flags;
        _stor_flags = titem->_stor_flags;
        _last_value = titem->_last_value;
        _size = titem->_size;
    }

    PyObject* mvInputFloatMulti::getPyValue()
    {
        return ToPyFloatList(_value->data(), 4);
    }

    void mvInputFloatMulti::setPyValue(PyObject* value)
    {
        std::vector<float> temp = ToFloatVect(value);
        while (temp.size() < 4)
            temp.push_back(0.0f);
        std::array<float, 4> temp_array;
        for (size_t i = 0; i < temp_array.size(); i++)
            temp_array[i] = temp[i];
        if (_value)
            *_value = temp_array;
        else
            _value = std::make_shared<std::array<float, 4>>(temp_array);
    }

    void mvInputFloatMulti::setDataSource(mvUUID dataSource)
    {
        if (dataSource == _source) return;
        _source = dataSource;

        mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (GetEntityValueType(item->getType()) != GetEntityValueType(getType()))
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
                "Values types do not match: " + std::to_string(dataSource), this);
            return;
        }
        _value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());
    }

    void mvInputFloatMulti::draw(ImDrawList* drawlist, float x, float y)
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

            bool res = false;

            switch (_size)
            {
            case 2:
                res = ImGui::InputFloat2(_internalLabel.c_str(), _value->data(), _format.c_str(), _flags);
                break;
            case 3:
                res = ImGui::InputFloat3(_internalLabel.c_str(), _value->data(), _format.c_str(), _flags);
                break;
            case 4:
                res = ImGui::InputFloat4(_internalLabel.c_str(), _value->data(), _format.c_str(), _flags);
                break;
            default:
                break;
            }

            if (res)
            {
                auto inital_value = *_value;
                // determines clamped cases
                if (_min_clamped && _max_clamped)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (_value->data()[i] < _min) _value->data()[i] = _min;
                        else if (_value->data()[i] > _max) _value->data()[i] = _max;
                    }
                }
                else if (_min_clamped)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (_value->data()[i] < _min) _value->data()[i] = _min;
                    }
                }
                else if (_max_clamped)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (_value->data()[i] > _max) _value->data()[i] = _max;
                    }
                }

                // If the widget is edited through ctrl+click mode the active value will be entered every frame.
                // If the value is out of bounds the value will be overwritten with max or min so each frame the value will be switching between the
                // ctrl+click value and the bounds value until the widget is not in ctrl+click mode. To prevent the callback from running every 
                // frame we check if the value was already submitted.
                if (_last_value != *_value)
                {
                    _last_value = *_value;
                    auto value = *_value;

                    if(_alias.empty())
                        mvSubmitCallback([=]() {
                            mvAddCallback(getCallback(false), _uuid, ToPyFloatList(value.data(), (int)value.size()), _user_data);
                            });
                    else
                        mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), _alias, ToPyFloatList(value.data(), (int)value.size()), _user_data);
                            });
                }
            }

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

    void mvInputIntMulti::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? _flags |= ImGuiInputTextFlags_EnterReturnsTrue : _flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? _stor_flags |= ImGuiInputTextFlags_EnterReturnsTrue : _stor_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        if (PyObject* item = PyDict_GetItemString(dict, "readonly")) ToBool(item) ? _flags |= ImGuiInputTextFlags_ReadOnly : _flags &= ~ImGuiInputTextFlags_ReadOnly;
        if (PyObject* item = PyDict_GetItemString(dict, "readonly")) ToBool(item) ? _stor_flags |= ImGuiInputTextFlags_ReadOnly : _stor_flags &= ~ImGuiInputTextFlags_ReadOnly;
        if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToInt(item);

        if (PyObject* item = PyDict_GetItemString(dict, "min_value"))
        {
            _min = ToInt(item);
            _min_clamped = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "max_value"))
        {
            _max = ToInt(item);
            _max_clamped = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) _min_clamped = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) _max_clamped = ToBool(item);

        if (_enabledLastFrame)
        {
            _enabledLastFrame = false;
            _flags = _stor_flags;
        }

        if (_disabledLastFrame)
        {
            _disabledLastFrame = false;
            _stor_flags = _flags;
            _flags |= ImGuiInputTextFlags_ReadOnly;
            _flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }
    }

    void mvInputIntMulti::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "on_enter", mvPyObject(ToPyBool(_flags & ImGuiInputTextFlags_EnterReturnsTrue)));
        PyDict_SetItemString(dict, "readonly", mvPyObject(ToPyBool(_flags & ImGuiInputTextFlags_ReadOnly)));
        PyDict_SetItemString(dict, "min_value", mvPyObject(ToPyInt(_min)));
        PyDict_SetItemString(dict, "max_value", mvPyObject(ToPyInt(_max)));
        PyDict_SetItemString(dict, "min_clamped", mvPyObject(ToPyBool(_min_clamped)));
        PyDict_SetItemString(dict, "max_clamped", mvPyObject(ToPyBool(_max_clamped)));
        PyDict_SetItemString(dict, "size", mvPyObject(ToPyInt(_size)));
    }

    void mvInputFloatMulti::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) _format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToInt(item);

        if (PyObject* item = PyDict_GetItemString(dict, "min_value"))
        {
            _min = ToFloat(item);
            _min_clamped = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "max_value"))
        {
            _max = ToFloat(item);
            _max_clamped = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) _min_clamped = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) _max_clamped = ToBool(item);

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

        if (_enabledLastFrame)
        {
            _enabledLastFrame = false;
            _flags = _stor_flags;
        }

        if (_disabledLastFrame)
        {
            _disabledLastFrame = false;
            _stor_flags = _flags;
            _flags |= ImGuiInputTextFlags_ReadOnly;
            _flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

    }

    void mvInputFloatMulti::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", mvPyObject(ToPyString(_format)));
        PyDict_SetItemString(dict, "min_value", mvPyObject(ToPyFloat(_min)));
        PyDict_SetItemString(dict, "max_value", mvPyObject(ToPyFloat(_max)));
        PyDict_SetItemString(dict, "min_clamped", mvPyObject(ToPyBool(_min_clamped)));
        PyDict_SetItemString(dict, "max_clamped", mvPyObject(ToPyBool(_max_clamped)));
        PyDict_SetItemString(dict, "size", mvPyObject(ToPyInt(_size)));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
        };

        // window flags
        checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, _flags);
        checkbitset("readonly", ImGuiInputTextFlags_ReadOnly, _flags);
    }

    mvInputInt::mvInputInt(mvUUID uuid)
        : mvAppItem(uuid)
    {
        _last_value = *_value;
    }

    void mvInputInt::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvInputInt*>(item);
        if (_source != 0) _value = titem->_value;
        _disabled_value = titem->_disabled_value;
        _min = titem->_min;
        _max = titem->_max;
        _min_clamped = titem->_min_clamped;
        _max_clamped = titem->_max_clamped;
        _flags = titem->_flags;
        _stor_flags = titem->_stor_flags;
        _last_value = titem->_last_value;
        _step = titem->_step;
        _step_fast = titem->_step_fast;
    }

    void mvInputInt::setDataSource(mvUUID dataSource)
    {
        if (dataSource == _source) return;
        _source = dataSource;

        mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (GetEntityValueType(item->getType()) != GetEntityValueType(getType()))
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
        _last_value = *_value;
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

                    if (_alias.empty())
                        mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), _uuid, ToPyInt(value), _user_data);
                            });
                    else
                        mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), _alias, ToPyInt(value), _user_data);
                            });
                }
            }

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
                        mvAddCallback(_dropCallback, _uuid, payloadActual->getDragData(), nullptr);
                    else
                        mvAddCallback(_dropCallback, _alias, payloadActual->getDragData(), nullptr);
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

    void mvInputFloat::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvInputFloat*>(item);
        if (_source != 0) _value = titem->_value;
        _disabled_value = titem->_disabled_value;
        _min = titem->_min;
        _max = titem->_max;
        _min_clamped = titem->_min_clamped;
        _max_clamped = titem->_max_clamped;
        _flags = titem->_flags;
        _format = titem->_format;
        _stor_flags = titem->_stor_flags;
        _last_value = titem->_last_value;
        _step = titem->_step;
        _step_fast = titem->_step_fast;
    }

    PyObject* mvInputFloat::getPyValue()
    {
        return ToPyFloat(*_value);
    }

    void mvInputFloat::setPyValue(PyObject* value)
    {
        *_value = ToFloat(value);
        _last_value = *_value;
    }

    void mvInputFloat::setDataSource(mvUUID dataSource)
    {
        if (dataSource == _source) return;
        _source = dataSource;

        mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (GetEntityValueType(item->getType()) != GetEntityValueType(getType()))
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

            if (ImGui::InputFloat(_internalLabel.c_str(), _value.get(), _step, _step_fast, _format.c_str(), _flags))
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

                    if (_alias.empty())
                        mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), _uuid, ToPyFloat(value), _user_data);
                            });
                    else
                        mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), _alias, ToPyFloat(value), _user_data);
                            });
                }
            }
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
                        mvAddCallback(_dropCallback, _uuid, payloadActual->getDragData(), nullptr);
                    else
                        mvAddCallback(_dropCallback, _alias, payloadActual->getDragData(), nullptr);
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

        if (PyObject* item = PyDict_GetItemString(dict, "min_value"))
        {
            _min = ToInt(item);
            _min_clamped = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "max_value"))
        {
            _max = ToInt(item);
            _max_clamped = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) _min_clamped = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) _max_clamped = ToBool(item);

        if (_enabledLastFrame)
        {
            _enabledLastFrame = false;
            _flags = _stor_flags;
        }

        if (_disabledLastFrame)
        {
            _disabledLastFrame = false;
            _stor_flags = _flags;
            _flags |= ImGuiInputTextFlags_ReadOnly;
            _flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }
    }

    void mvInputInt::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "on_enter", mvPyObject(ToPyBool(_flags & ImGuiInputTextFlags_EnterReturnsTrue)));
        PyDict_SetItemString(dict, "readonly", mvPyObject(ToPyBool(_flags & ImGuiInputTextFlags_ReadOnly)));
        PyDict_SetItemString(dict, "step", mvPyObject(ToPyInt(_step)));
        PyDict_SetItemString(dict, "step_fast", mvPyObject(ToPyInt(_step_fast)));
        PyDict_SetItemString(dict, "min_value", mvPyObject(ToPyInt(_min)));
        PyDict_SetItemString(dict, "max_value", mvPyObject(ToPyInt(_max)));
        PyDict_SetItemString(dict, "min_clamped", mvPyObject(ToPyBool(_min_clamped)));
        PyDict_SetItemString(dict, "max_clamped", mvPyObject(ToPyBool(_max_clamped)));

    }

    void mvInputFloat::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) _format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "step")) _step = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "step_fast")) _step_fast = ToFloat(item);


        if (PyObject* item = PyDict_GetItemString(dict, "min_value"))
        {
            _min = ToFloat(item);
            _min_clamped = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "max_value"))
        {
            _max = ToFloat(item);
            _max_clamped = true;
        }


        if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) _min_clamped = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) _max_clamped = ToBool(item);

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

        if (_enabledLastFrame)
        {
            _enabledLastFrame = false;
            _flags = _stor_flags;
        }

        if (_disabledLastFrame)
        {
            _disabledLastFrame = false;
            _stor_flags = _flags;
            _flags |= ImGuiInputTextFlags_ReadOnly;
            _flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

    }

    void mvInputFloat::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", mvPyObject(ToPyString(_format)));
        PyDict_SetItemString(dict, "step", mvPyObject(ToPyFloat(_step)));
        PyDict_SetItemString(dict, "step_fast", mvPyObject(ToPyFloat(_step_fast)));
        PyDict_SetItemString(dict, "min_value", mvPyObject(ToPyFloat(_min)));
        PyDict_SetItemString(dict, "max_value", mvPyObject(ToPyFloat(_max)));
        PyDict_SetItemString(dict, "min_clamped", mvPyObject(ToPyBool(_min_clamped)));
        PyDict_SetItemString(dict, "max_clamped", mvPyObject(ToPyBool(_max_clamped)));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
        };

        // window flags
        checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, _flags);
        checkbitset("readonly", ImGuiInputTextFlags_ReadOnly, _flags);

    }
}
