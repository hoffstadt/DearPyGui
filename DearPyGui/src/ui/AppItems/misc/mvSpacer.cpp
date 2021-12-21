#include "mvSpacer.h"
#include "mvItemRegistry.h"

namespace Marvel {

    mvSpacer::mvSpacer(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvSpacer::draw(ImDrawList* drawlist, float x, float y)
    {
        if(_width == 0 && _height == 0)
            ImGui::Spacing();
        else
            ImGui::Dummy({ (float)_width, (float)_height });
    }

}
