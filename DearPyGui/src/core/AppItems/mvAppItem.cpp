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
        if (GContext->started)
        {
            RemoveAlias(*GContext->itemRegistry, _alias, true);
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

                if (childset[i]->getDescFlags() & MV_ITEM_DESC_CONTAINER)
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

                if (childset[i]->getDescFlags() & MV_ITEM_DESC_CONTAINER)
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
                    item->_location = (i32)_children[item->getTarget()].size();
                    _children[item->getTarget()].push_back(item);
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
                        if (child->getDescFlags() & MV_ITEM_DESC_CONTAINER 
                            || item->getDescFlags() & MV_ITEM_DESC_HANDLER)
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
                if (child->getDescFlags() & MV_ITEM_DESC_CONTAINER
                    || item->getDescFlags() & MV_ITEM_DESC_HANDLER)
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
        item->_location = (i32)_children[item->getTarget()].size();
        _children[item->getTarget()].push_back(item);
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

            std::vector<mvRef<mvAppItem>> oldchildren = _children[item->getTarget()];
            _children[item->getTarget()].clear();

            for (auto& child : oldchildren)
            {
                _children[item->getTarget()].push_back(child);
                if (child->_uuid == prev)
                {
                    _children[item->getTarget()].push_back(item);
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

                if (item->getDescFlags() & MV_ITEM_DESC_CONTAINER)
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

}
