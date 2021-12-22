#pragma once

#include "mvItemRegistry.h"
#include "mvThemeComponent.h"

namespace Marvel {

    void apply_local_theming  (mvAppItem* item);
    void cleanup_local_theming(mvAppItem* item);

    class mvTheme : public mvAppItem
    {

    public:

        explicit mvTheme(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override { assert(false); }
        void setSpecificType(int specificType) { _specificType = specificType; }
        void setSpecificEnabled(int enabled) { _specificEnabled = enabled; }

        void push_theme_components();
        void pop_theme_components();

    public:

        int _specificType = (int)mvAppItemType::All;
        bool _specificEnabled = true;

    };

}
