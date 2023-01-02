#include "mvAppItem.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvCore.h"
#include "mvAppItemCommons.h"
#include "mvPyUtils.h"

static void
UpdateLocations(std::vector<std::shared_ptr<mvAppItem>>* children, i32 slots)
{
    for (i32 i = 0; i < slots; i++)
    {
        i32 index = 0;
        for (auto& child : children[i])
        {
            child->info.location = index;
            index++;
        }
    }
}

mvAppItem::mvAppItem(mvUUID uuid)
{
    this->uuid = uuid;
    info.internalLabel = "###" + std::to_string(uuid);
    state.parent = this;
}

mvAppItem::~mvAppItem()
{
    for (auto& childset : childslots)
    {
        childset.clear();
    }

    if (type == mvAppItemType::mvTable)
        static_cast<mvTable*>(this)->onChildrenRemoved();

    mvGlobalIntepreterLock gil;
    if (config.callback) Py_DECREF(config.callback);
    if (config.user_data) Py_DECREF(config.user_data);
    if (config.dragCallback) Py_DECREF(config.dragCallback);
    if (config.dropCallback) Py_DECREF(config.dropCallback);

    // in case item registry is destroyed
    if (GContext->itemRegistry)
    {
        if (GContext->itemRegistry->aliases.count(config.alias) != 0)
        {
            if (!GContext->IO.manualAliasManagement)
                GContext->itemRegistry->aliases.erase(config.alias);
        }
        CleanUpItem(*GContext->itemRegistry, uuid);
    }
}

PyObject* 
mvAppItem::getCallback(bool ignore_enabled)
{
    if (config.enabled)
        return config.callback;

    return ignore_enabled ? config.callback : nullptr;
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

    if (PyObject* item = PyDict_GetItemString(dict, "use_internal_label")) config.useInternalLabel = ToBool(item); // must be before label

    if (PyObject* item = PyDict_GetItemString(dict, "label"))
    {
        if (item != Py_None)
        {
            const std::string label = ToString(item);
            config.specifiedLabel = label;
            if (config.useInternalLabel)
                info.internalLabel = label + "###" + std::to_string(uuid);
            else
                info.internalLabel = label;
        }
    }

    if (PyObject* item = PyDict_GetItemString(dict, "width"))
    {
        info.dirty_size = true;
        config.width = ToInt(item);
    }
    if (PyObject* item = PyDict_GetItemString(dict, "height"))
    {
        info.dirty_size = true;
        config.height = ToInt(item);
    }

    if (PyObject* item = PyDict_GetItemString(dict, "pos")) {
        std::vector<f32> position = ToFloatVect(item);
        if (!position.empty())
        {
            info.dirtyPos = true;
            state.pos = mvVec2{ position[0], position[1] };
        }
    }
    if (PyObject* item = PyDict_GetItemString(dict, "indent")) config.indent = (f32)ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "show")) 
    {
        config.show = ToBool(item);
        if (config.show)
            info.shownLastFrame = true;
        else
            info.hiddenLastFrame = true;
    }

    if (PyObject* item = PyDict_GetItemString(dict, "filter_key")) config.filter = ToString(item);
    if (PyObject* item = PyDict_GetItemString(dict, "payload_type")) config.payloadType = ToString(item);
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

        if (config.enabled != value)
        {
            config.enabled = value;

            if (value)
                info.enabledLastFrame = true;
            else
                info.disabledLastFrame = true;
        }
    }
    if (PyObject* item = PyDict_GetItemString(dict, "tracked")) config.tracked = ToBool(item);
    if (PyObject* item = PyDict_GetItemString(dict, "delay_search")) config.searchLast = ToBool(item);
    if (PyObject* item = PyDict_GetItemString(dict, "track_offset"))
    {
        config.trackOffset = ToFloat(item);
    }
    if (PyObject* item = PyDict_GetItemString(dict, "default_value"))
    {
        if(config.source == 0)
            setPyValue(item);
    }

    if (PyObject* item = PyDict_GetItemString(dict, "callback"))
    {
        if (config.callback)
            Py_XDECREF(config.callback);

        // TODO: investigate if PyNone should be increffed
        Py_XINCREF(item);
        if (item == Py_None)
            config.callback = nullptr;
        else
            config.callback = item;
    }

    if (PyObject* item = PyDict_GetItemString(dict, "drag_callback"))
    {
        if (config.dragCallback)
            Py_XDECREF(config.dragCallback);

        Py_XINCREF(item);
        if (item == Py_None)
            config.dragCallback = nullptr;
        else
            config.dragCallback = item;
    }

    if (PyObject* item = PyDict_GetItemString(dict, "drop_callback"))
    {
        if (config.dropCallback)
            Py_XDECREF(config.dropCallback);

        Py_XINCREF(item);

        if (item == Py_None)
            config.dropCallback = nullptr;
        else
            config.dropCallback = item;
    }

    if (PyObject* item = PyDict_GetItemString(dict, "user_data"))
    {
        if (config.user_data)
            Py_XDECREF(config.user_data);
            
        Py_XINCREF(item);
        if (item == Py_None)
            config.user_data = nullptr;
        else
            config.user_data = item;
    }

    handleSpecificKeywordArgs(dict);
}

void 
mvAppItem::setDataSource(mvUUID value)
{
    config.source = value; 
}

static bool
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
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvImage:
    case mvAppItemType::mvImageButton:
    case mvAppItemType::mvInputText:
    case mvAppItemType::mvInputInt:
    case mvAppItemType::mvInputFloat:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvInputIntMulti:
    case mvAppItemType::mvInputFloatMulti:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvListbox:
    case mvAppItemType::mvRadioButton:
    case mvAppItemType::mvSelectable:
    case mvAppItemType::mvSliderInt:
    case mvAppItemType::mvSliderFloat:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvSliderDoubleMulti:
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

static bool
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
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvImageButton:
    case mvAppItemType::mvInputText:
    case mvAppItemType::mvInputInt:
    case mvAppItemType::mvInputFloat:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvInputIntMulti:
    case mvAppItemType::mvInputFloatMulti:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvListbox:
    case mvAppItemType::mvRadioButton:
    case mvAppItemType::mvSelectable:
    case mvAppItemType::mvSliderInt:
    case mvAppItemType::mvSliderFloat:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvSliderDoubleMulti:
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

static bool
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
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvImageButton:
    case mvAppItemType::mvInputText:
    case mvAppItemType::mvInputInt:
    case mvAppItemType::mvInputFloat:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvInputIntMulti:
    case mvAppItemType::mvInputFloatMulti:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvListbox:
    case mvAppItemType::mvRadioButton:
    case mvAppItemType::mvSelectable:
    case mvAppItemType::mvSliderInt:
    case mvAppItemType::mvSliderFloat:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvSliderDoubleMulti:
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

static bool
CanItemTypeBeClicked(mvAppItemType type)
{
    switch (type)
    {
    case mvAppItemType::mvCheckbox:
    case mvAppItemType::mvCombo:
    case mvAppItemType::mvDragInt:
    case mvAppItemType::mvDragFloat:
    case mvAppItemType::mvDragFloatMulti:
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvDragIntMulti:
    case mvAppItemType::mvImageButton:
    case mvAppItemType::mvImage:
    case mvAppItemType::mvInputText:
    case mvAppItemType::mvInputInt:
    case mvAppItemType::mvInputFloat:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvInputIntMulti:
    case mvAppItemType::mvInputFloatMulti:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvListbox:
    case mvAppItemType::mvRadioButton:
    case mvAppItemType::mvSelectable:
    case mvAppItemType::mvSliderInt:
    case mvAppItemType::mvSliderFloat:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvSliderDoubleMulti:
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

static bool
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
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvImage:
    case mvAppItemType::mvImageButton:
    case mvAppItemType::mvInputText:
    case mvAppItemType::mvInputInt:
    case mvAppItemType::mvInputIntMulti:
    case mvAppItemType::mvInputFloat:
    case mvAppItemType::mvInputFloatMulti:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvListbox:
    case mvAppItemType::mvRadioButton:
    case mvAppItemType::mvSelectable:
    case mvAppItemType::mvSliderInt:
    case mvAppItemType::mvSliderIntMulti:
    case mvAppItemType::mvSliderFloat:
    case mvAppItemType::mvSliderFloatMulti:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvSliderDoubleMulti:
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

static bool
CanItemTypeBeEdited(mvAppItemType type)
{
    switch (type)
    {
    case mvAppItemType::mvDragInt:
    case mvAppItemType::mvDragIntMulti:
    case mvAppItemType::mvDragFloat:
    case mvAppItemType::mvDragFloatMulti:
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvInputText:
    case mvAppItemType::mvInputInt:
    case mvAppItemType::mvInputIntMulti:
    case mvAppItemType::mvInputFloat:
    case mvAppItemType::mvInputFloatMulti:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvListbox:
    case mvAppItemType::mvRadioButton:
    case mvAppItemType::mvSelectable:
    case mvAppItemType::mvSliderInt:
    case mvAppItemType::mvSliderIntMulti:
    case mvAppItemType::mvSliderFloat:
    case mvAppItemType::mvSliderFloatMulti:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvSliderDoubleMulti:
    case mvAppItemType::mvColorEdit:
    case mvAppItemType::mvColorMapSlider:
    case mvAppItemType::mvColorPicker:
    case mvAppItemType::mvGroup:
    case mvAppItemType::mvKnobFloat:
    case mvAppItemType::mvCombo: return true;
    default: return false;
    }

}

static bool
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
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvImageButton:
    case mvAppItemType::mvInputText:
    case mvAppItemType::mvInputInt:
    case mvAppItemType::mvInputIntMulti:
    case mvAppItemType::mvInputFloat:
    case mvAppItemType::mvInputFloatMulti:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvListbox:
    case mvAppItemType::mvRadioButton:
    case mvAppItemType::mvSelectable:
    case mvAppItemType::mvSliderInt:
    case mvAppItemType::mvSliderIntMulti:
    case mvAppItemType::mvSliderFloat:
    case mvAppItemType::mvSliderFloatMulti:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvSliderDoubleMulti:
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

static bool
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
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvImageButton:
    case mvAppItemType::mvInputText:
    case mvAppItemType::mvInputInt:
    case mvAppItemType::mvInputIntMulti:
    case mvAppItemType::mvInputFloat:
    case mvAppItemType::mvInputFloatMulti:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvListbox:
    case mvAppItemType::mvRadioButton:
    case mvAppItemType::mvSelectable:
    case mvAppItemType::mvSliderInt:
    case mvAppItemType::mvSliderIntMulti:
    case mvAppItemType::mvSliderFloat:
    case mvAppItemType::mvSliderFloatMulti:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvSliderDoubleMulti:
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

static bool
CanItemTypeBeDeactivatedAE(mvAppItemType type)
{
    switch (type)
    {
    case mvAppItemType::mvDragInt:
    case mvAppItemType::mvDragIntMulti:
    case mvAppItemType::mvDragFloat:
    case mvAppItemType::mvDragFloatMulti:
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvCombo:
    case mvAppItemType::mvInputText:
    case mvAppItemType::mvInputInt:
    case mvAppItemType::mvInputIntMulti:
    case mvAppItemType::mvInputFloat:
    case mvAppItemType::mvInputFloatMulti:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvListbox:
    case mvAppItemType::mvRadioButton:
    case mvAppItemType::mvSelectable:
    case mvAppItemType::mvSliderInt:
    case mvAppItemType::mvSliderIntMulti:
    case mvAppItemType::mvSliderFloat:
    case mvAppItemType::mvSliderFloatMulti:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvSliderDoubleMulti:
    case mvAppItemType::mvColorEdit:
    case mvAppItemType::mvColorMapSlider:
    case mvAppItemType::mvColorPicker:
    case mvAppItemType::mvGroup:
    case mvAppItemType::mvCheckbox: return true;
    default: return false;
    }

}

static bool
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

static bool
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
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvImage:
    case mvAppItemType::mvImageButton:
    case mvAppItemType::mvInputText:
    case mvAppItemType::mvInputInt:
    case mvAppItemType::mvInputIntMulti:
    case mvAppItemType::mvInputFloat:
    case mvAppItemType::mvInputFloatMulti:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvListbox:
    case mvAppItemType::mvRadioButton:
    case mvAppItemType::mvSelectable:
    case mvAppItemType::mvSliderInt:
    case mvAppItemType::mvSliderIntMulti:
    case mvAppItemType::mvSliderFloat:
    case mvAppItemType::mvSliderFloatMulti:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvSliderDoubleMulti:
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
    case mvAppItemType::mvNode:
    case mvAppItemType::mvNodeEditor:
    case mvAppItemType::mvPlot:
    case mvAppItemType::mvButton: return true;
    default: return false;
    }

}

static bool
CanItemTypeHaveRectMax(mvAppItemType type)
{
    return CanItemTypeHaveRectMin(type);
}

static bool
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
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvImage:
    case mvAppItemType::mvImageButton:
    case mvAppItemType::mvInputText:
    case mvAppItemType::mvInputInt:
    case mvAppItemType::mvInputIntMulti:
    case mvAppItemType::mvInputFloat:
    case mvAppItemType::mvInputFloatMulti:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvListbox:
    case mvAppItemType::mvRadioButton:
    case mvAppItemType::mvSelectable:
    case mvAppItemType::mvSliderInt:
    case mvAppItemType::mvSliderIntMulti:
    case mvAppItemType::mvSliderFloat:
    case mvAppItemType::mvSliderFloatMulti:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvSliderDoubleMulti:
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

static bool
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
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvImage:
    case mvAppItemType::mvImageButton:
    case mvAppItemType::mvInputText:
    case mvAppItemType::mvInputInt:
    case mvAppItemType::mvInputIntMulti:
    case mvAppItemType::mvInputFloat:
    case mvAppItemType::mvInputFloatMulti:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvListbox:
    case mvAppItemType::mvRadioButton:
    case mvAppItemType::mvSelectable:
    case mvAppItemType::mvSliderInt:
    case mvAppItemType::mvSliderIntMulti:
    case mvAppItemType::mvSliderFloat:
    case mvAppItemType::mvSliderFloatMulti:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvSliderDoubleMulti:
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

int
DearPyGui::GetApplicableState(mvAppItemType type)
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

int 
DearPyGui::GetEntityDesciptionFlags(mvAppItemType type)
{
    switch (type)
    {

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
    case mvAppItemType::mvDrawArrow: return MV_ITEM_DESC_DRAW_CMP;

    case mvAppItemType::mvDrawNode:
    case mvAppItemType::mvDrawLayer:
    case mvAppItemType::mvDrawlist: return MV_ITEM_DESC_DRAW_CMP | MV_ITEM_DESC_CONTAINER;

    case mvAppItemType::mvThemeComponent:
    case mvAppItemType::mvTable:
    case mvAppItemType::mvTableCell:
    case mvAppItemType::mvTableRow:
    case mvAppItemType::mv2dHistogramSeries:
    case mvAppItemType::mvAreaSeries:
    case mvAppItemType::mvBarSeries:
    case mvAppItemType::mvCandleSeries:
    case mvAppItemType::mvCustomSeries:
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

    case mvAppItemType::mvActivatedHandler:
    case mvAppItemType::mvActiveHandler:
    case mvAppItemType::mvClickedHandler:
    case mvAppItemType::mvDoubleClickedHandler:
    case mvAppItemType::mvDeactivatedAfterEditHandler:
    case mvAppItemType::mvDeactivatedHandler:
    case mvAppItemType::mvEditedHandler:
    case mvAppItemType::mvFocusHandler:
    case mvAppItemType::mvHoverHandler:
    case mvAppItemType::mvResizeHandler:
    case mvAppItemType::mvToggledOpenHandler:
    case mvAppItemType::mvVisibleHandler: return MV_ITEM_DESC_HANDLER;

    case mvAppItemType::mvTooltip:
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

int
DearPyGui::GetEntityTargetSlot(mvAppItemType type)
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
DearPyGui::GetEntityValueType(mvAppItemType type)
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
    case mvAppItemType::mvCustomSeries:
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
    case mvAppItemType::mvDragDouble:
    case mvAppItemType::mvInputDouble:
    case mvAppItemType::mvSliderDouble:
    case mvAppItemType::mvDragLine: return StorageValueTypes::Double;
        
    case mvAppItemType::mvDouble4Value:
    case mvAppItemType::mvDragDoubleMulti:
    case mvAppItemType::mvInputDoubleMulti:
    case mvAppItemType::mvSliderDoubleMulti:
    case mvAppItemType::mvAnnotation:
    case mvAppItemType::mvDragPoint: return StorageValueTypes::Double4;

    case mvAppItemType::mvStaticTexture:
    case mvAppItemType::mvDynamicTexture:
    case mvAppItemType::mvSimplePlot: return StorageValueTypes::FloatVect;

    default: return StorageValueTypes::None;
    }
}

const char* 
DearPyGui::GetEntityTypeString(mvAppItemType type)
{
    #define X(el) "mvAppItemType::" #el,
    static const char* entity_type_strings[(size_t)mvAppItemType::ItemTypeCount] =
    {
        "All, an error occured", // shouldn't actually occur
        MV_ITEM_TYPES
    };
    #undef X
    return entity_type_strings[(size_t)type];
}

std::shared_ptr<mvAppItem>
DearPyGui::CreateEntity(mvAppItemType type, mvUUID id)
{
    #define X(el) case mvAppItemType::el: {auto item = std::make_shared<el>(id); item->type = mvAppItemType::el; return item;};
    switch (type)
    {
        MV_ITEM_TYPES
        default: return nullptr;
    }
    #undef X
}

const std::vector<std::pair<std::string, i32>>& 
DearPyGui::GetAllowableParents(mvAppItemType type)
{

    // TODO: possibly index into array instead of switch

    #define MV_ADD_PARENT(x){#x, (int)x}
    #define MV_START_PARENTS {static std::vector<std::pair<std::string, i32>> parents = {
    #define MV_END_PARENTS };return parents;}

    switch (type)
    {

    case mvAppItemType::mvActivatedHandler:
    case mvAppItemType::mvActiveHandler:
    case mvAppItemType::mvClickedHandler:
    case mvAppItemType::mvDoubleClickedHandler:
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
        MV_ADD_PARENT(mvAppItemType::mvCustomSeries),
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
        MV_ADD_PARENT(mvAppItemType::mvDragDoubleMulti),
        MV_ADD_PARENT(mvAppItemType::mvDragInt),
        MV_ADD_PARENT(mvAppItemType::mvDragFloat),
        MV_ADD_PARENT(mvAppItemType::mvDragDouble),
        MV_ADD_PARENT(mvAppItemType::mvImage),
        MV_ADD_PARENT(mvAppItemType::mvImageButton),
        MV_ADD_PARENT(mvAppItemType::mvInputIntMulti),
        MV_ADD_PARENT(mvAppItemType::mvInputFloatMulti),
        MV_ADD_PARENT(mvAppItemType::mvInputDoubleMulti),
        MV_ADD_PARENT(mvAppItemType::mvInputInt),
        MV_ADD_PARENT(mvAppItemType::mvInputFloat),
        MV_ADD_PARENT(mvAppItemType::mvInputDouble),
        MV_ADD_PARENT(mvAppItemType::mvInputText),
        MV_ADD_PARENT(mvAppItemType::mvListbox),
        MV_ADD_PARENT(mvAppItemType::mvMenuItem),
        MV_ADD_PARENT(mvAppItemType::mvRadioButton),
        MV_ADD_PARENT(mvAppItemType::mvSelectable),
        MV_ADD_PARENT(mvAppItemType::mvSliderIntMulti),
        MV_ADD_PARENT(mvAppItemType::mvSliderFloatMulti),
        MV_ADD_PARENT(mvAppItemType::mvSliderDoubleMulti),
        MV_ADD_PARENT(mvAppItemType::mvSliderInt),
        MV_ADD_PARENT(mvAppItemType::mvSliderFloat),
        MV_ADD_PARENT(mvAppItemType::mvSliderDouble),
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
        static std::vector<std::pair<std::string, i32>> parents = { {"All", 0} };
        return parents;
    }
    }

    #undef MV_ADD_PARENT
    #undef MV_START_PARENTS
    #undef MV_END_PARENTS
}

const std::vector<std::pair<std::string, i32>>&
DearPyGui::GetAllowableChildren(mvAppItemType type)
{

    // TODO: possibly index into array instead of switch

    #define MV_ADD_CHILD(x){#x, (int)x}
    #define MV_START_CHILDREN {static std::vector<std::pair<std::string, i32>> children = {
    #define MV_END_CHILDREN };return children;}

    switch (type)
    {

    case mvAppItemType::mvItemHandlerRegistry:
        MV_START_CHILDREN
        MV_ADD_CHILD(mvAppItemType::mvActivatedHandler),
        MV_ADD_CHILD(mvAppItemType::mvActiveHandler),
        MV_ADD_CHILD(mvAppItemType::mvClickedHandler),
        MV_ADD_CHILD(mvAppItemType::mvDoubleClickedHandler),
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
        MV_ADD_CHILD(mvAppItemType::mvTooltip),
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
        MV_ADD_CHILD(mvAppItemType::mvDoubleClickedHandler),
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
        MV_ADD_CHILD(mvAppItemType::mvDoubleClickedHandler),
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
        MV_ADD_CHILD(mvAppItemType::mvDoubleClickedHandler),
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
        MV_ADD_CHILD(mvAppItemType::mvDoubleClickedHandler),
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
            static std::vector<std::pair<std::string, i32>> parents = { {"All", 0} };
            return parents;
        }
    }

    #undef MV_ADD_CHILD
    #undef MV_START_CHILDREN
    #undef MV_END_CHILDREN
}

std::shared_ptr<mvThemeComponent>&
DearPyGui::GetClassThemeComponent(mvAppItemType type)
{
    #define X(el) case mvAppItemType::el: { static std::shared_ptr<mvThemeComponent> s_class_theme = nullptr; return s_class_theme; }
    switch (type)
    {
    MV_ITEM_TYPES
    default:
        {
            static std::shared_ptr<mvThemeComponent> s_class_theme = nullptr;
            return s_class_theme;
        }
    }
    #undef X
}

std::shared_ptr<mvThemeComponent>&
DearPyGui::GetDisabledClassThemeComponent(mvAppItemType type)
{
    #define X(el) case mvAppItemType::el: { static std::shared_ptr<mvThemeComponent> s_class_theme = nullptr; return s_class_theme; }
    switch (type)
    {
    MV_ITEM_TYPES
    default:
        {
            static std::shared_ptr<mvThemeComponent> s_class_theme = nullptr;
            return s_class_theme;
        }
    }
    #undef X
}

mvPythonParser
DearPyGui::GetEntityParser(mvAppItemType type)
{
    std::vector<mvPythonDataElement> args;
    mvPythonParserSetup setup;
    setup.returnType = mvPyDataType::UUID;
    setup.category = { "Widgets" };

    switch (type)
    {
    case mvAppItemType::mvButton:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
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

        args.push_back({ mvPyDataType::Bool, "small", mvArgType::KEYWORD_ARG, "False", "Shrinks the size of the button to the text of the label it contains. Useful for embedding in text." });
        args.push_back({ mvPyDataType::Bool, "arrow", mvArgType::KEYWORD_ARG, "False", "Displays an arrow in place of the text string. This requires the direction keyword." });
        args.push_back({ mvPyDataType::Integer, "direction", mvArgType::KEYWORD_ARG, "0", "Sets the cardinal direction for the arrow by using constants mvDir_Left, mvDir_Up, mvDir_Down, mvDir_Right, mvDir_None. Arrow keyword must be set to True." });
   
        setup.about = "Adds a button.";
        break;
    }

    case mvAppItemType::mvCheckbox:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
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

        args.push_back({ mvPyDataType::Bool, "default_value", mvArgType::KEYWORD_ARG, "False", "Sets the default value of the checkmark" });

        setup.about = "Adds a checkbox.";
        break;
    }

    case mvAppItemType::mvCombo:
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::StringList, "items", mvArgType::POSITIONAL_ARG, "()", "A tuple of items to be shown in the drop down window. Can consist of any combination of types but will convert all items to strings to be shown." });
        args.push_back({ mvPyDataType::String, "default_value", mvArgType::KEYWORD_ARG, "''", "Sets a selected item from the drop down by specifying the string value." });
        args.push_back({ mvPyDataType::Bool, "popup_align_left", mvArgType::KEYWORD_ARG, "False", "Align the contents on the popup toward the left." });
        args.push_back({ mvPyDataType::Bool, "no_arrow_button", mvArgType::KEYWORD_ARG, "False", "Display the preview box without the square arrow button indicating dropdown activity." });
        args.push_back({ mvPyDataType::Bool, "no_preview", mvArgType::KEYWORD_ARG, "False", "Display only the square arrow button and not the selected value." });
        args.push_back({ mvPyDataType::Long, "height_mode", mvArgType::KEYWORD_ARG, "1", "Controlls the number of items shown in the dropdown by the constants mvComboHeight_Small, mvComboHeight_Regular, mvComboHeight_Large, mvComboHeight_Largest" });

        setup.about = "Adds a combo dropdown that allows a user to select a single option from a drop down window. All items will be shown as selectables on the dropdown.";
        break;
    }

    case mvAppItemType::mvInputText:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
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

        args.push_back({ mvPyDataType::String, "default_value", mvArgType::KEYWORD_ARG, "''" });
        args.push_back({ mvPyDataType::String, "hint", mvArgType::KEYWORD_ARG, "''", "Displayed only when value is an empty string. Will reappear if input value is set to empty string. Will not show if default value is anything other than default empty string." });
        args.push_back({ mvPyDataType::Bool, "multiline", mvArgType::KEYWORD_ARG, "False", "Allows for multiline text input." });
        args.push_back({ mvPyDataType::Bool, "no_spaces", mvArgType::KEYWORD_ARG, "False", "Filter out spaces and tabs." });
        args.push_back({ mvPyDataType::Bool, "uppercase", mvArgType::KEYWORD_ARG, "False", "Automatically make all inputs uppercase." });
        args.push_back({ mvPyDataType::Bool, "tab_input", mvArgType::KEYWORD_ARG, "False", "Allows tabs to be input into the string value instead of changing item focus." });
        args.push_back({ mvPyDataType::Bool, "decimal", mvArgType::KEYWORD_ARG, "False", "Only allow characters 0123456789.+-*/" });
        args.push_back({ mvPyDataType::Bool, "hexadecimal", mvArgType::KEYWORD_ARG, "False", "Only allow characters 0123456789ABCDEFabcdef" });
        args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode where no text can be input but text can still be highlighted." });
        args.push_back({ mvPyDataType::Bool, "password", mvArgType::KEYWORD_ARG, "False", "Display all input characters as '*'." });
        args.push_back({ mvPyDataType::Bool, "scientific", mvArgType::KEYWORD_ARG, "False", "Only allow characters 0123456789.+-*/eE (Scientific notation input)" });
        args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press." });

        setup.about = "Adds input for text.";
        break;
    }
    case mvAppItemType::mvRadioButton: 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
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

        args.push_back({ mvPyDataType::StringList, "items", mvArgType::POSITIONAL_ARG, "()", "A tuple of items to be shown as radio options. Can consist of any combination of types. All types will be shown as strings." });
        args.push_back({ mvPyDataType::String, "default_value", mvArgType::KEYWORD_ARG, "''", "Default selected radio option. Set by using the string value of the item." });
        args.push_back({ mvPyDataType::Bool, "horizontal", mvArgType::KEYWORD_ARG, "False", "Displays the radio options horizontally." });

        setup.about = "Adds a set of radio buttons. If items keyword is empty, nothing will be shown.";
        break;
    }

    case mvAppItemType::mvTabBar: 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Bool, "reorderable", mvArgType::KEYWORD_ARG, "False", "Allows for the user to change the order of the tabs." });

        setup.about = "Adds a tab bar.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvTab:
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        setup.about = "Adds a tab to a tab bar.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvImage:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::UUID, "texture_tag", mvArgType::REQUIRED_ARG, "", "The texture_tag should come from a texture that was added to a texture registry." });
        args.push_back({ mvPyDataType::FloatList, "tint_color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "Applies a color tint to the entire texture." });
        args.push_back({ mvPyDataType::FloatList, "border_color", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)", "Displays a border of the specified color around the texture. If the theme style has turned off the border it will not be shown." });
        args.push_back({ mvPyDataType::FloatList, "uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "Normalized texture coordinates min point." });
        args.push_back({ mvPyDataType::FloatList, "uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "Normalized texture coordinates max point." });

        setup.about = "Adds an image from a specified texture. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) for texture coordinates will generally display the entire texture.";
        setup.category = { "Widgets", "Textures" };
        break;
    }
    case mvAppItemType::mvMenuBar:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_SHOW)
        );

        setup.about = "Adds a menu bar to a window.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvViewportMenuBar:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_SHOW)
        );

        setup.about = "Adds a menubar to the viewport.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvMenu:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_ENABLED)
        );

        setup.about = "Adds a menu to an existing menu bar.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvMenuItem:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_ENABLED)
        );
        args.push_back({ mvPyDataType::Callable, "drag_callback", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });
        args.push_back({ mvPyDataType::Bool, "default_value", mvArgType::KEYWORD_ARG, "False", "This value also controls the checkmark when shown." });
        args.push_back({ mvPyDataType::String, "shortcut", mvArgType::KEYWORD_ARG, "''", "Displays text on the menu item. Typically used to show a shortcut key command." });
        args.push_back({ mvPyDataType::Bool, "check", mvArgType::KEYWORD_ARG, "False", "Displays a checkmark on the menu item when it is selected and placed in a menu." });

        setup.about = "Adds a menu item to an existing menu. Menu items act similar to selectables and has a bool value. When placed in a menu the checkmark will reflect its value.";
        break;
    }
    case mvAppItemType::mvChildWindow:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Bool, "border", mvArgType::KEYWORD_ARG, "True", "Shows/Hides the border around the sides." });
        args.push_back({ mvPyDataType::Bool, "autosize_x", mvArgType::KEYWORD_ARG, "False", "Autosize the window to its parents size in x." });
        args.push_back({ mvPyDataType::Bool, "autosize_y", mvArgType::KEYWORD_ARG, "False", "Autosize the window to its parents size in y." });
        args.push_back({ mvPyDataType::Bool, "no_scrollbar", mvArgType::KEYWORD_ARG, "False", " Disable scrollbars (window can still scroll with mouse or programmatically)." });
        args.push_back({ mvPyDataType::Bool, "horizontal_scrollbar", mvArgType::KEYWORD_ARG, "False", "Allow horizontal scrollbar to appear (off by default)." });
        args.push_back({ mvPyDataType::Bool, "menubar", mvArgType::KEYWORD_ARG, "False", "Shows/Hides the menubar at the top." });

        setup.about = "Adds an embedded child window. Will show scrollbars when items do not fit.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvGroup:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Bool, "horizontal", mvArgType::KEYWORD_ARG, "False", "Forces child widgets to be added in a horizontal layout." });
        args.push_back({ mvPyDataType::Float, "horizontal_spacing", mvArgType::KEYWORD_ARG, "-1", "Spacing for the horizontal layout." });
        args.push_back({ mvPyDataType::Float, "xoffset", mvArgType::KEYWORD_ARG, "0.0", "Offset from containing window x item location within group." });

        setup.about = "Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvSliderFloat:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
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

        args.push_back({ mvPyDataType::Float, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::Bool, "vertical", mvArgType::KEYWORD_ARG, "False", "Sets orientation of the slidebar and slider to vertical." });
        args.push_back({ mvPyDataType::Bool, "no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods double-click or ctrl+click or Enter key allowing to input text directly into the item." });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click." });
        args.push_back({ mvPyDataType::Float, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Applies a limit only to sliding entry only." });
        args.push_back({ mvPyDataType::Float, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Applies a limit only to sliding entry only." });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%.3f'", "Determines the format the float will be displayed as use python string formatting." });

        setup.about = "Adds slider for a single float value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes.";
        break;
    }
    case mvAppItemType::mvSliderDouble:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
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

        args.push_back({ mvPyDataType::Double, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::Bool, "vertical", mvArgType::KEYWORD_ARG, "False", "Sets orientation of the slidebar and slider to vertical." });
        args.push_back({ mvPyDataType::Bool, "no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods double-click or ctrl+click or Enter key allowing to input text directly into the item." });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click." });
        args.push_back({ mvPyDataType::Double, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Applies a limit only to sliding entry only." });
        args.push_back({ mvPyDataType::Double, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Applies a limit only to sliding entry only." });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%.3f'", "Determines the format the float will be displayed as use python string formatting." });

        setup.about = "Adds slider for a single double value. Useful when slider float is not accurate enough. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes.";
        break;
    }
    case mvAppItemType::mvSliderInt:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
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

        args.push_back({ mvPyDataType::Integer, "default_value", mvArgType::KEYWORD_ARG, "0" });
        args.push_back({ mvPyDataType::Bool, "vertical", mvArgType::KEYWORD_ARG, "False", "Sets orientation of the slidebar and slider to vertical." });
        args.push_back({ mvPyDataType::Bool, "no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods double-click or ctrl+click or Enter key allowing to input text directly into the item." });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click." });
        args.push_back({ mvPyDataType::Integer, "min_value", mvArgType::KEYWORD_ARG, "0", "Applies a limit only to sliding entry only." });
        args.push_back({ mvPyDataType::Integer, "max_value", mvArgType::KEYWORD_ARG, "100", "Applies a limit only to sliding entry only." });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%d'", "Determines the format the int will be displayed as use python string formatting." });

        setup.about = "Adds slider for a single int value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes.";
        break;
    }
    case mvAppItemType::mvFilterSet:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_SHOW)
        );

        setup.about = "Helper to parse and apply text filters (e.g. aaaaa[, bbbbb][, ccccc])";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvDragFloat:
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::Float, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%0.3f'", "Determines the format the float will be displayed as use python string formatting." });
        args.push_back({ mvPyDataType::Float, "speed", mvArgType::KEYWORD_ARG, "1.0", "Sets the sensitivity the float will be modified while dragging." });
        args.push_back({ mvPyDataType::Float, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Applies a limit only to draging entry only." });
        args.push_back({ mvPyDataType::Float, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Applies a limit only to draging entry only." });
        args.push_back({ mvPyDataType::Bool, "no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods or Enter key allowing to input text directly into the widget." });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click." });

        setup.about = "Adds drag for a single float value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes.";
        break;
    }
    case mvAppItemType::mvDragDouble:
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::Double, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%0.3f'", "Determines the format the float will be displayed as use python string formatting." });
        args.push_back({ mvPyDataType::Float, "speed", mvArgType::KEYWORD_ARG, "1.0", "Sets the sensitivity the float will be modified while dragging." });
        args.push_back({ mvPyDataType::Double, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Applies a limit only to draging entry only." });
        args.push_back({ mvPyDataType::Double, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Applies a limit only to draging entry only." });
        args.push_back({ mvPyDataType::Bool, "no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods or Enter key allowing to input text directly into the widget." });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click." });

        setup.about = "Adds drag for a single double value. Useful when drag float is not accurate enough. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes.";
        break;
    }
    case mvAppItemType::mvDragInt:
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::Integer, "default_value", mvArgType::KEYWORD_ARG, "0" });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%d'", "Determines the format the float will be displayed as use python string formatting." });
        args.push_back({ mvPyDataType::Float, "speed", mvArgType::KEYWORD_ARG, "1.0", "Sets the sensitivity the float will be modified while dragging." });
        args.push_back({ mvPyDataType::Integer, "min_value", mvArgType::KEYWORD_ARG, "0", "Applies a limit only to draging entry only." });
        args.push_back({ mvPyDataType::Integer, "max_value", mvArgType::KEYWORD_ARG, "100", "Applies a limit only to draging entry only." });
        args.push_back({ mvPyDataType::Bool, "no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods or Enter key allowing to input text directly into the widget." });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click." });

        setup.about = "Adds drag for a single int value. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes.";
        break;
    }
    case mvAppItemType::mvInputFloat:
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::Float, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%.3f'", "Determines the format the float will be displayed as use python string formatting." });
        args.push_back({ mvPyDataType::Float, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Value for lower limit of input. By default this limits the step buttons. Use min_clamped to limit manual input." });
        args.push_back({ mvPyDataType::Float, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Value for upper limit of input. By default this limits the step buttons. Use max_clamped to limit manual input." });
        args.push_back({ mvPyDataType::Float, "step", mvArgType::KEYWORD_ARG, "0.1", "Increment to change value by when the step buttons are pressed. Setting this and step_fast to a value of 0 or less will turn off step buttons." });
        args.push_back({ mvPyDataType::Float, "step_fast", mvArgType::KEYWORD_ARG, "1.0", "Increment to change value by when ctrl + step buttons are pressed. Setting this and step to a value of 0 or less will turn off step buttons." });
        args.push_back({ mvPyDataType::Bool, "min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of min_value." });
        args.push_back({ mvPyDataType::Bool, "max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of max_value." });
        args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press." });
        args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode where no text can be input but text can still be highlighted." });

        setup.about = "Adds input for an float. +/- buttons can be activated by setting the value of step.";
        break;
    }
    case mvAppItemType::mvInputDouble:
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::Double, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%.3f'", "Determines the format the float will be displayed as use python string formatting." });
        args.push_back({ mvPyDataType::Double, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Value for lower limit of input. By default this limits the step buttons. Use min_clamped to limit manual input." });
        args.push_back({ mvPyDataType::Double, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Value for upper limit of input. By default this limits the step buttons. Use max_clamped to limit manual input." });
        args.push_back({ mvPyDataType::Double, "step", mvArgType::KEYWORD_ARG, "0.1", "Increment to change value by when the step buttons are pressed. Setting this and step_fast to a value of 0 or less will turn off step buttons." });
        args.push_back({ mvPyDataType::Double, "step_fast", mvArgType::KEYWORD_ARG, "1.0", "Increment to change value by when ctrl + step buttons are pressed. Setting this and step to a value of 0 or less will turn off step buttons." });
        args.push_back({ mvPyDataType::Bool, "min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of min_value." });
        args.push_back({ mvPyDataType::Bool, "max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of max_value." });
        args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press." });
        args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode where no text can be input but text can still be highlighted." });

        setup.about = "Adds input for an double. Useful when input float is not accurate enough. +/- buttons can be activated by setting the value of step.";
        break;
    }
    case mvAppItemType::mvInputInt:
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::Integer, "default_value", mvArgType::KEYWORD_ARG, "0" });
        args.push_back({ mvPyDataType::Integer, "min_value", mvArgType::KEYWORD_ARG, "0", "Value for lower limit of input. By default this limits the step buttons. Use min_clamped to limit manual input." });
        args.push_back({ mvPyDataType::Integer, "max_value", mvArgType::KEYWORD_ARG, "100", "Value for upper limit of input. By default this limits the step buttons. Use max_clamped to limit manual input." });
        args.push_back({ mvPyDataType::Integer, "step", mvArgType::KEYWORD_ARG, "1", "Increment to change value by when the step buttons are pressed. Setting this and step_fast to a value of 0 or less will turn off step buttons." });
        args.push_back({ mvPyDataType::Integer, "step_fast", mvArgType::KEYWORD_ARG, "100", "Increment to change value by when ctrl + step buttons are pressed. Setting this and step to a value of 0 or less will turn off step buttons." });
        args.push_back({ mvPyDataType::Bool, "min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of min_value." });
        args.push_back({ mvPyDataType::Bool, "max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of max_value." });
        args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press." });
        args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode where no text can be input but text can still be highlighted." });

        setup.about = "Adds input for an int. +/- buttons can be activated by setting the value of step.";
        break;
    }
    case mvAppItemType::mvColorEdit:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
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

        args.push_back({ mvPyDataType::IntList, "default_value", mvArgType::POSITIONAL_ARG, "(0, 0, 0, 255)" });
        args.push_back({ mvPyDataType::Bool, "no_alpha", mvArgType::KEYWORD_ARG, "False", "Removes the displayed slider that can change alpha channel." });
        args.push_back({ mvPyDataType::Bool, "no_picker", mvArgType::KEYWORD_ARG, "False", "Disable picker popup when color square is clicked." });
        args.push_back({ mvPyDataType::Bool, "no_options", mvArgType::KEYWORD_ARG, "False", "Disable toggling options menu when right-clicking on inputs/small preview." });
        args.push_back({ mvPyDataType::Bool, "no_small_preview", mvArgType::KEYWORD_ARG, "False", "Disable colored square preview next to the inputs. (e.g. to show only the inputs). This only displays if the side preview is not shown." });
        args.push_back({ mvPyDataType::Bool, "no_inputs", mvArgType::KEYWORD_ARG, "False", "Disable inputs sliders/text widgets. (e.g. to show only the small preview colored square)" });
        args.push_back({ mvPyDataType::Bool, "no_tooltip", mvArgType::KEYWORD_ARG, "False", "Disable tooltip when hovering the preview." });
        args.push_back({ mvPyDataType::Bool, "no_label", mvArgType::KEYWORD_ARG, "False", "Disable display of inline text label." });
        args.push_back({ mvPyDataType::Bool, "no_drag_drop", mvArgType::KEYWORD_ARG, "False", "Disable ability to drag and drop small preview (color square) to apply colors to other items." });
        args.push_back({ mvPyDataType::Bool, "alpha_bar", mvArgType::KEYWORD_ARG, "False", "Show vertical alpha bar/gradient in picker." });
        args.push_back({ mvPyDataType::Long, "alpha_preview", mvArgType::KEYWORD_ARG, "0", "mvColorEdit_AlphaPreviewNone, mvColorEdit_AlphaPreview, or mvColorEdit_AlphaPreviewHalf" });
        args.push_back({ mvPyDataType::Long, "display_mode", mvArgType::KEYWORD_ARG, "1048576", "mvColorEdit_rgb, mvColorEdit_hsv, or mvColorEdit_hex" });
        args.push_back({ mvPyDataType::Long, "display_type", mvArgType::KEYWORD_ARG, "8388608", "mvColorEdit_uint8 or mvColorEdit_float" });
        args.push_back({ mvPyDataType::Long, "input_mode", mvArgType::KEYWORD_ARG, "134217728", "mvColorEdit_input_rgb or mvColorEdit_input_hsv" });

        setup.about = "Adds an RGBA color editor. Left clicking the small color preview will provide a color picker. Click and draging the small color preview will copy the color to be applied on any other color widget.";
        setup.category = { "Widgets", "Colors" };
        break;
    }
    case mvAppItemType::mvClipper:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_SHOW)
        );

        setup.about = "Helper to manually clip large list of items. Increases performance by not searching or drawing widgets outside of the clipped region.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvColorPicker:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
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

        args.push_back({ mvPyDataType::IntList, "default_value", mvArgType::POSITIONAL_ARG, "(0, 0, 0, 255)" });
        args.push_back({ mvPyDataType::Bool, "no_alpha", mvArgType::KEYWORD_ARG, "False", "Removes the displayed slider that can change alpha channel." });
        args.push_back({ mvPyDataType::Bool, "no_side_preview", mvArgType::KEYWORD_ARG, "False", "Disable bigger color preview on right side of the picker, use small colored square preview instead , unless small preview is also hidden." });
        args.push_back({ mvPyDataType::Bool, "no_small_preview", mvArgType::KEYWORD_ARG, "False", "Disable colored square preview next to the inputs. (e.g. to show only the inputs). This only displays if the side preview is not shown." });
        args.push_back({ mvPyDataType::Bool, "no_inputs", mvArgType::KEYWORD_ARG, "False", "Disable inputs sliders/text widgets. (e.g. to show only the small preview colored square)" });
        args.push_back({ mvPyDataType::Bool, "no_tooltip", mvArgType::KEYWORD_ARG, "False", "Disable tooltip when hovering the preview." });
        args.push_back({ mvPyDataType::Bool, "no_label", mvArgType::KEYWORD_ARG, "False", "Disable display of inline text label." });
        args.push_back({ mvPyDataType::Bool, "alpha_bar", mvArgType::KEYWORD_ARG, "False", "Show vertical alpha bar/gradient in picker." });
        args.push_back({ mvPyDataType::Bool, "display_rgb", mvArgType::KEYWORD_ARG, "False", "Override _display_ type among RGB/HSV/Hex." });
        args.push_back({ mvPyDataType::Bool, "display_hsv", mvArgType::KEYWORD_ARG, "False", "Override _display_ type among RGB/HSV/Hex." });
        args.push_back({ mvPyDataType::Bool, "display_hex", mvArgType::KEYWORD_ARG, "False", "Override _display_ type among RGB/HSV/Hex." });
        args.push_back({ mvPyDataType::Long, "picker_mode", mvArgType::KEYWORD_ARG, "33554432", "mvColorPicker_bar or mvColorPicker_wheel" });
        args.push_back({ mvPyDataType::Long, "alpha_preview", mvArgType::KEYWORD_ARG, "0", "mvColorEdit_AlphaPreviewNone, mvColorEdit_AlphaPreview, or mvColorEdit_AlphaPreviewHalf" });
        args.push_back({ mvPyDataType::Long, "display_type", mvArgType::KEYWORD_ARG, "8388608", "mvColorEdit_uint8 or mvColorEdit_float" });
        args.push_back({ mvPyDataType::Long, "input_mode", mvArgType::KEYWORD_ARG, "134217728", "mvColorEdit_input_rgb or mvColorEdit_input_hsv" });

        setup.about = "Adds an RGB color picker. Right click the color picker for options. Click and drag the color preview to copy the color and drop on any other color widget to apply. Right Click allows the style of the color picker to be changed.";
        setup.category = { "Widgets", "Colors" };
        break;
    }
    case mvAppItemType::mvTooltip:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::UUID, "parent" });
        args.push_back({ mvPyDataType::Float, "delay", mvArgType::KEYWORD_ARG, "0.5", "Activation delay: time, in seconds, during which the mouse should stay still in order to display the tooltip.  May be zero for instant activation." });
        args.push_back({ mvPyDataType::Bool, "hide_on_activity", mvArgType::KEYWORD_ARG, "False", "Hide the tooltip if the user has moved the mouse.  If False, the tooltip will follow mouse pointer." });

        setup.about = "Adds a tooltip window.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvCollapsingHeader:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Bool, "closable", mvArgType::KEYWORD_ARG, "False", "Adds the ability to hide this widget by pressing the (x) in the top right of widget." });
        args.push_back({ mvPyDataType::Bool, "default_open", mvArgType::KEYWORD_ARG, "False", "Sets the collapseable header open by default." });
        args.push_back({ mvPyDataType::Bool, "open_on_double_click", mvArgType::KEYWORD_ARG, "False", "Need double-click to open node." });
        args.push_back({ mvPyDataType::Bool, "open_on_arrow", mvArgType::KEYWORD_ARG, "False", "Only open when clicking on the arrow part." });
        args.push_back({ mvPyDataType::Bool, "leaf", mvArgType::KEYWORD_ARG, "False", "No collapsing, no arrow (use as a convenience for leaf nodes)." });
        args.push_back({ mvPyDataType::Bool, "bullet", mvArgType::KEYWORD_ARG, "False", "Display a bullet instead of arrow." });

        setup.about = "Adds a collapsing header to add items to. Must be closed with the end command.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvSeparator:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_POS)
        );

        setup.about = "Adds a horizontal line separator.";
        break;
    }
    case mvAppItemType::mvListbox:                     
    {
        AddCommonArgs(args, (CommonParserArgs)(
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
        args.push_back({ mvPyDataType::String, "default_value", mvArgType::KEYWORD_ARG, "''", "String value of the item that will be selected by default." });
        args.push_back({ mvPyDataType::Integer, "num_items", mvArgType::KEYWORD_ARG, "3", "Expands the height of the listbox to show specified number of items." });


        setup.about = "Adds a listbox. If height is not large enough to show all items a scroll bar will appear.";
        break;
    }
    case mvAppItemType::mvText:                        
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::String, "default_value", mvArgType::POSITIONAL_ARG, "''" });
        args.push_back({ mvPyDataType::Integer, "wrap", mvArgType::KEYWORD_ARG, "-1", "Number of pixels from the start of the item until wrapping starts." });
        args.push_back({ mvPyDataType::Bool, "bullet", mvArgType::KEYWORD_ARG, "False", "Places a bullet to the left of the text." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(-255, 0, 0, 255)", "Color of the text (rgba)." });
        args.push_back({ mvPyDataType::Bool, "show_label", mvArgType::KEYWORD_ARG, "False", "Displays the label to the right of the text." });

        setup.about = "Adds text. Text can have an optional label that will display to the right of the text.";
        break;
    }
    case mvAppItemType::mvPlot:                        
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        // plot flags
        args.push_back({ mvPyDataType::Bool, "no_title", mvArgType::KEYWORD_ARG, "False", "the plot title will not be displayed"});
        args.push_back({ mvPyDataType::Bool, "no_menus", mvArgType::KEYWORD_ARG, "False", "the user will not be able to open context menus with right-click"});
        args.push_back({ mvPyDataType::Bool, "no_box_select", mvArgType::KEYWORD_ARG, "False", "the user will not be able to box-select with right-click drag"});
        args.push_back({ mvPyDataType::Bool, "no_mouse_pos", mvArgType::KEYWORD_ARG, "False", "the mouse position, in plot coordinates, will not be displayed inside of the plot"});
        args.push_back({ mvPyDataType::Bool, "no_highlight", mvArgType::KEYWORD_ARG, "False", "plot items will not be highlighted when their legend entry is hovered"});
        args.push_back({ mvPyDataType::Bool, "no_child", mvArgType::KEYWORD_ARG, "False", "a child window region will not be used to capture mouse scroll (can boost performance for single ImGui window applications)"});
        args.push_back({ mvPyDataType::Bool, "query", mvArgType::KEYWORD_ARG, "False", "the user will be able to draw query rects with middle - mouse or CTRL + right - click drag"});
        args.push_back({ mvPyDataType::Bool, "crosshairs", mvArgType::KEYWORD_ARG, "False", "the default mouse cursor will be replaced with a crosshair when hovered"});
        args.push_back({ mvPyDataType::Bool, "anti_aliased", mvArgType::KEYWORD_ARG, "False", "plot lines will be software anti-aliased (not recommended for high density plots, prefer MSAA)"});
        args.push_back({ mvPyDataType::Bool, "equal_aspects", mvArgType::KEYWORD_ARG, "False", "primary x and y axes will be constrained to have the same units/pixel (does not apply to auxiliary y-axes)"});
        args.push_back({ mvPyDataType::Bool, "use_local_time", mvArgType::KEYWORD_ARG, "False", "axis labels will be formatted for your timezone when" });
        args.push_back({ mvPyDataType::Bool, "use_ISO8601", mvArgType::KEYWORD_ARG, "False", "dates will be formatted according to ISO 8601 where applicable (e.g. YYYY-MM-DD, YYYY-MM, --MM-DD, etc.)" });
        args.push_back({ mvPyDataType::Bool, "use_24hour_clock", mvArgType::KEYWORD_ARG, "False", "times will be formatted using a 24 hour clock" });

        // key modifiers
        args.push_back({ mvPyDataType::Integer, "pan_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Left", "enables panning when held" });
        args.push_back({ mvPyDataType::Integer, "pan_mod", mvArgType::KEYWORD_ARG, "-1", "optional modifier that must be held for panning" });
        args.push_back({ mvPyDataType::Integer, "fit_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Left", "fits visible data when double clicked" });
        args.push_back({ mvPyDataType::Integer, "context_menu_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Right", "opens plot context menu (if enabled) when clicked" });
        args.push_back({ mvPyDataType::Integer, "box_select_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Right", "begins box selection when pressed and confirms selection when released" });
        args.push_back({ mvPyDataType::Integer, "box_select_mod", mvArgType::KEYWORD_ARG, "-1", "begins box selection when pressed and confirms selection when released" });
        args.push_back({ mvPyDataType::Integer, "box_select_cancel_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Left", "cancels active box selection when pressed" });
        args.push_back({ mvPyDataType::Integer, "query_button", mvArgType::KEYWORD_ARG, "internal_dpg.mvMouseButton_Middle", "begins query selection when pressed and end query selection when released" });
        args.push_back({ mvPyDataType::Integer, "query_mod", mvArgType::KEYWORD_ARG, "-1", "optional modifier that must be held for query selection" });
        args.push_back({ mvPyDataType::Integer, "query_toggle_mod", mvArgType::KEYWORD_ARG, "internal_dpg.mvKey_Control", "when held, active box selections turn into queries" });
        args.push_back({ mvPyDataType::Integer, "horizontal_mod", mvArgType::KEYWORD_ARG, "internal_dpg.mvKey_Alt", "expands active box selection/query horizontally to plot edge when held" });
        args.push_back({ mvPyDataType::Integer, "vertical_mod", mvArgType::KEYWORD_ARG, "internal_dpg.mvKey_Shift", "expands active box selection/query vertically to plot edge when held" });

        setup.about = "Adds a plot which is used to hold series, and can be drawn to with draw commands.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvSimplePlot:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::FloatList, "default_value", mvArgType::KEYWORD_ARG, "()" });
        args.push_back({ mvPyDataType::String, "overlay", mvArgType::KEYWORD_ARG, "''", "overlays text (similar to a plot title)" });
        args.push_back({ mvPyDataType::Bool, "histogram", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "autosize", mvArgType::KEYWORD_ARG, "True" });
        args.push_back({ mvPyDataType::Float, "min_scale", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::Float, "max_scale", mvArgType::KEYWORD_ARG, "0.0" });

        setup.about = "Adds a simple plot for visualization of a 1 dimensional set of values.";
        setup.category = { "Plotting", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawlist:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Integer, "width" });
        args.push_back({ mvPyDataType::Integer, "height" });

        setup.about = "Adds a drawing canvas.";
        setup.category = { "Drawlist", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvWindowAppItem:               
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::IntList, "min_size", mvArgType::KEYWORD_ARG, "[100, 100]", "Minimum window size." });
        args.push_back({ mvPyDataType::IntList, "max_size", mvArgType::KEYWORD_ARG, "[30000, 30000]", "Maximum window size." });
        args.push_back({ mvPyDataType::Bool, "menubar", mvArgType::KEYWORD_ARG, "False", "Shows or hides the menubar." });
        args.push_back({ mvPyDataType::Bool, "collapsed", mvArgType::KEYWORD_ARG, "False", "Collapse the window." });
        args.push_back({ mvPyDataType::Bool, "autosize", mvArgType::KEYWORD_ARG, "False", "Autosized the window to fit it's items." });
        args.push_back({ mvPyDataType::Bool, "no_resize", mvArgType::KEYWORD_ARG, "False", "Allows for the window size to be changed or fixed." });
        args.push_back({ mvPyDataType::Bool, "no_title_bar", mvArgType::KEYWORD_ARG, "False", "Title name for the title bar of the window." });
        args.push_back({ mvPyDataType::Bool, "no_move", mvArgType::KEYWORD_ARG, "False", "Allows for the window's position to be changed or fixed." });
        args.push_back({ mvPyDataType::Bool, "no_scrollbar", mvArgType::KEYWORD_ARG, "False", " Disable scrollbars. (window can still scroll with mouse or programmatically)" });
        args.push_back({ mvPyDataType::Bool, "no_collapse", mvArgType::KEYWORD_ARG, "False", "Disable user collapsing window by double-clicking on it." });
        args.push_back({ mvPyDataType::Bool, "horizontal_scrollbar", mvArgType::KEYWORD_ARG, "False", "Allow horizontal scrollbar to appear. (off by default)" });
        args.push_back({ mvPyDataType::Bool, "no_focus_on_appearing", mvArgType::KEYWORD_ARG, "False", "Disable taking focus when transitioning from hidden to visible state." });
        args.push_back({ mvPyDataType::Bool, "no_bring_to_front_on_focus", mvArgType::KEYWORD_ARG, "False", "Disable bringing window to front when taking focus. (e.g. clicking on it or programmatically giving it focus)" });
        args.push_back({ mvPyDataType::Bool, "no_close", mvArgType::KEYWORD_ARG, "False", "Disable user closing the window by removing the close button." });
        args.push_back({ mvPyDataType::Bool, "no_background", mvArgType::KEYWORD_ARG, "False", "Sets Background and border alpha to transparent." });
        args.push_back({ mvPyDataType::Bool, "modal", mvArgType::KEYWORD_ARG, "False", "Fills area behind window according to the theme and disables user ability to interact with anything except the window." });
        args.push_back({ mvPyDataType::Bool, "popup", mvArgType::KEYWORD_ARG, "False", "Fills area behind window according to the theme, removes title bar, collapse and close. Window can be closed by selecting area in the background behind the window." });
        args.push_back({ mvPyDataType::Bool, "no_saved_settings", mvArgType::KEYWORD_ARG, "False", "Never load/save settings in .ini file." });
        args.push_back({ mvPyDataType::Bool, "no_open_over_existing_popup", mvArgType::KEYWORD_ARG, "True", "Don't open if there's already a popup" });

        args.push_back({ mvPyDataType::Callable, "on_close", mvArgType::KEYWORD_ARG, "None", "Callback ran when window is closed." });

        setup.about = "Creates a new window for following items to be added to.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvSelectable:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
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

        args.push_back({ mvPyDataType::Bool, "default_value", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "span_columns", mvArgType::KEYWORD_ARG, "False", "Forces the selectable to span the width of all columns if placed in a table." });
        args.push_back({ mvPyDataType::Bool, "disable_popup_close", mvArgType::KEYWORD_ARG, "False", "Disable closing a modal or popup window." });

        setup.about = "Adds a selectable. Similar to a button but can indicate its selected state.";
        break;
    }
    case mvAppItemType::mvTreeNode:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Bool, "default_open", mvArgType::KEYWORD_ARG, "False", "Sets the tree node open by default." });
        args.push_back({ mvPyDataType::Bool, "open_on_double_click", mvArgType::KEYWORD_ARG, "False", "Need double-click to open node." });
        args.push_back({ mvPyDataType::Bool, "open_on_arrow", mvArgType::KEYWORD_ARG, "False", "Only open when clicking on the arrow part." });
        args.push_back({ mvPyDataType::Bool, "leaf", mvArgType::KEYWORD_ARG, "False", "No collapsing, no arrow (use as a convenience for leaf nodes)." });
        args.push_back({ mvPyDataType::Bool, "bullet", mvArgType::KEYWORD_ARG, "False", "Display a bullet instead of arrow." });
        args.push_back({ mvPyDataType::Bool, "selectable", mvArgType::KEYWORD_ARG, "False", "Makes the tree selectable." });

        setup.about = "Adds a tree node to add items to.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvProgressBar:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::String, "overlay", mvArgType::KEYWORD_ARG, "''", "Overlayed text onto the bar that typically used to display the value of the progress." });
        args.push_back({ mvPyDataType::Float, "default_value", mvArgType::KEYWORD_ARG, "0.0", "Normalized value to fill the bar from 0.0 to 1.0." });

        setup.about = "Adds a progress bar.";
        break;
    }
    case mvAppItemType::mvSpacer:                      
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_POS)
        );

        setup.about = "Adds a spacer item that can be used to help with layouts or can be used as a placeholder item.";
        break;
    }
    case mvAppItemType::mvImageButton:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
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

        args.push_back({ mvPyDataType::UUID, "texture_tag", mvArgType::REQUIRED_ARG, "", "The texture_tag should come from a texture that was added to a texture registry." });
        args.push_back({ mvPyDataType::Integer, "frame_padding", mvArgType::KEYWORD_ARG, "-1", "Empty space around the outside of the texture. Button will show around the texture." });
        args.push_back({ mvPyDataType::FloatList, "tint_color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "Applies a color tint to the entire texture." });
        args.push_back({ mvPyDataType::FloatList, "background_color", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)", "Displays a border of the specified color around the texture." });
        args.push_back({ mvPyDataType::FloatList, "uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "Normalized texture coordinates min point." });
        args.push_back({ mvPyDataType::FloatList, "uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "Normalized texture coordinates max point." });

        setup.about = "Adds an button with a texture. uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using range (0.0,0.0)->(1.0,1.0) texture coordinates will generally display the entire texture";
        setup.category = { "Widgets", "Textures" };
        break;
    }
    case mvAppItemType::mvTimePicker:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Dict, "default_value", mvArgType::KEYWORD_ARG, "{'hour': 14, 'min': 32, 'sec': 23}" });
        args.push_back({ mvPyDataType::Bool, "hour24", mvArgType::KEYWORD_ARG, "False", "Show 24 hour clock instead of 12 hour." });

        setup.about = "Adds a time picker.";
        break;
    }
    case mvAppItemType::mvDatePicker:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Dict, "default_value", mvArgType::KEYWORD_ARG, "{'month_day': 14, 'year':20, 'month':5}" });
        args.push_back({ mvPyDataType::Integer, "level", mvArgType::KEYWORD_ARG, "0", "Use avaliable constants. mvDatePickerLevel_Day, mvDatePickerLevel_Month, mvDatePickerLevel_Year" });

        setup.about = "Adds a data picker.";
        break;
    }
    case mvAppItemType::mvColorButton:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
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

        args.push_back({ mvPyDataType::IntList, "default_value", mvArgType::POSITIONAL_ARG, "(0, 0, 0, 255)" });
        args.push_back({ mvPyDataType::Bool, "no_alpha", mvArgType::KEYWORD_ARG, "False", "Removes the displayed slider that can change alpha channel." });
        args.push_back({ mvPyDataType::Bool, "no_border", mvArgType::KEYWORD_ARG, "False", "Disable border around the image." });
        args.push_back({ mvPyDataType::Bool, "no_drag_drop", mvArgType::KEYWORD_ARG, "False", "Disable ability to drag and drop small preview (color square) to apply colors to other items." });

        setup.about = "Adds a color button.";
        setup.category = { "Widgets", "Colors" };
        break;
    }
    case mvAppItemType::mvFileDialog:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::String, "default_path", mvArgType::KEYWORD_ARG, "''", "Path that the file dialog will default to when opened." });
        args.push_back({ mvPyDataType::String, "default_filename", mvArgType::KEYWORD_ARG, "'.'", "Default name that will show in the file name input." });
        args.push_back({ mvPyDataType::Integer, "file_count", mvArgType::KEYWORD_ARG, "0", "Number of visible files in the dialog." });
        args.push_back({ mvPyDataType::Bool, "modal", mvArgType::KEYWORD_ARG, "False", "Forces user interaction with the file selector." });
        args.push_back({ mvPyDataType::Bool, "directory_selector", mvArgType::KEYWORD_ARG, "False", "Shows only directory/paths as options. Allows selection of directory/paths only." });
        args.push_back({ mvPyDataType::IntList, "min_size", mvArgType::KEYWORD_ARG, "[100, 100]", "Minimum window size." });
        args.push_back({ mvPyDataType::IntList, "max_size", mvArgType::KEYWORD_ARG, "[30000, 30000]", "Maximum window size." });
		args.push_back({ mvPyDataType::Callable, "cancel_callback", mvArgType::KEYWORD_ARG, "None", "Callback called when cancel button is clicked." });

        setup.about = "Displays a file or directory selector depending on keywords. Displays a file dialog by default. Callback will be ran when the file or directory picker is closed. The app_data arguemnt will be populated with information related to the file and directory as a dictionary.";
        setup.category = { "Containers", "Widgets", "File Dialog" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvTabButton:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Bool, "no_reorder", mvArgType::KEYWORD_ARG, "False", "Disable reordering this tab or having another tab cross over this tab. Fixes the position of this tab in relation to the order of neighboring tabs at start. " });
        args.push_back({ mvPyDataType::Bool, "leading", mvArgType::KEYWORD_ARG, "False", "Enforce the tab position to the left of the tab bar (after the tab list popup button)." });
        args.push_back({ mvPyDataType::Bool, "trailing", mvArgType::KEYWORD_ARG, "False", "Enforce the tab position to the right of the tab bar (before the scrolling buttons)." });
        args.push_back({ mvPyDataType::Bool, "no_tooltip", mvArgType::KEYWORD_ARG, "False", "Disable tooltip for the given tab." });

        setup.about = "Adds a tab button to a tab bar.";
        break;
    }
    case mvAppItemType::mvDrawNode:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        setup.about = "New in 1.1. Creates a drawing node to associate a transformation matrix. Child node matricies will concatenate.";
        setup.category = { "Drawlist", "Widgets", "Matrix Operations" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvNodeEditor:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Callable, "delink_callback", mvArgType::KEYWORD_ARG, "None", "Callback ran when a link is detached." });
        args.push_back({ mvPyDataType::Bool, "menubar", mvArgType::KEYWORD_ARG, "False", "Shows or hides the menubar." });
        args.push_back({ mvPyDataType::Bool, "minimap", mvArgType::KEYWORD_ARG, "False", "Shows or hides the Minimap. New in 1.6." });
        args.push_back({ mvPyDataType::Integer, "minimap_location", mvArgType::KEYWORD_ARG, "2", "mvNodeMiniMap_Location_* constants. New in 1.6." });

        setup.about = "Adds a node editor.";
        setup.category = { "Node Editor", "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvNode:                        
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_POS |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Bool, "draggable", mvArgType::KEYWORD_ARG, "True", "Allow node to be draggable." });

        setup.about = "Adds a node to a node editor.";
        setup.category = { "Node Editor", "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvNodeAttribute:               
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Long, "attribute_type", mvArgType::KEYWORD_ARG, "0", "mvNode_Attr_Input, mvNode_Attr_Output, or mvNode_Attr_Static." });
        args.push_back({ mvPyDataType::Integer, "shape", mvArgType::KEYWORD_ARG, "1", "Pin shape." });
        args.push_back({ mvPyDataType::String, "category", mvArgType::KEYWORD_ARG, "'general'", "Category" });

        setup.about = "Adds a node attribute to a node.";
        setup.category = { "Node Editor", "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvTable:                       
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Bool, "header_row", mvArgType::KEYWORD_ARG, "True", "show headers at the top of the columns" });
        args.push_back({ mvPyDataType::Bool, "clipper", mvArgType::KEYWORD_ARG, "False", "Use clipper (rows must be same height)." });
        args.push_back({ mvPyDataType::Integer, "inner_width", mvArgType::KEYWORD_ARG, "0" });
        args.push_back({ mvPyDataType::Integer, "policy", mvArgType::KEYWORD_ARG, "0" });
        args.push_back({ mvPyDataType::Integer, "freeze_rows", mvArgType::KEYWORD_ARG, "0" });
        args.push_back({ mvPyDataType::Integer, "freeze_columns", mvArgType::KEYWORD_ARG, "0" });
        args.push_back({ mvPyDataType::Bool, "sort_multi", mvArgType::KEYWORD_ARG, "False", "Hold shift when clicking headers to sort on multiple column." });
        args.push_back({ mvPyDataType::Bool, "sort_tristate", mvArgType::KEYWORD_ARG, "False", "Allow no sorting, disable default sorting." });
        args.push_back({ mvPyDataType::Bool, "resizable", mvArgType::KEYWORD_ARG, "False", "Enable resizing columns" });
        args.push_back({ mvPyDataType::Bool, "reorderable", mvArgType::KEYWORD_ARG, "False", "Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)" });
        args.push_back({ mvPyDataType::Bool, "hideable", mvArgType::KEYWORD_ARG, "False", "Enable hiding/disabling columns in context menu." });
        args.push_back({ mvPyDataType::Bool, "sortable", mvArgType::KEYWORD_ARG, "False", "Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see ImGuiTableFlags_SortMulti and ImGuiTableFlags_SortTristate." });
        args.push_back({ mvPyDataType::Bool, "context_menu_in_body", mvArgType::KEYWORD_ARG, "False", "Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow()." });
        args.push_back({ mvPyDataType::Bool, "row_background", mvArgType::KEYWORD_ARG, "False", "Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)" });
        args.push_back({ mvPyDataType::Bool, "borders_innerH", mvArgType::KEYWORD_ARG, "False", "Draw horizontal borders between rows." });
        args.push_back({ mvPyDataType::Bool, "borders_outerH", mvArgType::KEYWORD_ARG, "False", "Draw horizontal borders at the top and bottom." });
        args.push_back({ mvPyDataType::Bool, "borders_innerV", mvArgType::KEYWORD_ARG, "False", "Draw vertical borders between columns." });
        args.push_back({ mvPyDataType::Bool, "borders_outerV", mvArgType::KEYWORD_ARG, "False", "Draw vertical borders on the left and right sides." });
        args.push_back({ mvPyDataType::Bool, "no_host_extendX", mvArgType::KEYWORD_ARG, "False", "Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used." });
        args.push_back({ mvPyDataType::Bool, "no_host_extendY", mvArgType::KEYWORD_ARG, "False", "Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible." });
        args.push_back({ mvPyDataType::Bool, "no_keep_columns_visible", mvArgType::KEYWORD_ARG, "False", "Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable." });
        args.push_back({ mvPyDataType::Bool, "precise_widths", mvArgType::KEYWORD_ARG, "False", "Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth." });
        args.push_back({ mvPyDataType::Bool, "no_clip", mvArgType::KEYWORD_ARG, "False", "Disable clipping rectangle for every individual columns." });
        args.push_back({ mvPyDataType::Bool, "pad_outerX", mvArgType::KEYWORD_ARG, "False", "Default if BordersOuterV is on. Enable outer-most padding. Generally desirable if you have headers." });
        args.push_back({ mvPyDataType::Bool, "no_pad_outerX", mvArgType::KEYWORD_ARG, "False", "Default if BordersOuterV is off. Disable outer-most padding." });
        args.push_back({ mvPyDataType::Bool, "no_pad_innerX", mvArgType::KEYWORD_ARG, "False", "Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off)." });
        args.push_back({ mvPyDataType::Bool, "scrollX", mvArgType::KEYWORD_ARG, "False", "Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this create a child window, ScrollY is currently generally recommended when using ScrollX." });
        args.push_back({ mvPyDataType::Bool, "scrollY", mvArgType::KEYWORD_ARG, "False", "Enable vertical scrolling." });
        args.push_back({ mvPyDataType::Bool, "no_saved_settings", mvArgType::KEYWORD_ARG, "False", "Never load/save settings in .ini file." });

        setup.about = "Adds a table.";
        setup.category = { "Tables", "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvTableColumn:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_ENABLED |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Float, "init_width_or_weight", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::Bool, "default_hide", mvArgType::KEYWORD_ARG, "False", "Default as a hidden/disabled column." });
        args.push_back({ mvPyDataType::Bool, "default_sort", mvArgType::KEYWORD_ARG, "False", "Default as a sorting column." });
        args.push_back({ mvPyDataType::Bool, "width_stretch", mvArgType::KEYWORD_ARG, "False", "Column will stretch. Preferable with horizontal scrolling disabled (default if table sizing policy is _SizingStretchSame or _SizingStretchProp)." });
        args.push_back({ mvPyDataType::Bool, "width_fixed", mvArgType::KEYWORD_ARG, "False", "Column will not stretch. Preferable with horizontal scrolling enabled (default if table sizing policy is _SizingFixedFit and table is resizable)." });
        args.push_back({ mvPyDataType::Bool, "no_resize", mvArgType::KEYWORD_ARG, "False", "Disable manual resizing." });
        args.push_back({ mvPyDataType::Bool, "no_reorder", mvArgType::KEYWORD_ARG, "False", "Disable manual reordering this column, this will also prevent other columns from crossing over this column." });
        args.push_back({ mvPyDataType::Bool, "no_hide", mvArgType::KEYWORD_ARG, "False", "Disable ability to hide/disable this column." });
        args.push_back({ mvPyDataType::Bool, "no_clip", mvArgType::KEYWORD_ARG, "False", "Disable clipping for this column (all NoClip columns will render in a same draw command)." });
        args.push_back({ mvPyDataType::Bool, "no_sort", mvArgType::KEYWORD_ARG, "False", "Disable ability to sort on this field (even if ImGuiTableFlags_Sortable is set on the table)." });
        args.push_back({ mvPyDataType::Bool, "no_sort_ascending", mvArgType::KEYWORD_ARG, "False", "Disable ability to sort in the ascending direction." });
        args.push_back({ mvPyDataType::Bool, "no_sort_descending", mvArgType::KEYWORD_ARG, "False", "Disable ability to sort in the descending direction." });
        args.push_back({ mvPyDataType::Bool, "no_header_width", mvArgType::KEYWORD_ARG, "False", "Disable header text width contribution to automatic column width." });
        args.push_back({ mvPyDataType::Bool, "prefer_sort_ascending", mvArgType::KEYWORD_ARG, "True", "Make the initial sort direction Ascending when first sorting on this column (default)." });
        args.push_back({ mvPyDataType::Bool, "prefer_sort_descending", mvArgType::KEYWORD_ARG, "False", "Make the initial sort direction Descending when first sorting on this column." });
        args.push_back({ mvPyDataType::Bool, "indent_enable", mvArgType::KEYWORD_ARG, "False", "Use current Indent value when entering cell (default for column 0)." });
        args.push_back({ mvPyDataType::Bool, "indent_disable", mvArgType::KEYWORD_ARG, "False", "Ignore current Indent value when entering cell (default for columns > 0). Indentation changes _within_ the cell will still be honored." });

        setup.about = "Adds a table column.";
        setup.category = { "Tables", "Widgets" };
        break;
    }
    case mvAppItemType::mvTableRow:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_SHOW)
        );
        setup.about = "Adds a table row.";
        setup.category = { "Tables", "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvDrawLine:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::FloatList, "p1", mvArgType::REQUIRED_ARG, "...", "Start of line." });
        args.push_back({ mvPyDataType::FloatList, "p2", mvArgType::REQUIRED_ARG, "...", "End of line." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });

        setup.about = "Adds a line.";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawArrow:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::FloatList, "p1", mvArgType::REQUIRED_ARG, "...", "Arrow tip." });
        args.push_back({ mvPyDataType::FloatList, "p2", mvArgType::REQUIRED_ARG, "...", "Arrow tail." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
        args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4" });

        setup.about = "Adds an arrow.";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawTriangle:                
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::FloatList, "p1" });
        args.push_back({ mvPyDataType::FloatList, "p2" });
        args.push_back({ mvPyDataType::FloatList, "p3" });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
        args.push_back({ mvPyDataType::IntList, "fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });

        setup.about = "Adds a triangle.";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawImageQuad:               
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::UUID, "texture_tag" });
        args.push_back({ mvPyDataType::FloatList, "p1" });
        args.push_back({ mvPyDataType::FloatList, "p2" });
        args.push_back({ mvPyDataType::FloatList, "p3" });
        args.push_back({ mvPyDataType::FloatList, "p4" });
        args.push_back({ mvPyDataType::FloatList, "uv1", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "Normalized coordinates on texture that will be drawn." });
        args.push_back({ mvPyDataType::FloatList, "uv2", mvArgType::KEYWORD_ARG, "(1.0, 0.0)", "Normalized coordinates on texture that will be drawn." });
        args.push_back({ mvPyDataType::FloatList, "uv3", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "Normalized coordinates on texture that will be drawn." });
        args.push_back({ mvPyDataType::FloatList, "uv4", mvArgType::KEYWORD_ARG, "(0.0, 1.0)", "Normalized coordinates on texture that will be drawn." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });

        setup.about = "Adds an image (for a drawing).";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawCircle:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::FloatList, "center" });
        args.push_back({ mvPyDataType::Float, "radius" });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
        args.push_back({ mvPyDataType::IntList, "fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
        args.push_back({ mvPyDataType::Integer, "segments", mvArgType::KEYWORD_ARG, "0", "Number of segments to approximate circle." });

        setup.about = "Adds a circle";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawEllipse:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::FloatList, "pmin", mvArgType::REQUIRED_ARG, "...", "Min point of bounding rectangle." });
        args.push_back({ mvPyDataType::FloatList, "pmax", mvArgType::REQUIRED_ARG, "...", "Max point of bounding rectangle." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
        args.push_back({ mvPyDataType::IntList, "fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
        args.push_back({ mvPyDataType::Integer, "segments", mvArgType::KEYWORD_ARG, "32", "Number of segments to approximate bezier curve." });

        setup.about = "Adds an ellipse.";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawBezierCubic:             
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::FloatList, "p1", mvArgType::REQUIRED_ARG, "...", "First point in curve." });
        args.push_back({ mvPyDataType::FloatList, "p2", mvArgType::REQUIRED_ARG, "...", "Second point in curve." });
        args.push_back({ mvPyDataType::FloatList, "p3", mvArgType::REQUIRED_ARG, "...", "Third point in curve." });
        args.push_back({ mvPyDataType::FloatList, "p4", mvArgType::REQUIRED_ARG, "...", "Fourth point in curve." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
        args.push_back({ mvPyDataType::Integer, "segments", mvArgType::KEYWORD_ARG, "0", "Number of segments to approximate bezier curve." });

        setup.about = "Adds a cubic bezier curve.";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawBezierQuadratic:         
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::FloatList, "p1", mvArgType::REQUIRED_ARG, "...", "First point in curve." });
        args.push_back({ mvPyDataType::FloatList, "p2", mvArgType::REQUIRED_ARG, "...", "Second point in curve." });
        args.push_back({ mvPyDataType::FloatList, "p3", mvArgType::REQUIRED_ARG, "...", "Third point in curve." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
        args.push_back({ mvPyDataType::Integer, "segments", mvArgType::KEYWORD_ARG, "0", "Number of segments to approximate bezier curve." });

        setup.about = "Adds a quadratic bezier curve.";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawQuad:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::FloatList, "p1" });
        args.push_back({ mvPyDataType::FloatList, "p2" });
        args.push_back({ mvPyDataType::FloatList, "p3" });
        args.push_back({ mvPyDataType::FloatList, "p4" });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
        args.push_back({ mvPyDataType::IntList, "fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });

        setup.about = "Adds a quad.";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawRect:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::FloatList, "pmin", mvArgType::REQUIRED_ARG, "...", "Min point of bounding rectangle." });
        args.push_back({ mvPyDataType::FloatList, "pmax", mvArgType::REQUIRED_ARG, "...", "Max point of bounding rectangle." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
        args.push_back({ mvPyDataType::IntList, "color_upper_left", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "'multicolor' must be set to 'True'" });
        args.push_back({ mvPyDataType::IntList, "color_upper_right", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "'multicolor' must be set to 'True'" });
        args.push_back({ mvPyDataType::IntList, "color_bottom_right", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "'multicolor' must be set to 'True'" });
        args.push_back({ mvPyDataType::IntList, "color_bottom_left", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "'multicolor' must be set to 'True'" });
        args.push_back({ mvPyDataType::IntList, "fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
        args.push_back({ mvPyDataType::Bool, "multicolor", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Float, "rounding", mvArgType::KEYWORD_ARG, "0.0", "Number of pixels of the radius that will round the corners of the rectangle. Note: doesn't work with multicolor" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });

        setup.about = "Adds a rectangle.";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawText:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::FloatList, "pos", mvArgType::REQUIRED_ARG, "...", "Top left point of bounding text rectangle." });
        args.push_back({ mvPyDataType::String, "text", mvArgType::REQUIRED_ARG, "...", "Text to draw." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
        args.push_back({ mvPyDataType::Float, "size", mvArgType::KEYWORD_ARG, "10.0" });

        setup.about = "Adds text (drawlist).";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawPolygon:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::ListFloatList, "points" });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
        args.push_back({ mvPyDataType::IntList, "fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });

        setup.about = "Adds a polygon.";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawPolyline:                
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::ListFloatList, "points" });
        args.push_back({ mvPyDataType::Bool, "closed", mvArgType::KEYWORD_ARG, "False", "Will close the polyline by returning to the first point." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });

        setup.about = "Adds a polyline.";
        setup.category = { "Drawlist", "Widgets" };
        break;
    }
    case mvAppItemType::mvDrawImage:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::UUID, "texture_tag" });
        args.push_back({ mvPyDataType::FloatList, "pmin", mvArgType::REQUIRED_ARG, "...", "Point of to start drawing texture." });
        args.push_back({ mvPyDataType::FloatList, "pmax", mvArgType::REQUIRED_ARG, "...", "Point to complete drawing texture." });
        args.push_back({ mvPyDataType::FloatList, "uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "Normalized coordinates on texture that will be drawn." });
        args.push_back({ mvPyDataType::FloatList, "uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "Normalized coordinates on texture that will be drawn." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });

        setup.about = "Adds an image (for a drawing).";
        setup.category = { "Drawlist", "Widgets", "Textures" };
        break;
    }
    case mvAppItemType::mvDragFloatMulti:              
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::FloatList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)" });
        args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4", "Number of floats to be displayed." });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%0.3f'", "Determines the format the float will be displayed as use python string formatting." });
        args.push_back({ mvPyDataType::Float, "speed", mvArgType::KEYWORD_ARG, "1.0", "Sets the sensitivity the float will be modified while dragging." });
        args.push_back({ mvPyDataType::Float, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Applies a limit only to draging entry only." });
        args.push_back({ mvPyDataType::Float, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Applies a limit only to draging entry only." });
        args.push_back({ mvPyDataType::Bool, "no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods or Enter key allowing to input text directly into the widget." });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click." });

        setup.about = "Adds drag input for a set of float values up to 4. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes.";
        setup.category = { "Widgets" };
        break;
    }
    case mvAppItemType::mvDragDoubleMulti:
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::DoubleList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)" });
        args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4", "Number of doubles to be displayed." });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%0.3f'", "Determines the format the float will be displayed as use python string formatting." });
        args.push_back({ mvPyDataType::Float, "speed", mvArgType::KEYWORD_ARG, "1.0", "Sets the sensitivity the float will be modified while dragging." });
        args.push_back({ mvPyDataType::Double, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Applies a limit only to draging entry only." });
        args.push_back({ mvPyDataType::Double, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Applies a limit only to draging entry only." });
        args.push_back({ mvPyDataType::Bool, "no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods or Enter key allowing to input text directly into the widget." });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click." });

        setup.about = "Adds drag input for a set of double values up to 4. Useful when drag float is not accurate enough. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes.";
        setup.category = { "Widgets" };
        break;
    }
    case mvAppItemType::mvDragIntMulti:                
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::IntList, "default_value", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)" });
        args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4", "Number of ints to be displayed." });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%d'", "Determines the format the int will be displayed as use python string formatting." });
        args.push_back({ mvPyDataType::Float, "speed", mvArgType::KEYWORD_ARG, "1.0", "Sets the sensitivity the float will be modified while dragging." });
        args.push_back({ mvPyDataType::Integer, "min_value", mvArgType::KEYWORD_ARG, "0", "Applies a limit only to draging entry only." });
        args.push_back({ mvPyDataType::Integer, "max_value", mvArgType::KEYWORD_ARG, "100", "Applies a limit only to draging entry only." });
        args.push_back({ mvPyDataType::Bool, "no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods or Enter key allowing to input text directly into the widget." });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click." });

        setup.about = "Adds drag input for a set of int values up to 4. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the drag. Use clamped keyword to also apply limits to the direct entry modes.";
        break;
    }
    case mvAppItemType::mvSliderFloatMulti:            
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::FloatList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)" });
        args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4", "Number of floats to be displayed." });
        args.push_back({ mvPyDataType::Bool, "no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods double-click or ctrl+click or Enter key allowing to input text directly into the item." });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click." });
        args.push_back({ mvPyDataType::Float, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Applies a limit only to sliding entry only." });
        args.push_back({ mvPyDataType::Float, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Applies a limit only to sliding entry only." });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%.3f'", "Determines the format the int will be displayed as use python string formatting." });

        setup.about = "Adds multi slider for up to 4 float values. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes.";
        break;
    }
    case mvAppItemType::mvSliderDoubleMulti:
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::DoubleList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)" });
        args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4", "Number of doubles to be displayed." });
        args.push_back({ mvPyDataType::Bool, "no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods double-click or ctrl+click or Enter key allowing to input text directly into the item." });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click." });
        args.push_back({ mvPyDataType::Double, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Applies a limit only to sliding entry only." });
        args.push_back({ mvPyDataType::Double, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Applies a limit only to sliding entry only." });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%.3f'", "Determines the format the int will be displayed as use python string formatting." });

        setup.about = "Adds multi slider for up to 4 double values. Usueful for when multi slide float is not accurate enough. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes.";
        break;
    }
    case mvAppItemType::mvSliderIntMulti:              
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::IntList, "default_value", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)" });
        args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4", "Number of ints to be displayed." });
        args.push_back({ mvPyDataType::Bool, "no_input", mvArgType::KEYWORD_ARG, "False", "Disable direct entry methods double-click or ctrl+click or Enter key allowing to input text directly into the item." });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "False", "Applies the min and max limits to direct entry methods also such as double click and CTRL+Click." });
        args.push_back({ mvPyDataType::Integer, "min_value", mvArgType::KEYWORD_ARG, "0", "Applies a limit only to sliding entry only." });
        args.push_back({ mvPyDataType::Integer, "max_value", mvArgType::KEYWORD_ARG, "100", "Applies a limit only to sliding entry only." });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%d'", "Determines the format the int will be displayed as use python string formatting." });

        setup.about = "Adds multi slider for up to 4 int values. Directly entry can be done with double click or CTRL+Click. Min and Max alone are a soft limit for the slider. Use clamped keyword to also apply limits to the direct entry modes.";
        break;
    }
    case mvAppItemType::mvInputIntMulti:               
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::IntList, "default_value", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)" });
        args.push_back({ mvPyDataType::Integer, "min_value", mvArgType::KEYWORD_ARG, "0", "Value for lower limit of input for each cell. Use min_clamped to turn on." });
        args.push_back({ mvPyDataType::Integer, "max_value", mvArgType::KEYWORD_ARG, "100", "Value for upper limit of input for each cell. Use max_clamped to turn on." });
        args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4", "Number of components displayed for input." });
        args.push_back({ mvPyDataType::Bool, "min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of min_value." });
        args.push_back({ mvPyDataType::Bool, "max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of max_value." });
        args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter." });
        args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode where no text can be input but text can still be highlighted." });

        setup.about = "Adds multi int input for up to 4 integer values.";
        break;
    }
    case mvAppItemType::mvInputFloatMulti:             
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::FloatList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)" });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%.3f'", "Determines the format the float will be displayed as use python string formatting." });
        args.push_back({ mvPyDataType::Float, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Value for lower limit of input for each cell. Use min_clamped to turn on." });
        args.push_back({ mvPyDataType::Float, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Value for upper limit of input for each cell. Use max_clamped to turn on." });
        args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4", "Number of components displayed for input." });
        args.push_back({ mvPyDataType::Bool, "min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of min_value." });
        args.push_back({ mvPyDataType::Bool, "max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of max_value." });
        args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press." });
        args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode where no text can be input but text can still be highlighted." });

        setup.about = "Adds multi float input for up to 4 float values.";
        break;
    }
    case mvAppItemType::mvInputDoubleMulti:
    {
        AddCommonArgs(args, (CommonParserArgs)(
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

        args.push_back({ mvPyDataType::DoubleList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)" });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%.3f'", "Determines the format the float will be displayed as use python string formatting." });
        args.push_back({ mvPyDataType::Double, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Value for lower limit of input for each cell. Use min_clamped to turn on." });
        args.push_back({ mvPyDataType::Double, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Value for upper limit of input for each cell. Use max_clamped to turn on." });
        args.push_back({ mvPyDataType::Integer, "size", mvArgType::KEYWORD_ARG, "4", "Number of components displayed for input." });
        args.push_back({ mvPyDataType::Bool, "min_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of min_value." });
        args.push_back({ mvPyDataType::Bool, "max_clamped", mvArgType::KEYWORD_ARG, "False", "Activates and deactivates the enforcment of max_value." });
        args.push_back({ mvPyDataType::Bool, "on_enter", mvArgType::KEYWORD_ARG, "False", "Only runs callback on enter key press." });
        args.push_back({ mvPyDataType::Bool, "readonly", mvArgType::KEYWORD_ARG, "False", "Activates read only mode where no text can be input but text can still be highlighted." });

        setup.about = "Adds multi double input for up to 4 double values. Useful when input float mulit is not accurate enough.";
        break;
    }
    case mvAppItemType::mvDragPoint:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0)" });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
        args.push_back({ mvPyDataType::Bool, "show_label", mvArgType::KEYWORD_ARG, "True" });

        setup.about = "Adds a drag point to a plot.";
        setup.category = { "Plotting", "Widgets" };
        break;
    }
    case mvAppItemType::mvDragLine:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
        args.push_back({ mvPyDataType::Bool, "show_label", mvArgType::KEYWORD_ARG, "True" });
        args.push_back({ mvPyDataType::Bool, "vertical", mvArgType::KEYWORD_ARG, "True" });

        setup.about = "Adds a drag line to a plot.";
        setup.category = { "Plotting", "Widgets" };
        break;
    }
    case mvAppItemType::mvAnnotation:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0)" });
        args.push_back({ mvPyDataType::FloatList, "offset", mvArgType::KEYWORD_ARG, "(0.0, 0.0)" });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
        args.push_back({ mvPyDataType::Bool, "clamped", mvArgType::KEYWORD_ARG, "True" });

        setup.about = "Adds an annotation to a plot.";
        setup.category = { "Plotting", "Widgets" };
        break;
    }
    case mvAppItemType::mvLineSeries:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });
        args.push_back({ mvPyDataType::DoubleList, "y" });

        setup.about = "Adds a line series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvScatterSeries:               
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });
        args.push_back({ mvPyDataType::DoubleList, "y" });

        setup.about = "Adds a scatter series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvStemSeries:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });
        args.push_back({ mvPyDataType::DoubleList, "y" });

        setup.about = "Adds a stem series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvStairSeries:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });
        args.push_back({ mvPyDataType::DoubleList, "y" });

        setup.about = "Adds a stair series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvBarSeries:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });
        args.push_back({ mvPyDataType::DoubleList, "y" });
        args.push_back({ mvPyDataType::Float, "weight", mvArgType::KEYWORD_ARG, "1.0" });
        args.push_back({ mvPyDataType::Bool, "horizontal", mvArgType::KEYWORD_ARG, "False" });

        setup.about = "Adds a bar series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvErrorSeries:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });
        args.push_back({ mvPyDataType::DoubleList, "y" });
        args.push_back({ mvPyDataType::DoubleList, "negative" });
        args.push_back({ mvPyDataType::DoubleList, "positive" });
        args.push_back({ mvPyDataType::Bool, "contribute_to_bounds", mvArgType::KEYWORD_ARG, "True" });
        args.push_back({ mvPyDataType::Bool, "horizontal", mvArgType::KEYWORD_ARG, "False" });

        setup.about = "Adds an error series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvVLineSeries:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });

        setup.about = "Adds an infinite vertical line series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvHLineSeries:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });

        setup.about = "Adds an infinite horizontal line series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvHeatSeries:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });
        args.push_back({ mvPyDataType::Integer, "rows" });
        args.push_back({ mvPyDataType::Integer, "cols" });
        args.push_back({ mvPyDataType::Double, "scale_min", mvArgType::KEYWORD_ARG, "0.0", "Sets the color scale min. Typically paired with the color scale widget scale_min." });
        args.push_back({ mvPyDataType::Double, "scale_max", mvArgType::KEYWORD_ARG, "1.0", "Sets the color scale max. Typically paired with the color scale widget scale_max." });
        args.push_back({ mvPyDataType::DoubleList, "bounds_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)" });
        args.push_back({ mvPyDataType::DoubleList, "bounds_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)" });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%0.1f'" });
        args.push_back({ mvPyDataType::Bool, "contribute_to_bounds", mvArgType::KEYWORD_ARG, "True" });

        setup.about = "Adds a heat series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvImageSeries:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::UUID, "texture_tag" });
        args.push_back({ mvPyDataType::DoubleList, "bounds_min" });
        args.push_back({ mvPyDataType::DoubleList, "bounds_max" });
        args.push_back({ mvPyDataType::FloatList, "uv_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)", "normalized texture coordinates" });
        args.push_back({ mvPyDataType::FloatList, "uv_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)", "normalized texture coordinates" });
        args.push_back({ mvPyDataType::IntList, "tint_color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });

        setup.about = "Adds an image series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets", "Textures" };
        break;
    }
    case mvAppItemType::mvPieSeries:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Double, "x" });
        args.push_back({ mvPyDataType::Double, "y" });
        args.push_back({ mvPyDataType::Double, "radius" });
        args.push_back({ mvPyDataType::DoubleList, "values" });
        args.push_back({ mvPyDataType::StringList, "labels" });
        args.push_back({ mvPyDataType::String, "format", mvArgType::KEYWORD_ARG, "'%0.2f'" });
        args.push_back({ mvPyDataType::Double, "angle", mvArgType::KEYWORD_ARG, "90.0" });
        args.push_back({ mvPyDataType::Bool, "normalize", mvArgType::KEYWORD_ARG, "False" });

        setup.about = "Adds an pie series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvShadeSeries:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });
        args.push_back({ mvPyDataType::DoubleList, "y1" });
        args.push_back({ mvPyDataType::DoubleList, "y2", mvArgType::KEYWORD_ARG, "[]" });

        setup.about = "Adds a shade series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvLabelSeries:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Double, "x" });
        args.push_back({ mvPyDataType::Double, "y" });
        args.push_back({ mvPyDataType::Integer, "x_offset", mvArgType::KEYWORD_ARG });
        args.push_back({ mvPyDataType::Integer, "y_offset", mvArgType::KEYWORD_ARG });
        args.push_back({ mvPyDataType::Bool, "vertical", mvArgType::KEYWORD_ARG, "False" });

        setup.about = "Adds a label series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvHistogramSeries:             
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });
        args.push_back({ mvPyDataType::Integer, "bins", mvArgType::KEYWORD_ARG, "-1" });
        args.push_back({ mvPyDataType::Float, "bar_scale", mvArgType::KEYWORD_ARG, "1.0" });
        args.push_back({ mvPyDataType::Double, "min_range", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::Double, "max_range", mvArgType::KEYWORD_ARG, "1.0" });
        args.push_back({ mvPyDataType::Bool, "cumlative", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "density", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "outliers", mvArgType::KEYWORD_ARG, "True" });
        args.push_back({ mvPyDataType::Bool, "contribute_to_bounds", mvArgType::KEYWORD_ARG, "True" });

        setup.about = "Adds a histogram series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mv2dHistogramSeries:           
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });
        args.push_back({ mvPyDataType::DoubleList, "y" });
        args.push_back({ mvPyDataType::Integer, "xbins", mvArgType::KEYWORD_ARG, "-1" });
        args.push_back({ mvPyDataType::Integer, "ybins", mvArgType::KEYWORD_ARG, "-1" });
        args.push_back({ mvPyDataType::Double, "xmin_range", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::Double, "xmax_range", mvArgType::KEYWORD_ARG, "1.0" });
        args.push_back({ mvPyDataType::Double, "ymin_range", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::Double, "ymax_range", mvArgType::KEYWORD_ARG, "1.0" });
        args.push_back({ mvPyDataType::Bool, "density", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "outliers", mvArgType::KEYWORD_ARG, "True" });

        setup.about = "Adds a 2d histogram series.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvCandleSeries:                
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "dates" });
        args.push_back({ mvPyDataType::DoubleList, "opens" });
        args.push_back({ mvPyDataType::DoubleList, "closes" });
        args.push_back({ mvPyDataType::DoubleList, "lows" });
        args.push_back({ mvPyDataType::DoubleList, "highs" });
        args.push_back({ mvPyDataType::IntList, "bull_color", mvArgType::KEYWORD_ARG, "(0, 255, 113, 255)" });
        args.push_back({ mvPyDataType::IntList, "bear_color", mvArgType::KEYWORD_ARG, "(218, 13, 79, 255)" });
        args.push_back({ mvPyDataType::Float, "weight", mvArgType::KEYWORD_ARG, "0.25" });
        args.push_back({ mvPyDataType::Bool, "tooltip", mvArgType::KEYWORD_ARG, "True" });
        args.push_back({ mvPyDataType::Integer, "time_unit", mvArgType::KEYWORD_ARG, "5", "mvTimeUnit_* constants. Default mvTimeUnit_Day."});

        setup.about = "Adds a candle series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvCustomSeries:
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });
        args.push_back({ mvPyDataType::DoubleList, "y" });
        args.push_back({ mvPyDataType::Integer, "channel_count" });
        args.push_back({ mvPyDataType::DoubleList, "y1", mvArgType::KEYWORD_ARG, "[]" });
        args.push_back({ mvPyDataType::DoubleList, "y2", mvArgType::KEYWORD_ARG, "[]" });
        args.push_back({ mvPyDataType::DoubleList, "y3", mvArgType::KEYWORD_ARG, "[]" });
        args.push_back({ mvPyDataType::Bool, "tooltip", mvArgType::KEYWORD_ARG, "True", "Show tooltip when plot is hovered." });

        setup.about = "Adds a custom series to a plot. New in 1.6.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvAreaSeries:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::DoubleList, "x" });
        args.push_back({ mvPyDataType::DoubleList, "y" });
        args.push_back({ mvPyDataType::IntList, "fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
        args.push_back({ mvPyDataType::Bool, "contribute_to_bounds", mvArgType::KEYWORD_ARG, "True" });

        setup.about = "Adds an area series to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        break;
    }
    case mvAppItemType::mvColorMapScale:               
    {

        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Callable, "drag_callback", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });
        args.push_back({ mvPyDataType::UUID, "colormap", mvArgType::KEYWORD_ARG, "0", "mvPlotColormap_* constants or mvColorMap uuid from a color map registry" });
        args.push_back({ mvPyDataType::Float, "min_scale", mvArgType::KEYWORD_ARG, "0.0", "Sets the min number of the color scale. Typically is the same as the min scale from the heat series." });
        args.push_back({ mvPyDataType::Float, "max_scale", mvArgType::KEYWORD_ARG, "1.0", "Sets the max number of the color scale. Typically is the same as the max scale from the heat series." });

        setup.about = "Adds a legend that pairs values with colors. This is typically used with a heat series. ";
        setup.category = { "Widgets", "Colors" };
        break;
    }
    case mvAppItemType::mvSlider3D:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::FloatList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)" });
        args.push_back({ mvPyDataType::Float, "max_x", mvArgType::KEYWORD_ARG, "100.0", "Applies upper limit to slider." });
        args.push_back({ mvPyDataType::Float, "max_y", mvArgType::KEYWORD_ARG, "100.0", "Applies upper limit to slider." });
        args.push_back({ mvPyDataType::Float, "max_z", mvArgType::KEYWORD_ARG, "100.0", "Applies upper limit to slider." });
        args.push_back({ mvPyDataType::Float, "min_x", mvArgType::KEYWORD_ARG, "0.0", "Applies lower limit to slider." });
        args.push_back({ mvPyDataType::Float, "min_y", mvArgType::KEYWORD_ARG, "0.0", "Applies lower limit to slider." });
        args.push_back({ mvPyDataType::Float, "min_z", mvArgType::KEYWORD_ARG, "0.0", "Applies lower limit to slider." });
        args.push_back({ mvPyDataType::Float, "scale", mvArgType::KEYWORD_ARG, "1.0", "Size of the widget." });

        setup.about = "Adds a 3D box slider.";
        break;
    }
    case mvAppItemType::mvKnobFloat:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SOURCE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_ENABLED |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_DRAG_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Float, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::Float, "min_value", mvArgType::KEYWORD_ARG, "0.0", "Applies lower limit to value." });
        args.push_back({ mvPyDataType::Float, "max_value", mvArgType::KEYWORD_ARG, "100.0", "Applies upper limit to value." });

        setup.about = "Adds a knob that rotates based on change in x mouse position.";
        break;
    }
    case mvAppItemType::mvLoadingIndicator:            
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Integer, "style", mvArgType::KEYWORD_ARG, "0", "0 is rotating dots style, 1 is rotating bar style." });
        args.push_back({ mvPyDataType::Integer, "circle_count", mvArgType::KEYWORD_ARG, "8", "Number of dots show if dots or size of circle if circle." });
        args.push_back({ mvPyDataType::Float, "speed", mvArgType::KEYWORD_ARG, "1.0", "Speed the anamation will rotate." });
        args.push_back({ mvPyDataType::Float, "radius", mvArgType::KEYWORD_ARG, "3.0", "Radius size of the loading indicator." });
        args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0", "Thickness of the circles or line." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(51, 51, 55, 255)", "Color of the growing center circle." });
        args.push_back({ mvPyDataType::IntList, "secondary_color", mvArgType::KEYWORD_ARG, "(29, 151, 236, 103)", "Background of the dots in dot mode." });

        setup.about = "Adds a rotating animated loading symbol.";
        break;
    }
    case mvAppItemType::mvNodeLink:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::UUID, "attr_1" });
        args.push_back({ mvPyDataType::UUID, "attr_2" });

        setup.about = "Adds a node link between 2 node attributes.";
        setup.category = { "Node Editor", "Widgets" };
        break;
    }
    case mvAppItemType::mvTextureRegistry:             
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID)
        );

        args.push_back({ mvPyDataType::Bool, "show", mvArgType::KEYWORD_ARG, "False", "Attempt to render widget." });

        setup.about = "Adds a dynamic texture.";
        setup.category = { "Textures", "Registries", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvStaticTexture:               
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID)
        );

        args.push_back({ mvPyDataType::Integer, "width" });
        args.push_back({ mvPyDataType::Integer, "height" });
        args.push_back({ mvPyDataType::FloatList, "default_value" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_2", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a static texture.";
        setup.category = { "Textures", "Widgets" };
        break;
    }
    case mvAppItemType::mvDynamicTexture:              
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID)
        );

        args.push_back({ mvPyDataType::Integer, "width" });
        args.push_back({ mvPyDataType::Integer, "height" });
        args.push_back({ mvPyDataType::FloatList, "default_value" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_2", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a dynamic texture.";
        setup.category = { "Textures", "Widgets" };
        break;
    }
    case mvAppItemType::mvStage:                       
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID)
        );

        setup.about = "Adds a stage.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvDrawLayer:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Bool, "perspective_divide", mvArgType::KEYWORD_ARG, "False", "New in 1.1. apply perspective divide" });
        args.push_back({ mvPyDataType::Bool, "depth_clipping", mvArgType::KEYWORD_ARG, "False", "New in 1.1. apply depth clipping" });
        args.push_back({ mvPyDataType::Integer, "cull_mode", mvArgType::KEYWORD_ARG, "0", "New in 1.1. culling mode, mvCullMode_* constants. Only works with triangles currently." });

        setup.about = "New in 1.1. Creates a layer useful for grouping drawlist items.";
        setup.category = { "Drawlist", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvViewportDrawlist:            
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Bool, "front", mvArgType::KEYWORD_ARG, "True", "Draws to the front of the view port instead of the back." });

        setup.about = "A container that is used to present draw items or layers directly to the viewport. By default this will draw to the back of the viewport. Layers and draw items should be added to this widget as children.";
        setup.category = { "Drawlist", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvFileExtension:               
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE)
        );

        args.push_back({ mvPyDataType::String, "extension", mvArgType::REQUIRED_ARG, "*", "Extension that will show as an when the parent is a file dialog." });
        args.push_back({ mvPyDataType::String, "custom_text", mvArgType::KEYWORD_ARG, "''", "Replaces the displayed text in the drop down for this extension." });
        args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(-255, 0, 0, 255)", "Color for the text that will be shown with specified extensions." });

        setup.about = "Creates a file extension filter option in the file dialog.";
        setup.category = { "File Dialog" };
        break;
    }
    case mvAppItemType::mvPlotLegend:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Integer, "location", mvArgType::KEYWORD_ARG, "5", "location, mvPlot_Location_*" });
        args.push_back({ mvPyDataType::Bool, "horizontal", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "outside", mvArgType::KEYWORD_ARG, "False" });

        setup.about = "Adds a plot legend to a plot.";
        setup.category = { "Plotting", "Widgets" };
        break;
    }
    case mvAppItemType::mvPlotAxis:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Integer, "axis" });
        args.push_back({ mvPyDataType::Bool, "no_gridlines", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "no_tick_marks", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "no_tick_labels", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "log_scale", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "invert", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "lock_min", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "lock_max", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "time", mvArgType::KEYWORD_ARG, "False" });

        setup.about = "Adds an axis to a plot.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvHandlerRegistry:             
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW)
        );

        setup.about = "Adds a handler registry.";
        setup.category = { "Events", "Widgets", "Registries" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvKeyDownHandler:              
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Integer, "key", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all keys" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a key down handler.";
        setup.category = { "Events", "Widgets" };
        break;
    }
    case mvAppItemType::mvKeyPressHandler:             
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Integer, "key", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all keys" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a key press handler.";
        setup.category = { "Events", "Widgets" };
        break;
    }
    case mvAppItemType::mvKeyReleaseHandler:           
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Integer, "key", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all keys" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a key release handler.";
        setup.category = { "Events", "Widgets" };
        break;
    }
    case mvAppItemType::mvMouseMoveHandler:            
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_CALLBACK)
        );
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a mouse move handler.";
        setup.category = { "Events", "Widgets" };
        break;
    }
    case mvAppItemType::mvMouseWheelHandler:           
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_CALLBACK)
        );
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a mouse wheel handler.";
        setup.category = { "Events", "Widgets" };
        break;
    }
    case mvAppItemType::mvMouseClickHandler:           
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Integer, "button", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all mouse buttons" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a mouse click handler.";
        setup.category = { "Events", "Widgets" };
        break;
    }
    case mvAppItemType::mvMouseDoubleClickHandler:     
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Integer, "button", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all mouse buttons" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a mouse double click handler.";
        setup.category = { "Events", "Widgets" };
        break;
    }
    case mvAppItemType::mvMouseDownHandler:            
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Integer, "button", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all mouse buttons" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a mouse down handler.";
        setup.category = { "Events", "Widgets" };
        break;
    }
    case mvAppItemType::mvMouseReleaseHandler:         
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Integer, "button", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all mouse buttons" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a mouse release handler.";
        setup.category = { "Events", "Widgets" };
        break;
    }
    case mvAppItemType::mvMouseDragHandler:            
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Integer, "button", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all mouse buttons" });
        args.push_back({ mvPyDataType::Float, "threshold", mvArgType::POSITIONAL_ARG, "10.0", "The threshold the mouse must be dragged before the callback is ran" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a mouse drag handler.";
        setup.category = { "Events", "Widgets" };
        break;
    }
    case mvAppItemType::mvHoverHandler:                
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        setup.about = "Adds a hover handler.";
        setup.category = { "Widgets", "Events" };
        break;
    }
    case mvAppItemType::mvActiveHandler:               
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        setup.about = "Adds a active handler.";
        setup.category = { "Widgets", "Events" };
        break;
    }
    case mvAppItemType::mvFocusHandler:                
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        setup.about = "Adds a focus handler.";
        setup.category = { "Widgets", "Events" };
        break;
    }
    case mvAppItemType::mvVisibleHandler:              
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        setup.about = "Adds a visible handler.";
        setup.category = { "Widgets", "Events" };
        break;
    }
    case mvAppItemType::mvEditedHandler:               
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        setup.about = "Adds an edited handler.";
        setup.category = { "Widgets", "Events" };
        break;
    }
    case mvAppItemType::mvActivatedHandler:            
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        setup.about = "Adds a activated handler.";
        setup.category = { "Widgets", "Events" };
        break;
    }
    case mvAppItemType::mvDeactivatedHandler:          
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        setup.about = "Adds a deactivated handler.";
        setup.category = { "Widgets", "Events" };
        break;
    }
    case mvAppItemType::mvDeactivatedAfterEditHandler: 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        setup.about = "Adds a deactivated after edit handler.";
        setup.category = { "Widgets", "Events" };
        break;
    }
    case mvAppItemType::mvToggledOpenHandler:          
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        setup.about = "Adds a togged open handler.";
        setup.category = { "Widgets", "Events" };
        break;
    }
    case mvAppItemType::mvClickedHandler:              
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        args.push_back({ mvPyDataType::Integer, "button", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all mouse buttons" });

        setup.about = "Adds a clicked handler.";
        setup.category = { "Widgets", "Events" };
        break;
    }
    case mvAppItemType::mvDoubleClickedHandler:              
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        args.push_back({ mvPyDataType::Integer, "button", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all mouse buttons" });

        setup.about = "Adds a double click handler.";
        setup.category = { "Widgets", "Events" };
        break;
    }
    case mvAppItemType::mvDragPayload:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::Object, "drag_data", mvArgType::KEYWORD_ARG, "None", "Drag data" });
        args.push_back({ mvPyDataType::Object, "drop_data", mvArgType::KEYWORD_ARG, "None", "Drop data" });
        args.push_back({ mvPyDataType::String, "payload_type", mvArgType::KEYWORD_ARG, "'$$DPG_PAYLOAD'" });

        setup.about = "User data payload for drag and drop operations.";
        setup.category = { "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvResizeHandler:               
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        setup.about = "Adds a resize handler.";
        setup.category = { "Widgets", "Events" };
        break;
    }
    case mvAppItemType::mvFont:                        
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID)
        );

        args.push_back({ mvPyDataType::String, "file" });
        args.push_back({ mvPyDataType::Integer, "size" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_0", "Parent to add this item to. (runtime adding)" });
        args.push_back({ mvPyDataType::Bool, "default_font", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });

        setup.about = "Adds font to a font registry.";
        setup.category = { "Fonts", "Containers" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvFontRegistry:                
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW)
        );

        setup.about = "Adds a font registry.";
        setup.category = { "Fonts", "Containers", "Registries" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvTheme:                       
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID)
        );

        args.push_back({ mvPyDataType::Bool, "default_theme", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });

        setup.about = "Adds a theme.";
        setup.category = { "Themes", "Containers" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvThemeColor:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT)
        );

        args.push_back({ mvPyDataType::Long, "target", mvArgType::POSITIONAL_ARG, "0" });
        args.push_back({ mvPyDataType::IntList, "value", mvArgType::POSITIONAL_ARG, "(0, 0, 0, 255)" });
        args.push_back({ mvPyDataType::Integer, "category", mvArgType::KEYWORD_ARG, "0", "Options include mvThemeCat_Core, mvThemeCat_Plots, mvThemeCat_Nodes." });

        setup.about = "Adds a theme color.";
        setup.category = { "Themes" };
        break;
    }
    case mvAppItemType::mvThemeStyle:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT)
        );

        args.push_back({ mvPyDataType::Long, "target", mvArgType::POSITIONAL_ARG, "0" });
        args.push_back({ mvPyDataType::Float, "x", mvArgType::POSITIONAL_ARG, "1.0" });
        args.push_back({ mvPyDataType::Float, "y", mvArgType::POSITIONAL_ARG, "-1.0" });
        args.push_back({ mvPyDataType::Integer, "category", mvArgType::KEYWORD_ARG, "0", "Options include mvThemeCat_Core, mvThemeCat_Plots, mvThemeCat_Nodes." });

        setup.about = "Adds a theme style.";
        setup.category = { "Themes" };
        break;
    }
    case mvAppItemType::mvThemeComponent:              
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE
            ));

        args.push_back({ mvPyDataType::Integer, "item_type", mvArgType::POSITIONAL_ARG, "0" });
        args.push_back({ mvPyDataType::Bool, "enabled_state", mvArgType::KEYWORD_ARG, "True" });

        setup.about = "Adds a theme component.";
        setup.category = { "Themes", "Containers" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvFontRangeHint:               
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT)
        );

        args.push_back({ mvPyDataType::Integer, "hint" });

        setup.about = "Adds a range of font characters (mvFontRangeHint_ constants).";
        setup.category = { "Fonts", "Widgets" };
        break;
    }
    case mvAppItemType::mvFontRange:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT)
        );

        args.push_back({ mvPyDataType::Integer, "first_char" });
        args.push_back({ mvPyDataType::Integer, "last_char" });

        setup.about = "Adds a range of font characters to a font.";
        setup.category = { "Fonts", "Widgets" };
        break;
    }
    case mvAppItemType::mvFontChars:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT)
        );

        args.push_back({ mvPyDataType::IntList, "chars" });

        setup.about = "Adds specific font characters to a font.";
        setup.category = { "Fonts", "Widgets" };
        break;
    }
    case mvAppItemType::mvCharRemap:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT)
        );

        args.push_back({ mvPyDataType::Integer, "source" });
        args.push_back({ mvPyDataType::Integer, "target" });

        setup.category = { "Fonts", "Widgets" };
        setup.about = "Remaps a character.";
        break;
    }
    case mvAppItemType::mvValueRegistry:               
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID)
        );

        setup.about = "Adds a value registry.";
        setup.category = { "Widgets", "Values", "Containers" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvIntValue:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        args.push_back({ mvPyDataType::Integer, "default_value", mvArgType::KEYWORD_ARG, "0" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a int value.";
        setup.category = { "Widgets", "Values" };
        break;
    }
    case mvAppItemType::mvFloatValue:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        args.push_back({ mvPyDataType::Float, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a float value.";
        setup.category = { "Widgets", "Values" };
        break;
    }
    case mvAppItemType::mvFloat4Value:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        args.push_back({ mvPyDataType::FloatList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a float4 value.";
        setup.category = { "Widgets", "Values" };
        break;
    }
    case mvAppItemType::mvInt4Value:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        args.push_back({ mvPyDataType::IntList, "default_value", mvArgType::KEYWORD_ARG, "(0, 0, 0, 0)" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a int4 value.";
        setup.category = { "Widgets", "Values" };
        break;
    }
    case mvAppItemType::mvBoolValue:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        args.push_back({ mvPyDataType::Bool, "default_value", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a bool value.";
        setup.category = { "Widgets", "Values" };
        break;
    }
    case mvAppItemType::mvStringValue:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        args.push_back({ mvPyDataType::String, "default_value", mvArgType::KEYWORD_ARG, "''" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a string value.";
        setup.category = { "Widgets", "Values" };
        break;
    }
    case mvAppItemType::mvDoubleValue:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        args.push_back({ mvPyDataType::Double, "default_value", mvArgType::KEYWORD_ARG, "0.0" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a double value.";
        setup.category = { "Widgets", "Values" };
        break;
    }
    case mvAppItemType::mvDouble4Value:                
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        args.push_back({ mvPyDataType::DoubleList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a double value.";
        setup.category = { "Widgets", "Values" };
        break;
    }
    case mvAppItemType::mvColorValue:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        args.push_back({ mvPyDataType::FloatList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a color value.";
        setup.category = { "Widgets", "Values" };
        break;
    }
    case mvAppItemType::mvFloatVectValue:              
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        args.push_back({ mvPyDataType::FloatList, "default_value", mvArgType::KEYWORD_ARG, "()" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a float vect value.";
        setup.category = { "Widgets", "Values" };
        break;
    }
    case mvAppItemType::mvSeriesValue:                 
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

        args.push_back({ mvPyDataType::DoubleList, "default_value", mvArgType::KEYWORD_ARG, "()" });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a plot series value.";
        setup.category = { "Widgets", "Values" };
        break;
    }
    case mvAppItemType::mvRawTexture:                  
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID)
        );

        args.push_back({ mvPyDataType::Integer, "width" });
        args.push_back({ mvPyDataType::Integer, "height" });
        args.push_back({ mvPyDataType::FloatList, "default_value" });
        args.push_back({ mvPyDataType::Integer, "format", mvArgType::KEYWORD_ARG, "internal_dpg.mvFormat_Float_rgba", "Data format." });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_2", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a raw texture.";
        setup.category = { "Textures", "Widgets" };
        break;
    }
    case mvAppItemType::mvSubPlots:                    
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SEARCH_DELAY |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Integer, "rows" });
        args.push_back({ mvPyDataType::Integer, "columns" });
        args.push_back({ mvPyDataType::FloatList, "row_ratios", mvArgType::KEYWORD_ARG, "[]" });
        args.push_back({ mvPyDataType::FloatList, "column_ratios", mvArgType::KEYWORD_ARG, "[]" });

        // plot flags
        args.push_back({ mvPyDataType::Bool, "no_title", mvArgType::KEYWORD_ARG, "False" });
        args.push_back({ mvPyDataType::Bool, "no_menus", mvArgType::KEYWORD_ARG, "False", "the user will not be able to open context menus with right-click" });
        args.push_back({ mvPyDataType::Bool, "no_resize", mvArgType::KEYWORD_ARG, "False", "resize splitters between subplot cells will be not be provided" });
        args.push_back({ mvPyDataType::Bool, "no_align", mvArgType::KEYWORD_ARG, "False", "subplot edges will not be aligned vertically or horizontally" });
        args.push_back({ mvPyDataType::Bool, "link_rows", mvArgType::KEYWORD_ARG, "False", "link the y-axis limits of all plots in each row (does not apply auxiliary y-axes)" });
        args.push_back({ mvPyDataType::Bool, "link_columns", mvArgType::KEYWORD_ARG, "False", "link the x-axis limits of all plots in each column" });
        args.push_back({ mvPyDataType::Bool, "link_all_x", mvArgType::KEYWORD_ARG, "False", "link the x-axis limits in every plot in the subplot" });
        args.push_back({ mvPyDataType::Bool, "link_all_y", mvArgType::KEYWORD_ARG, "False", "link the y-axis limits in every plot in the subplot (does not apply to auxiliary y-axes)" });
        args.push_back({ mvPyDataType::Bool, "column_major", mvArgType::KEYWORD_ARG, "False", "subplots are added in column major order instead of the default row major order" });

        setup.about = "Adds a collection of plots.";
        setup.category = { "Plotting", "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvColorMap:                    
    {

        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW)
        );

        args.push_back({ mvPyDataType::ListListInt, "colors", mvArgType::REQUIRED_ARG, "", "colors that will be mapped to the normalized value 0.0->1.0" });
        args.push_back({ mvPyDataType::Bool, "qualitative", mvArgType::REQUIRED_ARG, "", "Qualitative will create hard transitions for color boundries across the value range when enabled." });
        args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_4", "Parent to add this item to. (runtime adding)" });

        setup.about = "Adds a legend that pairs colors with normalized value 0.0->1.0. Each color will be  This is typically used with a heat series. (ex. [[0, 0, 0, 255], [255, 255, 255, 255]] will be mapped to a soft transition from 0.0-1.0)";
        setup.category = { "Widgets", "Colors" };
        break;
    }
    case mvAppItemType::mvColorMapRegistry:            
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID)
        );

        args.push_back({ mvPyDataType::Bool, "show", mvArgType::KEYWORD_ARG, "False", "Attempt to render widget." });

        setup.about = "Adds a colormap registry.";
        setup.category = { "Containers", "Widgets", "Colors" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvColorMapButton:              
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
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

        args.push_back({ mvPyDataType::IntList, "default_value", mvArgType::POSITIONAL_ARG, "(0, 0, 0, 255)" });

        setup.about = "Adds a button that a color map can be bound to.";
        setup.category = { "Widgets", "Colors" };
        break;
    }
    case mvAppItemType::mvColorMapSlider:              
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_WIDTH |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_INDENT |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_CALLBACK |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_DROP_CALLBACK |
            MV_PARSER_ARG_PAYLOAD_TYPE |
            MV_PARSER_ARG_TRACKED |
            MV_PARSER_ARG_POS)
        );

        args.push_back({ mvPyDataType::Callable, "drag_callback", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });
        args.push_back({ mvPyDataType::Float, "default_value", mvArgType::KEYWORD_ARG, "0.0" });

        setup.about = "Adds a color slider that a color map can be bound to.";
        setup.category = { "Widgets", "Colors" };
        break;
    }
    case mvAppItemType::mvTemplateRegistry:            
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID)
        );

        setup.about = "Adds a template registry.";
        setup.category = { "Containers", "Widgets", "Registries" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvTableCell:                   
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE |
            MV_PARSER_ARG_HEIGHT |
            MV_PARSER_ARG_FILTER |
            MV_PARSER_ARG_SHOW)
        );

        setup.about = "Adds a table.";
        setup.category = { "Tables", "Containers", "Widgets" };
        setup.createContextManager = true;
        break;
    }
    case mvAppItemType::mvItemHandlerRegistry:         
    {
        AddCommonArgs(args, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW)
        );

        setup.about = "Adds an item handler registry.";
        setup.category = { "Widgets", "Events", "Registries", "Containers" };
        setup.createContextManager = true;
        break;
    }

    case mvAppItemType::All:
    case mvAppItemType::None:
    case mvAppItemType::ItemTypeCount:
    default: break;
    }

    return FinalizeParser(setup, args);
}

void
DearPyGui::OnChildAdded(mvAppItem* item, std::shared_ptr<mvAppItem> child)
{
    switch (item->type)
    {

        case mvAppItemType::mvWindowAppItem:
        {
            mvWindowAppItem* actualItem = (mvWindowAppItem*)item;
            if (child->type == mvAppItemType::mvMenuBar)
                actualItem->configData.windowflags |= ImGuiWindowFlags_MenuBar;
            return;
        }

        case mvAppItemType::mvFontRegistry:
        {
            mvFontRegistry* actualItem = (mvFontRegistry*)item;
            actualItem->onChildAdd(child);
            return;
        }

        case mvAppItemType::mvPlot:
        {
            mvPlot* actualItem = (mvPlot*)item;
            if (child->type == mvAppItemType::mvPlotLegend)
                actualItem->configData._flags &= ~ImPlotFlags_NoLegend;

            if (child->type == mvAppItemType::mvPlotAxis)
            {
                actualItem->updateFlags();
                actualItem->updateAxesNames();
            }
            return;
        }

        case mvAppItemType::mvSubPlots:
        {
            mvSubPlots* actualItem = (mvSubPlots*)item;
            actualItem->onChildAdd(child);
            return;
        }

        case mvAppItemType::mvTable:
        {
            mvTable* actualItem = (mvTable*)item;
            actualItem->onChildAdd(child);
            return;
        }

        default: return;
    }
}
    
void
DearPyGui::OnChildRemoved(mvAppItem* item, std::shared_ptr<mvAppItem> child)
{
    switch (item->type)
    {

        case mvAppItemType::mvWindowAppItem:
        {
            mvWindowAppItem* actualItem = (mvWindowAppItem*)item;
            if (child->type == mvAppItemType::mvMenuBar)
                actualItem->configData.windowflags &= ~ImGuiWindowFlags_MenuBar;
            return;
        }

        case mvAppItemType::mvNodeEditor:
        {
            mvNodeEditor* actualItem = (mvNodeEditor*)item;
            actualItem->onChildRemoved(child);
            return;
        }

        case mvAppItemType::mvPlot:
        {
            mvPlot* actualItem = (mvPlot*)item;
            if (child->type == mvAppItemType::mvPlotLegend)
                actualItem->configData._flags |= ImPlotFlags_NoLegend;
            if (child->type == mvAppItemType::mvPlotAxis)
                actualItem->updateFlags();
            return;
        }

        case mvAppItemType::mvSubPlots:
        {
            mvSubPlots* actualItem = (mvSubPlots*)item;
            actualItem->onChildRemoved(child);
            return;
        }

        case mvAppItemType::mvTable:
        {
            mvTable* actualItem = (mvTable*)item;
            actualItem->onChildRemoved(child);
            return;
        }

        default:
            return;
    }
}
