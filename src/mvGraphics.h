#pragma once

#include "mvCore.h"
#include "mvTextures.h"

// forward declarations
struct mvGraphics;
struct mvViewport;

mvGraphics setup_graphics(mvViewport& viewport);
void       resize_swapchain(mvGraphics& graphics, int width, int height);
void       cleanup_graphics(mvGraphics& graphics);
void       present(mvGraphics& graphics, mvColor& clearColor, bool vsync);

struct mvGraphics
{
    bool           ok = false;
    void* backendSpecifics = nullptr;
};