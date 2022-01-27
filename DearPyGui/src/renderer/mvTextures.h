#pragma once

namespace Marvel {

    // forward declarations
    struct mvGraphics;
    struct mvDevice;
    struct mvTexture;
    struct mvTextureSpec;

    //mvTexture load_texture(mvGraphics& graphics, mvTextureSpec& spec);

    enum class mvTextureFormat
    {
        RGBA32F,
        RGB32F,
        RGBA8,
        RGB8,
    };

    enum class mvTextureUsage
    {
        STATIC,  // typically, GPU access only
        DYNAMIC, // typically, CPU/GPU access
        RAW,     // typically, same as dynamic but no python type checks
    };

    struct mvTextureSpec
    {
        mvTextureFormat format = mvTextureFormat::RGBA32F;
        mvTextureUsage usage = mvTextureUsage::STATIC;
    };

    struct mvTexture
    {
        mvTextureSpec spec;
        void* resouce = nullptr;
    };

}