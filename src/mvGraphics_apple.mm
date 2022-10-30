#include "mvGraphics.h"
#include "mvAppleSpecifics.h"
#include "mvViewport.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"

mvGraphics
setup_graphics(mvViewport &viewport)
{
    mvGraphics graphics{};
    graphics.backendSpecifics = new mvGraphics_Metal();

    auto viewportData = (mvViewportData*)viewport.platformSpecifics;
    auto graphicsData = (mvGraphics_Metal*)graphics.backendSpecifics;

    graphicsData->device = MTLCreateSystemDefaultDevice();
    graphicsData->commandQueue = [graphicsData->device  newCommandQueue];

    ImGui_ImplMetal_Init(graphicsData->device);

    NSWindow *nswin = glfwGetCocoaWindow(viewportData->handle);
    viewportData->layer = [CAMetalLayer layer];
    viewportData->layer.device = graphicsData->device;
    viewportData->layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    nswin.contentView.layer = viewportData->layer;
    nswin.contentView.wantsLayer = YES;

    graphicsData->renderPassDescriptor = [MTLRenderPassDescriptor new];

    return graphics;
}

void
resize_swapchain(mvGraphics &graphics, int width, int height)
{

}

void
cleanup_graphics(mvGraphics &graphics)
{

}

void
present(mvGraphics &graphics, mvColor &clearColor, bool vsync)
{

}