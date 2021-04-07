#include "mvInputSingle.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvModule_Core.h"
#include "mvApp.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

    void mvInputInt::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_command, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::Integer, "default_value", "", "0"},
            {mvPythonDataType::Integer, "min_value", "", "0"},
            {mvPythonDataType::Integer, "max_value", "", "100"},
            {mvPythonDataType::Bool, "min_clamped", "activates and deactivates min_value", "False"},
            {mvPythonDataType::Bool, "max_clamped", "activates and deactivates max_value", "False"},
            {mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
            {mvPythonDataType::Object, "callback_data", "Callback data", "None"},
            {mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
            {mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
            {mvPythonDataType::String, "source", "", "''"},
            {mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
            {mvPythonDataType::Integer, "width","", "0"},
            {mvPythonDataType::Bool, "on_enter", "Only runs callback on enter", "False"},
            {mvPythonDataType::String, "label", "", "''"},
            {mvPythonDataType::Bool, "show", "Attempt to render", "True"},
            {mvPythonDataType::Integer, "step", "", "1"},
            {mvPythonDataType::Integer, "step_fast", "", "100"},
            {mvPythonDataType::Bool, "readonly", "", "False"},
        }, "Adds input for integer values.", "None", "Adding Widgets") });
    }

    void mvInputFloat::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_command, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::Float, "default_value", "", "0.0"},
            {mvPythonDataType::Float, "min_value", "", "0"},
            {mvPythonDataType::Float, "max_value", "", "100"},
            {mvPythonDataType::Bool, "min_clamped", "activates and deactivates min_value", "False"},
            {mvPythonDataType::Bool, "max_clamped", "activates and deactivates max_value", "False"},
            {mvPythonDataType::String, "format", "", "'%.3f'"},
            {mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
            {mvPythonDataType::Object, "callback_data", "Callback data", "None"},
            {mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
            {mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
            {mvPythonDataType::String, "source", "", "''"},
            {mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
            {mvPythonDataType::Integer, "width","", "0"},
            {mvPythonDataType::Bool, "on_enter", "Only runs callback on enter", "False"},
            {mvPythonDataType::String, "label", "", "''"},
            {mvPythonDataType::Bool, "show", "Attempt to render", "True"},
            {mvPythonDataType::Float, "step", "", "0.1"},
            {mvPythonDataType::Float, "step_fast", "", "1.0"},
            {mvPythonDataType::Bool, "readonly", "", "False"},
        }, "Adds input for float values.", "None", "Adding Widgets") });
    }

    mvInputInt::mvInputInt(const std::string& name)
        : mvIntPtrBase(name)
    {
        m_last_value = *m_value;
    }
    
    void mvInputInt::setEnabled(bool value)
    {
        if (value == m_enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiInputTextFlags_ReadOnly;
            m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

        m_enabled = value;
    }

    void mvInputInt::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (ImGui::InputInt(m_label.c_str(), m_value.get(), m_step, m_step_fast, m_flags))
        {
            // determines clamped cases
            if (m_min_clamped && m_max_clamped) 
            {
                if (*m_value < m_min) *m_value = m_min;
                else if (*m_value > m_max) *m_value = m_max;
            }
            else if (m_min_clamped) 
            {
                if (*m_value < m_min) *m_value = m_min;
            }
            else if (m_max_clamped) 
            {
                if (*m_value > m_max) *m_value = m_max;
            }

            // If the widget is edited through ctrl+click mode the active value will be entered every frame.
            // If the value is out of bounds the value will be overwritten with max or min so each frame the value will be switching between the
            // ctrl+click value and the bounds value until the widget is not in ctrl+click mode. To prevent the callback from running every 
            // frame we check if the value was already submitted.
            if (m_last_value != *m_value)
            {
                m_last_value = *m_value;
                mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callback_data);
            }
        }

    }

    mvInputFloat::mvInputFloat(const std::string& name)
        : mvFloatPtrBase(name)
    {
        m_last_value = *m_value;
    }

    void mvInputFloat::setEnabled(bool value)
    {
        if (value == m_enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiInputTextFlags_ReadOnly;
            m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

        m_enabled = value;
    }

    void mvInputFloat::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (ImGui::InputFloat(m_label.c_str(), m_value.get(), m_step, m_step_fast, m_format.c_str(), m_flags))
        {
            auto inital_value = *m_value;
            // determines clamped cases
            if (m_min_clamped && m_max_clamped)
            {
                if (*m_value < m_min) *m_value = m_min;
                else if (*m_value > m_max) *m_value = m_max;
            }
            else if (m_min_clamped)
            {
                if (*m_value < m_min) *m_value = m_min;
            }
            else if (m_max_clamped)
            {
                if (*m_value > m_max) *m_value = m_max;
            }

            // If the widget is edited through ctrl+click mode the active value will be entered every frame.
            // If the value is out of bounds the value will be overwritten with max or min so each frame the value will be switching between the
            // ctrl+click value and the bounds value until the widget is not in ctrl+click mode. To prevent the callback from running every 
            // frame we check if the value was already submitted.
            if (m_last_value != *m_value)
            {
                m_last_value = *m_value;
                mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callback_data);
            }
        }
    }

    void mvInputInt::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? m_flags |= ImGuiInputTextFlags_EnterReturnsTrue : m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? m_stor_flags |= ImGuiInputTextFlags_EnterReturnsTrue : m_stor_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        if (PyObject* item = PyDict_GetItemString(dict, "readonly")) ToBool(item) ? m_flags |= ImGuiInputTextFlags_ReadOnly : m_flags &= ~ImGuiInputTextFlags_ReadOnly;
        if (PyObject* item = PyDict_GetItemString(dict, "readonly")) ToBool(item) ? m_stor_flags |= ImGuiInputTextFlags_ReadOnly : m_stor_flags &= ~ImGuiInputTextFlags_ReadOnly;
        if (PyObject* item = PyDict_GetItemString(dict, "step")) m_step = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "step_fast")) m_step_fast = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) { m_min = ToInt(item); m_min_clamped = true; }
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) { m_max = ToInt(item); m_max_clamped = true; }
        if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) m_min_clamped = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) m_max_clamped = ToBool(item);
    }

    void mvInputInt::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "on_enter", ToPyBool(m_flags & ImGuiInputTextFlags_EnterReturnsTrue));
        PyDict_SetItemString(dict, "readonly", ToPyBool(m_flags & ImGuiInputTextFlags_ReadOnly));
        PyDict_SetItemString(dict, "step", ToPyInt(m_step));
        PyDict_SetItemString(dict, "step_fast", ToPyInt(m_step_fast));
        PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));
        PyDict_SetItemString(dict, "min_clamped", ToPyBool(m_min_clamped));
        PyDict_SetItemString(dict, "max_clamped", ToPyBool(m_max_clamped));

    }

    void mvInputFloat::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "step")) m_step = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "step_fast")) m_step_fast = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) { m_min = ToFloat(item); m_min_clamped = true; }
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) { m_max = ToFloat(item); m_max_clamped = true; }
        if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) m_min_clamped = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) m_max_clamped = ToBool(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);
        flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_stor_flags);
        flagop("readonly", ImGuiInputTextFlags_ReadOnly, m_flags);
        flagop("readonly", ImGuiInputTextFlags_ReadOnly, m_stor_flags);

    }

    void mvInputFloat::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "step", ToPyFloat(m_step));
        PyDict_SetItemString(dict, "step_fast", ToPyFloat(m_step_fast));
        PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));
        PyDict_SetItemString(dict, "min_clamped", ToPyBool(m_min_clamped));
        PyDict_SetItemString(dict, "max_clamped", ToPyBool(m_max_clamped));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);
        checkbitset("readonly", ImGuiInputTextFlags_ReadOnly, m_flags);

    }

}
