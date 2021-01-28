#include "mvSliderItems.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvAppItem.h"
#include "mvValueStorage.h"
#include "mvMarvel.h"
#include "mvApp.h"
#include <string>

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvSliderFloat
    //-----------------------------------------------------------------------------
    mvSliderFloat::mvSliderFloat(const std::string& name, float default_value, const std::string& dataSource)
        : mvFloatPtrBase(name, default_value, dataSource)
    {
        m_description.disableAllowed = true;
    }

    void mvSliderFloat::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_enabled = value;
    }

    void mvSliderFloat::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrab, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrabActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            m_disabled_value = *m_value;
        }

        if (m_vertical)
        {
            if ((float)m_height < 1.0f)
                m_height = 100;
            if ((float)m_width < 1.0f)
                m_width = 20;

            if (ImGui::VSliderFloat(m_label.c_str(), ImVec2((float)m_width, (float)m_height), m_enabled ? m_value : &m_disabled_value, m_min, m_max, m_format.c_str()))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

        }
        else
        {
            if (ImGui::SliderFloat(m_label.c_str(), m_enabled ? m_value : &m_disabled_value, m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

        }

    }

    void mvSliderFloat::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "vertical")) m_vertical = ToBool(item);
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

    void mvSliderFloat::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "vertical", ToPyBool(m_vertical));
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

    //-----------------------------------------------------------------------------
    // mvSliderFloat2
    //-----------------------------------------------------------------------------
    mvSliderFloat2::mvSliderFloat2(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat2PtrBase(name, default_value, dataSource)
    {
        m_description.disableAllowed = true;
    }

    void mvSliderFloat2::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_enabled = value;
    }

    void mvSliderFloat2::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrab, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrabActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value, m_value + 2, m_disabled_value);
        }

        if (ImGui::SliderFloat2(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

    }

    void mvSliderFloat2::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
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

    void mvSliderFloat2::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", ToPyString(m_format));
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

    //-----------------------------------------------------------------------------
    // mvSliderFloat3
    //-----------------------------------------------------------------------------
    mvSliderFloat3::mvSliderFloat3(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat3PtrBase(name, default_value, dataSource)
    {
        m_description.disableAllowed = true;
    }

    void mvSliderFloat3::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_enabled = value;
    }

    void mvSliderFloat3::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrab, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrabActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value, m_value + 3, m_disabled_value);
        }

        if (ImGui::SliderFloat3(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

    }

    void mvSliderFloat3::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvSliderFloat3::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", ToPyString(m_format));
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

    //-----------------------------------------------------------------------------
    // mvSliderFloat4
    //-----------------------------------------------------------------------------
    mvSliderFloat4::mvSliderFloat4(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat4PtrBase(name, default_value, dataSource)
    {
        m_description.disableAllowed = true;
    }

    void mvSliderFloat4::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_enabled = value;
    }

    void mvSliderFloat4::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrab, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrabActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value, m_value + 4, m_disabled_value);
        }

        if (ImGui::SliderFloat4(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

    }

    void mvSliderFloat4::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
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

    void mvSliderFloat4::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", ToPyString(m_format));
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

    //-----------------------------------------------------------------------------
    // mvSliderInt
    //-----------------------------------------------------------------------------
    mvSliderInt::mvSliderInt(const std::string& name, int default_value, const std::string& dataSource)
        : mvIntPtrBase(name, default_value, dataSource)
    {
        m_description.disableAllowed = true;
    }

    void mvSliderInt::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_enabled = value;
    }

    void mvSliderInt::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrab, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrabActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            m_disabled_value = *m_value;
        }

        if (m_vertical)
        {
            if ((float)m_height < 1.0f)
                m_height = 100;
            if ((float)m_width < 1.0f)
                m_width = 20;

            if (ImGui::VSliderInt(m_label.c_str(), ImVec2((float)m_width, (float)m_height), m_enabled ? m_value : &m_disabled_value, m_min, m_max, m_format.c_str()))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);
        }
        else
        {
            if (ImGui::SliderInt(m_label.c_str(), m_enabled ? m_value : &m_disabled_value, m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

        }

    }

    void mvSliderInt::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "vertical")) m_vertical = ToBool(item);
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

    void mvSliderInt::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "vertical", ToPyBool(m_vertical));
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

    //-----------------------------------------------------------------------------
    // mvSliderInt2
    //-----------------------------------------------------------------------------
    mvSliderInt2::mvSliderInt2(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt2PtrBase(name, default_value, dataSource)
    {
        m_description.disableAllowed = true;
    }

    void mvSliderInt2::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_enabled = value;
    }

    void mvSliderInt2::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrab, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrabActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value, m_value + 2, m_disabled_value);
        }

        if (ImGui::SliderInt2(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);
    }

    void mvSliderInt2::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
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

    void mvSliderInt2::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", ToPyString(m_format));
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

    //-----------------------------------------------------------------------------
    // mvSliderInt3
    //-----------------------------------------------------------------------------
    mvSliderInt3::mvSliderInt3(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt3PtrBase(name, default_value, dataSource)
    {
        m_description.disableAllowed = true;
    }

    void mvSliderInt3::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_enabled = value;
    }

    void mvSliderInt3::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrab, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrabActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value, m_value + 3, m_disabled_value);
        }

        if (ImGui::SliderInt3(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

    }

    void mvSliderInt3::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
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

    void mvSliderInt3::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", ToPyString(m_format));
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

    //-----------------------------------------------------------------------------
    // mvSliderInt4
    //-----------------------------------------------------------------------------
    mvSliderInt4::mvSliderInt4(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt4PtrBase(name, default_value, dataSource)
    {
        m_description.disableAllowed = true;
    }

    void mvSliderInt4::setEnabled(bool value)
    {
        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_enabled = value;
    }

    void mvSliderInt4::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrab, disabled_color);
            styleManager.addColorStyle(ImGuiCol_SliderGrabActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value, m_value + 4, m_disabled_value);
        }

        if (ImGui::SliderInt4(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

    }

    void mvSliderInt4::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
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

    void mvSliderInt4::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", ToPyString(m_format));
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

}
