#include "mvSliderSingle.h"
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

    void mvSliderFloat::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_command, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::Float, "default_value", "", "0.0"},
            {mvPythonDataType::Float, "min_value", "", "0.0"},
            {mvPythonDataType::Float, "max_value", "", "100.0"},
            {mvPythonDataType::String, "format", "", "'%.3f'"},
            {mvPythonDataType::Bool, "vertical", "sets orientation to vertical", "False"},
            {mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
            {mvPythonDataType::Object, "callback_data", "Callback data", "None"},
            {mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
            {mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
            {mvPythonDataType::String, "source", "", "''"},
            {mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
            {mvPythonDataType::Integer, "width","", "0"},
            {mvPythonDataType::Integer, "height","Height of a vertical slider", "0"},
            {mvPythonDataType::Bool, "no_input", "Disable CTRL+Click or Enter key allowing to input text directly into the widget", "False"},
            {mvPythonDataType::Bool, "clamped", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.", "False"},
            {mvPythonDataType::String, "label", "", "''"},
            {mvPythonDataType::Bool, "show", "Attempt to render", "True"},
        }, "Adds slider for a single float value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

    }

    void mvSliderInt::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ "add_slider_int", mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::Integer, "default_value", "", "0"},
            {mvPythonDataType::Integer, "min_value", "", "0"},
            {mvPythonDataType::Integer, "max_value", "", "100"},
            {mvPythonDataType::String, "format", "", "'%d'"},
            {mvPythonDataType::Bool, "vertical", "sets orientation to vertical", "False"},
            {mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
            {mvPythonDataType::Object, "callback_data", "Callback data", "None"},
            {mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
            {mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
            {mvPythonDataType::String, "source", "", "''"},
            {mvPythonDataType::Bool, "enabled", "Display grayed out text so selectable cannot be selected", "True"},
            {mvPythonDataType::Integer, "width","", "0"},
            {mvPythonDataType::Integer, "height","Height of a vertical slider", "0"},
            {mvPythonDataType::Bool, "no_input", "Disable CTRL+Click or Enter key allowing to input text directly into the widget", "False"},
            {mvPythonDataType::Bool, "clamped", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.", "False"},
            {mvPythonDataType::String, "label", "", "''"},
            {mvPythonDataType::Bool, "show", "Attempt to render", "True"},
        }, "Adds slider for a single int value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });

    }

    mvSliderFloat::mvSliderFloat(const std::string& name)
        : mvFloatPtrBase(name)
    {
    }

    void mvSliderFloat::setEnabled(bool value)
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

    void mvSliderFloat::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (!m_enabled) m_disabled_value = *m_value;

        if (m_vertical)
        {
            if ((float)m_height < 1.0f)
                m_height = 100;
            if ((float)m_width < 1.0f)
                m_width = 20;

            if (ImGui::VSliderFloat(m_label.c_str(), ImVec2((float)m_width, (float)m_height), m_enabled ? m_value.get() : &m_disabled_value, m_min, m_max, m_format.c_str()))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);

        }
        else
        {
            if (ImGui::SliderFloat(m_label.c_str(), m_enabled ? m_value.get() : &m_disabled_value, m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);

        }
    }

    mvSliderInt::mvSliderInt(const std::string& name)
        : mvIntPtrBase(name)
    {
    }

    void mvSliderInt::setEnabled(bool value)
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

    void mvSliderInt::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (!m_enabled) m_disabled_value = *m_value;

        if (m_vertical)
        {
            if ((float)m_height < 1.0f)
                m_height = 100;
            if ((float)m_width < 1.0f)
                m_width = 20;

            if (ImGui::VSliderInt(m_label.c_str(), ImVec2((float)m_width, (float)m_height), m_enabled ? m_value.get() : &m_disabled_value, m_min, m_max, m_format.c_str()))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
        }
        else
        {
            if (ImGui::SliderInt(m_label.c_str(), m_enabled ? m_value.get() : &m_disabled_value, m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);

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
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_stor_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_stor_flags);

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
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_stor_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_stor_flags);

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

}
