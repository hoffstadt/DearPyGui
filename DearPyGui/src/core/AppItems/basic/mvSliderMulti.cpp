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
        mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Widgets" });
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_LABEL |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_USER_DATA |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_ENABLED |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        parser.addArg<mvPyDataType::FloatList>("default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)");

        parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD_ARG, "4", "number of components");

        parser.addArg<mvPyDataType::Bool>("no_input", mvArgType::KEYWORD_ARG, "False", "Disable CTRL+Click or Enter key allowing to input text directly into the widget");
        parser.addArg<mvPyDataType::Bool>("clamped", mvArgType::KEYWORD_ARG, "False", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.");

        parser.addArg<mvPyDataType::Float>("min_value", mvArgType::KEYWORD_ARG, "0.0");
        parser.addArg<mvPyDataType::Float>("max_value", mvArgType::KEYWORD_ARG, "100.0");

        parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%.3f'");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    void mvSliderIntMulti::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Widgets" });
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_LABEL |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_USER_DATA |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_ENABLED |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        parser.addArg<mvPyDataType::IntList>("default_value", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)");

        parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD_ARG, "4", "number of components");

        parser.addArg<mvPyDataType::Bool>("no_input", mvArgType::KEYWORD_ARG, "False", "Disable CTRL+Click or Enter key allowing to input text directly into the widget");
        parser.addArg<mvPyDataType::Bool>("clamped", mvArgType::KEYWORD_ARG, "False", "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.");

        parser.addArg<mvPyDataType::Integer>("min_value", mvArgType::KEYWORD_ARG, "0");
        parser.addArg<mvPyDataType::Integer>("max_value", mvArgType::KEYWORD_ARG, "100");

        parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%d'");

        parser.finalize();

        parsers->insert({ s_command, parser });

    }

    mvSliderFloatMulti::mvSliderFloatMulti(mvUUID uuid)
        : mvFloat4PtrBase(uuid)
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
        ScopedID id(m_uuid);
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (!m_enabled) std::copy(m_value->data(), m_value->data() + 4, m_disabled_value);

        switch (m_size)
        {
        case 2:
            if (ImGui::SliderFloat2(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);
            break;
        case 3:
            if (ImGui::SliderFloat3(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);
            break;
        case 4:
            if (ImGui::SliderFloat4(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);
            break;
        default:
            break;
        }
    }

    mvSliderIntMulti::mvSliderIntMulti(mvUUID uuid)
        : mvInt4PtrBase(uuid)
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
        ScopedID id(m_uuid);
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (!m_enabled) std::copy(m_value->data(), m_value->data() + 4, m_disabled_value);

        switch (m_size)
        {
        case 2:
            if (ImGui::SliderInt2(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);
            break;
        case 3:
            if (ImGui::SliderInt3(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);
            break;
        case 4:
            if (ImGui::SliderInt4(m_label.c_str(), m_enabled ? m_value->data() : &m_disabled_value[0], m_min, m_max, m_format.c_str(), m_flags))
                mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_uuid, nullptr, m_user_data);
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
