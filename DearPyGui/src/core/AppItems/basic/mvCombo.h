#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvCombo);
    class mvCombo : public mvAppItem
    {

        enum class ComboHeightMode {
            mvComboHeight_Small = 0L,
            mvComboHeight_Regular,
            mvComboHeight_Large,
            mvComboHeight_Largest
        };

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCombo, add_combo)

        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN

        MV_CREATE_CONSTANT(mvComboHeight_Small, 0L);
        MV_CREATE_CONSTANT(mvComboHeight_Regular, 1L);
        MV_CREATE_CONSTANT(mvComboHeight_Large, 2L);
        MV_CREATE_CONSTANT(mvComboHeight_Largest, 3L);

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
            MV_STATE_RECT_MIN |
            MV_STATE_RECT_MAX |
            MV_STATE_RECT_SIZE |
            MV_STATE_CONT_AVAIL
        );

        MV_START_CONSTANTS
            MV_ADD_CONSTANT(mvComboHeight_Small),
            MV_ADD_CONSTANT(mvComboHeight_Regular),
            MV_ADD_CONSTANT(mvComboHeight_Large),
            MV_ADD_CONSTANT(mvComboHeight_Largest)
        MV_END_CONSTANTS

    public:

        explicit mvCombo(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificPositionalArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<std::string>       _value = CreateRef<std::string>("");
        std::string              _disabled_value = "";
        ImGuiComboFlags          _flags = ImGuiComboFlags_None;
        std::vector<std::string> _items;
        bool                     _popup_align_left = false;
        bool                     _no_preview = false;
    };

}
