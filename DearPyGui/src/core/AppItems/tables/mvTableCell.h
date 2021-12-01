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
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvTableRow)
        MV_END_PARENTS

    public:

        explicit mvTableCell(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
