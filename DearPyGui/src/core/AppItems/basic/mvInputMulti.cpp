#include "mvInputMulti.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvModule_DearPyGui.h"
#include "mvApp.h"
#include <string>
#include "mvItemRegistry.h"

namespace Marvel {

    void mvInputIntMulti::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        mvPythonParser parser(mvPyDataType::UUID, "Adds multi int input for up to 4 integer values.", { "Widgets" });
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

        parser.addArg<mvPyDataType::IntList>("default_value", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)");

        parser.addArg<mvPyDataType::Integer>("min_value", mvArgType::KEYWORD_ARG, "0", "Value for lower limit of input for each cell. Use clamped to turn on.");
        parser.addArg<mvPyDataType::Integer>("max_value", mvArgType::KEYWORD_ARG, "100", "Value for upper limit of input for each cell. Use clamped to turn on.");
        parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD_ARG, "4", "Number of components.");

        parser.addArg<mvPyDataType::Bool>("min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates min_value.");
        parser.addArg<mvPyDataType::Bool>("max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates max_value.");
        parser.addArg<mvPyDataType::Bool>("on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter.");
        parser.addArg<mvPyDataType::Bool>("readonly", mvArgType::KEYWORD_ARG, "False", "Activates a read only mode for the inputs.");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    void mvInputFloatMulti::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::UUID, "Adds multi float input for up to 4 float values.", { "Widgets" });
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

        parser.addArg<mvPyDataType::FloatList>("default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)");

        parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%.3f'");

        parser.addArg<mvPyDataType::Float>("min_value", mvArgType::KEYWORD_ARG, "0.0", "Value for lower limit of input for each cell. Use clamped to turn on.");
        parser.addArg<mvPyDataType::Float>("max_value", mvArgType::KEYWORD_ARG, "100.0", "Value for upper limit of input for each cell. Use clamped to turn on.");

        parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD_ARG, "4", "Number of components.");

        parser.addArg<mvPyDataType::Bool>("min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates min_value.");
        parser.addArg<mvPyDataType::Bool>("max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates max_value.");
        parser.addArg<mvPyDataType::Bool>("on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press.");
        parser.addArg<mvPyDataType::Bool>("readonly", mvArgType::KEYWORD_ARG, "False", "Activates a read only mode for the inputs.");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvInputIntMulti::mvInputIntMulti(mvUUID uuid)
        : mvInt4PtrBase(uuid)
    {
        _last_value = *_value;
    }

    void mvInputIntMulti::setEnabled(bool value)
    {
        if (value == _enabled)
            return;

        if (value)
            _flags = _stor_flags;

        else
        {
            _stor_flags = _flags;
            _flags |= ImGuiInputTextFlags_ReadOnly;
            _flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

        _enabled = value;
    }

    void mvInputIntMulti::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id(_uuid);

        bool res = false;

        switch (_size)
        {
        case 2:
            res = ImGui::InputInt2(_label.c_str(), _value->data(), _flags);
            break;
        case 3:
            res = ImGui::InputInt3(_label.c_str(), _value->data(), _flags);
            break;
        case 4:
            res = ImGui::InputInt4(_label.c_str(), _value->data(), _flags);
            break;
        default:
            break;
        }

        if (res)
        {
            auto inital_value = *_value;
            // determines clamped cases
            if (_min_clamped && _max_clamped)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (_value->data()[i] < _min) _value->data()[i] = _min;
                    else if (_value->data()[i] > _max) _value->data()[i] = _max;
                }
            }
            else if (_min_clamped)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (_value->data()[i] < _min) _value->data()[i] = _min;
                }
            }
            else if (_max_clamped)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (_value->data()[i] > _max) _value->data()[i] = _max;
                }
            }

            // If the widget is edited through ctrl+click mode the active value will be entered every frame.
            // If the value is out of bounds the value will be overwritten with max or min so each frame the value will be switching between the
            // ctrl+click value and the bounds value until the widget is not in ctrl+click mode. To prevent the callback from running every 
            // frame we check if the value was already submitted.
            if (_last_value != *_value)
            {
                _last_value = *_value;

                auto value = *_value;
                mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                    mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyIntList(value.data(), value.size()), _user_data);
                    });
            }
        }
    }

    mvInputFloatMulti::mvInputFloatMulti(mvUUID uuid)
        : mvFloat4PtrBase(uuid)
    {
        _last_value = *_value;
    }

    void mvInputFloatMulti::setEnabled(bool value)
    {
        if (value == _enabled)
            return;

        if (value)
            _flags = _stor_flags;

        else
        {
            _stor_flags = _flags;
            _flags |= ImGuiInputTextFlags_ReadOnly;
            _flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        }

        _enabled = value;
    }

    void mvInputFloatMulti::draw(ImDrawList* drawlist, float x, float y)
    {
        ScopedID id(_uuid);
        ////mvImGuiThemeScope scope(this);
        //mvFontScope fscope(this);

        bool res = false;

        switch (_size)
        {
        case 2:
            res = ImGui::InputFloat2(_label.c_str(), _value->data(), _format.c_str(), _flags);
            break;
        case 3:
            res = ImGui::InputFloat3(_label.c_str(), _value->data(), _format.c_str(), _flags);
            break;
        case 4:
            res = ImGui::InputFloat4(_label.c_str(), _value->data(), _format.c_str(), _flags);
            break;
        default:
            break;
        }

        if (res)
        {
            auto inital_value = *_value;
            // determines clamped cases
            if (_min_clamped && _max_clamped)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (_value->data()[i] < _min) _value->data()[i] = _min;
                    else if (_value->data()[i] > _max) _value->data()[i] = _max;
                }
            }
            else if (_min_clamped)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (_value->data()[i] < _min) _value->data()[i] = _min;
                }
            }
            else if (_max_clamped)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (_value->data()[i] > _max) _value->data()[i] = _max;
                }
            }

            // If the widget is edited through ctrl+click mode the active value will be entered every frame.
            // If the value is out of bounds the value will be overwritten with max or min so each frame the value will be switching between the
            // ctrl+click value and the bounds value until the widget is not in ctrl+click mode. To prevent the callback from running every 
            // frame we check if the value was already submitted.
            if (_last_value != *_value)
            {
                _last_value = *_value;
                auto value = *_value;
                mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                    mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyFloatList(value.data(), value.size()), _user_data);
                    });
            }
        }
    }

    void mvInputIntMulti::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? _flags |= ImGuiInputTextFlags_EnterReturnsTrue : _flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        if (PyObject* item = PyDict_GetItemString(dict, "on_enter")) ToBool(item) ? _stor_flags |= ImGuiInputTextFlags_EnterReturnsTrue : _stor_flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
        if (PyObject* item = PyDict_GetItemString(dict, "readonly")) ToBool(item) ? _flags |= ImGuiInputTextFlags_ReadOnly : _flags &= ~ImGuiInputTextFlags_ReadOnly;
        if (PyObject* item = PyDict_GetItemString(dict, "readonly")) ToBool(item) ? _stor_flags |= ImGuiInputTextFlags_ReadOnly : _stor_flags &= ~ImGuiInputTextFlags_ReadOnly;
        if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToInt(item);

        bool minmax_set = false;
        if (PyObject* item = PyDict_GetItemString(dict, "min_value"))
        {
            _min = ToInt(item);
            _min_clamped = true;
            minmax_set = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "max_value"))
        {
            _max = ToInt(item);
            _max_clamped = true;
            minmax_set = true;
        }

        if (!minmax_set)
        {
            if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) _min_clamped = ToBool(item);
            if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) _max_clamped = ToBool(item);
        }
    }

    void mvInputIntMulti::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "on_enter", ToPyBool(_flags & ImGuiInputTextFlags_EnterReturnsTrue));
        PyDict_SetItemString(dict, "readonly", ToPyBool(_flags & ImGuiInputTextFlags_ReadOnly));
        PyDict_SetItemString(dict, "min_value", ToPyInt(_min));
        PyDict_SetItemString(dict, "max_value", ToPyInt(_max));
        PyDict_SetItemString(dict, "min_clamped", ToPyBool(_min_clamped));
        PyDict_SetItemString(dict, "max_clamped", ToPyBool(_max_clamped));
        PyDict_SetItemString(dict, "size", ToPyInt(_size));
    }

    void mvInputFloatMulti::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "format")) _format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToInt(item);

        bool minmax_set = false;
        if (PyObject* item = PyDict_GetItemString(dict, "min_value"))
        {
            _min = ToFloat(item);
            _min_clamped = true;
            minmax_set = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "max_value"))
        {
            _max = ToFloat(item);
            _max_clamped = true;
            minmax_set = true;
        }

        if (!minmax_set)
        {
            if (PyObject* item = PyDict_GetItemString(dict, "min_clamped")) _min_clamped = ToBool(item);
            if (PyObject* item = PyDict_GetItemString(dict, "max_clamped")) _max_clamped = ToBool(item);
        }

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, _flags);
        flagop("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, _stor_flags);
        flagop("readonly", ImGuiInputTextFlags_ReadOnly, _flags);
        flagop("readonly", ImGuiInputTextFlags_ReadOnly, _stor_flags);

    }

    void mvInputFloatMulti::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "format", ToPyString(_format));
        PyDict_SetItemString(dict, "min_value", ToPyFloat(_min));
        PyDict_SetItemString(dict, "max_value", ToPyFloat(_max));
        PyDict_SetItemString(dict, "min_clamped", ToPyBool(_min_clamped));
        PyDict_SetItemString(dict, "max_clamped", ToPyBool(_max_clamped));
        PyDict_SetItemString(dict, "size", ToPyInt(_size));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("on_enter", ImGuiInputTextFlags_EnterReturnsTrue, _flags);
        checkbitset("readonly", ImGuiInputTextFlags_ReadOnly, _flags);
    }

}
