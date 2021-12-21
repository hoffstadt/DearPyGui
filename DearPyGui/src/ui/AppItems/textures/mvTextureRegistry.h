#pragma once

#include "mvItemRegistry.h"
#include "mvModule_DearPyGui.h"
#include "cpp.hint"

namespace Marvel {

    class mvTextureRegistry : public mvAppItem
    {

    public:

        explicit mvTextureRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void onChildRemoved(mvRef<mvAppItem> item) override;
        void onChildrenRemoved() override;

    private:

        void show_debugger();

    private:

        int _selection = -1;
    };

}
