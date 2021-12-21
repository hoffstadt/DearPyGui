#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "textures/mvStaticTexture.h"
#include "textures/mvDynamicTexture.h"
#include "textures/mvRawTexture.h"

namespace Marvel {

    class mvDrawImageQuad : public mvAppItem
    {

    public:

        explicit mvDrawImageQuad(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;


    private:


        mvUUID      _textureUUID = 0;
        mvVec4      _p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
        mvVec4      _p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
        mvVec4      _p3 = { 0.0f, 0.0f, 0.0f, 1.0f };
        mvVec4      _p4 = { 0.0f, 0.0f, 0.0f, 1.0f };
        mvVec2      _uv1= {0.0f, 0.0f};
        mvVec2      _uv2 = {1.0f, 0.0f};
        mvVec2      _uv3 = {1.0f, 1.0f};
        mvVec2      _uv4 = {0.0f, 1.0f};
        mvColor     _color = mvColor(255, 255, 255, 255);

        // pointer to existing item or internal
        std::shared_ptr<mvAppItem> _texture = nullptr;
        bool _internalTexture = false; // create a local texture if necessary

    };

}
