#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "mvModule_DearPyGui.h"
#include "cpp.hint"

namespace Marvel {

    class mvColorMapRegistry : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorMapRegistry, add_colormap_registry)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvColorMap)
        MV_END_CHILDREN

    public:

        explicit mvColorMapRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
