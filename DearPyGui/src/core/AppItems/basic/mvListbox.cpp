#include <utility>
#include "mvListbox.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

    void mvListbox::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
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

        args.push_back({ mvPyDataType::StringList, "items", mvArgType::POSITIONAL_ARG, "()", "A tuple of items to be shown in the listbox. Can consist of any combination of types. All items will be displayed as strings." });
        args.push_back({ mvPyDataType::String, "default_value", mvArgType::KEYWORD_ARG, "''", "String value fo the item that will be selected by default."});
        args.push_back({ mvPyDataType::Integer, "num_items", mvArgType::KEYWORD_ARG, "3", "Expands the height of the listbox to show specified number of items." });

        mvPythonParserSetup setup;
        setup.about = "Adds a listbox. If height is not large enough to show all items a scroll bar will appear.";
        setup.category = { "Widgets" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    mvListbox::mvListbox(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvListbox::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvListbox*>(item);
        if (_source != 0) _value = titem->_value;
        _disabled_value = titem->_disabled_value;
        _names = titem->_names;
        _itemsHeight = titem->_itemsHeight;
        _charNames = titem->_charNames;
        _index = titem->_index;
        _disabledindex = titem->_disabledindex;
    }

    void mvListbox::setPyValue(PyObject* value)
    {
        *_value = ToString(value);
        updateIndex();
    }
    
    PyObject* mvListbox::getPyValue()
    {
        return ToPyString(*_value);
    }

    void mvListbox::setDataSource(mvUUID dataSource)
    {
        if (dataSource == _source) return;
        _source = dataSource;

        mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
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
        _value = *static_cast<std::shared_ptr<std::string>*>(item->getValue());
    }

    void mvListbox::updateIndex()
    {
        _index = 0;
        _disabledindex = 0;

        int index = 0;
        for (const auto& name : _names)
        {
            if (name == *_value)
            {
                _index = index;
                _disabledindex = index;
                break;
            }
            index++;
        }
    }

    void mvListbox::draw(ImDrawList* drawlist, float x, float y)
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
            ImGui::SetCursorPos(_state.pos);

        // update widget's position state
        _state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

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

        // themes
        if (_enabled)
        {
            if (auto classTheme = getClassThemeComponent())
                static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
        }
        else
        {
            if (auto classTheme = getClassDisabledThemeComponent())
                static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
        }

        if (_theme)
        {
            static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
            static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
            static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
        }

        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {

            ScopedID id(_uuid);

            if (!_enabled)
            {
                _disabled_value = *_value;
                _disabledindex = _index;
            }

            // remap Header to FrameBgActive
            ImGuiStyle* style = &ImGui::GetStyle();
            ImGui::PushStyleColor(ImGuiCol_Header, style->Colors[ImGuiCol_FrameBgActive]);

            if (ImGui::ListBox(_internalLabel.c_str(), _enabled ? &_index : &_disabledindex, _charNames.data(), (int)_names.size(), _itemsHeight))
            {
                *_value = _names[_index];
                _disabled_value = _names[_index];
                auto value = *_value;

                if(_alias.empty())
                    mvSubmitCallback([=]() {
                        mvAddCallback(getCallback(false), _uuid, ToPyString(value), _user_data);
                        });
                else
                    mvSubmitCallback([=]() {
                    mvAddCallback(getCallback(false), _alias, ToPyString(value), _user_data);
                        });
            }

            ImGui::PopStyleColor();
        }

        //-----------------------------------------------------------------------------
        // update state
        //-----------------------------------------------------------------------------
        UpdateAppItemState(_state);

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

        // handle popping themes
        if (_enabled)
        {
            if (auto classTheme = getClassThemeComponent())
                static_cast<mvThemeComponent*>(classTheme.get())->customAction();
        }
        else
        {
            if (auto classTheme = getClassDisabledThemeComponent())
                static_cast<mvThemeComponent*>(classTheme.get())->customAction();
        }

        if (_theme)
        {
            static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
            static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
            static_cast<mvTheme*>(_theme.get())->customAction();
        }

        if (_handlerRegistry)
            _handlerRegistry->customAction(&_state);

        // handle drag & drop payloads
        for (auto& item : _children[3])
            item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

        // handle drag & drop if used
        if (_dropCallback)
        {
            ScopedID id(_uuid);
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
                {
                    auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
                    if (_alias.empty())
                        mvAddCallback(_dropCallback,_uuid, payloadActual->getDragData(), nullptr);
                    else
                        mvAddCallback(_dropCallback,_alias, payloadActual->getDragData(), nullptr);
                }

                ImGui::EndDragDropTarget();
            }
        }
    }

    void mvListbox::handleSpecificPositionalArgs(PyObject* dict)
    {
        if (!VerifyPositionalArguments(GetParsers()[s_command], dict))
            return;

        for (int i = 0; i < PyTuple_Size(dict); i++)
        {
            PyObject* item = PyTuple_GetItem(dict, i);
            switch (i)
            {
            case 0:
                _names = ToStringVect(item);
                _charNames.clear();
                for (const std::string& item : _names)
                    _charNames.emplace_back(item.c_str());
                break;

            default:
                break;
            }
        }
    }

    void mvListbox::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "num_items")) _itemsHeight = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "items"))
        {
            _names = ToStringVect(item);
            _charNames.clear();
            for (const std::string& item : _names)
                _charNames.emplace_back(item.c_str());
            updateIndex();
        }

        if(_value->empty())
        {
            if(!_names.empty())
                *_value = _names[0];
        }
    }

    void mvListbox::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "items", mvPyObject(ToPyList(_names)));
        PyDict_SetItemString(dict, "num_items", mvPyObject(ToPyInt(_itemsHeight)));
    }

}
