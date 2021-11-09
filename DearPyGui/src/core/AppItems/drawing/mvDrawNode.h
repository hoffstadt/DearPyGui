#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvDrawNode, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 2);
    class mvDrawNode : public mvAppItem
    {
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawNode, add_draw_node)
        MV_NO_CONSTANTS

        MV_CREATE_COMMAND(apply_transform);
        
        MV_CREATE_COMMAND(create_rotation_matrix);
        MV_CREATE_COMMAND(create_translation_matrix);
        MV_CREATE_COMMAND(create_scale_matrix);
        MV_CREATE_COMMAND(create_lookat_matrix);
        MV_CREATE_COMMAND(create_perspective_matrix);
        MV_CREATE_COMMAND(create_orthographic_matrix);
        MV_CREATE_COMMAND(create_fps_matrix);

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvDrawlist),
            MV_ADD_PARENT(mvAppItemType::mvDrawLayer),
            MV_ADD_PARENT(mvAppItemType::mvWindowAppItem),
            MV_ADD_PARENT(mvAppItemType::mvPlot),
            MV_ADD_PARENT(mvAppItemType::mvViewportDrawlist),
            MV_ADD_PARENT(mvAppItemType::mvDrawNode),
        MV_END_PARENTS

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvDrawLine),
            MV_ADD_CHILD(mvAppItemType::mvDrawArrow),
            MV_ADD_CHILD(mvAppItemType::mvDrawTriangle),
            MV_ADD_CHILD(mvAppItemType::mvDrawCircle),
            MV_ADD_CHILD(mvAppItemType::mvDrawEllipse),
            MV_ADD_CHILD(mvAppItemType::mvDrawBezierCubic),
            MV_ADD_CHILD(mvAppItemType::mvDrawBezierQuadratic),
            MV_ADD_CHILD(mvAppItemType::mvDrawQuad),
            MV_ADD_CHILD(mvAppItemType::mvDrawRect),
            MV_ADD_CHILD(mvAppItemType::mvDrawText),
            MV_ADD_CHILD(mvAppItemType::mvDrawPolygon),
            MV_ADD_CHILD(mvAppItemType::mvDrawPolyline),
            MV_ADD_CHILD(mvAppItemType::mvDrawImage),
            MV_ADD_CHILD(mvAppItemType::mvDrawNode),
            MV_ADD_CHILD(mvAppItemType::mvDrawImageQuad),
        MV_END_CHILDREN

        MV_START_COMMANDS
            MV_ADD_COMMAND(apply_transform);
            MV_ADD_COMMAND(create_rotation_matrix);
            MV_ADD_COMMAND(create_translation_matrix);
            MV_ADD_COMMAND(create_scale_matrix);
            MV_ADD_COMMAND(create_lookat_matrix);
            MV_ADD_COMMAND(create_perspective_matrix);
            MV_ADD_COMMAND(create_orthographic_matrix);
            MV_ADD_COMMAND(create_fps_matrix);
        MV_END_COMMANDS

    public:

        explicit mvDrawNode(mvUUID uuid);
        ~mvDrawNode();

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
    };

}
