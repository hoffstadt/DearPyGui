#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvMouseMoveHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMouseMoveHandler, add_mouse_move_handler)

    public:

        explicit mvMouseMoveHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    private:

        ImVec2 _oldPos = {};

    };

}
