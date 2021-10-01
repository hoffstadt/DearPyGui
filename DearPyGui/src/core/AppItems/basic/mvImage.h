#pragma once

#include <utility>
#include <array>
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "textures/mvStaticTexture.h"
#include "textures/mvDynamicTexture.h"
#include "textures/mvRawTexture.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvImage, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
    class mvImage : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvImage, add_image)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_SET_STATES(
            MV_STATE_HOVER |
            MV_STATE_CLICKED |
            MV_STATE_VISIBLE |
            MV_STATE_RECT_MIN |
            MV_STATE_RECT_MAX |
            MV_STATE_RECT_SIZE |
            MV_STATE_CONT_AVAIL
        );

    public:

        explicit mvImage(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void   setValue(mvUUID value);
        mvUUID get1Value() const;

    private:

        // config
        mvUUID  _textureUUID = 0;
        mvVec2  _uv_min = {0.0f, 0.0f};
        mvVec2  _uv_max = {1.0f, 1.0f};
        mvColor _tintColor = {1.0f, 1.0f, 1.0f, 1.0f};
        mvColor _borderColor = {0.0f, 0.0f, 0.0f, 0.0f};

        // pointer to existing item or internal
        std::shared_ptr<mvAppItem> _texture = nullptr;
        bool _internalTexture = false; // create a local texture if necessary

    };

}
