#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvTabButton : public mvAppItem
    {

    public:

        explicit mvTabButton(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        ImGuiTabItemFlags _flags = ImGuiTabItemFlags_None;

    };
}
