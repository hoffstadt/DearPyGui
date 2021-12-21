#pragma once

#include "mvItemRegistry.h"
#include <vector>

namespace Marvel {

    class mvTableColumn : public mvAppItem
    {
        
    public:

        explicit mvTableColumn(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    public:

        ImGuiTableColumnFlags _flags = 0;
        float _init_width_or_weight = 0.0f;
        ImGuiID _id = 0u;

    };

}
