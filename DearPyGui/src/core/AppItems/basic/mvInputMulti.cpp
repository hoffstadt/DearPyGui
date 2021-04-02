#include "mvInputMulti.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvModule_Core.h"
#include "mvApp.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

    void mvInputIntMulti::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_command, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::IntList, "default_value", "", "(0, 0, 0, 0)"},
            {mvPythonDataType::Integer, "size", "number of components", "4"},
            {mvPythonDataType::Integer, "min_value", "", "0"},
            {mvPythonDataType::Integer, "max_value", "", "100"},
            {mvPythonDataType::Bool, "min_clamped", "activates and deactivates min_value", "False"},
            {mvPythonDataType::Bool, "max_clamped", "activates and deactivates max_value", "False"},
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
            {mvPythonDataType::Bool, "readonly", "", "False"},
        }, "Adds input for 4 integer values.", "None", "Adding Widgets") });
    }

    void mvInputFloatMulti::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_command, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0, 0.0, 0.0)"},
            {mvPythonDataType::Integer, "size", "number of components", "4"},
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
            {mvPythonDataType::Bool, "readonly", "", "False"},
        }, "Adds input for 4 float values.", "None", "Adding Widgets") });
    }

    mvInputIntMulti::mvInputIntMulti(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt4PtrBase(name, default_value)
    {
        m_last_value = *m_value;
    }

    void mvInputIntMulti::setEnabled(bool value)
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

    void mvInputIntMulti::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        bool res = false;

        switch (m_size)
        {
        case 2:
            res = ImGui::InputInt2(m_label.c_str(), m_value->data(), m_flags);
            break;
        case 3:
            res = ImGui::InputInt3(m_label.c_str(), m_value->data(), m_flags);
            break;
        case 4:
            res = ImGui::InputInt4(m_label.c_str(), m_value->data(), m_flags);
            break;
        default:
            break;
        }

        if (res)
        {
            auto inital_value = *m_value;
            // determines clamped cases
            if (m_min_clamped && m_max_clamped)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (m_value->data()[i] < m_min) m_value->data()[i] = m_min;
                    else if (m_value->data()[i] > m_max) m_value->data()[i] = m_max;
                }
            }
            else if (m_min_clamped)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (m_value->data()[i] < m_min) m_value->data()[i] = m_min;
                }
            }
            else if (m_max_clamped)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (m_value->data()[i] > m_max) m_value->data()[i] = m_max;
                }
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

    mvInputFloatMulti::mvInputFloatMulti(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat4PtrBase(name, default_value)
    {
        m_last_value = *m_value;
    }

    void mvInputFloatMulti::setEnabled(bool value)
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

    void mvInputFloatMulti::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        bool res = false;

        switch (m_size)
        {
        case 2:
            res = ImGui::InputFloat2(m_label.c_str(), m_value->data(), m_format.c_str(), m_flags);
            break;
        case 3:
            res = ImGui::InputFloat3(m_label.c_str(), m_value->data(), m_format.c_str(), m_flags);
            break;
        case 4:
            res = ImGui::InputFloat4(m_label.c_str(), m_value->data(), m_format.c_str(), m_flags);
            break;
        default:
            break;
        }

        if (res)
        {
            auto inital_value = *m_value;
            // determines clamped cases
            if (m_min_clamped && m_max_clamped)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (m_value->data()[i] < m_min) m_value->data()[i] = m_min;
                    else if (m_value->data()[i] > m_max) m_value->data()[i] = m_max;
                }
            }
            else if (m_min_clamped)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (m_value->data()[i] < m_min) m_value->data()[i] = m_min;
                }
            }
            else if (m_max_clamped)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (m_value->data()[i] > m_max) m_value->data()[i] = m_max;
                }
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

    void mvInputIntMulti::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? m_flags |= ImGuiInputTextFlags_EnterReturnsTrue : m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? m_stor_flags |= ImGuiInputTextFlags_EnterReturnsTrue : m_stor_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        if (PyObject* item = PyDict_GetItemString(dict, "readonly")) ToBool(item) ? m_flags |= ImGuiInputTextFlags_ReadOnly : m_flags &= ~ImGuiInputTextFlags_ReadOnly;
        if (PyObject* item = PyDict_GetItemString(dict, "readonly")) ToBool(item) ? m_stor_flags |= ImGuiInputTextFlags_ReadOnly : m_stor_flags &= ~ImGuiInputTextFlags_ReadOnly;
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) { m_min = ToInt(item); m_min_clamped = true; }
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) { m_max = ToInt(item); m_max_clamped = true; }
        if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) m_min_clamped = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) m_max_clamped = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "size")) m_size = ToInt(item);
    }

    void mvInputIntMulti::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "on_enter", ToPyBool(m_flags & ImGuiInputTextFlags_EnterReturnsTrue));
        PyDict_SetItemString(dict, "readonly", ToPyBool(m_flags & ImGuiInputTextFlags_ReadOnly));
        PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));
        PyDict_SetItemString(dict, "min_clamped", ToPyBool(m_min_clamped));
        PyDict_SetItemString(dict, "max_clamped", ToPyBool(m_max_clamped));
        PyDict_SetItemString(dict, "size", ToPyInt(m_size));
    }

    void mvInputFloatMulti::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) { m_min = ToFloat(item); m_min_clamped = true; }
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) { m_max = ToFloat(item); m_max_clamped = true; }
        if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) m_min_clamped = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) m_max_clamped = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "size")) m_size = ToInt(item);

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

    void mvInputFloatMulti::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", ToPyString(m_format));
        PyDict_SetItemString(dict, "min_value", ToPyFloat(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyFloat(m_max));
        PyDict_SetItemString(dict, "min_clamped", ToPyBool(m_min_clamped));
        PyDict_SetItemString(dict, "max_clamped", ToPyBool(m_max_clamped));
        PyDict_SetItemString(dict, "size", ToPyInt(m_size));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, m_flags);
        checkbitset("readonly", ImGuiInputTextFlags_ReadOnly, m_flags);
    }

    PyObject* mvInputIntMulti::add_input_intx(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        static int i = 0; i++;
        std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
        const char* name = sname.c_str();
        PyObject* default_value = PyTuple_New(4);
        PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
        int size = 4;
        int min_value = 0;
        int max_value = 100;
        int min_clamped = false;
        int max_clamped = false;
        PyObject* callback = nullptr;
        PyObject* callback_data = nullptr;
        int width = 0;
        const char* before = "";
        const char* parent = "";
        const char* source = "";
        int enabled = true;
        int on_enter = false;
        const char* label = "";
        int show = false;
        int readonly = false;

        if (!(mvApp::GetApp()->getParsers())["add_input_int4"].parse(args, kwargs, __FUNCTION__, &name,
            &default_value, &size, &min_value, &max_value, &min_clamped, &max_clamped, &callback, &callback_data, &parent, &before, &source, &enabled, &width, &on_enter,
            &label, &show, &readonly))
            return ToPyBool(false);

        auto vec = ToIntVect(default_value);
        auto item = CreateRef<mvInputIntMulti>(name, vec.data(), source);
        if (callback)
            Py_XINCREF(callback);
        item->setCallback(callback);
        if (callback_data)
            Py_XINCREF(callback_data);
        item->setCallbackData(callback_data);

        item->checkConfigDict(kwargs);
        item->setConfigDict(kwargs);
        item->setExtraConfigDict(kwargs);

        mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

        return ToPyString(name);
    }

    PyObject* mvInputFloatMulti::add_input_floatx(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        static int i = 0; i++;
        std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
        const char* name = sname.c_str();
        PyObject* default_value = PyTuple_New(4);
        PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
        int size = 4;
        float min_value = 0.0f;
        float max_value = 100.0f;
        int min_clamped = false;
        int max_clamped = false;
        const char* format = "%.3f";
        PyObject* callback = nullptr;
        PyObject* callback_data = nullptr;
        int width = 0;
        const char* before = "";
        const char* parent = "";
        const char* source = "";
        int enabled = true;
        int on_enter = false;
        const char* label = "";
        int show = false;
        int readonly = false;

        if (!(mvApp::GetApp()->getParsers())["add_input_float4"].parse(args, kwargs, __FUNCTION__, &name,
            &default_value, &size, &min_value, &max_value, &min_clamped, &max_clamped, &format, &callback, &callback_data,
            &parent, &before, &source, &enabled, &width, &on_enter,
            &label, &show, &readonly))
            return ToPyBool(false);

        auto vec = ToFloatVect(default_value);

        auto item = CreateRef<mvInputFloatMulti>(name, vec.data(), source);
        if (callback)
            Py_XINCREF(callback);
        item->setCallback(callback);
        if (callback_data)
            Py_XINCREF(callback_data);
        item->setCallbackData(callback_data);

        item->checkConfigDict(kwargs);
        item->setConfigDict(kwargs);
        item->setExtraConfigDict(kwargs);

        mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

        return ToPyString(name);
    }

}
