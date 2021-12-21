#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvButton : public mvAppItem
    {
    public:

        explicit mvButton(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        ImGuiDir _direction    = ImGuiDir_Up;
        bool     _small_button = false;
        bool     _arrow        = false;

    };

}
