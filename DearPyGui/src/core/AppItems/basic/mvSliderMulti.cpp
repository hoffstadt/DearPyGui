#include "mvSliderMulti.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvAppItem.h"
#include "mvModule_Core.h"
#include "mvApp.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {


    void mvSliderFloatMulti::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        mvPythonParser parser(mvPyDataType::String);
        mvAppItem::AddCommonArgs(parser);
        parser.removeArg("height");

        parser.addArg<mvPyDataType::FloatList>("default_value", mvArgType::KEYWORD, "(0.0, 0.0, 0.0, 0.0)");

        parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD, "4", "number of components");

        parser.addArg<mvPyDataType::Bool>("no_input", mvArgType::KEYWORD, "False", "Disable CTRL+Click or Enter key allowing to input text directly into the widget");
        parser.addArg<mvPyDataType::Bool>("clamped", mvArgType::KEYWORD, "False", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.");

        parser.addArg<mvPyDataType::Float>("min_value", mvArgType::KEYWORD, "0.0");
        parser.addArg<mvPyDataType::Float>("max_value", mvArgType::KEYWORD, "100.0");

        parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD, "'%.3f'");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    void mvSliderIntMulti::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::String);
        mvAppItem::AddCommonArgs(parser);
        parser.removeArg("height");

        parser.addArg<mvPyDataType::IntList>("default_value", mvArgType::KEYWORD, "(0, 0, 0, 0)");

        parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD, "4", "number of components");

        parser.addArg<mvPyDataType::Bool>("no_input", mvArgType::KEYWORD, "False", "Disable CTRL+Click or Enter key allowing to input text directly into the widget");
        parser.addArg<mvPyDataType::Bool>("clamped", mvArgType::KEYWORD, "False", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.");

        parser.addArg<mvPyDataType::Integer>("min_value", mvArgType::KEYWORD, "0");
        parser.addArg<mvPyDataType::Integer>("max_value", mvArgType::KEYWORD, "100");

        parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD, "'%d'");

        parser.finalize();

        parsers->insert({ s_command, parser });

    }

    mvSliderFloatMulti::mvSliderFloatMulti(const std::string& name)
        : mvFloat4PtrBase(name)
    {
    }

    void mvSliderFloatMulti::setEnabled(bool value)
    {
        if (value == m_enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_enabled = value;
    }

    void mvSliderFloatMulti::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (!m_enabled) std::copy(m_value->data(), m_value->data() + 4, m_disabled_value);

        switch (m_size)
        {
        case 2:
            if (ImGui::SliderFloat2(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
            break;
        case 3:
            if (ImGui::SliderFloat3(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
            break;
        case 4:
            if (ImGui::SliderFloat4(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
            break;
        default:
            break;
        }
    }

    mvSliderIntMulti::mvSliderIntMulti(const std::string& name)
        : mvInt4PtrBase(name)
    {
    }

    void mvSliderIntMulti::setEnabled(bool value)
    {
        if (value == m_enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_enabled = value;
    }

    void mvSliderIntMulti::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (!m_enabled) std::copy(m_value->data(), m_value->data() + 4, m_disabled_value);

        switch (m_size)
        {
        case 2:
            if (ImGui::SliderInt2(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
            break;
        case 3:
            if (ImGui::SliderInt3(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
            break;
        case 4:
            if (ImGui::SliderInt4(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
            break;
        default:
            break;
        }

    }

    void mvSliderFloatMulti::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "size")) m_size = ToInt(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_stor_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_stor_flags);

    }

    void mvSliderFloatMulti::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));
        PyDict_SetItemString(dict, "size", ToPyInt(m_size));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, m_flags);

    }

    void mvSliderIntMulti::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) m_min = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) m_max = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "size")) m_size = ToInt(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_flags);
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_stor_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_stor_flags);

    }

    void mvSliderIntMulti::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));
        PyDict_SetItemString(dict, "size", ToPyInt(m_size));

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
