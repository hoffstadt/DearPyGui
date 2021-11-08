#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "textures/mvStaticTexture.h"
#include "textures/mvDynamicTexture.h"
#include "textures/mvRawTexture.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvDrawImage, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 2);
    class mvDrawImage : public mvAppItem
    {
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawImage, draw_image)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_SET_STATES(MV_STATE_NONE);

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

        explicit mvDrawImage(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;


    private:


        mvUUID      _textureUUID = 0;
        mvVec4      _pmax = {0.0f, 0.0f, 0.0f, 1.0f};
        mvVec4      _pmin = {0.0f, 0.0f, 0.0f, 1.0f};
        mvVec2      _uv_min = {0.0f, 0.0f};
        mvVec2      _uv_max = {1.0f, 1.0f};
        mvColor     _color = mvImGuiCol_Text;

        // pointer to existing item or internal
        std::shared_ptr<mvAppItem> _texture = nullptr;
        bool _internalTexture = false; // create a local texture if necessary

    };

}
