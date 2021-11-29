#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvColorMapSlider);
    class mvColorMapSlider : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorMapSlider, add_colormap_slider)
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
            MV_STATE_RECT_MIN |
            MV_STATE_RECT_MAX |
            MV_STATE_RECT_SIZE |
            MV_STATE_CONT_AVAIL
        );

    public:

        explicit mvColorMapSlider(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void applySpecificTemplate(mvAppItem* item) override;
        void setColorMap(ImPlotColormap colormap) { _colormap = colormap; }

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<float>    _value = CreateRef<float>(0.0f);
        ImVec4          _color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        ImPlotColormap  _colormap = 0;

    };

}
