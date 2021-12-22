#include "mvSpacer.h"
#include "mvItemRegistry.h"

namespace Marvel {

    mvSpacer::mvSpacer(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvSpacer::draw(ImDrawList* drawlist, float x, float y)
    {
        if(config.width == 0 && config.height == 0)
            ImGui::Spacing();
        else
            ImGui::Dummy({ (float)config.width, (float)config.height });
    }

}
