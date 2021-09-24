/***************************************************************************//*/
Copyright (c) 2021 Dear PyGui, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

/******************************************************************************/

#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvDragPayload, MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_HANDLER, StorageValueTypes::None, 3);
    class mvDragPayload : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragPayload, add_drag_payload)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_SET_STATES(MV_STATE_NONE);

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
