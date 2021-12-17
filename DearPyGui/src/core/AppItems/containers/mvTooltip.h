#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvTooltip : public mvAppItem
    {
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTooltip, add_tooltip)

    public:

        explicit mvTooltip(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* dict) override;

    };

}
