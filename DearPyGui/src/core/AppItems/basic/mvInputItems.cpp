#include "mvInputItems.h"
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
        parsers->insert({ s_parser, mvPythonParser({
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

    void mvInputInt2::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        parsers->insert({ s_parser, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::IntList, "default_value", "", "(0, 0)"},
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
        }, "Adds input for 2 integer values.", "None", "Adding Widgets") });
    }

    void mvInputInt3::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_parser, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::IntList, "default_value", "", "(0, 0, 0)"},
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
        }, "Adds input for 3 integer values.", "None", "Adding Widgets") });
    }

    void mvInputInt4::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_parser, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::IntList, "default_value", "", "(0, 0, 0, 0)"},
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

    void mvInputFloat::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_parser, mvPythonParser({
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

    void mvInputFloat2::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_parser, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0)"},
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
        }, "Adds input for 2 float values.", "None", "Adding Widgets") });
    }

    void mvInputFloat3::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_parser, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0, 0.0)"},
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
        }, "Adds input for 3 float values.", "None", "Adding Widgets") });
    }

    void mvInputFloat4::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        parsers->insert({ s_parser, mvPythonParser({
            {mvPythonDataType::Optional},
            {mvPythonDataType::String, "name"},
            {mvPythonDataType::KeywordOnly},
            {mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0, 0.0, 0.0)"},
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

    mvInputInt::mvInputInt(const std::string& name, int default_value, const std::string& dataSource)
        : mvIntPtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
        m_last_value = *m_value;
    }
    
    void mvInputInt::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiInputTextFlags_ReadOnly;
            m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

        m_core_config.enabled = value;
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
                mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);
            }
        }

    }

    void mvInputInt::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvInputIntConfig*)config;

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

    mvAppItemConfig* mvInputInt::getConfig()
    {
        return &m_config;
    }

    mvInputInt2::mvInputInt2(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt2PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
        m_last_value = *m_value;
    }

    void mvInputInt2::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiInputTextFlags_ReadOnly;
            m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

        m_core_config.enabled = value;
    }

    void mvInputInt2::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (ImGui::InputInt2(m_label.c_str(), m_value->data(), m_flags))
        {
            // determines clamped cases
            if (m_min_clamped && m_max_clamped)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (m_value->data()[i] < m_min) m_value->data()[i] = m_min;
                    else if (m_value->data()[i] > m_max) m_value->data()[i] = m_max;
                }
            }
            else if (m_min_clamped)
            {
                for (int i = 0 ; i < 2 ; i++)
                {
                    if (m_value->data()[i] < m_min) m_value->data()[i] = m_min;
                }
            }
            else if (m_max_clamped)
            {
                for (int i = 0; i < 2; i++)
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
                mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);
            }
        }
    }

    void mvInputInt2::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvInputIntsConfig*)config;

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

    mvAppItemConfig* mvInputInt2::getConfig()
    {
        return &m_config;
    }

    mvInputInt3::mvInputInt3(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt3PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
        m_last_value = *m_value;
    }

    void mvInputInt3::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiInputTextFlags_ReadOnly;
            m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

        m_core_config.enabled = value;
    }

    void mvInputInt3::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (ImGui::InputInt3(m_label.c_str(), m_value->data(), m_flags))
        {
            auto inital_value = *m_value;
            // determines clamped cases
            if (m_min_clamped && m_max_clamped)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (m_value->data()[i] < m_min) m_value->data()[i] = m_min;
                    else if (m_value->data()[i] > m_max) m_value->data()[i] = m_max;
                }
            }
            else if (m_min_clamped)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (m_value->data()[i] < m_min) m_value->data()[i] = m_min;
                }
            }
            else if (m_max_clamped)
            {
                for (int i = 0; i < 3; i++)
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
                mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);
            }
        }
    }

    void mvInputInt3::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvInputIntsConfig*)config;

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

    mvAppItemConfig* mvInputInt3::getConfig()
    {
        return &m_config;
    }

    mvInputInt4::mvInputInt4(const std::string& name, int* default_value, const std::string& dataSource)
        : mvInt4PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
        m_last_value = *m_value;
    }

    void mvInputInt4::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiInputTextFlags_ReadOnly;
            m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

        m_core_config.enabled = value;
    }

    void mvInputInt4::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (ImGui::InputInt4(m_label.c_str(), m_value->data(), m_flags))
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
                mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);
            }
        }
    }

    void mvInputInt4::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvInputIntsConfig*)config;

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

    mvAppItemConfig* mvInputInt4::getConfig()
    {
        return &m_config;
    }

    mvInputFloat::mvInputFloat(const std::string& name, float default_value, const std::string& dataSource)
        : mvFloatPtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
        m_last_value = *m_value;
    }

    void mvInputFloat::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiInputTextFlags_ReadOnly;
            m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

        m_core_config.enabled = value;
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
                mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);
            }
        }
    }

    void mvInputFloat::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvInputFloatConfig*)config;

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

    mvAppItemConfig* mvInputFloat::getConfig()
    {
        return &m_config;
    }

    mvInputFloat2::mvInputFloat2(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat2PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
        m_last_value = *m_value;
    }

    void mvInputFloat2::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiInputTextFlags_ReadOnly;
            m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

        m_core_config.enabled = value;
    }

    void mvInputFloat2::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (ImGui::InputFloat2(m_label.c_str(), m_value->data(), m_format.c_str(), m_flags))
        {
            auto inital_value = *m_value;
            // determines clamped cases
            if (m_min_clamped && m_max_clamped)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (m_value->data()[i] < m_min) m_value->data()[i] = m_min;
                    else if (m_value->data()[i] > m_max) m_value->data()[i] = m_max;
                }
            }
            else if (m_min_clamped)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (m_value->data()[i] < m_min) m_value->data()[i] = m_min;
                }
            }
            else if (m_max_clamped)
            {
                for (int i = 0; i < 2; i++)
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
                mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);
            }
        }
    }

    void mvInputFloat2::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvInputFloatsConfig*)config;

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

    mvAppItemConfig* mvInputFloat2::getConfig()
    {
        return &m_config;
    }

    mvInputFloat3::mvInputFloat3(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat3PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
        m_last_value = *m_value;
    }

    void mvInputFloat3::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiInputTextFlags_ReadOnly;
            m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

        m_core_config.enabled = value;
    }

    void mvInputFloat3::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (ImGui::InputFloat3(m_label.c_str(), m_value->data(), m_format.c_str(), m_flags))
        {
            auto inital_value = *m_value;
            // determines clamped cases
            if (m_min_clamped && m_max_clamped)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (m_value->data()[i] < m_min) m_value->data()[i] = m_min;
                    else if (m_value->data()[i] > m_max) m_value->data()[i] = m_max;
                }
            }
            else if (m_min_clamped)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (m_value->data()[i] < m_min) m_value->data()[i] = m_min;
                }
            }
            else if (m_max_clamped)
            {
                for (int i = 0; i < 3; i++)
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
                mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);
            }
        }
    }

    void mvInputFloat3::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvInputFloatsConfig*)config;

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

    mvAppItemConfig* mvInputFloat3::getConfig()
    {
        return &m_config;
    }

    mvInputFloat4::mvInputFloat4(const std::string& name, float* default_value, const std::string& dataSource)
        : mvFloat4PtrBase(name, default_value)
    {
        m_description.disableAllowed = true;
        m_last_value = *m_value;
    }

    void mvInputFloat4::setEnabled(bool value)
    {
        if (value == m_core_config.enabled)
            return;

        if (value)
            m_flags = m_stor_flags;

        else
        {
            m_stor_flags = m_flags;
            m_flags |= ImGuiInputTextFlags_ReadOnly;
            m_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

        m_core_config.enabled = value;
    }

    void mvInputFloat4::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id;
        mvImGuiThemeScope scope(this);
        mvFontScope fscope(this);

        if (ImGui::InputFloat4(m_label.c_str(), m_value->data(), m_format.c_str(), m_flags))
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
                mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);
            }
        }
    }

    void mvInputFloat4::updateConfig(mvAppItemConfig* config)
    {
        auto aconfig = (mvInputFloatsConfig*)config;

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

    mvAppItemConfig* mvInputFloat4::getConfig()
    {
        return &m_config;
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

    void mvInputInt2::setExtraConfigDict(PyObject* dict)
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
    }

    void mvInputInt2::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "on_enter", ToPyBool(m_flags & ImGuiInputTextFlags_EnterReturnsTrue));
        PyDict_SetItemString(dict, "readonly", ToPyBool(m_flags & ImGuiInputTextFlags_ReadOnly));
        PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));
        PyDict_SetItemString(dict, "min_clamped", ToPyBool(m_min_clamped));
        PyDict_SetItemString(dict, "max_clamped", ToPyBool(m_max_clamped));

    }

    void mvInputInt3::setExtraConfigDict(PyObject* dict)
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
    }

    void mvInputInt3::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "on_enter", ToPyBool(m_flags & ImGuiInputTextFlags_EnterReturnsTrue));
        PyDict_SetItemString(dict, "readonly", ToPyBool(m_flags & ImGuiInputTextFlags_ReadOnly));
        PyDict_SetItemString(dict, "min_value", ToPyInt(m_min));
        PyDict_SetItemString(dict, "max_value", ToPyInt(m_max));
        PyDict_SetItemString(dict, "min_clamped", ToPyBool(m_min_clamped));
        PyDict_SetItemString(dict, "max_clamped", ToPyBool(m_max_clamped));
    }

    void mvInputInt4::setExtraConfigDict(PyObject* dict)
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
    }

    void mvInputInt4::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "on_enter", ToPyBool(m_flags & ImGuiInputTextFlags_EnterReturnsTrue));
        PyDict_SetItemString(dict, "readonly", ToPyBool(m_flags & ImGuiInputTextFlags_ReadOnly));
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

    void mvInputFloat2::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
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

    void mvInputFloat2::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(m_format));
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

    void mvInputFloat3::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
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

    void mvInputFloat3::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(m_format));
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

    void mvInputFloat4::setExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
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

    void mvInputFloat4::getExtraConfigDict(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", ToPyString(m_format));
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

    PyObject* mvInputInt::add_input_int(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        static int i = 0; i++;
        std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
        const char* name = sname.c_str();
        int default_value = 0;
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
        int step = 1;
        int step_fast = 100;
        int readonly = false;

        if (!(mvApp::GetApp()->getParsers())["add_input_int"].parse(args, kwargs, __FUNCTION__, &name,
            &default_value, &min_value, &max_value, &min_clamped, &max_clamped, &callback, &callback_data, &parent, &before, &source, &enabled, &width, &on_enter,
            &label, &show, &step, &step_fast, &readonly))
            return ToPyBool(false);

        auto item = CreateRef<mvInputInt>(name, default_value, source);
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

    PyObject* mvInputInt2::add_input_int2(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        static int i = 0; i++;
        std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
        const char* name = sname.c_str();
        PyObject* default_value = PyTuple_New(2);
        PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
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

        if (!(mvApp::GetApp()->getParsers())["add_input_int2"].parse(args, kwargs, __FUNCTION__, &name,
            &default_value, &min_value, &max_value, &min_clamped, &max_clamped, &callback, &callback_data, &parent, &before, &source, &enabled, &width, &on_enter,
            &label, &show, &readonly))
            return ToPyBool(false);

        auto vec = ToIntVect(default_value);

        auto item = CreateRef<mvInputInt2>(name, vec.data(), source);
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

    PyObject* mvInputInt3::add_input_int3(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        static int i = 0; i++;
        std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
        const char* name = sname.c_str();
        PyObject* default_value = PyTuple_New(3);
        PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
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
        const char* popup = "";
        int show = false;
        int readonly = false;

        if (!(mvApp::GetApp()->getParsers())["add_input_int3"].parse(args, kwargs, __FUNCTION__, &name,
            &default_value, &min_value, &max_value, &min_clamped, &max_clamped, &callback, &callback_data, &parent, &before, &source, &enabled, &width, &on_enter,
            &label, &popup, &show, &readonly))
            return ToPyBool(false);

        auto vec = ToIntVect(default_value);

        auto item = CreateRef<mvInputInt3>(name, vec.data(), source);
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

    PyObject* mvInputInt4::add_input_int4(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        static int i = 0; i++;
        std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
        const char* name = sname.c_str();
        PyObject* default_value = PyTuple_New(4);
        PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
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
            &default_value, &min_value, &max_value, &min_clamped, &max_clamped, &callback, &callback_data, &parent, &before, &source, &enabled, &width, &on_enter,
            &label, &show, &readonly))
            return ToPyBool(false);

        auto vec = ToIntVect(default_value);
        auto item = CreateRef<mvInputInt4>(name, vec.data(), source);
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

    PyObject* mvInputFloat::add_input_float(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        static int i = 0; i++;
        std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
        const char* name = sname.c_str();
        float default_value = 0.0f;
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
        float step = 0.1f;
        float step_fast = 1.0f;
        int readonly = false;

        if (!(mvApp::GetApp()->getParsers())["add_input_float"].parse(args, kwargs, __FUNCTION__, &name,
            &default_value, &min_value, &max_value, &min_clamped, &max_clamped, &format, &callback, &callback_data,
            &parent, &before, &source, &enabled, &width, &on_enter,
            &label, &show, &step, &step_fast, &readonly))
            return ToPyBool(false);

        auto item = CreateRef<mvInputFloat>(name, default_value, source);
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

    PyObject* mvInputFloat2::add_input_float2(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        static int i = 0; i++;
        std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
        const char* name = sname.c_str();
        PyObject* default_value = PyTuple_New(2);
        PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
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

        if (!(mvApp::GetApp()->getParsers())["add_input_float2"].parse(args, kwargs, __FUNCTION__, &name,
            &default_value, &min_value, &max_value, &min_clamped, &max_clamped, &format, &callback, &callback_data,
            &parent, &before, &source, &enabled, &width, &on_enter,
            &label, &show, &readonly))
            return ToPyBool(false);

        auto vec = ToFloatVect(default_value);
        auto item = CreateRef<mvInputFloat2>(name, vec.data(), source);
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

    PyObject* mvInputFloat3::add_input_float3(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        static int i = 0; i++;
        std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
        const char* name = sname.c_str();
        PyObject* default_value = PyTuple_New(3);
        PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
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

        if (!(mvApp::GetApp()->getParsers())["add_input_float3"].parse(args, kwargs, __FUNCTION__, &name,
            &default_value, &min_value, &max_value, &min_clamped, &max_clamped, &format, &callback, &callback_data,
            &parent, &before, &source, &enabled, &width, &on_enter,
            &label, &show, &readonly))
            return ToPyBool(false);

        auto vec = ToFloatVect(default_value);

        auto item = CreateRef<mvInputFloat3>(name, vec.data(), source);
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

    PyObject* mvInputFloat4::add_input_float4(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        static int i = 0; i++;
        std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
        const char* name = sname.c_str();
        PyObject* default_value = PyTuple_New(4);
        PyTuple_SetItem(default_value, 0, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 1, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 2, PyLong_FromLong(0));
        PyTuple_SetItem(default_value, 3, PyLong_FromLong(0));
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
            &default_value, &min_value, &max_value, &min_clamped, &max_clamped, &format, &callback, &callback_data,
            &parent, &before, &source, &enabled, &width, &on_enter,
            &label, &show, &readonly))
            return ToPyBool(false);

        auto vec = ToFloatVect(default_value);

        auto item = CreateRef<mvInputFloat4>(name, vec.data(), source);
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
