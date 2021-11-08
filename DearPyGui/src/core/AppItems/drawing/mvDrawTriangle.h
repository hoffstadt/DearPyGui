#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvDrawTriangle, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 2);
    class mvDrawTriangle : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawTriangle, draw_triangle)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_CREATE_CONSTANT(mvCullMode_None, 0);
        MV_CREATE_CONSTANT(mvCullMode_Back, 1);
        MV_CREATE_CONSTANT(mvCullMode_Front, 2);

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvDrawlist),
            MV_ADD_PARENT(mvAppItemType::mvDrawLayer),
            MV_ADD_PARENT(mvAppItemType::mvDrawNode),
            MV_ADD_PARENT(mvAppItemType::mvWindowAppItem),
            MV_ADD_PARENT(mvAppItemType::mvPlot),
            MV_ADD_PARENT(mvAppItemType::mvViewportDrawlist)
        MV_END_PARENTS


        MV_START_CONSTANTS
            MV_ADD_CONSTANT(mvCullMode_None),
            MV_ADD_CONSTANT(mvCullMode_Back),
            MV_ADD_CONSTANT(mvCullMode_Front)
        MV_END_CONSTANTS

    public:

        explicit mvDrawTriangle(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        mvVec4  _p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
        mvVec4  _p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
        mvVec4  _p3 = { 0.0f, 0.0f, 0.0f, 1.0f };
        mvColor _color;
        mvColor _fill;
        float   _thickness = 1.0f;

    };

}
