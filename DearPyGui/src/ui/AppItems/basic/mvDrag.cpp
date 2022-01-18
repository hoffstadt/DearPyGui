#include "mvDrag.h"
#include <utility>
#include "mvContext.h"
#include "dearpygui.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

namespace Marvel {

    mvDragFloatMulti::mvDragFloatMulti(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvDragFloatMulti::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvDragFloatMulti*>(item);
        if (config.source != 0) value = titem->value;
        disabled_value[0] = titem->disabled_value[0];
        disabled_value[1] = titem->disabled_value[1];
        disabled_value[2] = titem->disabled_value[2];
        disabled_value[3] = titem->disabled_value[3];
        configData.speed = titem->configData.speed;
        configData.minv = titem->configData.minv;
        configData.maxv = titem->configData.maxv;
        configData.format = titem->configData.format;
        configData.flags = titem->configData.flags;
        configData.stor_flags = titem->configData.stor_flags;
        configData.size = titem->configData.size;
    }

    PyObject* mvDragFloatMulti::getPyValue()
    {
        return ToPyFloatList(value->data(), 4);
    }

    void mvDragFloatMulti::setPyValue(PyObject* value)
    {
        std::vector<float> temp = ToFloatVect(value);
        while (temp.size() < 4)
            temp.push_back(0.0f);
        std::array<float, 4> temp_array;
        for (size_t i = 0; i < temp_array.size(); i++)
            temp_array[i] = temp[i];
        if (this->value)
            *this->value = temp_array;
        else
            this->value = std::make_shared<std::array<float, 4>>(temp_array);
    }

    void mvDragFloatMulti::setDataSource(mvUUID dataSource)
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
        this->value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());
    }

    void mvDragFloatMulti::draw(ImDrawList* drawlist, float x, float y)
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

            if(!config.enabled) std::copy(this->value->data(), this->value->data() + 2, this->disabled_value);

            switch (configData.size)
            {
            case 2:
                activated = ImGui::DragFloat2(info.internalLabel.c_str(), config.enabled ? value->data() : &disabled_value[0], configData.speed, configData.minv, configData.maxv, configData.format.c_str(), configData.flags);
                break;
            case 3:
                activated = ImGui::DragFloat3(info.internalLabel.c_str(), config.enabled ? value->data() : &disabled_value[0], configData.speed, configData.minv, configData.maxv, configData.format.c_str(), configData.flags);
                break;
            case 4:
                activated = ImGui::DragFloat4(info.internalLabel.c_str(), config.enabled ? value->data() : &disabled_value[0], configData.speed, configData.minv, configData.maxv, configData.format.c_str(), configData.flags);
                break;
            default:
                break;
            }

            if (activated)
            {
                auto value = *this->value;
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

    void mvDragFloatMulti::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) configData.format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) configData.speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) configData.minv = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) configData.maxv = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "size")) configData.size = ToInt(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, configData.flags);
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, configData.stor_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, configData.flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, configData.stor_flags);

        if (info.enabledLastFrame)
        {
            info.enabledLastFrame = false;
            configData.flags = configData.stor_flags;
        }

        if (info.disabledLastFrame)
        {
            info.disabledLastFrame = false;
            configData.stor_flags = configData.flags;
            configData.flags |= ImGuiSliderFlags_NoInput;
        }

    }

    void mvDragFloatMulti::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        mvPyObject py_format = ToPyString(configData.format);
        mvPyObject py_speed = ToPyFloat(configData.speed);
        mvPyObject py_min_value = ToPyFloat(configData.minv);
        mvPyObject py_max_value = ToPyFloat(configData.maxv);
        mvPyObject py_size = ToPyInt(configData.size);

        PyDict_SetItemString(dict, "format", py_format);
        PyDict_SetItemString(dict, "speed", py_speed);
        PyDict_SetItemString(dict, "min_value", py_min_value);
        PyDict_SetItemString(dict, "max_value", py_max_value);
        PyDict_SetItemString(dict, "size", py_size);

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            mvPyObject py_result = ToPyBool(flags & flag);
            PyDict_SetItemString(dict, keyword, py_result);
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, configData.flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, configData.flags);

    }

    mvDragIntMulti::mvDragIntMulti(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvDragIntMulti::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvDragIntMulti*>(item);
        if (config.source != 0) value = titem->value;
        disabled_value[0] = titem->disabled_value[0];
        disabled_value[1] = titem->disabled_value[1];
        disabled_value[2] = titem->disabled_value[2];
        disabled_value[3] = titem->disabled_value[3];
        configData.speed = titem->configData.speed;
        configData.minv = titem->configData.minv;
        configData.maxv = titem->configData.maxv;
        configData.format = titem->configData.format;
        configData.flags = titem->configData.flags;
        configData.stor_flags = titem->configData.stor_flags;
        configData.size = titem->configData.size;
    }

    PyObject* mvDragIntMulti::getPyValue()
    {
        return ToPyIntList(value->data(), 4);
    }

    void mvDragIntMulti::setPyValue(PyObject* value)
    {
        std::vector<int> temp = ToIntVect(value);
        while (temp.size() < 4)
            temp.push_back(0);
        std::array<int, 4> temp_array;
        for (size_t i = 0; i < temp_array.size(); i++)
            temp_array[i] = temp[i];
        if (this->value)
            *this->value = temp_array;
        else
            this->value = std::make_shared<std::array<int, 4>>(temp_array);
    }

    void mvDragIntMulti::setDataSource(mvUUID dataSource)
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
        value = *static_cast<std::shared_ptr<std::array<int, 4>>*>(item->getValue());
    }

    void mvDragIntMulti::draw(ImDrawList* drawlist, float x, float y)
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

            if(!config.enabled) std::copy(value->data(), value->data() + 2, disabled_value);

            switch (configData.size)
            {
            case 2:
                activated = ImGui::DragInt2(info.internalLabel.c_str(), config.enabled ? value->data() : &disabled_value[0], configData.speed, configData.minv, configData.maxv, configData.format.c_str(), configData.flags);
                break;
            case 3:
                activated = ImGui::DragInt3(info.internalLabel.c_str(), config.enabled ? value->data() : &disabled_value[0], configData.speed, configData.minv, configData.maxv, configData.format.c_str(), configData.flags);
                break;
            case 4:
                activated = ImGui::DragInt4(info.internalLabel.c_str(), config.enabled ? value->data() : &disabled_value[0], configData.speed, configData.minv, configData.maxv, configData.format.c_str(), configData.flags);
                break;
            default:
                break;
            }

            if (activated)
            {
                auto value = *this->value;
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

    void mvDragIntMulti::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) configData.format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) configData.speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) configData.minv = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) configData.maxv = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "size")) configData.size = ToInt(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, configData.flags);
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, configData.stor_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, configData.flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, configData.stor_flags);

        if (info.enabledLastFrame)
        {
            info.enabledLastFrame = false;
            configData.flags = configData.stor_flags;
        }

        if (info.disabledLastFrame)
        {
            info.disabledLastFrame = false;
            configData.stor_flags = configData.flags;
            configData.flags |= ImGuiSliderFlags_NoInput;
        }

    }

    void mvDragIntMulti::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        mvPyObject py_format = ToPyString(configData.format);
        mvPyObject py_speed = ToPyFloat(configData.speed);
        mvPyObject py_min_value = ToPyInt(configData.minv);
        mvPyObject py_max_value = ToPyInt(configData.maxv);
        mvPyObject py_size = ToPyInt(configData.size);

        PyDict_SetItemString(dict, "format", py_format);
        PyDict_SetItemString(dict, "speed", py_speed);
        PyDict_SetItemString(dict, "min_value", py_min_value);
        PyDict_SetItemString(dict, "max_value", py_max_value);
        PyDict_SetItemString(dict, "size", py_size);

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            mvPyObject py_result = ToPyBool(flags & flag);
            PyDict_SetItemString(dict, keyword, py_result);
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, configData.flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, configData.flags);

    }

    mvDragFloat::mvDragFloat(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvDragFloat::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvDragFloat*>(item);
        if (config.source != 0) value = titem->value;
        disabled_value = titem->disabled_value;
        configData.speed = titem->configData.speed;
        configData.minv = titem->configData.minv;
        configData.maxv = titem->configData.maxv;
        configData.format = titem->configData.format;
        configData.flags = titem->configData.flags;
        configData.stor_flags = titem->configData.stor_flags;
    }

    PyObject* mvDragFloat::getPyValue()
    {
        return ToPyFloat(*value);
    }

    void mvDragFloat::setPyValue(PyObject* value)
    {
        *this->value = ToFloat(value);
    }

    void mvDragFloat::setDataSource(mvUUID dataSource)
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
        value = *static_cast<std::shared_ptr<float>*>(item->getValue());
    }

    void mvDragFloat::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id(uuid);


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

            if(!config.enabled) disabled_value = *value;

            if (ImGui::DragFloat(info.internalLabel.c_str(), config.enabled ? value.get() : &disabled_value, configData.speed, configData.minv, configData.maxv, configData.format.c_str(), configData.flags))
            {
                auto value = *this->value;

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

    mvDragInt::mvDragInt(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvDragInt::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvDragInt*>(item);
        if (config.source != 0) value = titem->value;
        disabled_value = titem->disabled_value;
        configData.speed = titem->configData.speed;
        configData.minv = titem->configData.minv;
        configData.maxv = titem->configData.maxv;
        configData.format = titem->configData.format;
        configData.flags = titem->configData.flags;
        configData.stor_flags = titem->configData.stor_flags;
    }

    void mvDragInt::setDataSource(mvUUID dataSource)
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
        value = *static_cast<std::shared_ptr<int>*>(item->getValue());
    }

    PyObject* mvDragInt::getPyValue()
    {
        return ToPyInt(*value);
    }

    void mvDragInt::setPyValue(PyObject* value)
    {
        *this->value = ToInt(value);
    }

    void mvDragInt::draw(ImDrawList* drawlist, float x, float y)
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

            if(!config.enabled) disabled_value = *value;

            if (ImGui::DragInt(info.internalLabel.c_str(), config.enabled ? value.get() : &disabled_value, configData.speed, configData.minv, configData.maxv, configData.format.c_str(), configData.flags))
            {
                auto value = *this->value;
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

    void mvDragFloat::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) configData.format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) configData.speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) configData.minv = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) configData.maxv = ToFloat(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, configData.flags);
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, configData.stor_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, configData.flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, configData.stor_flags);

        if (info.enabledLastFrame)
        {
            info.enabledLastFrame = false;
            configData.flags = configData.stor_flags;
        }

        if (info.disabledLastFrame)
        {
            info.disabledLastFrame = false;
            configData.stor_flags = configData.flags;
            configData.flags |= ImGuiSliderFlags_NoInput;
        }

    }

    void mvDragFloat::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        mvPyObject py_format = ToPyString(configData.format);
        mvPyObject py_speed = ToPyFloat(configData.speed);
        mvPyObject py_min_value = ToPyFloat(configData.minv);
        mvPyObject py_max_value = ToPyFloat(configData.maxv);

        PyDict_SetItemString(dict, "format", py_format);
        PyDict_SetItemString(dict, "speed", py_speed);
        PyDict_SetItemString(dict, "min_value", py_min_value);
        PyDict_SetItemString(dict, "max_value", py_max_value);

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            mvPyObject py_result = ToPyBool(flags & flag);
            PyDict_SetItemString(dict, keyword, py_result);
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, configData.flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, configData.flags);

    }

    void mvDragInt::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) configData.format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) configData.speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) configData.minv = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) configData.maxv = ToInt(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, configData.flags);
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, configData.stor_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, configData.flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, configData.stor_flags);

        if (info.enabledLastFrame)
        {
            info.enabledLastFrame = false;
            configData.flags = configData.stor_flags;
        }

        if (info.disabledLastFrame)
        {
            info.disabledLastFrame = false;
            configData.stor_flags = configData.flags;
            configData.flags |= ImGuiSliderFlags_NoInput;
        }
    }

    void mvDragInt::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        mvPyObject py_format = ToPyString(configData.format);
        mvPyObject py_speed = ToPyFloat(configData.speed);
        mvPyObject py_min_value = ToPyInt(configData.minv);
        mvPyObject py_max_value = ToPyInt(configData.maxv);

        PyDict_SetItemString(dict, "format", py_format);
        PyDict_SetItemString(dict, "speed", py_speed);
        PyDict_SetItemString(dict, "min_value", py_min_value);
        PyDict_SetItemString(dict, "max_value", py_max_value);

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            mvPyObject py_result = ToPyBool(flags & flag);
            PyDict_SetItemString(dict, keyword, py_result);
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, configData.flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, configData.flags);

    }

}