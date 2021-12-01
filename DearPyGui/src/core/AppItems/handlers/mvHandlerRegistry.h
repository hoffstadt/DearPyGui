#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "mvModule_DearPyGui.h"
#include "cpp.hint"

namespace Marvel {

    class mvHandlerRegistry : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvHandlerRegistry, add_handler_registry)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvKeyDownHandler),
            MV_ADD_CHILD(mvAppItemType::mvKeyPressHandler),
            MV_ADD_CHILD(mvAppItemType::mvKeyReleaseHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseMoveHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseWheelHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseClickHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseDoubleClickHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseDownHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseReleaseHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseDragHandler)
        MV_END_CHILDREN

    public:

        explicit mvHandlerRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
