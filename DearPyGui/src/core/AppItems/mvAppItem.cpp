#include "mvAppItem.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvCore.h"
#include "mvPythonExceptions.h"
#include "mvGlobalIntepreterLock.h"
#include "mvAppItemCommons.h"
#include "mvLog.h"
#include "mvPythonTypeChecker.h"
#include "mvPyObject.h"

namespace Marvel {

    mv_internal void 
    UpdateLocations(std::vector<mvRef<mvAppItem>>* children, i32 slots)
    {
        for (i32 i = 0; i < slots; i++)
        {
            i32 index = 0;
            for (auto& child : children[i])
            {
                child->_location = index;
                index++;
            }
        }
    }

    mvAppItem::mvAppItem(mvUUID uuid)
    {
        _uuid = uuid;
        _internalLabel = "###" + std::to_string(_uuid);
        _state.parent = this;
    }

    mvAppItem::~mvAppItem()
    {
        for (auto& childset : _children)
        {
            childset.clear();
            childset.shrink_to_fit();
        }
        onChildrenRemoved();

        mvGlobalIntepreterLock gil;
        if (_callback) Py_DECREF(_callback);
        if (_user_data) Py_DECREF(_user_data);
        if (_dragCallback) Py_DECREF(_dragCallback);
        if (_dropCallback) Py_DECREF(_dropCallback);

        // in case item registry is destroyed
        if (GContext->itemRegistry)
        {
            if (GContext->itemRegistry->aliases.count(_alias) != 0)
            {
                if (!GContext->IO.manualAliasManagement)
                    GContext->itemRegistry->aliases.erase(_alias);
            }
            CleanUpItem(*GContext->itemRegistry, _uuid);
        }
    }

    void 
    mvAppItem::applyTemplate(mvAppItem* item)
    {
        _useInternalLabel = item->_useInternalLabel;
        _tracked = item->_tracked;
        _trackOffset = item->_trackOffset;
        _searchLast = item->_searchLast;
        _indent = item->_indent;
        _show = item->_show;
        _filter = item->_filter;
        _payloadType = item->_payloadType;
        _enabled = item->_enabled;
        _source = item->_source;
        _font = item->_font;
        _theme = item->_theme;
        _width = item->_width;
        _height = item->_height;
        _dirty_size = true;
        //setPos(item->_state.pos);

        if (!item->_specifiedLabel.empty())
        {
            _specifiedLabel = item->_specifiedLabel;
            if (_useInternalLabel)
                _internalLabel = item->_specifiedLabel + "###" + std::to_string(_uuid);
            else
                _internalLabel = item->_specifiedLabel;
        }

        if (_enabled) _enabledLastFrame = true;
        else _disabledLastFrame = true;

        if (item->_callback)
        {
            Py_XINCREF(item->_callback);

            if (item->_callback == Py_None)
                _callback = nullptr;
            else
                _callback = item->_callback;

            
        }

        if (item->_dragCallback)
        {
            Py_XINCREF(item->_dragCallback);
            if (item->_dragCallback == Py_None)
                _dragCallback = nullptr;
            else
                _dragCallback = item->_dragCallback;
        }

        if (item->_dropCallback)
        {
            Py_XINCREF(item->_dropCallback);
            if (item->_dropCallback == Py_None)
                _dropCallback = nullptr;
            else
                _dropCallback = item->_dropCallback;
        }

        if (item->_user_data)
        {
            Py_XINCREF(item->_user_data);
            if (item->_user_data == Py_None)
                _user_data = nullptr;
            else
                _user_data = item->_user_data;
        }

        applySpecificTemplate(item);
    }

    b8 
    mvAppItem::moveChildUp(mvUUID uuid)
    {
        b8 found = false;
        i32 index = 0;

        for (auto& childset : _children)
        {
            // check children
            for (size_t i = 0; i < childset.size(); i++)
            {

                if (childset[i]->_uuid == uuid)
                {
                    found = true;
                    index = (i32)i;
                    break;
                }

                if (GetEntityDesciptionFlags(childset[i]->getType()) & MV_ITEM_DESC_CONTAINER)
                {
                    found = childset[i]->moveChildUp(uuid);
                    if (found)
                        return true;
                }

            }

            if (found)
            {
                if (index > 0)
                {
                    auto upperitem = childset[index - 1];
                    auto loweritem = childset[index];

                    childset[index] = upperitem;
                    childset[index - 1] = loweritem;

                    UpdateLocations(_children, 4);
                }

                return true;
            }
        }

        return false;
    }

    b8 
    mvAppItem::moveChildDown(mvUUID uuid)
    {
        b8 found = false;
        size_t index = 0;

        for (auto& childset : _children)
        {
            // check children
            for (size_t i = 0; i < childset.size(); i++)
            {

                if (childset[i]->_uuid == uuid)
                {
                    found = true;
                    index = i;
                    break;
                }

                if (GetEntityDesciptionFlags(childset[i]->getType()) & MV_ITEM_DESC_CONTAINER)
                {
                    found = childset[i]->moveChildDown(uuid);
                    if (found)
                        return true;
                }

            }

            if (found)
            {
                if (index < childset.size() - 1)
                {
                    auto upperitem = childset[index];
                    auto loweritem = childset[index + 1];

                    childset[index] = loweritem;
                    childset[index + 1] = upperitem;

                    UpdateLocations(_children, 4);
                }

                return true;
            }

            
        }

        return false;
    }

    b8
    mvAppItem::addRuntimeChild(mvUUID parent, mvUUID before, mvRef<mvAppItem> item)
    {
        if (before == 0 && parent == 0)
            return false;

        for (auto& children : _children)
        {
            //this is the container, add item to end.
            if (before == 0)
            {

                if (_uuid == parent)
                {
                    i32 targetSlot = GetEntityTargetSlot(item->getType());
                    item->_location = (i32)_children[targetSlot].size();
                    _children[targetSlot].push_back(item);
                    onChildAdd(item);
                    item->_parentPtr = this;
                    item->_parent = _uuid;
                    return true;
                }

                // check children
                for (auto& childslot : _children)
                {
                    for (auto& child : childslot)
                    {
                        if (GetEntityDesciptionFlags(child->getType()) & MV_ITEM_DESC_CONTAINER
                            || GetEntityDesciptionFlags(item->getType()) & MV_ITEM_DESC_HANDLER)
                        {
                            // parent found
                            if (child->addRuntimeChild(parent, before, item))
                                return true;
                        }
                    }
                }
            }

            // this is the container, add item to beginning.
            else
            {
                bool beforeFound = false;

                // check children
                for (auto& child : children)
                {

                    if (child->_uuid == before)
                    {
                        beforeFound = true;
                        break;
                    }
                }


                // after item is in this container
                if (beforeFound)
                {
                    item->_parentPtr = this;

                    std::vector<mvRef<mvAppItem>> oldchildren = children;
                    children.clear();

                    for (auto& child : oldchildren)
                    {
                        if (child->_uuid == before)
                        {
                            children.push_back(item);
                            onChildAdd(item);
                        }
                        children.push_back(child);

                    }

                    UpdateLocations(_children, 4);

                    return true;
                }
            }

            // check children
            for (auto& child : children)
            {
                if (GetEntityDesciptionFlags(child->getType()) & MV_ITEM_DESC_CONTAINER
                    || GetEntityDesciptionFlags(item->getType()) & MV_ITEM_DESC_HANDLER)
                {
                    // parent found
                    if (child->addRuntimeChild(parent, before, item))
                        return true;
                }
            }

        };

        return false;
    }

    b8
    mvAppItem::addItem(mvRef<mvAppItem> item)
    {
        i32 targetSlot = GetEntityTargetSlot(item->getType());
        item->_location = (i32)_children[targetSlot].size();
        _children[targetSlot].push_back(item);
        onChildAdd(item);
        return true;
    }

    b8
    mvAppItem::addChildAfter(mvUUID prev, mvRef<mvAppItem> item)
    {
        if (prev == 0)
            return false;

        b8 prevFound = false;

        // check children
        for (auto& childslot : _children)
        {
            for (auto& child : childslot)
            {

                if (child->_uuid == prev)
                {
                    item->_parentPtr = this;
                    prevFound = true;
                    break;
                }

            }
        }

        // prev item is in this container
        if (prevFound)
        {
            //item->setParent(this);
            i32 targetSlot = GetEntityTargetSlot(item->getType());
            std::vector<mvRef<mvAppItem>> oldchildren = _children[targetSlot];
            _children[targetSlot].clear();

            for (auto& child : oldchildren)
            {
                _children[targetSlot].push_back(child);
                if (child->_uuid == prev)
                {
                    _children[targetSlot].push_back(item);
                    onChildAdd(item);
                }
            }

            return true;
        }


        // check children
        for (auto& childslot : _children)
        {
            for (auto& child : childslot)
            {
                // parent found
                if (child->addChildAfter(prev, item))
                    return true;
            }
        }

        return false;
    }

    b8 
    mvAppItem::deleteChild(mvUUID uuid)
    {

        for (auto& childset : _children)
        {
            b8 childfound = false;
            b8 itemDeleted = false;

            for (auto& item : childset)
            {
                if (item->_uuid == uuid)
                {
                    childfound = true;
                    break;
                }

                itemDeleted = item->deleteChild(uuid);
                if (itemDeleted)
                    break;
            }

            if (childfound)
            {
                std::vector<mvRef<mvAppItem>> oldchildren = childset;

                childset.clear();

                for (auto& item : oldchildren)
                {
                    if (item->_uuid == uuid)
                    {
                        itemDeleted = true;
                        onChildRemoved(item);
                        continue;
                    }

                    childset.push_back(item);
                }
            }

            if (itemDeleted)
            {
                UpdateLocations(_children, 4);
                return true;
            }
        }

        return false;
    }

    mvRef<mvAppItem>
    mvAppItem::stealChild(mvUUID uuid)
    {
        mvRef<mvAppItem> stolenChild = nullptr;

        for (auto& childset : _children)
        {
            b8 childfound = false;

            for (auto& item : childset)
            {
                if (item->_uuid == uuid)
                {
                    childfound = true;
                    break;
                }

                if (GetEntityDesciptionFlags(item->getType()) & MV_ITEM_DESC_CONTAINER)
                {
                    stolenChild = item->stealChild(uuid);
                    if (stolenChild)
                        return stolenChild;
                }
            }

            if (childfound)
            {
                std::vector<mvRef<mvAppItem>> oldchildren = childset;

                childset.clear();

                for (auto& item : oldchildren)
                {
                    if (item->_uuid == uuid)
                    {
                        stolenChild = item;
                        onChildRemoved(item);
                        continue;
                    }

                    childset.push_back(item);
                }

                UpdateLocations(_children, 4);

                return stolenChild;
            }


            //return static_cast<mvRef<mvAppItem>>(CreateRef<mvButton>("Not possible"));
        }

        return stolenChild;
    }

    mvAppItem* 
    mvAppItem::getChild(mvUUID uuid)
    {

        if (_uuid == uuid)
            return this;

        if (_searchLast)
        {
            if (_searchDelayed)
                _searchDelayed = false;
            else
            {
                _searchDelayed = true;
                DelaySearch(*GContext->itemRegistry, this);
                return nullptr;
            }
        }

        for (auto& childset : _children)
        {
            for (auto& item : childset)
            {
                if (item->_uuid == uuid)
                    return item.get();

                auto child = item->getChild(uuid);
                if (child)
                    return child;
            }
        }

        return nullptr;
    }

    mvRef<mvAppItem> 
    mvAppItem::getChildRef(mvUUID uuid)
    {

        for (auto& childset : _children)
        {
            for (auto& item : childset)
            {
                if (item->_uuid == uuid)
                    return item;

                auto child = item->getChildRef(uuid);
                if (child)
                    return child;
            }
        }

        return nullptr;
    }

    PyObject* 
    mvAppItem::getCallback(bool ignore_enabled)
    {
        if (_enabled)
            return _callback;

        return ignore_enabled ? _callback : nullptr;
    }

    void 
    mvAppItem::handleKeywordArgs(PyObject* dict, const std::string& parser)
    {
        if (dict == nullptr)
            return;

        if (VerifyKeywordArguments(GetParsers()[parser], dict))
            return;

        if (PyArg_ValidateKeywordArguments(dict) == 0)
        {
            assert(false);
            mvThrowPythonError(mvErrorCode::mvNone, "Dictionary keywords must be strings");
            return;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "use_internal_label")) _useInternalLabel = ToBool(item); // must be before label

        if (PyObject* item = PyDict_GetItemString(dict, "label"))
        {
            if (item != Py_None)
            {
                const std::string label = ToString(item);
                _specifiedLabel = label;
                if (_useInternalLabel)
                    _internalLabel = label + "###" + std::to_string(_uuid);
                else
                    _internalLabel = label;
            }
        }

        if (PyObject* item = PyDict_GetItemString(dict, "width"))
        {
            _dirty_size = true;
            _width = ToInt(item);
        }
        if (PyObject* item = PyDict_GetItemString(dict, "height"))
        {
            _dirty_size = true;
            _height = ToInt(item);
        }

        if (PyObject* item = PyDict_GetItemString(dict, "pos")) {
            std::vector<f32> position = ToFloatVect(item);
            if (!position.empty())
            {
                _dirtyPos = true;
                _state.pos = mvVec2{ position[0], position[1] };
            }
        }
        if (PyObject* item = PyDict_GetItemString(dict, "indent")) _indent = (f32)ToInt(item);
        if (PyObject* item = PyDict_GetItemString(dict, "show")) 
        {
            _show = ToBool(item);
            if (_show)
                _shownLastFrame = true;
            else
                _hiddenLastFrame = true;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "filter_key")) _filter = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "payload_type")) _payloadType = ToString(item);
        if (PyObject* item = PyDict_GetItemString(dict, "source"))
        {
            if (isPyObject_Int(item))
                setDataSource(ToUUID(item));
            else if (isPyObject_String(item))
            {
                std::string alias = ToString(item);
                setDataSource(GetIdFromAlias(*GContext->itemRegistry, alias));
            }
        }
        if (PyObject* item = PyDict_GetItemString(dict, "enabled"))
        {
            b8 value = ToBool(item);

            if (_enabled != value)
            {
                _enabled = value;

                if (value)
                    _enabledLastFrame = true;
                else
                    _disabledLastFrame = true;
            }
        }
        if (PyObject* item = PyDict_GetItemString(dict, "tracked")) _tracked = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "delay_search")) _searchLast = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "track_offset"))
        {
            _trackOffset = ToFloat(item);
        }
        if (PyObject* item = PyDict_GetItemString(dict, "default_value"))
        {
            if(_source == 0)
                setPyValue(item);
        }

        if (PyObject* item = PyDict_GetItemString(dict, "callback"))
        {
            if (_callback)
                Py_XDECREF(_callback);

            // TODO: investigate if PyNone should be increffed
            Py_XINCREF(item);
            if (item == Py_None)
                _callback = nullptr;
            else
                _callback = item;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "drag_callback"))
        {
            if (_dragCallback)
                Py_XDECREF(_dragCallback);

            Py_XINCREF(item);
            if (item == Py_None)
                _dragCallback = nullptr;
            else
                _dragCallback = item;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "drop_callback"))
        {
            if (_dropCallback)
                Py_XDECREF(_dropCallback);

            Py_XINCREF(item);

            if (item == Py_None)
                _dropCallback = nullptr;
            else
                _dropCallback = item;
        }

        if (PyObject* item = PyDict_GetItemString(dict, "user_data"))
        {
            if (_user_data)
                Py_XDECREF(_user_data);
            
            Py_XINCREF(item);
            if (item == Py_None)
                _user_data = nullptr;
            else
                _user_data = item;
        }

        handleSpecificKeywordArgs(dict);
    }

    void 
    mvAppItem::setDataSource(mvUUID value)
    {
        _source = value; 
    }

    mv_internal bool
    CanItemTypeBeHovered(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvCheckbox:
        case mvAppItemType::mvCombo:
        case mvAppItemType::mvDragInt:
        case mvAppItemType::mvDragIntMulti:
        case mvAppItemType::mvDragFloat:
        case mvAppItemType::mvDragFloatMulti:
        case mvAppItemType::mvImage:
        case mvAppItemType::mvImageButton:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvText:
        case mvAppItemType::mvColorButton:
        case mvAppItemType::mvColorEdit:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvColorMapButton:
        case mvAppItemType::mvColorMapScale:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvChildWindow:
        case mvAppItemType::mvCollapsingHeader:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvMenu:
        case mvAppItemType::mvTab:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvWindowAppItem:
        case mvAppItemType::mvDatePicker:
        case mvAppItemType::mvKnobFloat:
        case mvAppItemType::mvLoadingIndicator:
        case mvAppItemType::mvSlider3D:
        case mvAppItemType::mvDrawlist:
        case mvAppItemType::mvNode:
        case mvAppItemType::mvNodeAttribute:
        case mvAppItemType::mvNodeEditor:
        case mvAppItemType::mvNodeLink:
        case mvAppItemType::mvPlot:
        case mvAppItemType::mvTableColumn:
        case mvAppItemType::mvButton: return true;
        default: return false;
        }

    }

    mv_internal bool
    CanItemTypeBeActive(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvCheckbox:
        case mvAppItemType::mvCombo:
        case mvAppItemType::mvDragInt:
        case mvAppItemType::mvDragIntMulti:
        case mvAppItemType::mvDragFloat:
        case mvAppItemType::mvDragFloatMulti:
        case mvAppItemType::mvImageButton:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvColorButton:
        case mvAppItemType::mvColorEdit:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvColorMapButton:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvChildWindow:
        case mvAppItemType::mvCollapsingHeader:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvMenu:
        case mvAppItemType::mvTab:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvDatePicker:
        case mvAppItemType::mvKnobFloat:
        case mvAppItemType::mvDrawlist:
        case mvAppItemType::mvPlot:
        case mvAppItemType::mvButton: return true;
        default: return false;
        }

    }

    mv_internal bool
    CanItemTypeBeFocused(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvCheckbox:
        case mvAppItemType::mvCombo:
        case mvAppItemType::mvDragInt:
        case mvAppItemType::mvDragIntMulti:
        case mvAppItemType::mvDragFloat:
        case mvAppItemType::mvDragFloatMulti:
        case mvAppItemType::mvImageButton:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvColorButton:
        case mvAppItemType::mvColorEdit:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvColorMapButton:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvChildWindow:
        case mvAppItemType::mvCollapsingHeader:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvMenu:
        case mvAppItemType::mvTab:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvWindowAppItem:
        case mvAppItemType::mvDatePicker:
        case mvAppItemType::mvKnobFloat:
        case mvAppItemType::mvDrawlist:
        case mvAppItemType::mvPlot:
        case mvAppItemType::mvButton: return true;
        default: return false;
        }

    }

    mv_internal bool
    CanItemTypeBeClicked(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvCheckbox:
        case mvAppItemType::mvCombo:
        case mvAppItemType::mvDragInt:
        case mvAppItemType::mvDragFloat:
        case mvAppItemType::mvDragFloatMulti:
        case mvAppItemType::mvDragIntMulti:
        case mvAppItemType::mvImageButton:
        case mvAppItemType::mvImage:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvText:
        case mvAppItemType::mvColorButton:
        case mvAppItemType::mvColorEdit:
        case mvAppItemType::mvColorMapButton:
        case mvAppItemType::mvColorMapScale:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvCollapsingHeader:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvTab:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvDatePicker:
        case mvAppItemType::mvKnobFloat:
        case mvAppItemType::mvLoadingIndicator:
        case mvAppItemType::mvDrawlist:
        case mvAppItemType::mvNode:
        case mvAppItemType::mvPlot:
        case mvAppItemType::mvButton: return true;
        default: return false;
        }

    }

    mv_internal bool
    CanItemTypeBeVisible(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvCheckbox:
        case mvAppItemType::mvCombo:
        case mvAppItemType::mvDragInt:
        case mvAppItemType::mvDragIntMulti:
        case mvAppItemType::mvDragFloat:
        case mvAppItemType::mvDragFloatMulti:
        case mvAppItemType::mvImage:
        case mvAppItemType::mvImageButton:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvText:
        case mvAppItemType::mvColorButton:
        case mvAppItemType::mvColorEdit:
        case mvAppItemType::mvColorMapButton:
        case mvAppItemType::mvColorMapScale:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvCollapsingHeader:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvMenuBar:
        case mvAppItemType::mvTab:
        case mvAppItemType::mvTabBar:
        case mvAppItemType::mvTooltip:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvWindowAppItem:
        case mvAppItemType::mvDatePicker:
        case mvAppItemType::mvKnobFloat:
        case mvAppItemType::mvLoadingIndicator:
        case mvAppItemType::mvSlider3D:
        case mvAppItemType::mvTimePicker:
        case mvAppItemType::mvDrawlist:
        case mvAppItemType::mvNode:
        case mvAppItemType::mvNodeEditor:
        case mvAppItemType::mvPlot:
        case mvAppItemType::mvTable:
        case mvAppItemType::mvTableColumn:
        case mvAppItemType::mvTableRow:
        case mvAppItemType::mvButton: return true;
        default: return false;
        }

    }

    mv_internal bool
    CanItemTypeBeEdited(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvDragInt:
        case mvAppItemType::mvDragIntMulti:
        case mvAppItemType::mvDragFloat:
        case mvAppItemType::mvDragFloatMulti:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvColorEdit:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvKnobFloat:
        case mvAppItemType::mvCombo: return true;
        default: return false;
        }

    }

    mv_internal bool
    CanItemTypeBeActivated(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvCheckbox:
        case mvAppItemType::mvCombo:
        case mvAppItemType::mvDragInt:
        case mvAppItemType::mvDragIntMulti:
        case mvAppItemType::mvDragFloat:
        case mvAppItemType::mvDragFloatMulti:
        case mvAppItemType::mvImageButton:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvColorButton:
        case mvAppItemType::mvColorEdit:
        case mvAppItemType::mvColorMapButton:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvCollapsingHeader:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvMenu:
        case mvAppItemType::mvTab:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvDatePicker:
        case mvAppItemType::mvKnobFloat:
        case mvAppItemType::mvDrawlist:
        case mvAppItemType::mvPlot:
        case mvAppItemType::mvButton: return true;
        default: return false;
        }

    }

    mv_internal bool
    CanItemTypeBeDeactivated(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvCheckbox:
        case mvAppItemType::mvCombo:
        case mvAppItemType::mvDragInt:
        case mvAppItemType::mvDragIntMulti:
        case mvAppItemType::mvDragFloat:
        case mvAppItemType::mvDragFloatMulti:
        case mvAppItemType::mvImageButton:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvColorButton:
        case mvAppItemType::mvColorEdit:
        case mvAppItemType::mvColorMapButton:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvChildWindow:
        case mvAppItemType::mvCollapsingHeader:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvMenu:
        case mvAppItemType::mvTab:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvDatePicker:
        case mvAppItemType::mvKnobFloat:
        case mvAppItemType::mvDrawlist:
        case mvAppItemType::mvPlot:
        case mvAppItemType::mvButton: return true;
        default: return false;
        }

    }

    mv_internal bool
    CanItemTypeBeDeactivatedAE(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvDragInt:
        case mvAppItemType::mvDragIntMulti:
        case mvAppItemType::mvDragFloat:
        case mvAppItemType::mvDragFloatMulti:
        case mvAppItemType::mvCombo:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvColorEdit:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvCheckbox: return true;
        default: return false;
        }

    }

    mv_internal bool
    CanItemTypeBeToggledOpen(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvWindowAppItem:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvCollapsingHeader: return true;
        default: return false;
        }

    }

    mv_internal bool
    CanItemTypeHaveRectMin(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvCheckbox:
        case mvAppItemType::mvCombo:
        case mvAppItemType::mvDragInt:
        case mvAppItemType::mvDragIntMulti:
        case mvAppItemType::mvDragFloat:
        case mvAppItemType::mvDragFloatMulti:
        case mvAppItemType::mvImage:
        case mvAppItemType::mvImageButton:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvText:
        case mvAppItemType::mvColorButton:
        case mvAppItemType::mvColorEdit:
        case mvAppItemType::mvColorMapButton:
        case mvAppItemType::mvColorMapScale:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvCollapsingHeader:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvTab:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvDatePicker:
        case mvAppItemType::mvKnobFloat:
        case mvAppItemType::mvLoadingIndicator:
        case mvAppItemType::mvSlider3D:
        case mvAppItemType::mvTimePicker:
        case mvAppItemType::mvDrawlist:
        case mvAppItemType::mvPlot:
        case mvAppItemType::mvButton: return true;
        default: return false;
        }

    }

    mv_internal bool
    CanItemTypeHaveRectMax(mvAppItemType type)
    {
        return CanItemTypeHaveRectMin(type);
    }

    mv_internal bool
    CanItemTypeHaveRectSize(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvCheckbox:
        case mvAppItemType::mvCombo:
        case mvAppItemType::mvDragInt:
        case mvAppItemType::mvDragIntMulti:
        case mvAppItemType::mvDragFloat:
        case mvAppItemType::mvDragFloatMulti:
        case mvAppItemType::mvImage:
        case mvAppItemType::mvImageButton:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvText:
        case mvAppItemType::mvColorButton:
        case mvAppItemType::mvColorEdit:
        case mvAppItemType::mvColorMapButton:
        case mvAppItemType::mvColorMapScale:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvChildWindow:
        case mvAppItemType::mvCollapsingHeader:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvMenu:
        case mvAppItemType::mvTab:
        case mvAppItemType::mvTooltip:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvWindowAppItem:
        case mvAppItemType::mvDatePicker:
        case mvAppItemType::mvKnobFloat:
        case mvAppItemType::mvLoadingIndicator:
        case mvAppItemType::mvSlider3D:
        case mvAppItemType::mvTimePicker:
        case mvAppItemType::mvDrawlist:
        case mvAppItemType::mvNode:
        case mvAppItemType::mvNodeEditor:
        case mvAppItemType::mvPlot:
        case mvAppItemType::mvButton: return true;
        default: return false;
        }

    }

    mv_internal bool
    CanItemTypeHaveContAvail(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvCheckbox:
        case mvAppItemType::mvCombo:
        case mvAppItemType::mvDragInt:
        case mvAppItemType::mvDragIntMulti:
        case mvAppItemType::mvDragFloat:
        case mvAppItemType::mvDragFloatMulti:
        case mvAppItemType::mvImage:
        case mvAppItemType::mvImageButton:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvText:
        case mvAppItemType::mvColorButton:
        case mvAppItemType::mvColorEdit:
        case mvAppItemType::mvColorMapButton:
        case mvAppItemType::mvColorMapScale:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvChildWindow:
        case mvAppItemType::mvCollapsingHeader:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvTab:
        case mvAppItemType::mvTooltip:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvDatePicker:
        case mvAppItemType::mvKnobFloat:
        case mvAppItemType::mvLoadingIndicator:
        case mvAppItemType::mvSlider3D:
        case mvAppItemType::mvDrawlist:
        case mvAppItemType::mvPlot:
        case mvAppItemType::mvButton: return true;
        default: return false;
        }

    }

    i32
    GetApplicableState(mvAppItemType type)
    {
        i32 applicableState = MV_STATE_NONE;
        if(CanItemTypeBeHovered(type)) applicableState |= MV_STATE_HOVER;
        if(CanItemTypeBeActive(type)) applicableState |= MV_STATE_ACTIVE;
        if(CanItemTypeBeFocused(type)) applicableState |= MV_STATE_FOCUSED;
        if(CanItemTypeBeClicked(type)) applicableState |= MV_STATE_CLICKED;
        if(CanItemTypeBeVisible(type)) applicableState |= MV_STATE_VISIBLE;
        if(CanItemTypeBeEdited(type)) applicableState |= MV_STATE_EDITED;
        if(CanItemTypeBeActivated(type)) applicableState |= MV_STATE_ACTIVATED;
        if(CanItemTypeBeDeactivated(type)) applicableState |= MV_STATE_DEACTIVATED;
        if(CanItemTypeBeDeactivatedAE(type)) applicableState |= MV_STATE_DEACTIVATEDAE;
        if(CanItemTypeBeToggledOpen(type)) applicableState |= MV_STATE_TOGGLED_OPEN;
        if(CanItemTypeHaveRectMin(type)) applicableState |= MV_STATE_RECT_MIN;
        if(CanItemTypeHaveRectMax(type)) applicableState |= MV_STATE_RECT_MAX;
        if(CanItemTypeHaveRectSize(type)) applicableState |= MV_STATE_RECT_SIZE;
        if(CanItemTypeHaveContAvail(type)) applicableState |= MV_STATE_CONT_AVAIL;

        return applicableState;
    }

    i32 
    GetEntityDesciptionFlags(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvThemeComponent:
        case mvAppItemType::mvTable:
        case mvAppItemType::mvTableCell:
        case mvAppItemType::mvTableRow:
        case mvAppItemType::mv2dHistogramSeries:
        case mvAppItemType::mvAreaSeries:
        case mvAppItemType::mvBarSeries:
        case mvAppItemType::mvCandleSeries:
        case mvAppItemType::mvErrorSeries:
        case mvAppItemType::mvHeatSeries:
        case mvAppItemType::mvHistogramSeries:
        case mvAppItemType::mvImageSeries:
        case mvAppItemType::mvVLineSeries:
        case mvAppItemType::mvHLineSeries:
        case mvAppItemType::mvLabelSeries:
        case mvAppItemType::mvLineSeries:
        case mvAppItemType::mvPieSeries:
        case mvAppItemType::mvScatterSeries:
        case mvAppItemType::mvShadeSeries:
        case mvAppItemType::mvStairSeries:
        case mvAppItemType::mvStemSeries:
        case mvAppItemType::mvPlot:
        case mvAppItemType::mvSubPlots:
        case mvAppItemType::mvPlotAxis:
        case mvAppItemType::mvPlotLegend:
        case mvAppItemType::mvNode:
        case mvAppItemType::mvNodeAttribute:
        case mvAppItemType::mvNodeEditor:
        case mvAppItemType::mvFont:
        case mvAppItemType::mvDrawLayer:
        case mvAppItemType::mvDrawlist:
        case mvAppItemType::mvDrawNode:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvTab:
        case mvAppItemType::mvTabBar:
        case mvAppItemType::mvMenuBar:
        case mvAppItemType::mvMenu:
        case mvAppItemType::mvGroup:
        case mvAppItemType::mvCollapsingHeader:
        case mvAppItemType::mvClipper:
        case mvAppItemType::mvChildWindow:
        case mvAppItemType::mvFilterSet: return MV_ITEM_DESC_CONTAINER;

        case mvAppItemType::mvTooltip:
        case mvAppItemType::mvActivatedHandler:
        case mvAppItemType::mvActiveHandler:
        case mvAppItemType::mvClickedHandler:
        case mvAppItemType::mvDeactivatedAfterEditHandler:
        case mvAppItemType::mvDeactivatedHandler:
        case mvAppItemType::mvEditedHandler:
        case mvAppItemType::mvFocusHandler:
        case mvAppItemType::mvHoverHandler:
        case mvAppItemType::mvResizeHandler:
        case mvAppItemType::mvToggledOpenHandler:
        case mvAppItemType::mvVisibleHandler:
        case mvAppItemType::mvDragPayload: return MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_HANDLER;

        case mvAppItemType::mvTheme:
        case mvAppItemType::mvValueRegistry:
        case mvAppItemType::mvItemHandlerRegistry:
        case mvAppItemType::mvTextureRegistry:
        case mvAppItemType::mvHandlerRegistry:
        case mvAppItemType::mvFontRegistry:
        case mvAppItemType::mvWindowAppItem:
        case mvAppItemType::mvViewportDrawlist:
        case mvAppItemType::mvViewportMenuBar:
        case mvAppItemType::mvTemplateRegistry:
        case mvAppItemType::mvFileDialog:
        case mvAppItemType::mvColorMapRegistry:
        case mvAppItemType::mvStage: return MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER;
        default: return MV_ITEM_DESC_DEFAULT;
        }
    }

    i32
    GetEntityTargetSlot(mvAppItemType type)
    {
        switch (type)
        {
        case mvAppItemType::mvFileExtension:
        case mvAppItemType::mvFontRangeHint:
        case mvAppItemType::mvNodeLink:
        case mvAppItemType::mvAnnotation:
        case mvAppItemType::mvDragLine:
        case mvAppItemType::mvDragPoint:
        case mvAppItemType::mvPlotLegend:
        case mvAppItemType::mvTableColumn: return 0;

        case mvAppItemType::mvDrawBezierCubic:
        case mvAppItemType::mvDrawBezierQuadratic:
        case mvAppItemType::mvDrawCircle:
        case mvAppItemType::mvDrawEllipse:
        case mvAppItemType::mvDrawImage:
        case mvAppItemType::mvDrawImageQuad:
        case mvAppItemType::mvDrawLayer:
        case mvAppItemType::mvDrawLine:
        case mvAppItemType::mvDrawNode:
        case mvAppItemType::mvDrawPolygon:
        case mvAppItemType::mvDrawPolyline:
        case mvAppItemType::mvDrawQuad:
        case mvAppItemType::mvDrawRect:
        case mvAppItemType::mvDrawText:
        case mvAppItemType::mvDrawTriangle:
        case mvAppItemType::mvDrawArrow: return 2;

        case mvAppItemType::mvDragPayload: return 3;

        default: return 1;
        }
    }

    StorageValueTypes
    GetEntityValueType(mvAppItemType type)
    {
        switch (type)
        {

        case mvAppItemType::mvTimePicker:
        case mvAppItemType::mvDatePicker: return StorageValueTypes::Time;

        case mvAppItemType::mvTabBar: return StorageValueTypes::UUID;

        case mvAppItemType::mvColorValue:
        case mvAppItemType::mvThemeColor:
        case mvAppItemType::mvColorPicker:
        case mvAppItemType::mvColorEdit: return StorageValueTypes::Color;

        case mvAppItemType::mvIntValue:
        case mvAppItemType::mvSliderInt:
        case mvAppItemType::mvInputInt:
        case mvAppItemType::mvDragInt: return StorageValueTypes::Int;

        case mvAppItemType::mvFloatValue:
        case mvAppItemType::mvProgressBar:
        case mvAppItemType::mvKnobFloat:
        case mvAppItemType::mvColorMapSlider:
        case mvAppItemType::mvSliderFloat:
        case mvAppItemType::mvInputFloat:
        case mvAppItemType::mvDragFloat: return StorageValueTypes::Float;
        
        case mvAppItemType::mvFloat4Value:
        case mvAppItemType::mvThemeStyle:
        case mvAppItemType::mvAnnotation:
        case mvAppItemType::mvSlider3D:
        case mvAppItemType::mvInputFloatMulti:
        case mvAppItemType::mvSliderFloatMulti:
        case mvAppItemType::mvDragFloatMulti: return StorageValueTypes::Float4;

        case mvAppItemType::mvInt4Value:
        case mvAppItemType::mvInputIntMulti:
        case mvAppItemType::mvSliderIntMulti:
        case mvAppItemType::mvDragIntMulti: return StorageValueTypes::Int4;

        case mvAppItemType::mvStringValue:
        case mvAppItemType::mvText:
        case mvAppItemType::mvRadioButton:
        case mvAppItemType::mvListbox:
        case mvAppItemType::mvInputText:
        case mvAppItemType::mvCombo: return StorageValueTypes::String;

        case mvAppItemType::mvBoolValue:
        case mvAppItemType::mvTreeNode:
        case mvAppItemType::mvTooltip:
        case mvAppItemType::mvTab:
        case mvAppItemType::mvMenuBar:
        case mvAppItemType::mvMenu:
        case mvAppItemType::mvCollapsingHeader:
        case mvAppItemType::mvChildWindow:
        case mvAppItemType::mvFileDialog:
        case mvAppItemType::mvSelectable:
        case mvAppItemType::mvMenuItem:
        case mvAppItemType::mvCheckbox: return StorageValueTypes::Bool;
        
        case mvAppItemType::mvSeriesValue:
        case mvAppItemType::mv2dHistogramSeries:
        case mvAppItemType::mvAreaSeries:
        case mvAppItemType::mvBarSeries:
        case mvAppItemType::mvCandleSeries:
        case mvAppItemType::mvErrorSeries:
        case mvAppItemType::mvHeatSeries:
        case mvAppItemType::mvHistogramSeries:
        case mvAppItemType::mvImageSeries:
        case mvAppItemType::mvVLineSeries:
        case mvAppItemType::mvHLineSeries:
        case mvAppItemType::mvLabelSeries:
        case mvAppItemType::mvLineSeries:
        case mvAppItemType::mvPieSeries:
        case mvAppItemType::mvScatterSeries:
        case mvAppItemType::mvShadeSeries:
        case mvAppItemType::mvStairSeries:
        case mvAppItemType::mvStemSeries: return StorageValueTypes::Series;

        case mvAppItemType::mvDoubleValue:
        case mvAppItemType::mvDragLine: return StorageValueTypes::Double;
        
        case mvAppItemType::mvDouble4Value:
        case mvAppItemType::mvDragPoint: return StorageValueTypes::Double4;

        case mvAppItemType::mvStaticTexture:
        case mvAppItemType::mvDynamicTexture:
        case mvAppItemType::mvSimplePlot: return StorageValueTypes::FloatVect;

        default: return StorageValueTypes::None;
        }
    }

    const char* 
    GetEntityTypeString(mvAppItemType type)
    {
        #define X(el) "mvAppItemType::" #el,
        mv_local_persist const char* entity_type_strings[(size_t)mvAppItemType::ItemTypeCount] =
        {
            "All, an error occured", // shouldn't actually occur
            MV_ITEM_TYPES
        };
        #undef X
        return entity_type_strings[(size_t)type];
    }

    mvRef<mvAppItem>
    CreateEntity(mvAppItemType type, mvUUID id)
    {
        #define X(el) case mvAppItemType::el: return CreateRef<el>(id);
        switch (type)
        {
            MV_ITEM_TYPES
            default: return nullptr;
        }
        #undef X
    }

    const std::vector<std::pair<std::string, i32>>& 
    GetAllowableParents(mvAppItemType type)
    {

        // TODO: possibly index into array instead of switch

        #define MV_ADD_PARENT(x){#x, (int)x}
        #define MV_START_PARENTS {mv_local_persist std::vector<std::pair<std::string, i32>> parents = {
        #define MV_END_PARENTS };return parents;}

        switch (type)
        {

        case mvAppItemType::mvActivatedHandler:
        case mvAppItemType::mvActiveHandler:
        case mvAppItemType::mvClickedHandler:
        case mvAppItemType::mvDeactivatedAfterEditHandler:
        case mvAppItemType::mvDeactivatedHandler:
        case mvAppItemType::mvEditedHandler:
        case mvAppItemType::mvFocusHandler:
        case mvAppItemType::mvHoverHandler:
        case mvAppItemType::mvResizeHandler:
        case mvAppItemType::mvToggledOpenHandler:
        case mvAppItemType::mvVisibleHandler:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvItemHandlerRegistry)
            MV_END_PARENTS

        case mvAppItemType::mvBoolValue:
        case mvAppItemType::mvColorValue:
        case mvAppItemType::mvDouble4Value:
        case mvAppItemType::mvDoubleValue:
        case mvAppItemType::mvFloat4Value:
        case mvAppItemType::mvFloatValue:
        case mvAppItemType::mvFloatVectValue:
        case mvAppItemType::mvInt4Value:
        case mvAppItemType::mvIntValue:
        case mvAppItemType::mvSeriesValue:
        case mvAppItemType::mvStringValue:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvValueRegistry)
            MV_END_PARENTS

        case mvAppItemType::mvThemeComponent:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTheme),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry)
            MV_END_PARENTS

        case mvAppItemType::mvThemeStyle:
        case mvAppItemType::mvThemeColor:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvThemeComponent),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_END_PARENTS

        case mvAppItemType::mvDynamicTexture:
        case mvAppItemType::mvStaticTexture:
        case mvAppItemType::mvRawTexture:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvTextureRegistry)
            MV_END_PARENTS

        case mvAppItemType::mvTableCell:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvTableRow)
            MV_END_PARENTS

        case mvAppItemType::mvTableColumn:
        case mvAppItemType::mvTableRow:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvTable)
            MV_END_PARENTS

        case mvAppItemType::mvDrawBezierCubic:
        case mvAppItemType::mvDrawBezierQuadratic:
        case mvAppItemType::mvDrawCircle:
        case mvAppItemType::mvDrawEllipse:
        case mvAppItemType::mvDrawImage:
        case mvAppItemType::mvDrawImageQuad:
        case mvAppItemType::mvDrawLine:
        case mvAppItemType::mvDrawPolygon:
        case mvAppItemType::mvDrawPolyline:
        case mvAppItemType::mvDrawQuad:
        case mvAppItemType::mvDrawRect:
        case mvAppItemType::mvDrawText:
        case mvAppItemType::mvDrawTriangle:
        case mvAppItemType::mvDrawArrow:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvDrawlist),
            MV_ADD_PARENT(mvAppItemType::mvDrawLayer),
            MV_ADD_PARENT(mvAppItemType::mvWindowAppItem),
            MV_ADD_PARENT(mvAppItemType::mvPlot),
            MV_ADD_PARENT(mvAppItemType::mvDrawNode),
            MV_ADD_PARENT(mvAppItemType::mvViewportDrawlist),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_END_PARENTS

        case mvAppItemType::mvDrawLayer:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvDrawlist),
            MV_ADD_PARENT(mvAppItemType::mvWindowAppItem),
            MV_ADD_PARENT(mvAppItemType::mvPlot),
            MV_ADD_PARENT(mvAppItemType::mvViewportDrawlist)
            MV_END_PARENTS

        case mvAppItemType::mvDrawNode:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvDrawlist),
            MV_ADD_PARENT(mvAppItemType::mvDrawLayer),
            MV_ADD_PARENT(mvAppItemType::mvWindowAppItem),
            MV_ADD_PARENT(mvAppItemType::mvPlot),
            MV_ADD_PARENT(mvAppItemType::mvViewportDrawlist),
            MV_ADD_PARENT(mvAppItemType::mvDrawNode),
            MV_END_PARENTS

        case mvAppItemType::mvMenuBar:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvWindowAppItem),
            MV_ADD_PARENT(mvAppItemType::mvChildWindow),
            MV_ADD_PARENT(mvAppItemType::mvNodeEditor),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_END_PARENTS

        case mvAppItemType::mvColorMap:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvColorMapRegistry),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry)
            MV_END_PARENTS

        case mvAppItemType::mvFileExtension:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvFileDialog),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_END_PARENTS

        case mvAppItemType::mvTab:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTabBar),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_END_PARENTS

        case mvAppItemType::mvTabButton:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTabBar),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry)
            MV_END_PARENTS

        case mvAppItemType::mvNodeAttribute:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvNode)
            MV_END_PARENTS

        case mvAppItemType::mvNodeLink:
        case mvAppItemType::mvNode:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvNodeEditor)
            MV_END_PARENTS

        case mvAppItemType::mvPlotLegend:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvPlot),
            MV_ADD_PARENT(mvAppItemType::mvSubPlots)
            MV_END_PARENTS

        case mvAppItemType::mvPlotAxis:
        case mvAppItemType::mvDragLine:
        case mvAppItemType::mvDragPoint:
        case mvAppItemType::mvAnnotation:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvPlot)
            MV_END_PARENTS

        case mvAppItemType::mvAreaSeries:
        case mvAppItemType::mvBarSeries:
        case mvAppItemType::mvCandleSeries:
        case mvAppItemType::mvErrorSeries:
        case mvAppItemType::mvHeatSeries:
        case mvAppItemType::mvHistogramSeries:
        case mvAppItemType::mvImageSeries:
        case mvAppItemType::mvVLineSeries:
        case mvAppItemType::mvHLineSeries:
        case mvAppItemType::mvLabelSeries:
        case mvAppItemType::mvLineSeries:
        case mvAppItemType::mvPieSeries:
        case mvAppItemType::mvScatterSeries:
        case mvAppItemType::mvShadeSeries:
        case mvAppItemType::mvStairSeries:
        case mvAppItemType::mvStemSeries:
        case mvAppItemType::mv2dHistogramSeries:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvPlotAxis),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_END_PARENTS

        case mvAppItemType::mvMouseClickHandler:
        case mvAppItemType::mvMouseDoubleClickHandler:
        case mvAppItemType::mvMouseDownHandler:
        case mvAppItemType::mvMouseDragHandler:
        case mvAppItemType::mvMouseMoveHandler:
        case mvAppItemType::mvMouseReleaseHandler:
        case mvAppItemType::mvMouseWheelHandler:
        case mvAppItemType::mvKeyPressHandler:
        case mvAppItemType::mvKeyReleaseHandler:
        case mvAppItemType::mvKeyDownHandler:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvHandlerRegistry)
            MV_END_PARENTS

        case mvAppItemType::mvFontChars:
        case mvAppItemType::mvFontRange:
        case mvAppItemType::mvFontRangeHint:
        case mvAppItemType::mvCharRemap:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvFont),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry)
            MV_END_PARENTS

        case mvAppItemType::mvFont:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvFontRegistry)
            MV_END_PARENTS

        case mvAppItemType::mvDragPayload:
            MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvButton),
            MV_ADD_PARENT(mvAppItemType::mvCheckbox),
            MV_ADD_PARENT(mvAppItemType::mvCombo),
            MV_ADD_PARENT(mvAppItemType::mvDragIntMulti),
            MV_ADD_PARENT(mvAppItemType::mvDragFloatMulti),
            MV_ADD_PARENT(mvAppItemType::mvDragInt),
            MV_ADD_PARENT(mvAppItemType::mvDragFloat),
            MV_ADD_PARENT(mvAppItemType::mvImage),
            MV_ADD_PARENT(mvAppItemType::mvImageButton),
            MV_ADD_PARENT(mvAppItemType::mvInputIntMulti),
            MV_ADD_PARENT(mvAppItemType::mvInputFloatMulti),
            MV_ADD_PARENT(mvAppItemType::mvInputInt),
            MV_ADD_PARENT(mvAppItemType::mvInputFloat),
            MV_ADD_PARENT(mvAppItemType::mvInputText),
            MV_ADD_PARENT(mvAppItemType::mvListbox),
            MV_ADD_PARENT(mvAppItemType::mvMenuItem),
            MV_ADD_PARENT(mvAppItemType::mvRadioButton),
            MV_ADD_PARENT(mvAppItemType::mvSelectable),
            MV_ADD_PARENT(mvAppItemType::mvSliderIntMulti),
            MV_ADD_PARENT(mvAppItemType::mvSliderFloatMulti),
            MV_ADD_PARENT(mvAppItemType::mvSliderInt),
            MV_ADD_PARENT(mvAppItemType::mvSliderFloat),
            MV_ADD_PARENT(mvAppItemType::mvTabButton),
            MV_ADD_PARENT(mvAppItemType::mvText),
            MV_ADD_PARENT(mvAppItemType::mvColorButton),
            MV_ADD_PARENT(mvAppItemType::mvColorEdit),
            MV_ADD_PARENT(mvAppItemType::mvColorMapButton),
            MV_ADD_PARENT(mvAppItemType::mvColorPicker),
            MV_ADD_PARENT(mvAppItemType::mvCollapsingHeader),
            MV_ADD_PARENT(mvAppItemType::mvGroup),
            MV_ADD_PARENT(mvAppItemType::mvTreeNode),
            MV_ADD_PARENT(mvAppItemType::mvDatePicker),
            MV_ADD_PARENT(mvAppItemType::mvKnobFloat),
            MV_ADD_PARENT(mvAppItemType::mvLoadingIndicator),
            MV_ADD_PARENT(mvAppItemType::mvSlider3D),
            MV_ADD_PARENT(mvAppItemType::mvTimePicker),
            MV_ADD_PARENT(mvAppItemType::mvProgressBar),
            MV_ADD_PARENT(mvAppItemType::mvNode),
            MV_ADD_PARENT(mvAppItemType::mvPlot)
            MV_END_PARENTS

        default:
        {
            mv_local_persist std::vector<std::pair<std::string, i32>> parents = { {"All", 0} };
            return parents;
        }
        }

        #undef MV_ADD_PARENT
        #undef MV_START_PARENTS
        #undef MV_END_PARENTS
    }

    const std::vector<std::pair<std::string, i32>>&
    GetAllowableChildren(mvAppItemType type)
    {

        // TODO: possibly index into array instead of switch

        #define MV_ADD_CHILD(x){#x, (int)x}
        #define MV_START_CHILDREN {mv_local_persist std::vector<std::pair<std::string, i32>> children = {
        #define MV_END_CHILDREN };return children;}

        switch (type)
        {

        case mvAppItemType::mvItemHandlerRegistry:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvActivatedHandler),
            MV_ADD_CHILD(mvAppItemType::mvActiveHandler),
            MV_ADD_CHILD(mvAppItemType::mvClickedHandler),
            MV_ADD_CHILD(mvAppItemType::mvDeactivatedAfterEditHandler),
            MV_ADD_CHILD(mvAppItemType::mvDeactivatedHandler),
            MV_ADD_CHILD(mvAppItemType::mvEditedHandler),
            MV_ADD_CHILD(mvAppItemType::mvFocusHandler),
            MV_ADD_CHILD(mvAppItemType::mvHoverHandler),
            MV_ADD_CHILD(mvAppItemType::mvResizeHandler),
            MV_ADD_CHILD(mvAppItemType::mvToggledOpenHandler),
            MV_ADD_CHILD(mvAppItemType::mvVisibleHandler)
            MV_END_CHILDREN

        case mvAppItemType::mvValueRegistry:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvBoolValue),
            MV_ADD_CHILD(mvAppItemType::mvIntValue),
            MV_ADD_CHILD(mvAppItemType::mvInt4Value),
            MV_ADD_CHILD(mvAppItemType::mvFloatValue),
            MV_ADD_CHILD(mvAppItemType::mvFloat4Value),
            MV_ADD_CHILD(mvAppItemType::mvStringValue),
            MV_ADD_CHILD(mvAppItemType::mvDoubleValue),
            MV_ADD_CHILD(mvAppItemType::mvDouble4Value),
            MV_ADD_CHILD(mvAppItemType::mvColorValue),
            MV_ADD_CHILD(mvAppItemType::mvFloatVectValue),
            MV_ADD_CHILD(mvAppItemType::mvSeriesValue)
            MV_END_CHILDREN

        case mvAppItemType::mvThemeComponent:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvThemeColor),
            MV_ADD_CHILD(mvAppItemType::mvThemeStyle)
            MV_END_CHILDREN

        case mvAppItemType::mvTheme:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvThemeComponent)
            MV_END_CHILDREN

        case mvAppItemType::mvTextureRegistry:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvStaticTexture),
            MV_ADD_CHILD(mvAppItemType::mvDynamicTexture),
            MV_ADD_CHILD(mvAppItemType::mvRawTexture)
            MV_END_CHILDREN

        case mvAppItemType::mvTable:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvTableRow),
            MV_ADD_CHILD(mvAppItemType::mvTableColumn)
            MV_END_CHILDREN

        case mvAppItemType::mvSubPlots:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvPlot),
            MV_ADD_CHILD(mvAppItemType::mvPlotLegend)
            MV_END_CHILDREN

        case mvAppItemType::mvPlot:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvPlotLegend),
            MV_ADD_CHILD(mvAppItemType::mvPlotAxis),
            MV_ADD_CHILD(mvAppItemType::mvDragPoint),
            MV_ADD_CHILD(mvAppItemType::mvDragLine),
            MV_ADD_CHILD(mvAppItemType::mvAnnotation),
            MV_ADD_CHILD(mvAppItemType::mvDrawLine),
            MV_ADD_CHILD(mvAppItemType::mvDrawArrow),
            MV_ADD_CHILD(mvAppItemType::mvDrawTriangle),
            MV_ADD_CHILD(mvAppItemType::mvDrawCircle),
            MV_ADD_CHILD(mvAppItemType::mvDrawEllipse),
            MV_ADD_CHILD(mvAppItemType::mvDrawBezierCubic),
            MV_ADD_CHILD(mvAppItemType::mvDrawBezierQuadratic),
            MV_ADD_CHILD(mvAppItemType::mvDrawQuad),
            MV_ADD_CHILD(mvAppItemType::mvDrawRect),
            MV_ADD_CHILD(mvAppItemType::mvDrawText),
            MV_ADD_CHILD(mvAppItemType::mvDrawPolygon),
            MV_ADD_CHILD(mvAppItemType::mvDrawPolyline),
            MV_ADD_CHILD(mvAppItemType::mvDrawImage),
            MV_ADD_CHILD(mvAppItemType::mvDrawLayer),
            MV_ADD_CHILD(mvAppItemType::mvActivatedHandler),
            MV_ADD_CHILD(mvAppItemType::mvActiveHandler),
            MV_ADD_CHILD(mvAppItemType::mvClickedHandler),
            MV_ADD_CHILD(mvAppItemType::mvDeactivatedAfterEditHandler),
            MV_ADD_CHILD(mvAppItemType::mvDeactivatedHandler),
            MV_ADD_CHILD(mvAppItemType::mvEditedHandler),
            MV_ADD_CHILD(mvAppItemType::mvFocusHandler),
            MV_ADD_CHILD(mvAppItemType::mvHoverHandler),
            MV_ADD_CHILD(mvAppItemType::mvResizeHandler),
            MV_ADD_CHILD(mvAppItemType::mvToggledOpenHandler),
            MV_ADD_CHILD(mvAppItemType::mvVisibleHandler),
            MV_ADD_CHILD(mvAppItemType::mvDragPayload),
            MV_ADD_CHILD(mvAppItemType::mvDrawNode),
            MV_END_CHILDREN

        case mvAppItemType::mvNodeEditor:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvMenuBar),
            MV_ADD_CHILD(mvAppItemType::mvNode),
            MV_ADD_CHILD(mvAppItemType::mvNodeLink),
            MV_ADD_CHILD(mvAppItemType::mvActivatedHandler),
            MV_ADD_CHILD(mvAppItemType::mvActiveHandler),
            MV_ADD_CHILD(mvAppItemType::mvClickedHandler),
            MV_ADD_CHILD(mvAppItemType::mvDeactivatedAfterEditHandler),
            MV_ADD_CHILD(mvAppItemType::mvDeactivatedHandler),
            MV_ADD_CHILD(mvAppItemType::mvEditedHandler),
            MV_ADD_CHILD(mvAppItemType::mvFocusHandler),
            MV_ADD_CHILD(mvAppItemType::mvHoverHandler),
            MV_ADD_CHILD(mvAppItemType::mvResizeHandler),
            MV_ADD_CHILD(mvAppItemType::mvToggledOpenHandler),
            MV_ADD_CHILD(mvAppItemType::mvVisibleHandler)
            MV_END_CHILDREN

        case mvAppItemType::mvNode:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvNodeAttribute),
            MV_ADD_CHILD(mvAppItemType::mvActiveHandler),
            MV_ADD_CHILD(mvAppItemType::mvClickedHandler),
            MV_ADD_CHILD(mvAppItemType::mvHoverHandler),
            MV_ADD_CHILD(mvAppItemType::mvVisibleHandler),
            MV_ADD_CHILD(mvAppItemType::mvDragPayload),
            MV_END_CHILDREN

        case mvAppItemType::mvHandlerRegistry:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvKeyDownHandler),
            MV_ADD_CHILD(mvAppItemType::mvKeyPressHandler),
            MV_ADD_CHILD(mvAppItemType::mvKeyReleaseHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseMoveHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseWheelHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseClickHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseDoubleClickHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseDownHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseReleaseHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseDragHandler)
            MV_END_CHILDREN

        case mvAppItemType::mvFontRegistry:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvFont)
            MV_END_CHILDREN

        case mvAppItemType::mvFont:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvFontChars),
            MV_ADD_CHILD(mvAppItemType::mvFontRange),
            MV_ADD_CHILD(mvAppItemType::mvCharRemap),
            MV_ADD_CHILD(mvAppItemType::mvFontRangeHint),
            MV_ADD_CHILD(mvAppItemType::mvTemplateRegistry),
            MV_END_CHILDREN

        case mvAppItemType::mvViewportDrawlist:
        case mvAppItemType::mvDrawlist:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvDrawLayer),
            MV_ADD_CHILD(mvAppItemType::mvDrawLine),
            MV_ADD_CHILD(mvAppItemType::mvDrawArrow),
            MV_ADD_CHILD(mvAppItemType::mvDrawTriangle),
            MV_ADD_CHILD(mvAppItemType::mvDrawCircle),
            MV_ADD_CHILD(mvAppItemType::mvDrawEllipse),
            MV_ADD_CHILD(mvAppItemType::mvDrawBezierCubic),
            MV_ADD_CHILD(mvAppItemType::mvDrawBezierQuadratic),
            MV_ADD_CHILD(mvAppItemType::mvDrawQuad),
            MV_ADD_CHILD(mvAppItemType::mvDrawRect),
            MV_ADD_CHILD(mvAppItemType::mvDrawText),
            MV_ADD_CHILD(mvAppItemType::mvDrawPolygon),
            MV_ADD_CHILD(mvAppItemType::mvDrawPolyline),
            MV_ADD_CHILD(mvAppItemType::mvDrawImageQuad),
            MV_ADD_CHILD(mvAppItemType::mvDrawImage),
            MV_ADD_CHILD(mvAppItemType::mvDrawNode),
            MV_END_CHILDREN

        case mvAppItemType::mvDrawNode:
        case mvAppItemType::mvDrawLayer:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvDrawLine),
            MV_ADD_CHILD(mvAppItemType::mvDrawArrow),
            MV_ADD_CHILD(mvAppItemType::mvDrawTriangle),
            MV_ADD_CHILD(mvAppItemType::mvDrawCircle),
            MV_ADD_CHILD(mvAppItemType::mvDrawEllipse),
            MV_ADD_CHILD(mvAppItemType::mvDrawBezierCubic),
            MV_ADD_CHILD(mvAppItemType::mvDrawBezierQuadratic),
            MV_ADD_CHILD(mvAppItemType::mvDrawQuad),
            MV_ADD_CHILD(mvAppItemType::mvDrawRect),
            MV_ADD_CHILD(mvAppItemType::mvDrawText),
            MV_ADD_CHILD(mvAppItemType::mvDrawPolygon),
            MV_ADD_CHILD(mvAppItemType::mvDrawPolyline),
            MV_ADD_CHILD(mvAppItemType::mvDrawImage),
            MV_ADD_CHILD(mvAppItemType::mvDrawImageQuad),
            MV_ADD_CHILD(mvAppItemType::mvDrawNode),
            MV_END_CHILDREN

        case mvAppItemType::mvTabBar:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvTab),
            MV_ADD_CHILD(mvAppItemType::mvTabButton),
            MV_ADD_CHILD(mvAppItemType::mvActivatedHandler),
            MV_ADD_CHILD(mvAppItemType::mvActiveHandler),
            MV_ADD_CHILD(mvAppItemType::mvClickedHandler),
            MV_ADD_CHILD(mvAppItemType::mvDeactivatedAfterEditHandler),
            MV_ADD_CHILD(mvAppItemType::mvDeactivatedHandler),
            MV_ADD_CHILD(mvAppItemType::mvEditedHandler),
            MV_ADD_CHILD(mvAppItemType::mvFocusHandler),
            MV_ADD_CHILD(mvAppItemType::mvHoverHandler),
            MV_ADD_CHILD(mvAppItemType::mvResizeHandler),
            MV_ADD_CHILD(mvAppItemType::mvToggledOpenHandler),
            MV_ADD_CHILD(mvAppItemType::mvVisibleHandler)
            MV_END_CHILDREN

        case mvAppItemType::mvColorMapRegistry:
            MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvColorMap)
            MV_END_CHILDREN

        default:
            {
                mv_local_persist std::vector<std::pair<std::string, i32>> parents = { {"All", 0} };
                return parents;
            }
        }

        #undef MV_ADD_CHILD
        #undef MV_START_CHILDREN
        #undef MV_END_CHILDREN
    }

    mvRef<mvAppItem>
    GetClassThemeComponent(mvAppItemType type)
    {
        #define X(el) case mvAppItemType::el: { mv_local_persist mvRef<mvAppItem> s_class_theme = nullptr; return s_class_theme; }
        switch (type)
        {
        MV_ITEM_TYPES
        default:
            {
                mv_local_persist mvRef<mvAppItem> s_class_theme = nullptr;
                assert(false && "Class type now found.");
                return s_class_theme;
            }
        }
        #undef X
    }

    mvRef<mvAppItem>
    GetDisabledClassThemeComponent(mvAppItemType type)
    {
        #define X(el) case mvAppItemType::el: { mv_local_persist mvRef<mvAppItem> s_class_theme = nullptr; return s_class_theme; }
        switch (type)
        {
        MV_ITEM_TYPES
        default:
            {
                mv_local_persist mvRef<mvAppItem> s_class_theme = nullptr;
                assert(false && "Class type now found.");
                return s_class_theme;
            }
        }
        #undef X
    }
}
