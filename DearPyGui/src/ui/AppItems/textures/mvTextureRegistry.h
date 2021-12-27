#pragma once

#include "mvItemRegistry.h"
#include "dearpygui.h"
#include "cpp.hint"

namespace Marvel {

    class mvTextureRegistry : public mvAppItem
    {

    public:

        explicit mvTextureRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void show_debugger();

    private:

        int _selection = -1;
    };

}
