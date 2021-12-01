#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvSeparator : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSeparator, add_separator)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN

    public:

        explicit mvSeparator(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
