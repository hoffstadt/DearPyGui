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
        parsers->insert({ s_command, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0, 0.0, 0.0)"},
            {mvPythonDataType::Integer, "size", "number of components", "4"},
            {mvPythonDataType::Float, "min_value", "", "0.0"},
            {mvPythonDataType::Float, "max_value", "", "100.0"},
            {mvPythonDataType::String, "format", "", "'%.3f'"},
            {mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
            {mvPythonDataType::Object, "callback_data", "Callback data", "None"},
            {mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
            {mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
            {mvPythonDataType::String, "source", "", "''"},
            {mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
            {mvPythonDataType::Integer, "width","", "0"},
            {mvPythonDataType::Bool, "no_input", "Disable CTRL+Click or Enter key allowing to input text directly into the widget", "False"},
            {mvPythonDataType::Bool, "clamped", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.", "False"},
            {mvPythonDataType::String, "label", "", "''"},
            {mvPythonDataType::Bool, "show", "Attempt to render", "True"},
        }, "Adds slider for a 4 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

    }

    void mvSliderIntMulti::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_command, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::IntList, "default_value", "", "(0, 0, 0, 0)"},
            {mvPythonDataType::Integer, "size", "number of components", "4"},
            {mvPythonDataType::Integer, "min_value", "", "0"},
            {mvPythonDataType::Integer, "max_value", "", "100"},
            {mvPythonDataType::String, "format", "", "'%d'"},
            {mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
            {mvPythonDataType::Object, "callback_data", "Callback data", "None"},
            {mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
            {mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
            {mvPythonDataType::String, "source", "", "''"},
            {mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
            {mvPythonDataType::Integer, "width","", "0"},
            {mvPythonDataType::Bool, "no_input", "Disable CTRL+Click or Enter key allowing to input text directly into the widget", "False"},
            {mvPythonDataType::Bool, "clamped", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.", "False"},
            {mvPythonDataType::String, "label", "", "''"},
            {mvPythonDataType::Bool, "show", "Attempt to render", "True"},
        }, "Adds slider for a 4 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

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

    void mvSliderFloatMulti::setExtraConfigDict(PyObject* dict)
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

    void mvSliderFloatMulti::getExtraConfigDict(PyObject* dict)
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

    void mvSliderIntMulti::setExtraConfigDict(PyObject* dict)
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

    void mvSliderIntMulti::getExtraConfigDict(PyObject* dict)
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
