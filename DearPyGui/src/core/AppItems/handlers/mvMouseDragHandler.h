#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvMouseDragHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMouseDragHandler, add_mouse_drag_handler)

    public:

        explicit mvMouseDragHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificPositionalArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        int   _button = -1;
        float _threshold = 10.0f;

    };

}
