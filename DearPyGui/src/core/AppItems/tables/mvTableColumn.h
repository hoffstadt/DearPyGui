#pragma once

#include "mvItemRegistry.h"
#include <vector>

namespace Marvel {

    class mvTableColumn : public mvAppItem
    {
        
        // for access to ID
        friend class mvTable;

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTableColumn, add_table_column)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvTable)
        MV_END_PARENTS

    public:

        explicit mvTableColumn(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        ImGuiTableColumnFlags _flags = 0;
        float _init_width_or_weight = 0.0f;
        ImGuiID _id = 0u;

    };

}
