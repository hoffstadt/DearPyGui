#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    struct mvButtonConfig
    {
        ImGuiDir direction    = ImGuiDir_Up;
        bool     small_button = false;
        bool     arrow        = false;
    };

    class mvButton : public mvAppItem
    {
    public:

        explicit mvButton(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        mvButtonConfig configData{};

    };

}
