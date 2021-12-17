#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvViewportDrawlist : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvViewportDrawlist, add_viewport_drawlist)

    public:

        explicit mvViewportDrawlist(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        bool _front = true;

    };
}
