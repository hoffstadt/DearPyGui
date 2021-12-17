#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "mvModule_DearPyGui.h"
#include "cpp.hint"

namespace Marvel {

    class mvValueRegistry : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvValueRegistry, add_value_registry)

    public:

        explicit mvValueRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}

    };

}
