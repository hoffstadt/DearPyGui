#include "mvViewport.h"
#include <implot.h>
#include "imnodes.h"
#include "mvToolManager.h"
#include "mvFontManager.h"
#include "mvAppleSpecifics.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"
#include <stdio.h>

 mvViewport*
mvCreateViewport(unsigned width, unsigned height)
{
    auto viewport = new mvViewport();
    viewport->width = width;
    viewport->height = height;
    viewport->platformSpecifics = new mvViewportData();
    return viewport;
}

static void
window_close_callback(GLFWwindow* window)
{
    if (GContext->viewport->disableClose) {
        mvSubmitCallback([=]() {
            mvRunCallback(GContext->callbackRegistry->onCloseCallback, 0, nullptr, GContext->callbackRegistry->onCloseCallbackUserData);
            });
    }
    else {
        GContext->started = false;
    }
}

static void
window_size_callback(GLFWwindow* window, int width, int height)
{
    GContext->viewport->actualHeight = height;
    GContext->viewport->clientHeight = height;
    GContext->viewport->actualWidth = width;
    GContext->viewport->clientWidth = width;
    GContext->viewport->resized = true;
}

static void
glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

 void
mvCleanupViewport(mvViewport& viewport)
{
    auto viewportData = (mvViewportData*)viewport.platformSpecifics;

    // Cleanup
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwDestroyWindow(viewportData->handle);
    glfwTerminate();

    delete viewportData;
    viewport.platformSpecifics = nullptr;
}

 void
mvShowViewport(mvViewport& viewport, bool minimized, bool maximized)
{
    auto viewportData = (mvViewportData*)viewport.platformSpecifics;

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
    glfwInit();

    if (!viewport.resizable)
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    if(viewport.alwaysOnTop)
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    if(maximized)
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    else if(minimized)
        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
    if(!viewport.decorated)
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_FALSE);

    // Create window with graphics context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
    viewportData->handle = glfwCreateWindow((int)viewport.actualWidth, (int)viewport.actualHeight, viewport.title.c_str(), nullptr, nullptr);
    glfwSetWindowPos(viewportData->handle, viewport.xpos, viewport.ypos);
    glfwSetWindowSizeLimits(viewportData->handle, (int)viewport.minwidth, (int)viewport.minheight, (int)viewport.maxwidth, (int)viewport.maxheight);

    GContext->viewport->clientHeight = viewport.actualHeight;
    GContext->viewport->clientWidth = viewport.actualWidth;

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;

	if (GContext->IO.loadIniFile)
	{
		ImGui::LoadIniSettingsFromDisk(GContext->IO.iniFile.c_str());
		io.IniFilename = nullptr;
		if(GContext->IO.autoSaveIniFile)
			io.IniFilename = GContext->IO.iniFile.c_str();
	}
	else
	{
		if (GContext->IO.iniFile.empty())
			io.IniFilename = nullptr;
		else
			io.IniFilename = GContext->IO.iniFile.c_str();
	}

    (void) io;

    if(GContext->IO.kbdNavigation)
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup style
    ImGui::StyleColorsDark();
    SetDefaultTheme();

    ImGui_ImplGlfw_InitForOpenGL(viewportData->handle, true);

    // Setup callbacks
    glfwSetWindowSizeCallback(viewportData->handle, window_size_callback);
    //glfwSetFramebufferSizeCallback(m_window, window_size_callback);
    glfwSetWindowCloseCallback(viewportData->handle, window_close_callback);
}
  
 void
mvMaximizeViewport(mvViewport& viewport)
{
    auto viewportData = (mvViewportData*)viewport.platformSpecifics;
    glfwMaximizeWindow(viewportData->handle);
}

 void
mvMinimizeViewport(mvViewport& viewport)
{
    auto viewportData = (mvViewportData*)viewport.platformSpecifics;
    glfwIconifyWindow(viewportData->handle);
}

 void
mvRestoreViewport(mvViewport& viewport)
{
    auto viewportData = (mvViewportData*)viewport.platformSpecifics;
    glfwRestoreWindow(viewportData->handle);
}

 void
mvRenderFrame()
{
    mvViewport* viewport = GContext->viewport;
    auto viewportData = (mvViewportData*)viewport->platformSpecifics;

    mvGraphics& graphics = GContext->graphics;
    auto graphicsData = (mvGraphics_Metal*)graphics.backendSpecifics;

    viewport->running = !glfwWindowShouldClose(viewportData->handle);

    if(viewport->posDirty)
    {
        glfwSetWindowPos(viewportData->handle, viewport->xpos, viewport->ypos);
        viewport->posDirty = false;
    }

    if(viewport->sizeDirty)
    {
        glfwSetWindowSizeLimits(viewportData->handle, (int)viewport->minwidth, (int)viewport->minheight, (int)viewport->maxwidth, (int)viewport->maxheight);
        glfwSetWindowSize(viewportData->handle, viewport->actualWidth, viewport->actualHeight);
        viewport->sizeDirty = false;
    }

    if(viewport->modesDirty)
    {
        glfwSetWindowAttrib(viewportData->handle, GLFW_RESIZABLE, viewport->resizable ? GLFW_TRUE : GLFW_FALSE);
        glfwSetWindowAttrib(viewportData->handle, GLFW_DECORATED, viewport->decorated ? GLFW_TRUE : GLFW_FALSE);
        glfwSetWindowAttrib(viewportData->handle, GLFW_FLOATING, viewport->alwaysOnTop ? GLFW_TRUE : GLFW_FALSE);
        viewport->modesDirty = false;
    }

    if (viewport->titleDirty)
    {
        glfwSetWindowTitle(viewportData->handle, viewport->title.c_str());
        viewport->titleDirty = false;
    }

    if(glfwGetWindowAttrib(viewportData->handle, GLFW_ICONIFIED))
    {
        glfwWaitEvents();
        return;
    }

    @autoreleasepool {

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        if (GContext->IO.waitForInput)
            glfwWaitEvents();
        else
            glfwPollEvents();

        if (mvToolManager::GetFontManager().isInvalid())
        {
            mvToolManager::GetFontManager().rebuildAtlas();
            ImGui_ImplMetal_DestroyFontsTexture();
            mvToolManager::GetFontManager().updateAtlas();
            ImGui_ImplMetal_CreateFontsTexture(graphicsData->device);
        }

        NSWindow *nswin = glfwGetCocoaWindow(viewportData->handle);
        if(nswin.isVisible && (nswin.occlusionState & NSWindowOcclusionStateVisible) == 0)
            usleep(32000u);

        viewportData->layer.displaySyncEnabled = viewport->vsync;

        int width;
        int height;
        glfwGetFramebufferSize(viewportData->handle, &width, &height);
        viewportData->layer.drawableSize = CGSizeMake(width, height);
        id <CAMetalDrawable> drawable = [viewportData->layer nextDrawable];

        viewport->width = (unsigned)width;
        viewport->height = (unsigned)height;

        id <MTLCommandBuffer> commandBuffer = [graphicsData->commandQueue commandBuffer];
        graphicsData->renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(viewport->clearColor.r,
                                                                                    viewport->clearColor.g,
                                                                                    viewport->clearColor.b,
                                                                                    viewport->clearColor.a);
        graphicsData->renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
        graphicsData->renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
        graphicsData->renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:graphicsData->renderPassDescriptor];
        [renderEncoder pushDebugGroup:@"ImGui demo"];



        // Start the Dear ImGui frame
        ImGui_ImplMetal_NewFrame(graphicsData->renderPassDescriptor);
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Render();

        glfwGetWindowPos(viewportData->handle, &viewport->xpos, &viewport->ypos);

        // Rendering
        ImGui::Render();
        ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, renderEncoder);

        [renderEncoder popDebugGroup];
        [renderEncoder endEncoding];

        [commandBuffer presentDrawable:drawable];
        [commandBuffer commit];
    }
}

 void
mvToggleFullScreen(mvViewport& viewport)
{
    auto viewportData = (mvViewportData*)viewport.platformSpecifics;

    static size_t storedWidth = 0;
    static size_t storedHeight = 0;
    static int    storedXPos = 0;
    static int    storedYPos = 0;

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    int framerate = -1;
    if (GContext->viewport->vsync)
    {
            framerate = mode->refreshRate;
    }

    if(GContext->viewport->fullScreen)
    {
		glfwSetWindowMonitor(viewportData->handle, nullptr, storedXPos, storedYPos, storedWidth, storedHeight, framerate);
        GContext->viewport->fullScreen = false;
    }
    else
    {
        storedWidth = (size_t)GContext->viewport->actualWidth;
        storedHeight = (size_t)GContext->viewport->actualHeight;
        storedXPos = GContext->viewport->xpos;
        storedYPos = GContext->viewport->ypos;
        glfwSetWindowMonitor(viewportData->handle, monitor, 0, 0, mode->width, mode->height, framerate);
        GContext->viewport->fullScreen = true;
    }
}