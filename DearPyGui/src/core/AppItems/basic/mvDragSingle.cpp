#include "mvDragSingle.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

    void mvDragFloat::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_command, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::Float, "default_value", "", "0.0"},
            {mvPythonDataType::Float, "speed", "", "1.0"},
            {mvPythonDataType::Float, "min_value", "", "0.0"},
            {mvPythonDataType::Float, "max_value", "", "100.0"},
            {mvPythonDataType::String, "format", "", "'%0.3f'"},
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
        }, "Adds drag for a single float value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });
    }

    void mvDragInt::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_command, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::Integer, "default_value", "", "0"},
            {mvPythonDataType::Float, "speed", "", "1.0"},
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
        }, "Adds drag for a single int value. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });
    }

    mvDragFloat::mvDragFloat(const std::string& name)
        : mvFloatPtrBase(name)
    {
    }

    void mvDragFloat::setEnabled(bool value)
    {
        if (value == m_enabled)
            return;

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

    void mvDragFloat::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (!m_enabled) m_disabled_value = *m_value;

        if (ImGui::DragFloat(m_label.c_str(), m_enabled ? m_value.get() : &m_disabled_value, m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);

    }

    mvDragInt::mvDragInt(const std::string& name)
        : mvIntPtrBase(name)
    {
    }

    void mvDragInt::setEnabled(bool value)
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

    void mvDragInt::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (!m_enabled) m_disabled_value = *m_value;

        if (ImGui::DragInt(m_label.c_str(), m_enabled ? m_value.get() : &m_disabled_value, m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callback_data);
    }

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
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_stor_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_stor_flags);

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
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, m_stor_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, m_stor_flags);
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

}
