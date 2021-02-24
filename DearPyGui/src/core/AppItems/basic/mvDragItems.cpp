#include "mvDragItems.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvApp.h"
#include "mvMarvel.h"
#include <string>

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvDragFloat
    //-----------------------------------------------------------------------------
    mvDragFloat::mvDragFloat(const std::string& name, float default_value, const std::string& dataSource)
        : mvFloatPtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragFloat::setEnabled(bool value)
    {
        if (value)
        {
            m_flags = m_stor_flags;
        }
        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragFloat::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            m_disabled_value = *m_value;
        }


        if (ImGui::DragFloat(m_label.c_str(), m_core_config.enabled ? m_value.get() : &m_disabled_value, m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragFloat::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragFloatConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragFloat::getConfig()
    {
        return &m_config;
    }



    //-----------------------------------------------------------------------------
    // mvDragFloat2
    //-----------------------------------------------------------------------------
    mvDragFloat2::mvDragFloat2(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat2PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragFloat2::setEnabled(bool value)
    {
        if (value)
        {
            m_flags = m_stor_flags;
        }
        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragFloat2::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value->data(), m_value->data() + 2, m_disabled_value);
        }

        if (ImGui::DragFloat2(m_label.c_str(), m_core_config.enabled ? m_value->data() : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);
    }

    void mvDragFloat2::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragFloatsConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragFloat2::getConfig()
    {
        return &m_config;
    }

    //-----------------------------------------------------------------------------
    // mvDragFloat3
    //-----------------------------------------------------------------------------
    mvDragFloat3::mvDragFloat3(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat3PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragFloat3::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragFloat3::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value->data(), m_value->data() + 3, m_disabled_value);
        }

        if (ImGui::DragFloat3(m_label.c_str(), m_core_config.enabled ? m_value->data() : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragFloat3::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragFloatsConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragFloat3::getConfig()
    {
        return &m_config;
    }

    //-----------------------------------------------------------------------------
    // mvDragFloat4
    //-----------------------------------------------------------------------------
    mvDragFloat4::mvDragFloat4(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat4PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragFloat4::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragFloat4::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value->data(), m_value->data() + 4, m_disabled_value);
        }

        if (ImGui::DragFloat4(m_label.c_str(), m_core_config.enabled ? m_value->data() : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragFloat4::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragFloatsConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragFloat4::getConfig()
    {
        return &m_config;
    }

    //-----------------------------------------------------------------------------
    // mvDragInt
    //-----------------------------------------------------------------------------
    mvDragInt::mvDragInt(const std::string& name, int default_value, const std::string& dataSource)
        : mvIntPtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragInt::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragInt::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            m_disabled_value = *m_value;
        }

        if (ImGui::DragInt(m_label.c_str(), m_core_config.enabled ? m_value.get() : &m_disabled_value, m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragInt::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragIntConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragInt::getConfig()
    {
        return &m_config;
    }

    //-----------------------------------------------------------------------------
    // mvDragInt2
    //-----------------------------------------------------------------------------
    mvDragInt2::mvDragInt2(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt2PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragInt2::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragInt2::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value->data(), m_value->data() + 2, m_disabled_value);
        }

        if (ImGui::DragInt2(m_label.c_str(), m_core_config.enabled ? m_value->data() : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragInt2::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragIntsConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragInt2::getConfig()
    {
        return &m_config;
    }

    //-----------------------------------------------------------------------------
    // mvDragInt3
    //-----------------------------------------------------------------------------
    mvDragInt3::mvDragInt3(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt3PtrBase(name, default_value)
    {
        // empty constructor
    }

    void mvDragInt3::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragInt3::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value->data(), m_value->data() + 3, m_disabled_value);
        }

        if (ImGui::DragInt3(m_label.c_str(), m_core_config.enabled ? m_value->data() : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragInt3::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragIntsConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragInt3::getConfig()
    {
        return &m_config;
    }

    //-----------------------------------------------------------------------------
    // mvDragInt4
    //-----------------------------------------------------------------------------
    mvDragInt4::mvDragInt4(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt4PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragInt4::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragInt4::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value->data(), m_value->data() + 4, m_disabled_value);
        }

        if (ImGui::DragInt4(m_label.c_str(), m_core_config.enabled ? m_value->data() : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragInt4::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragIntsConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragInt4::getConfig()
    {
        return &m_config;
    }

#ifndef MV_CPP

    void mvDragFloat::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragFloat::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
        PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragFloat2::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragFloat2::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
        PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragFloat3::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragFloat3::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
        PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragFloat4::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragFloat4::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
        PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragInt::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToInt(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);
    }

    void mvDragInt::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
        PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragInt2::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToInt(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragInt2::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
        PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragInt3::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToInt(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragInt3::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
        PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragInt4::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) m_speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToInt(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvDragInt4::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "speed", ToPyFloat(m_speed));
        PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

#endif
}
