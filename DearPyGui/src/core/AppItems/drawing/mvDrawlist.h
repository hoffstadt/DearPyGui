#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawlist : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawlist, add_drawlist)

    public:

        explicit mvDrawlist(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        float  _startx = 0.0f;
        float  _starty = 0.0f;

    };
}
