#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawLayer : public mvAppItem
    {
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawLayer, add_draw_layer)

    public:

        explicit mvDrawLayer(mvUUID uuid);
        ~mvDrawLayer();

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;

    };

}
