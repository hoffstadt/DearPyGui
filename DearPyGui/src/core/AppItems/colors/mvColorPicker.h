#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvColorPicker);
    class mvColorPicker : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorPicker, add_color_picker)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN

        MV_CREATE_CONSTANT(mvColorPicker_bar, ImGuiColorEditFlags_PickerHueBar);
        MV_CREATE_CONSTANT(mvColorPicker_wheel, ImGuiColorEditFlags_PickerHueWheel);

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
            MV_ADD_CONSTANT(mvColorPicker_bar),
            MV_ADD_CONSTANT(mvColorPicker_wheel)
        MV_END_CONSTANTS

    public:

        explicit mvColorPicker(mvUUID uuid);

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

        mvRef<std::array<float, 4>> _value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
        float                       _disabled_value[4]{};
        ImGuiColorEditFlags         _flags = ImGuiColorEditFlags__OptionsDefault;
        bool                        _no_inputs = false;
        bool                        _no_label = false;
        bool                        _no_side_preview = false;
        bool                        _alpha_bar = false;

    };

}
