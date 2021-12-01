#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvFontRegistry : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFontRegistry, add_font_registry)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvFont)
        MV_END_CHILDREN

    public:

        explicit mvFontRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void customAction(void* data = nullptr) override;
        void onChildAdd(mvRef<mvAppItem> item) override { _show = true; }

        bool isInvalid() const { return _dirty; }
        void resetFont();

    private:

        bool _dirty = true;

    };

}
