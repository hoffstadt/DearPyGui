#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDragPoint : public mvAppItem
    {

    public:

        explicit mvDragPoint(mvUUID uuid);

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

        mvRef<std::array<double, 4>> _value = CreateRef<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
        double                       _disabled_value[4]{};
        bool                         _show_label = true;
        mvColor                      _color = mvColor(0.0f, 0.0f, 0.0f, -1.0f);
        float                        _radius = 4.0f;

    };

}
