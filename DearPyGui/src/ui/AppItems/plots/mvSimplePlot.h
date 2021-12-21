#pragma once

#include <utility>
#include "mvItemRegistry.h"

namespace Marvel{

    class mvSimplePlot : public mvAppItem
    {

    public:

        explicit mvSimplePlot(mvUUID uuid);

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

        mvRef<std::vector<float>> _value = CreateRef<std::vector<float>>(std::vector<float>{0.0f});
        std::string               _overlay;
        float                     _min = 0.0f;
        float                     _max = 0.0f;
        bool                      _histogram = false;
        bool                      _autosize = true;

    };

}
