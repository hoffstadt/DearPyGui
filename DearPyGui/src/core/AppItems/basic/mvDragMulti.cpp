#include "mvDragMulti.h"
#include <utility>
#include "mvApp.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

    void mvDragFloatMulti::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::UUID, "Adds drag input for a set of int values up to 4. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes.", { "Widgets" });
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

        parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD_ARG, "4", "Number of components");

        parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%0.3f'");

        parser.addArg<mvPyDataType::Float>("speed", mvArgType::KEYWORD_ARG, "1.0");
        parser.addArg<mvPyDataType::Float>("min_value", mvArgType::KEYWORD_ARG, "0.0", "Applies a limit only to draging entry only.");
        parser.addArg<mvPyDataType::Float>("max_value", mvArgType::KEYWORD_ARG, "100.0", "Applies a limit only to draging entry only.");
        
        parser.addArg<mvPyDataType::Bool>("no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods or Enter key allowing to input text directly into the widget.");
        parser.addArg<mvPyDataType::Bool>("clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click.");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    void mvDragIntMulti::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::UUID, "Adds drag input for a set of int values up to 4. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes.", { "Widgets" });
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

        parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD_ARG, "4", "Number of components.");

        parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%d'");

        parser.addArg<mvPyDataType::Float>("speed", mvArgType::KEYWORD_ARG, "1.0");

        parser.addArg<mvPyDataType::Integer>("min_value", mvArgType::KEYWORD_ARG, "0", "Applies a limit only to draging entry only.");
        parser.addArg<mvPyDataType::Integer>("max_value", mvArgType::KEYWORD_ARG, "100", "Applies a limit only to draging entry only.");

        parser.addArg<mvPyDataType::Bool>("no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods or Enter key allowing to input text directly into the widget.");
        parser.addArg<mvPyDataType::Bool>("clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click.");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvDragFloatMulti::mvDragFloatMulti(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    PyObject* mvDragFloatMulti::getPyValue()
    {
        return ToPyFloatList(_value->data(), 4);
    }

    void mvDragFloatMulti::setPyValue(PyObject* value)
    {
        std::vector<float> temp = ToFloatVect(value);
        while (temp.size() < 4)
            temp.push_back(0.0f);
        std::array<float, 4> temp_array;
        for (size_t i = 0; i < temp_array.size(); i++)
            temp_array[i] = temp[i];
        if (_value)
            *_value = temp_array;
        else
            _value = std::make_shared<std::array<float, 4>>(temp_array);
    }

    void mvDragFloatMulti::setDataSource(mvUUID dataSource)
    {
        if (dataSource == _source) return;
        _source = dataSource;

        mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (item->getValueType() != getValueType())
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
                "Values types do not match: " + std::to_string(dataSource), this);
            return;
        }
        _value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());
    }

    void mvDragFloatMulti::draw(ImDrawList* drawlist, float x, float y)
    {

        //-----------------------------------------------------------------------------
        // pre draw
        //-----------------------------------------------------------------------------

        // show/hide
        if (!_show)
            return;

        // focusing
        if (_focusNextFrame)
        {
            ImGui::SetKeyboardFocusHere();
            _focusNextFrame = false;
        }

        // cache old cursor position
        ImVec2 previousCursorPos = ImGui::GetCursorPos();

        // set cursor position if user set
        if (_dirtyPos)
            ImGui::SetCursorPos(_state.getItemPos());

        // update widget's position state
        _state.setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

        // set item width
        if (_width != 0)
            ImGui::SetNextItemWidth((float)_width);

        // set indent
        if (_indent > 0.0f)
            ImGui::Indent(_indent);

        // push font if a font object is attached
        if (_font)
        {
            ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
            ImGui::PushFont(fontptr);
        }

        // handle enabled theming
        if (_enabled)
        {
            // push class theme (if it exists)
            if (auto classTheme = getClassTheme())
                static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

            // push item theme (if it exists)
            if (_theme)
                static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
        }

        // handled disabled theming
        else
        {
            // push class theme (if it exists)
            if (auto classTheme = getClassDisabledTheme())
                static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

            // push item theme (if it exists)
            if (_disabledTheme)
                static_cast<mvTheme*>(_disabledTheme.get())->draw(nullptr, 0.0f, 0.0f);
        }


        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {
            ScopedID id(_uuid);

            if (!_enabled) std::copy(_value->data(), _value->data() + 2, _disabled_value);

            switch (_size)
            {
            case 2:
                if (ImGui::DragFloat2(_internalLabel.c_str(), _enabled ? _value->data() : &_disabled_value[0], _speed, _min, _max, _format.c_str(), _flags))
                {
                    auto value = *_value;
                    mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                        mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyFloatList(value.data(), value.size()), _user_data);
                        });
                }
                break;
            case 3:
                if (ImGui::DragFloat3(_internalLabel.c_str(), _enabled ? _value->data() : &_disabled_value[0], _speed, _min, _max, _format.c_str(), _flags))
                {
                    auto value = *_value;
                    mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                        mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyFloatList(value.data(), value.size()), _user_data);
                        });
                }
                break;
            case 4:
                if (ImGui::DragFloat4(_internalLabel.c_str(), _enabled ? _value->data() : &_disabled_value[0], _speed, _min, _max, _format.c_str(), _flags))
                {
                    auto value = *_value;
                    mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                        mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyFloatList(value.data(), value.size()), _user_data);
                        });
                }
                break;
            default:
                break;
            }
        }
        //-----------------------------------------------------------------------------
        // update state
        //   * only update if applicable
        //-----------------------------------------------------------------------------
        _state._lastFrameUpdate = mvApp::s_frame;
        _state._hovered = ImGui::IsItemHovered();
        _state._active = ImGui::IsItemActive();
        _state._focused = ImGui::IsItemFocused();
        _state._leftclicked = ImGui::IsItemClicked();
        _state._rightclicked = ImGui::IsItemClicked(1);
        _state._middleclicked = ImGui::IsItemClicked(2);
        _state._visible = ImGui::IsItemVisible();
        _state._activated = ImGui::IsItemActivated();
        _state._deactivated = ImGui::IsItemDeactivated();
        _state._rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
        _state._rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
        _state._rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
        _state._contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------

        // set cursor position to cached position
        if (_dirtyPos)
            ImGui::SetCursorPos(previousCursorPos);

        if (_indent > 0.0f)
            ImGui::Unindent(_indent);

        // pop font off stack
        if (_font)
            ImGui::PopFont();

        // handle popping styles
        if (_enabled)
        {
            if (auto classTheme = getClassTheme())
                static_cast<mvTheme*>(classTheme.get())->customAction();

            if (_theme)
                static_cast<mvTheme*>(_theme.get())->customAction();
        }
        else
        {
            if (auto classTheme = getClassDisabledTheme())
                static_cast<mvTheme*>(classTheme.get())->customAction();

            if (_disabledTheme)
                static_cast<mvTheme*>(_disabledTheme.get())->customAction();
        }

        // handle widget's event handlers
        for (auto& item : _children[3])
        {
            if (!item->preDraw())
                continue;

            item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
        }

        // handle drag & drop payloads
        for (auto& item : _children[4])
        {
            if (!item->preDraw())
                continue;

            item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
        }

        // handle drag & drop if used
        if (_dropCallback)
        {
            ScopedID id(_uuid);
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
                {
                    auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
                    mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
                }

                ImGui::EndDragDropTarget();
            }
        }
    }

    void mvDragFloatMulti::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) _format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) _speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) _min = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) _max = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToInt(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, _flags);
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, _stor_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, _flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, _stor_flags);

        if (wasEnabledLastFrameReset())
            _flags = _stor_flags;

        if (wasDisabledLastFrameReset())
        {
            _stor_flags = _flags;
            _flags |= ImGuiSliderFlags_NoInput;
        }

    }

    void mvDragFloatMulti::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(_format));
        PyDict_SetItemString(dict, "speed", ToPyFloat(_speed));
        PyDict_SetItemString(dict, "min_value", ToPyFloat(_min));
        PyDict_SetItemString(dict, "max_value", ToPyFloat(_max));
        PyDict_SetItemString(dict, "size", ToPyInt(_size));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, _flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, _flags);

    }

    mvDragIntMulti::mvDragIntMulti(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    PyObject* mvDragIntMulti::getPyValue()
    {
        return ToPyIntList(_value->data(), 4);
    }

    void mvDragIntMulti::setPyValue(PyObject* value)
    {
        std::vector<int> temp = ToIntVect(value);
        while (temp.size() < 4)
            temp.push_back(0);
        std::array<int, 4> temp_array;
        for (size_t i = 0; i < temp_array.size(); i++)
            temp_array[i] = temp[i];
        if (_value)
            *_value = temp_array;
        else
            _value = std::make_shared<std::array<int, 4>>(temp_array);
    }

    void mvDragIntMulti::setDataSource(mvUUID dataSource)
    {
        if (dataSource == _source) return;
        _source = dataSource;

        mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (item->getValueType() != getValueType())
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
                "Values types do not match: " + std::to_string(dataSource), this);
            return;
        }
        _value = *static_cast<std::shared_ptr<std::array<int, 4>>*>(item->getValue());
    }

    void mvDragIntMulti::draw(ImDrawList* drawlist, float x, float y)
    {

        //-----------------------------------------------------------------------------
        // pre draw
        //-----------------------------------------------------------------------------

        // show/hide
        if (!_show)
            return;

        // focusing
        if (_focusNextFrame)
        {
            ImGui::SetKeyboardFocusHere();
            _focusNextFrame = false;
        }

        // cache old cursor position
        ImVec2 previousCursorPos = ImGui::GetCursorPos();

        // set cursor position if user set
        if (_dirtyPos)
            ImGui::SetCursorPos(_state.getItemPos());

        // update widget's position state
        _state.setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

        // set item width
        if (_width != 0)
            ImGui::SetNextItemWidth((float)_width);

        // set indent
        if (_indent > 0.0f)
            ImGui::Indent(_indent);

        // push font if a font object is attached
        if (_font)
        {
            ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
            ImGui::PushFont(fontptr);
        }

        // handle enabled theming
        if (_enabled)
        {
            // push class theme (if it exists)
            if (auto classTheme = getClassTheme())
                static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

            // push item theme (if it exists)
            if (_theme)
                static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
        }

        // handled disabled theming
        else
        {
            // push class theme (if it exists)
            if (auto classTheme = getClassDisabledTheme())
                static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

            // push item theme (if it exists)
            if (_disabledTheme)
                static_cast<mvTheme*>(_disabledTheme.get())->draw(nullptr, 0.0f, 0.0f);
        }


        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {

            ScopedID id(_uuid);

            if (!_enabled) std::copy(_value->data(), _value->data() + 2, _disabled_value);

            switch (_size)
            {
            case 2:
                if (ImGui::DragInt2(_internalLabel.c_str(), _enabled ? _value->data() : &_disabled_value[0], _speed, _min, _max, _format.c_str(), _flags))
                {
                    auto value = *_value;
                    mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                        mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyIntList(value.data(), value.size()), _user_data);
                        });
                }
                break;
            case 3:
                if (ImGui::DragInt3(_internalLabel.c_str(), _enabled ? _value->data() : &_disabled_value[0], _speed, _min, _max, _format.c_str(), _flags))
                {
                    auto value = *_value;
                    mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                        mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyIntList(value.data(), value.size()), _user_data);
                        });
                }
                break;
            case 4:
                if (ImGui::DragInt4(_internalLabel.c_str(), _enabled ? _value->data() : &_disabled_value[0], _speed, _min, _max, _format.c_str(), _flags))
                {
                    auto value = *_value;
                    mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
                        mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyIntList(value.data(), value.size()), _user_data);
                        });
                }
                break;
            default:
                break;
            }
        }

        //-----------------------------------------------------------------------------
        // update state
        //   * only update if applicable
        //-----------------------------------------------------------------------------
        _state._lastFrameUpdate = mvApp::s_frame;
        _state._hovered = ImGui::IsItemHovered();
        _state._active = ImGui::IsItemActive();
        _state._focused = ImGui::IsItemFocused();
        _state._leftclicked = ImGui::IsItemClicked();
        _state._rightclicked = ImGui::IsItemClicked(1);
        _state._middleclicked = ImGui::IsItemClicked(2);
        _state._visible = ImGui::IsItemVisible();
        _state._activated = ImGui::IsItemActivated();
        _state._deactivated = ImGui::IsItemDeactivated();
        _state._rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
        _state._rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
        _state._rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
        _state._contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------

        // set cursor position to cached position
        if (_dirtyPos)
            ImGui::SetCursorPos(previousCursorPos);

        if (_indent > 0.0f)
            ImGui::Unindent(_indent);

        // pop font off stack
        if (_font)
            ImGui::PopFont();

        // handle popping styles
        if (_enabled)
        {
            if (auto classTheme = getClassTheme())
                static_cast<mvTheme*>(classTheme.get())->customAction();

            if (_theme)
                static_cast<mvTheme*>(_theme.get())->customAction();
        }
        else
        {
            if (auto classTheme = getClassDisabledTheme())
                static_cast<mvTheme*>(classTheme.get())->customAction();

            if (_disabledTheme)
                static_cast<mvTheme*>(_disabledTheme.get())->customAction();
        }

        // handle widget's event handlers
        for (auto& item : _children[3])
        {
            if (!item->preDraw())
                continue;

            item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
        }

        // handle drag & drop payloads
        for (auto& item : _children[4])
        {
            if (!item->preDraw())
                continue;

            item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
        }

        // handle drag & drop if used
        if (_dropCallback)
        {
            ScopedID id(_uuid);
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
                {
                    auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
                    mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
                }

                ImGui::EndDragDropTarget();
            }
        }
    }

    void mvDragIntMulti::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "format")) _format = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "speed")) _speed = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) _min = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) _max = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToInt(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // flags
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, _flags);
        flagop("clamped", ImGuiSliderFlags_ClampOnInput, _stor_flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, _flags);
        flagop("no_input", ImGuiSliderFlags_NoInput, _stor_flags);

        if (wasEnabledLastFrameReset())
            _flags = _stor_flags;

        if (wasDisabledLastFrameReset())
        {
            _stor_flags = _flags;
            _flags |= ImGuiSliderFlags_NoInput;
        }

    }

    void mvDragIntMulti::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "format", ToPyString(_format));
        PyDict_SetItemString(dict, "speed", ToPyFloat(_speed));
        PyDict_SetItemString(dict, "min_value", ToPyInt(_min));
        PyDict_SetItemString(dict, "max_value", ToPyInt(_max));
        PyDict_SetItemString(dict, "size", ToPyInt(_size));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
        };

        // window flags
        checkbitset("clamped", ImGuiSliderFlags_ClampOnInput, _flags);
        checkbitset("no_input", ImGuiSliderFlags_NoInput, _flags);

    }

}