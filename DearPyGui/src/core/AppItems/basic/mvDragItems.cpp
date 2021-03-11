#include "mvDragItems.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvThemeManager.h"

namespace Marvel {

    void mvDragFloat::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ "add_drag_float", mvPythonParser({
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

    void mvDragFloat2::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ "add_drag_float2", mvPythonParser({
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0)"},
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
        }, "Adds drag for a 2 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });
    }

    void mvDragFloat3::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ "add_drag_float3", mvPythonParser({
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0, 0.0)"},
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
        }, "Adds drag for a 3 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });
    }

    void mvDragFloat4::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ "add_drag_float4", mvPythonParser({
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0, 0.0, 0.0)"},
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
        }, "Adds drag for a 4 float values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });
    }

    void mvDragInt::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ "add_drag_int", mvPythonParser({
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

    void mvDragInt2::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ "add_drag_int2", mvPythonParser({
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::IntList, "default_value", "", "(0, 0)"},
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
        }, "Adds drag for a 2 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });


    }

    void mvDragInt3::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ "add_drag_int3", mvPythonParser({
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::IntList, "default_value", "", "(0, 0, 0)"},
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
        }, "Adds drag for a 3 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });
    }

    void mvDragInt4::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ "add_drag_int4", mvPythonParser({
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::IntList, "default_value", "", "(0, 0, 0, 0)"},
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
        }, "Adds drag for a 4 int values. CTRL+Click to directly modify the value.", "None", "Adding Widgets") });
    }

    mvDragFloat::mvDragFloat(const std::string& name, float default_value, const std::string& dataSource)
        : mvFloatPtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragFloat::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
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

        m_core_config.enabled = value;
    }

    void mvDragFloat::draw()
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            //ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            //disabled_color.w = 0.392f;
            //styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            //m_disabled_value = *m_value;
        }


        if (ImGui::DragFloat(m_label.c_str(), m_core_config.enabled ? m_value.get() : &m_disabled_value, m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragFloat::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragFloatConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragFloat::getConfig()
    {
        return &m_config;
    }

    mvDragFloat2::mvDragFloat2(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat2PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragFloat2::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
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

        m_core_config.enabled = value;
    }

    void mvDragFloat2::draw()
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            //ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            //disabled_color.w = 0.392f;
            //styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            //std::copy(m_value->data(), m_value->data() + 2, m_disabled_value);
        }

        if (ImGui::DragFloat2(m_label.c_str(), m_core_config.enabled ? m_value->data() : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);
    }

    void mvDragFloat2::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragFloatsConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragFloat2::getConfig()
    {
        return &m_config;
    }

    mvDragFloat3::mvDragFloat3(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat3PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragFloat3::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragFloat3::draw()
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            //ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            //disabled_color.w = 0.392f;
            //styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            //std::copy(m_value->data(), m_value->data() + 3, m_disabled_value);
        }

        if (ImGui::DragFloat3(m_label.c_str(), m_core_config.enabled ? m_value->data() : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragFloat3::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragFloatsConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragFloat3::getConfig()
    {
        return &m_config;
    }

    mvDragFloat4::mvDragFloat4(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat4PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragFloat4::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragFloat4::draw()
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            //ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            //disabled_color.w = 0.392f;
            //styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            //std::copy(m_value->data(), m_value->data() + 4, m_disabled_value);
        }

        if (ImGui::DragFloat4(m_label.c_str(), m_core_config.enabled ? m_value->data() : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragFloat4::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragFloatsConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragFloat4::getConfig()
    {
        return &m_config;
    }

    mvDragInt::mvDragInt(const std::string& name, int default_value, const std::string& dataSource)
        : mvIntPtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragInt::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragInt::draw()
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            //ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            //disabled_color.w = 0.392f;
            //styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            //m_disabled_value = *m_value;
        }

        if (ImGui::DragInt(m_label.c_str(), m_core_config.enabled ? m_value.get() : &m_disabled_value, m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragInt::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragIntConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragInt::getConfig()
    {
        return &m_config;
    }

    mvDragInt2::mvDragInt2(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt2PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragInt2::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragInt2::draw()
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            //ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            //disabled_color.w = 0.392f;
            //styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            //std::copy(m_value->data(), m_value->data() + 2, m_disabled_value);
        }

        if (ImGui::DragInt2(m_label.c_str(), m_core_config.enabled ? m_value->data() : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragInt2::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragIntsConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragInt2::getConfig()
    {
        return &m_config;
    }

    mvDragInt3::mvDragInt3(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt3PtrBase(name, default_value)
    {
        // empty constructor
    }

    void mvDragInt3::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragInt3::draw()
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            //ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            //disabled_color.w = 0.392f;
            //styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            //std::copy(m_value->data(), m_value->data() + 3, m_disabled_value);
        }

        if (ImGui::DragInt3(m_label.c_str(), m_core_config.enabled ? m_value->data() : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragInt3::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragIntsConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragInt3::getConfig()
    {
        return &m_config;
    }

    mvDragInt4::mvDragInt4(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt4PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
    }

    void mvDragInt4::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiSliderFlags_NoInput;
        }

        m_core_config.enabled = value;
    }

    void mvDragInt4::draw()
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);

        if (!m_core_config.enabled)
        {
            //ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            //disabled_color.w = 0.392f;
            //styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
            //styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
            //std::copy(m_value->data(), m_value->data() + 4, m_disabled_value);
        }

        if (ImGui::DragInt4(m_label.c_str(), m_core_config.enabled ? m_value->data() : &m_disabled_value[0], m_speed, m_min, m_max, m_format.c_str(), m_flags))
            mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

    }

    void mvDragInt4::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvDragIntsConfig*)config;

        m_core_config.width = config->width;
        m_core_config.label = config->label;
        m_core_config.show = config->show;
        m_core_config.callback = config->callback;
        m_core_config.callback_data = config->callback_data;
        m_core_config.enabled = config->enabled;

        m_config.source = aconfig->source;

        if (config != &m_config)
            m_config = *aconfig;
    }

    mvAppItemConfig* mvDragInt4::getConfig()
    {
        return &m_config;
    }

#ifdef MV_CPP
#else

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

    void mvDragFloat2::setExtraConfigDict(PyObject* dict)
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

    void mvDragFloat2::getExtraConfigDict(PyObject* dict)
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

    void mvDragFloat3::setExtraConfigDict(PyObject* dict)
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

    void mvDragFloat3::getExtraConfigDict(PyObject* dict)
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

    void mvDragFloat4::setExtraConfigDict(PyObject* dict)
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

    void mvDragFloat4::getExtraConfigDict(PyObject* dict)
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

    void mvDragInt2::setExtraConfigDict(PyObject* dict)
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

    void mvDragInt2::getExtraConfigDict(PyObject* dict)
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

    void mvDragInt3::setExtraConfigDict(PyObject* dict)
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

    void mvDragInt3::getExtraConfigDict(PyObject* dict)
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

    void mvDragInt4::setExtraConfigDict(PyObject* dict)
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

    void mvDragInt4::getExtraConfigDict(PyObject* dict)
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

    PyObject* add_drag_float(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* name;
        float default_value = 0.0f;
        float speed = 1.0f;
        float min_value = 0.0f;
        float max_value = 100.0f;
        const char* format = "%.3f";
        PyObject* callback = nullptr;
        PyObject* callback_data = nullptr;
        int width = 0;
        const char* before = "";
        const char* parent = "";
        const char* source = "";
        int enable = true;
        int no_input = false;
        int clamped = false;
        const char* label = "";
        int show = true;

        if (!(mvApp::GetApp()->getParsers())["add_drag_float"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
            &min_value, &max_value, &format, &callback, &callback_data, &parent, &before, &source, &enable, &width, &no_input, &clamped,
            &label, &show))
            return ToPyBool(false);

        auto item = CreateRef<mvDragFloat>(name, default_value, source);
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

        return GetPyNone();
    }

    PyObject* add_drag_float2(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* name;
        PyObject* default_value = PyTuple_New(2);
        PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
        PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
        float speed = 1.0f;
        float min_value = 0.0f;
        float max_value = 100.0f;
        const char* format = "%.3f";
        PyObject* callback = nullptr;
        PyObject* callback_data = nullptr;
        const char* parent = "";
        const char* before = "";
        const char* source = "";
        int enable = true;
        int width = 0;
        int no_input = false;
        int clamped = false;
        const char* label = "";
        int show = true;

        if (!(mvApp::GetApp()->getParsers())["add_drag_float2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
            &min_value, &max_value, &format, &callback, &callback_data, &parent, &before, &source, &enable, &width, &no_input, &clamped,
            &label, &show))
            return ToPyBool(false);

        auto vec = ToFloatVect(default_value);
        auto item = CreateRef<mvDragFloat2>(name, vec.data(), source);
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

        return GetPyNone();
    }

    PyObject* add_drag_float3(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* name;
        PyObject* default_value = PyTuple_New(3);
        PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
        PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
        PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
        float speed = 1.0f;
        float min_value = 0.0f;
        float max_value = 100.0f;
        const char* format = "%.3f";
        PyObject* callback = nullptr;
        PyObject* callback_data = nullptr;
        const char* parent = "";
        const char* before = "";
        const char* source = "";
        int enable = true;
        int width = 0;
        int no_input = false;
        int clamped = false;
        const char* label = "";
        int show = true;

        if (!(mvApp::GetApp()->getParsers())["add_drag_float3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
            &min_value, &max_value, &format, &callback, &callback_data, &parent, &before, &source, &enable, &width, &no_input, &clamped,
            &label, &show))
            return ToPyBool(false);

        auto vec = ToFloatVect(default_value);
        auto item = CreateRef<mvDragFloat3>(name, vec.data(), source);
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

        return GetPyNone();
    }

    PyObject* add_drag_float4(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* name;
        PyObject* default_value = PyTuple_New(4);
        PyTuple_SetItem(default_value, 0, PyFloat_FromDouble(0.0));
        PyTuple_SetItem(default_value, 1, PyFloat_FromDouble(0.0));
        PyTuple_SetItem(default_value, 2, PyFloat_FromDouble(0.0));
        PyTuple_SetItem(default_value, 3, PyFloat_FromDouble(0.0));
        float speed = 1.0f;
        float min_value = 0.0f;
        float max_value = 100.0f;
        const char* format = "%.3f";
        PyObject* callback = nullptr;
        PyObject* callback_data = nullptr;
        const char* parent = "";
        const char* before = "";
        const char* source = "";
        int enable = true;
        int width = 0;
        int no_input = false;
        int clamped = false;
        const char* label = "";
        int show = true;

        if (!(mvApp::GetApp()->getParsers())["add_drag_float4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
            &min_value, &max_value, &format, &callback, &callback_data, &parent, &before, &source, &enable, &width, &no_input, &clamped,
            &label, &show))
            return ToPyBool(false);

        auto vec = ToFloatVect(default_value);

        auto item = CreateRef<mvDragFloat4>(name, vec.data(), source);
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

        return GetPyNone();
    }

    PyObject* add_drag_int(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* name;
        int default_value = 0;
        float speed = 1.0f;
        int min_value = 0;
        int max_value = 100;
        const char* format = "%d";
        PyObject* callback = nullptr;
        PyObject* callback_data = nullptr;
        const char* parent = "";
        const char* before = "";
        const char* source = "";
        int enable = true;
        int width = 0;
        int no_input = false;
        int clamped = false;
        const char* label = "";
        const char* popup = "";
        int show = true;

        if (!(mvApp::GetApp()->getParsers())["add_drag_int"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
            &min_value, &max_value, &format, &callback, &callback_data, &parent, &before, &source, &enable, &width, &no_input, &clamped,
            &label, &popup, &show))
            return ToPyBool(false);

        auto item = CreateRef<mvDragInt>(name, default_value, source);
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

        return GetPyNone();
    }

    PyObject* add_drag_int2(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* name;
        PyObject* default_value = PyTuple_New(2);
        PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
        float speed = 1.0f;
        int min_value = 0;
        int max_value = 100;
        const char* format = "%d";
        PyObject* callback = nullptr;
        PyObject* callback_data = nullptr;
        const char* parent = "";
        const char* before = "";
        const char* source = "";
        int enable = true;
        int width = 0;
        int no_input = false;
        int clamped = false;
        const char* label = "";
        int show = true;

        if (!(mvApp::GetApp()->getParsers())["add_drag_int2"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
            &min_value, &max_value, &format, &callback, &callback_data, &parent, &before, &source, &enable, &width, &no_input, &clamped,
            &label, &show))
            return ToPyBool(false);

        auto vec = ToIntVect(default_value);
        auto item = CreateRef<mvDragInt2>(name, vec.data(), source);
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

        return GetPyNone();
    }

    PyObject* add_drag_int3(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* name;
        PyObject* default_value = PyTuple_New(3);
        PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
        float speed = 1.0f;
        int min_value = 0;
        int max_value = 100;
        const char* format = "%d";
        PyObject* callback = nullptr;
        PyObject* callback_data = nullptr;
        const char* parent = "";
        const char* before = "";
        const char* source = "";
        int enable = true;
        int width = 0;
        int no_input = false;
        int clamped = false;
        const char* label = "";
        int show = true;

        if (!(mvApp::GetApp()->getParsers())["add_drag_int3"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
            &min_value, &max_value, &format, &callback, &callback_data, &parent, &before, &source, &enable, &width, &no_input, &clamped,
            &label, &show))
            return ToPyBool(false);

        auto vec = ToIntVect(default_value);
        auto item = CreateRef<mvDragInt3>(name, vec.data(), source);
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

        return GetPyNone();
    }

    PyObject* add_drag_int4(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* name;
        PyObject* default_value = PyTuple_New(4);
        PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
        float speed = 1.0f;
        int min_value = 0;
        int max_value = 100;
        const char* format = "%d";
        PyObject* callback = nullptr;
        PyObject* callback_data = nullptr;
        const char* parent = "";
        const char* before = "";
        const char* source = "";
        int enable = true;
        int width = 0;
        int no_input = false;
        int clamped = false;
        const char* label = "";
        int show = true;

        if (!(mvApp::GetApp()->getParsers())["add_drag_int4"].parse(args, kwargs, __FUNCTION__, &name, &default_value, &speed,
            &min_value, &max_value, &format, &callback, &callback_data, &parent, &before, &source, &enable, &width, &no_input, &clamped,
            &label, &show))
            return ToPyBool(false);

        auto vec = ToIntVect(default_value);
        auto item = CreateRef<mvDragInt4>(name, vec.data(), source);
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

        return GetPyNone();
    }

#endif
}
