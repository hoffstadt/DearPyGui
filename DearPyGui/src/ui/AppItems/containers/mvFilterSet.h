#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvFilterSet : public mvAppItem
    {

    public:

        explicit mvFilterSet(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

        // values
        void setPyValue(PyObject* value) override;
        PyObject* getPyValue() override;

    private:

        ImGuiTextFilter _imguiFilter;
    };

}
