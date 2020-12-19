#include "mvDragItems.h"
#include "mvTypeBases.h"
#include "mvPythonTranslator.h"
#include <utility>
#include "mvValueStorage.h"
#include "mvGlobalIntepreterLock.h"
#include "mvMarvel.h"
#include <string>

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvDragFloat
    //-----------------------------------------------------------------------------
    mvDragFloat::mvDragFloat(const std::string& name, float default_value, const std::string& dataSource)
        : mvFloatPtrBase(name, default_value, dataSource)
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

        m_enabled = value;
    }

    void mvDragFloat::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;

        if (!m_enabled)
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


        if (ImGui::DragFloat(m_label.c_str(), m_enabled ? m_value : &m_disabled_value, m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

    }

    void mvDragFloat::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
        mvGlobalIntepreterLock gil;
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
        mvGlobalIntepreterLock gil;
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

    //-----------------------------------------------------------------------------
    // mvDragFloat2
    //-----------------------------------------------------------------------------
    mvDragFloat2::mvDragFloat2(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat2PtrBase(name, default_value, dataSource)
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

        m_enabled = value;
    }

    void mvDragFloat2::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value, m_value + 2, m_disabled_value);
        }

        if (ImGui::DragFloat2(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);
    }

    void mvDragFloat2::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
        mvGlobalIntepreterLock gil;
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
        mvGlobalIntepreterLock gil;
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

    //-----------------------------------------------------------------------------
    // mvDragFloat3
    //-----------------------------------------------------------------------------
    mvDragFloat3::mvDragFloat3(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat3PtrBase(name, default_value, dataSource)
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

        m_enabled = value;
    }

    void mvDragFloat3::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value, m_value + 3, m_disabled_value);
        }

        if (ImGui::DragFloat3(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

    }

    void mvDragFloat3::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
        mvGlobalIntepreterLock gil;
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
        mvGlobalIntepreterLock gil;
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

    //-----------------------------------------------------------------------------
    // mvDragFloat4
    //-----------------------------------------------------------------------------
    mvDragFloat4::mvDragFloat4(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat4PtrBase(name, default_value, dataSource)
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

        m_enabled = value;
    }

    void mvDragFloat4::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value, m_value + 4, m_disabled_value);
        }

        if (ImGui::DragFloat4(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

    }

    void mvDragFloat4::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
        mvGlobalIntepreterLock gil;
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
        mvGlobalIntepreterLock gil;
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

    //-----------------------------------------------------------------------------
    // mvDragInt
    //-----------------------------------------------------------------------------
    mvDragInt::mvDragInt(const std::string& name, int default_value, const std::string& dataSource)
        : mvIntPtrBase(name, default_value, dataSource)
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

        m_enabled = value;
    }

    void mvDragInt::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;

        if (!m_enabled)
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

        if (ImGui::DragInt(m_label.c_str(), m_enabled ? m_value : &m_disabled_value, m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

    }

    void mvDragInt::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
        mvGlobalIntepreterLock gil;
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
        mvGlobalIntepreterLock gil;
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

    //-----------------------------------------------------------------------------
    // mvDragInt2
    //-----------------------------------------------------------------------------
    mvDragInt2::mvDragInt2(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt2PtrBase(name, default_value, dataSource)
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

        m_enabled = value;
    }

    void mvDragInt2::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value, m_value + 2, m_disabled_value);
        }

        if (ImGui::DragInt2(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

    }

    void mvDragInt2::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
        mvGlobalIntepreterLock gil;
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
        mvGlobalIntepreterLock gil;
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

    //-----------------------------------------------------------------------------
    // mvDragInt3
    //-----------------------------------------------------------------------------
    mvDragInt3::mvDragInt3(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt3PtrBase(name, default_value, dataSource)
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

        m_enabled = value;
    }

    void mvDragInt3::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value, m_value + 3, m_disabled_value);
        }

        if (ImGui::DragInt3(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

    }

    void mvDragInt3::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
        mvGlobalIntepreterLock gil;
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
        mvGlobalIntepreterLock gil;
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

    //-----------------------------------------------------------------------------
    // mvDragInt4
    //-----------------------------------------------------------------------------
    mvDragInt4::mvDragInt4(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt4PtrBase(name, default_value, dataSource)
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

        m_enabled = value;
    }

    void mvDragInt4::draw()
    {
        auto styleManager = m_styleManager.getScopedStyleManager();
        ScopedID id;

        if (!m_enabled)
        {
            ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            disabled_color.w = 0.392f;
            styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            std::copy(m_value, m_value + 4, m_disabled_value);
        }

        if (ImGui::DragInt4(m_label.c_str(), m_enabled ? m_value : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

    }

    void mvDragInt4::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
        mvGlobalIntepreterLock gil;
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
        mvGlobalIntepreterLock gil;
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

}
