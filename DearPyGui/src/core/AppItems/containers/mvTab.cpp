#include "mvTab.h"
#include "mvTabBar.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

    void mvTab::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Bool, "closable", mvArgType::KEYWORD_ARG, "False", "Creates a button on the tab that can hide the tab." });
        args.push_back({ mvPyDataType::Bool, "no_tooltip", mvArgType::KEYWORD_ARG, "False", "Disable tooltip for the given tab." });       
        args.push_back({ mvPyDataType::Bool, "order_mode", mvArgType::KEYWORD_ARG, "0", "set using a constant: mvTabOrder_Reorderable: allows reordering, mvTabOrder_Fixed: fixed ordering, mvTabOrder_Leading: adds tab to front, mvTabOrder_Trailing: adds tab to back" });

        mvPythonParserSetup setup;
        setup.about = "Adds a tab to a tab bar.";
        setup.category = { "Containers", "Widgets"};
        setup.returnType = mvPyDataType::UUID;
        setup.createContextManager = true;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    mvTab::mvTab(mvUUID uuid)
        : 
        mvAppItem(uuid)
    {
    }

    void mvTab::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvTab*>(item);
        if (_source != 0) _value = titem->_value;
        _disabled_value = titem->_disabled_value;
        _closable = titem->_closable;
        _flags = titem->_flags;
    }

    void mvTab::addFlag(ImGuiTabItemFlags flag)
    {
        _flags |= flag;
    }

    void mvTab::removeFlag(ImGuiTabItemFlags flag)
    {
        _flags &= ~flag;
    }

    PyObject* mvTab::getPyValue()
    {
        return ToPyBool(*_value);
    }

    void mvTab::setPyValue(PyObject* value)
    {
        *_value = ToBool(value);
    }

    void mvTab::setDataSource(mvUUID dataSource)
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
        _value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
    }

    void mvTab::draw(ImDrawList* drawlist, float x, float y)
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

            // cast parent to mvTabBar
            auto parent = (mvTabBar*)_parentPtr;

            // check if this is first tab
            if (parent->getSpecificValue() == 0)
            {
                // set mvTabBar value to the first tab name
                parent->setValue(_uuid);
                *_value = true;
            }
            
            _state.lastFrameUpdate = GContext->frame;
            // create tab item and see if it is selected
            if (ImGui::BeginTabItem(_internalLabel.c_str(), _closable ? &_show : nullptr, _flags))
            {

                _state.hovered = ImGui::IsItemHovered();
                _state.active = ImGui::IsItemActive();
                _state.leftclicked = ImGui::IsItemClicked();
                _state.rightclicked = ImGui::IsItemClicked(1);
                _state.middleclicked = ImGui::IsItemClicked(2);
                _state.visible = ImGui::IsItemVisible();
                _state.activated = ImGui::IsItemActivated();
                _state.deactivated = ImGui::IsItemDeactivated();
                _state.rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
                _state.rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
                _state.rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
                _state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
        
                // set other tab's value false
                for (auto& child : parent->_children[1])
                {
                    if (child->getType() == mvAppItemType::mvTab)
                        *((mvTab*)child.get())->_value = false;
                }

                // set current tab value true
                *_value = true;

                // run call back if it exists
                if (parent->getSpecificValue() != _uuid)
                {
                    mvSubmitCallback([=]() {
                        if(parent->_alias.empty())
                            mvAddCallback(parent->getCallback(), parent->_uuid, ToPyUUID(_uuid), parent->_user_data);
                        else
                            mvAddCallback(parent->getCallback(), parent->_alias, ToPyUUID(_uuid), parent->_user_data);
                        });
                }

                parent->setValue(_uuid);

                for (auto& item : _children[1])
                    item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

                ImGui::EndTabItem();
            }

            else
            {
                _state.hovered = ImGui::IsItemHovered();
                _state.active = ImGui::IsItemActive();
                _state.leftclicked = ImGui::IsItemClicked();
                _state.rightclicked = ImGui::IsItemClicked(1);
                _state.middleclicked = ImGui::IsItemClicked(2);
                _state.visible = ImGui::IsItemVisible();
                _state.activated = ImGui::IsItemActivated();
                _state.deactivated = ImGui::IsItemDeactivated();
                _state.rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
                _state.rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
                _state.rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
                _state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
            }
        }

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

    void mvTab::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "closable")) _closable = ToBool(item);


        if (PyObject* item = PyDict_GetItemString(dict, "order_mode"))
        {
            long order_mode = (long)ToUUID(item);

            if (order_mode == (long)mvTab::TabOrdering::mvTabOrder_Fixed)
                _flags = ImGuiTabItemFlags_NoReorder;
            else if (order_mode == (long)mvTab::TabOrdering::mvTabOrder_Leading)
                _flags = ImGuiTabItemFlags_Leading;
            else if (order_mode == (long)mvTab::TabOrdering::mvTabOrder_Trailing)
                _flags = ImGuiTabItemFlags_Trailing;
            else
                _flags = ImGuiTabItemFlags_None;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "no_tooltip"))
        {
            bool value = ToBool(item);
            if (value)
                _flags |= ImGuiTabItemFlags_NoTooltip;
            else
                _flags &= ~ImGuiTabItemFlags_NoTooltip;
        }

    }

    void mvTab::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "closable", mvPyObject(ToPyBool(_closable)));

        // helper to check and set bit
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
        };

        checkbitset("no_tooltip", ImGuiTabItemFlags_NoTooltip, _flags);

        if(_flags & ImGuiTabItemFlags_Leading)
            PyDict_SetItemString(dict, "order_mode", mvPyObject(ToPyLong((long)mvTab::TabOrdering::mvTabOrder_Leading)));
        else if (_flags & ImGuiTabItemFlags_Trailing)
            PyDict_SetItemString(dict, "order_mode", mvPyObject(ToPyLong((long)mvTab::TabOrdering::mvTabOrder_Trailing)));
        else if (_flags & ImGuiTabBarFlags_Reorderable)
            PyDict_SetItemString(dict, "order_mode", mvPyObject(ToPyLong((long)mvTab::TabOrdering::mvTabOrder_Reorderable)));
        else
            PyDict_SetItemString(dict, "order_mode", mvPyObject(ToPyLong((long)mvTab::TabOrdering::mvTabOrder_Fixed)));

    }

}
