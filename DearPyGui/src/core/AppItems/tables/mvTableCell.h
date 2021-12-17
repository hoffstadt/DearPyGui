#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>

namespace Marvel {

    class mvTableCell : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTableCell, add_table_cell)

    public:

        explicit mvTableCell(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
