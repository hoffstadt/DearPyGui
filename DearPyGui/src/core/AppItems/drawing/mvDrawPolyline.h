#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawPolyline : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawPolyline, draw_polyline)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvDrawlist),
            MV_ADD_PARENT(mvAppItemType::mvDrawLayer),
            MV_ADD_PARENT(mvAppItemType::mvWindowAppItem),
            MV_ADD_PARENT(mvAppItemType::mvPlot),
            MV_ADD_PARENT(mvAppItemType::mvDrawNode),
            MV_ADD_PARENT(mvAppItemType::mvViewportDrawlist)
        MV_END_PARENTS

    public:

        explicit mvDrawPolyline(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;


    private:

        std::vector<mvVec4> _points;
        mvColor             _color;
        bool                _closed = false;
        float               _thickness = 1.0f;

    };

}
