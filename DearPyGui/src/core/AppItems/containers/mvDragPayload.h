#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvDragPayload : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragPayload, add_drag_payload)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

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

    public:

        explicit mvDragPayload(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;
        PyObject* getDragData() const { return _dragData; }
        PyObject* getDropData() const { return _dropData; }

    private:

        std::string _payloadType = "$$DPG_PAYLOAD";
        PyObject*   _dragData = nullptr;
        PyObject*   _dropData = nullptr;

    };

}
