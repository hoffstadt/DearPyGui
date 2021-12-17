#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawNode : public mvAppItem
    {
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawNode, add_draw_node)

    public:

        explicit mvDrawNode(mvUUID uuid);
        ~mvDrawNode();

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
    };

}
