#pragma once

#include <utility>
#include <array>
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "textures/mvStaticTexture.h"
#include "textures/mvDynamicTexture.h"
#include "textures/mvRawTexture.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvImage
//     * mvImageButton
//
//-----------------------------------------------------------------------------

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvImage
    //-----------------------------------------------------------------------------
    class mvImage : public mvAppItem
    {

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

    //-----------------------------------------------------------------------------
    // mvImageButton
    //-----------------------------------------------------------------------------
    class mvImageButton : public mvAppItem
    {

    public:

        explicit mvImageButton(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        // config
        mvUUID      _textureUUID = 0;
        mvVec2	    _uv_min = { 0.0f, 0.0f };
        mvVec2	    _uv_max = { 1.0f, 1.0f };
        mvColor     _tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        mvColor     _backgroundColor = { 0.0f, 0.0f, 0.0f, 0.0f };
        int         _framePadding = -1;

        // pointer to existing item or internal
        std::shared_ptr<mvAppItem> _texture = nullptr;
        bool _internalTexture = false; // create a local texture if necessary

    };
}
