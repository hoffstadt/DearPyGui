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

        mvPythonParser parser(mvPyDataType::String);
        mvAppItem::AddCommonArgs(parser);
        parser.removeArg("height");

        parser.addArg<mvPyDataType::IntList>("default_value", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)");

        parser.addArg<mvPyDataType::Integer>("min_value", mvArgType::KEYWORD_ARG, "0");
        parser.addArg<mvPyDataType::Integer>("max_value", mvArgType::KEYWORD_ARG, "100");
        parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD_ARG, "4", "number of components");
        parser.addArg<mvPyDataType::Integer>("step", mvArgType::KEYWORD_ARG, "1");
        parser.addArg<mvPyDataType::Integer>("step_fast", mvArgType::KEYWORD_ARG, "100");

        parser.addArg<mvPyDataType::Bool>("min_clamped", mvArgType::KEYWORD_ARG, "False", "activates and deactivates min_value");
        parser.addArg<mvPyDataType::Bool>("max_clamped", mvArgType::KEYWORD_ARG, "False", "activates and deactivates max_value");
        parser.addArg<mvPyDataType::Bool>("on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter");
        parser.addArg<mvPyDataType::Bool>("readonly", mvArgType::KEYWORD_ARG, "False");

        parser.finalize();

        parsers->insert({ s_command, parser });


    }

    void mvInputFloat::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        mvPythonParser parser(mvPyDataType::String);
        mvAppItem::AddCommonArgs(parser);
        parser.removeArg("height");

        parser.addArg<mvPyDataType::Float>("default_value", mvArgType::KEYWORD_ARG, "0.0");

        parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%.3f'");

        parser.addArg<mvPyDataType::Float>("min_value", mvArgType::KEYWORD_ARG, "0.0");
        parser.addArg<mvPyDataType::Float>("max_value", mvArgType::KEYWORD_ARG, "100.0");
        parser.addArg<mvPyDataType::Float>("step", mvArgType::KEYWORD_ARG, "0.1");
        parser.addArg<mvPyDataType::Float>("step_fast", mvArgType::KEYWORD_ARG, "1.0");

        parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD_ARG, "4", "number of components");

        parser.addArg<mvPyDataType::Bool>("min_clamped", mvArgType::KEYWORD_ARG, "False", "activates and deactivates min_value");
        parser.addArg<mvPyDataType::Bool>("max_clamped", mvArgType::KEYWORD_ARG, "False", "activates and deactivates max_value");
        parser.addArg<mvPyDataType::Bool>("on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter");
        parser.addArg<mvPyDataType::Bool>("readonly", mvArgType::KEYWORD_ARG, "False");

        parser.finalize();

        parsers->insert({ s_command, parser });
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

    void mvInputInt::handleSpecificKeywordArgs(PyObject* dict)
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

    void mvInputInt::getSpecificConfiguration(PyObject* dict)
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

    void mvInputFloat::handleSpecificKeywordArgs(PyObject* dict)
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

    void mvInputFloat::getSpecificConfiguration(PyObject* dict)
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
