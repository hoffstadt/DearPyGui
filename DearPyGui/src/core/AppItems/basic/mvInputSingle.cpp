#include "mvInputSingle.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvModule_DearPyGui.h"
#include "mvApp.h"
#include <string>
#include "mvItemRegistry.h"

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
        : mvIntPtrBase(uuid)
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
        ScopedID id(m_uuid);

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
                auto value = *m_value;
                mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                    mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, ToPyInt(value), m_user_data);
                    });
            }
        }

    }

    mvInputFloat::mvInputFloat(mvUUID uuid)
        : mvFloatPtrBase(uuid)
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
        ScopedID id(m_uuid);

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
                auto value = *m_value;
                mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                    mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, ToPyFloat(value), m_user_data);
                    });
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

        bool minmax_set = false;
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) 
        {
            m_min = ToInt(item); 
            m_min_clamped = true; 
            minmax_set = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) 
        { 
            m_max = ToInt(item); 
            m_max_clamped = true; 
            minmax_set = true;
        }

        if (!minmax_set)
        {
            if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) m_min_clamped = ToBool(item);
            if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) m_max_clamped = ToBool(item);
        }
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


        bool minmax_set = false;
        if (PyObject* item = PyDict_GetItemString(dict, "min_value"))
        {
            m_min = ToFloat(item);
            m_min_clamped = true;
            minmax_set = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "max_value"))
        {
            m_max = ToFloat(item);
            m_max_clamped = true;
            minmax_set = true;
        }

        if (!minmax_set)
        {
            if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) m_min_clamped = ToBool(item);
            if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) m_max_clamped = ToBool(item);
        }

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
