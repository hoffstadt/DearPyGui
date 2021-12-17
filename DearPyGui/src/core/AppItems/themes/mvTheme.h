#pragma once

#include "mvItemRegistry.h"
#include "mvThemeComponent.h"

namespace Marvel {

    class mvTheme : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTheme, add_theme)

    public:

        explicit mvTheme(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void customAction(void* data = nullptr) override;
        void setSpecificType(int specificType) { _specificType = specificType; }
        void setSpecificEnabled(int enabled) { _specificEnabled = enabled; }

    public:

        int _specificType = (int)mvAppItemType::All;
        bool _specificEnabled = true;

    };
}
