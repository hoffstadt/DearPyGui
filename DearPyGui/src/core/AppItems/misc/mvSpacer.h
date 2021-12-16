#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvSpacer : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSpacer, add_spacer)
        MV_DEFAULT_CHILDREN

    public:

        explicit mvSpacer(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
