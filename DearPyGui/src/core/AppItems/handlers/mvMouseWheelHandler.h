#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvMouseWheelHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMouseWheelHandler, add_mouse_wheel_handler)
        MV_DEFAULT_CHILDREN

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvHandlerRegistry)
        MV_END_PARENTS

    public:

        explicit mvMouseWheelHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
