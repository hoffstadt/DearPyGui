#pragma once

#include "mvCore.h"
#include "mvTextures.h"

namespace Marvel {

    // forward declarations
    struct mvGraphics;
    struct mvGraphicsSpec;
    struct mvDevice;
    struct mvViewport;
    enum class mvGraphicsBackend;

    mvGraphics setup_graphics(mvViewport& viewport, mvGraphicsSpec spec);
    void       resize_swapchain(mvGraphics& graphics, int width, int height);
    void       cleanup_graphics(mvGraphics& graphics);
    void       present(mvGraphics& graphics, mvColor& clearColor, bool vsync);

    enum class mvGraphicsBackend
    {
        NONE,
        D3D11,
        D3D12,
        VULKAN,
        METAL,
        OPENGL
    };

    struct mvGraphicsSpec
    {
        mvGraphicsBackend backend = mvGraphicsBackend::NONE;
    };

    struct mvGraphics
    {
        bool           ok = false;
        mvGraphicsSpec spec;
        void* backendSpecifics = nullptr;
    };
}