#include "mvInput.h"
#include <misc/cpp/imgui_stdlib.h>
#include <utility>
#include "dearpygui.h"
#include "mvContext.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

namespace Marvel {

    mvInputText::mvInputText(mvUUID uuid)
        :
        mvAppItem(uuid)
    {
    }

    void mvInputText::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvInputText*>(item);
        if (config.source != 0) _value = titem->_value;
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
        if (dataSource == config.source) return;
        config.source = dataSource;

        mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (GetEntityValueType(item->type) != GetEntityValueType(type))
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
        if (!config.show)
            return;

        // focusing
        if (info.focusNextFrame)
        {
            ImGui::SetKeyboardFocusHere();
            info.focusNextFrame = false;
        }

        // cache old cursor position
        ImVec2 previousCursorPos = ImGui::GetCursorPos();

        // set cursor position if user set
        if (info.dirtyPos)
            ImGui::SetCursorPos(state.pos);

        // update widget's position state
        state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

        // set item width
        if (config.width != 0)
            ImGui::SetNextItemWidth((float)config.width);

        // set indent
        if (config.indent > 0.0f)
            ImGui::Indent(config.indent);

        // push font if a font object is attached
        if (font)
        {
            ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
            ImGui::PushFont(fontptr);
        }

        // themes
        apply_local_theming(this);

        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {

            ScopedID id(uuid);

            bool activated = false;

            if (_multiline)
                _hint.clear();

            if (_hint.empty())
            {
                if (_multiline)
                    activated = ImGui::InputTextMultiline(info.internalLabel.c_str(), _value.get(), ImVec2((float)config.width, (float)config.height), _flags);
                else
                    activated = ImGui::InputText(info.internalLabel.c_str(), _value.get(), _flags);
            }
            else
                activated = ImGui::InputTextWithHint(info.internalLabel.c_str(), _hint.c_str(), _value.get(), _flags);

            if (activated)
            {
                auto value = *_value;
                if(config.alias.empty())
                    mvSubmitCallback([=]() {
                    mvAddCallback(getCallback(false), uuid, ToPyString(value), config.user_data);
                        });
                else
                    mvSubmitCallback([=]() {
                    mvAddCallback(getCallback(false), config.alias, ToPyString(value), config.user_data);
                        });
            }

        }

        //-----------------------------------------------------------------------------
        // update state
        //-----------------------------------------------------------------------------
        UpdateAppItemState(state);

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------

        // set cursor position to cached position
        if (info.dirtyPos)
            ImGui::SetCursorPos(previousCursorPos);

        if (config.indent > 0.0f)
            ImGui::Unindent(config.indent);

        // pop font off stack
        if (font)
            ImGui::PopFont();

        // handle popping themes
        cleanup_local_theming(this);

        if (handlerRegistry)
            handlerRegistry->checkEvents(&state);

        // handle drag & drop if used
        apply_drag_drop(this);
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

        if (info.enabledLastFrame)
        {
            info.enabledLastFrame = false;
            _flags = _stor_flags;
        }

        if (info.disabledLastFrame)
        {
            info.disabledLastFrame = false;
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
        if (config.source != 0) _value = titem->_value;
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
        if (dataSource == config.source) return;
        config.source = dataSource;

        mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (GetEntityValueType(item->type) != GetEntityValueType(type))
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
        if (!config.show)
            return;

        // focusing
        if (info.focusNextFrame)
        {
            ImGui::SetKeyboardFocusHere();
            info.focusNextFrame = false;
        }

        // cache old cursor position
        ImVec2 previousCursorPos = ImGui::GetCursorPos();

        // set cursor position if user set
        if (info.dirtyPos)
            ImGui::SetCursorPos(state.pos);

        // update widget's position state
        state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

        // set item width
        if (config.width != 0)
            ImGui::SetNextItemWidth((float)config.width);

        // set indent
        if (config.indent > 0.0f)
            ImGui::Indent(config.indent);

        // push font if a font object is attached
        if (font)
        {
            ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
            ImGui::PushFont(fontptr);
        }

        // themes
        apply_local_theming(this);

        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {
            ScopedID id(uuid);

            bool res = false;

            switch (_size)
            {
            case 2:
                res = ImGui::InputInt2(info.internalLabel.c_str(), _value->data(), _flags);
                break;
            case 3:
                res = ImGui::InputInt3(info.internalLabel.c_str(), _value->data(), _flags);
                break;
            case 4:
                res = ImGui::InputInt4(info.internalLabel.c_str(), _value->data(), _flags);
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

                    if(config.alias.empty())
                        mvSubmitCallback([=]() {
                            mvAddCallback(getCallback(false), uuid, ToPyIntList(value.data(), (int)value.size()), config.user_data);
                            });
                    else
                        mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), config.alias, ToPyIntList(value.data(), (int)value.size()), config.user_data);
                            });
                }
            }
        }

        //-----------------------------------------------------------------------------
        // update state
        //-----------------------------------------------------------------------------
        UpdateAppItemState(state);

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------

        // set cursor position to cached position
        if (info.dirtyPos)
            ImGui::SetCursorPos(previousCursorPos);

        if (config.indent > 0.0f)
            ImGui::Unindent(config.indent);

        // pop font off stack
        if (font)
            ImGui::PopFont();

        // handle popping themes
        cleanup_local_theming(this);

        if (handlerRegistry)
            handlerRegistry->checkEvents(&state);

        // handle drag & drop if used
        apply_drag_drop(this);
    }

    mvInputFloatMulti::mvInputFloatMulti(mvUUID uuid)
        : mvAppItem(uuid)
    {
        _last_value = *_value;
    }

    void mvInputFloatMulti::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvInputFloatMulti*>(item);
        if (config.source != 0) _value = titem->_value;
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
        if (dataSource == config.source) return;
        config.source = dataSource;

        mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (GetEntityValueType(item->type) != GetEntityValueType(type))
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
        if (!config.show)
            return;

        // focusing
        if (info.focusNextFrame)
        {
            ImGui::SetKeyboardFocusHere();
            info.focusNextFrame = false;
        }

        // cache old cursor position
        ImVec2 previousCursorPos = ImGui::GetCursorPos();

        // set cursor position if user set
        if (info.dirtyPos)
            ImGui::SetCursorPos(state.pos);

        // update widget's position state
        state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

        // set item width
        if (config.width != 0)
            ImGui::SetNextItemWidth((float)config.width);

        // set indent
        if (config.indent > 0.0f)
            ImGui::Indent(config.indent);

        // push font if a font object is attached
        if (font)
        {
            ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
            ImGui::PushFont(fontptr);
        }

        // themes
        apply_local_theming(this);

        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {

            ScopedID id(uuid);

            bool res = false;

            switch (_size)
            {
            case 2:
                res = ImGui::InputFloat2(info.internalLabel.c_str(), _value->data(), _format.c_str(), _flags);
                break;
            case 3:
                res = ImGui::InputFloat3(info.internalLabel.c_str(), _value->data(), _format.c_str(), _flags);
                break;
            case 4:
                res = ImGui::InputFloat4(info.internalLabel.c_str(), _value->data(), _format.c_str(), _flags);
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

                    if(config.alias.empty())
                        mvSubmitCallback([=]() {
                            mvAddCallback(getCallback(false), uuid, ToPyFloatList(value.data(), (int)value.size()), config.user_data);
                            });
                    else
                        mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), config.alias, ToPyFloatList(value.data(), (int)value.size()), config.user_data);
                            });
                }
            }

        }

        //-----------------------------------------------------------------------------
        // update state
        //-----------------------------------------------------------------------------
        UpdateAppItemState(state);

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------

        // set cursor position to cached position
        if (info.dirtyPos)
            ImGui::SetCursorPos(previousCursorPos);

        if (config.indent > 0.0f)
            ImGui::Unindent(config.indent);

        // pop font off stack
        if (font)
            ImGui::PopFont();

        // handle popping themes
        cleanup_local_theming(this);

        if (handlerRegistry)
            handlerRegistry->checkEvents(&state);

        // handle drag & drop if used
        apply_drag_drop(this);

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

        if (info.enabledLastFrame)
        {
            info.enabledLastFrame = false;
            _flags = _stor_flags;
        }

        if (info.disabledLastFrame)
        {
            info.disabledLastFrame = false;
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

        if (info.enabledLastFrame)
        {
            info.enabledLastFrame = false;
            _flags = _stor_flags;
        }

        if (info.disabledLastFrame)
        {
            info.disabledLastFrame = false;
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
        if (config.source != 0) _value = titem->_value;
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
        if (dataSource == config.source) return;
        config.source = dataSource;

        mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (GetEntityValueType(item->type) != GetEntityValueType(type))
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
        if (!config.show)
            return;

        // focusing
        if (info.focusNextFrame)
        {
            ImGui::SetKeyboardFocusHere();
            info.focusNextFrame = false;
        }

        // cache old cursor position
        ImVec2 previousCursorPos = ImGui::GetCursorPos();

        // set cursor position if user set
        if (info.dirtyPos)
            ImGui::SetCursorPos(state.pos);

        // update widget's position state
        state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

        // set item width
        if (config.width != 0)
            ImGui::SetNextItemWidth((float)config.width);

        // set indent
        if (config.indent > 0.0f)
            ImGui::Indent(config.indent);

        // push font if a font object is attached
        if (font)
        {
            ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
            ImGui::PushFont(fontptr);
        }

        // themes
        apply_local_theming(this);

        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {

            ScopedID id(uuid);

            if (ImGui::InputInt(info.internalLabel.c_str(), _value.get(), _step, _step_fast, _flags))
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

                    if(config.alias.empty())
                        mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), uuid, ToPyInt(value), config.user_data);
                            });
                    else
                        mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), config.alias, ToPyInt(value), config.user_data);
                            });
                }
            }

        }

        //-----------------------------------------------------------------------------
        // update state
        //-----------------------------------------------------------------------------
        UpdateAppItemState(state);

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------

        // set cursor position to cached position
        if (info.dirtyPos)
            ImGui::SetCursorPos(previousCursorPos);

        if (config.indent > 0.0f)
            ImGui::Unindent(config.indent);

        // pop font off stack
        if (font)
            ImGui::PopFont();

        // handle popping themes
        cleanup_local_theming(this);

        if (handlerRegistry)
            handlerRegistry->checkEvents(&state);

        // handle drag & drop if used
        apply_drag_drop(this);
    }

    mvInputFloat::mvInputFloat(mvUUID uuid)
        : mvAppItem(uuid)
    {
        _last_value = *_value;
    }

    void mvInputFloat::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvInputFloat*>(item);
        if (config.source != 0) _value = titem->_value;
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
        if (dataSource == config.source) return;
        config.source = dataSource;

        mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (GetEntityValueType(item->type) != GetEntityValueType(type))
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
        if (!config.show)
            return;

        // focusing
        if (info.focusNextFrame)
        {
            ImGui::SetKeyboardFocusHere();
            info.focusNextFrame = false;
        }

        // cache old cursor position
        ImVec2 previousCursorPos = ImGui::GetCursorPos();

        // set cursor position if user set
        if (info.dirtyPos)
            ImGui::SetCursorPos(state.pos);

        // update widget's position state
        state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

        // set item width
        if (config.width != 0)
            ImGui::SetNextItemWidth((float)config.width);

        // set indent
        if (config.indent > 0.0f)
            ImGui::Indent(config.indent);

        // push font if a font object is attached
        if (font)
        {
            ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
            ImGui::PushFont(fontptr);
        }

        // themes
        apply_local_theming(this);

        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {

            ScopedID id(uuid);

            if (ImGui::InputFloat(info.internalLabel.c_str(), _value.get(), _step, _step_fast, _format.c_str(), _flags))
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

                    if(config.alias.empty())
                        mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), uuid, ToPyFloat(value), config.user_data);
                            });
                    else
                        mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), config.alias, ToPyFloat(value), config.user_data);
                            });
                }
            }
        }

        //-----------------------------------------------------------------------------
        // update state
        //-----------------------------------------------------------------------------
        UpdateAppItemState(state);

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------

        // set cursor position to cached position
        if (info.dirtyPos)
            ImGui::SetCursorPos(previousCursorPos);

        if (config.indent > 0.0f)
            ImGui::Unindent(config.indent);

        // pop font off stack
        if (font)
            ImGui::PopFont();

        // handle popping themes
        cleanup_local_theming(this);

        if (handlerRegistry)
            handlerRegistry->checkEvents(&state);

        // handle drag & drop if used
        apply_drag_drop(this);
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

        if (info.enabledLastFrame)
        {
            info.enabledLastFrame = false;
            _flags = _stor_flags;
        }

        if (info.disabledLastFrame)
        {
            info.disabledLastFrame = false;
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

        if (info.enabledLastFrame)
        {
            info.enabledLastFrame = false;
            _flags = _stor_flags;
        }

        if (info.disabledLastFrame)
        {
            info.disabledLastFrame = false;
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
