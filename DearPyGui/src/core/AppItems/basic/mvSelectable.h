#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvSelectable : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSelectable, add_selectable)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

            MV_SET_STATES(
                MV_STATE_HOVER |
                MV_STATE_ACTIVE |
                MV_STATE_FOCUSED |
                MV_STATE_CLICKED |
                MV_STATE_VISIBLE |
                MV_STATE_EDITED |
                MV_STATE_ACTIVATED |
                MV_STATE_DEACTIVATED |
                MV_STATE_DEACTIVATEDAE |
                MV_STATE_TOGGLED_OPEN |
                MV_STATE_RECT_MIN |
                MV_STATE_RECT_MAX |
                MV_STATE_RECT_SIZE |
                MV_STATE_CONT_AVAIL
            );

    public:

        explicit mvSelectable(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<bool>          _value = CreateRef<bool>(false);
        bool                 _disabled_value = false;
        ImGuiSelectableFlags _flags = ImGuiSelectableFlags_None;
    };

}
