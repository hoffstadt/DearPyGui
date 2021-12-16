#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvMouseWheelHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMouseWheelHandler, add_mouse_wheel_handler)
        MV_DEFAULT_CHILDREN

    public:

        explicit mvMouseWheelHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
