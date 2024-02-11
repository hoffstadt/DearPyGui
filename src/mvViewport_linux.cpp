#include "mvViewport.h"
#include "mvFontManager.h"
#include "mvLinuxSpecifics.h"
#include "implot.h"
#include "imgui.h"
#include "imnodes.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <stb_image.h>
#include "mvToolManager.h"

static void
glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void
window_close_callback(GLFWwindow* window)
{
    if (GContext->viewport->disableClose) {
        GContext->callbackRegistry->exitCallbackPoint.run();
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
window_drop_callback(GLFWwindow* window, int count, const char* paths[])
{
    std::vector<std::string> filePaths;
    for (int i = 0; i < count; i++)
    {
        filePaths.emplace_back(paths[i]);
    }
    GContext->callbackRegistry->dropCallback(filePaths);
}

static void
mvPrerender()
{
    mvViewport* viewport = GContext->viewport;
    auto viewportData = (mvViewportData*)viewport->platformSpecifics;

    viewport->running = !glfwWindowShouldClose(viewportData->handle);

    if (viewport->posDirty)
    {
        glfwSetWindowPos(viewportData->handle, viewport->xpos, viewport->ypos);
        viewport->posDirty = false;
    }

    if (viewport->sizeDirty)
    {
        glfwSetWindowSizeLimits(viewportData->handle, (int)viewport->minwidth, (int)viewport->minheight, (int)viewport->maxwidth, (int)viewport->maxheight);
        glfwSetWindowSize(viewportData->handle, viewport->actualWidth, viewport->actualHeight);
        viewport->sizeDirty = false;
    }

    if (viewport->modesDirty)
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

    if (glfwGetWindowAttrib(viewportData->handle, GLFW_ICONIFIED))
    {
        glfwWaitEvents();
        return;
    }

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
        ImGui_ImplOpenGL3_DestroyDeviceObjects();
        mvToolManager::GetFontManager().updateAtlas();
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

 mvViewport*
mvCreateViewport(unsigned width, unsigned height)
{
    mvViewport* viewport = new mvViewport();
    viewport->width = width;
    viewport->height = height;
    viewport->platformSpecifics = new mvViewportData();
    return viewport;
}

 void
mvCleanupViewport(mvViewport& viewport)
{
    auto viewportData = (mvViewportData*)viewport.platformSpecifics;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwDestroyWindow(viewportData->handle);
    glfwTerminate();
    GContext->started = false;

    delete viewportData;
    viewportData = nullptr;
}

 void
mvShowViewport(mvViewport& viewport, bool minimized, bool maximized)
{
    auto viewportData = (mvViewportData*)viewport.platformSpecifics;

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    if (!viewport.resizable)
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    if (viewport.alwaysOnTop)
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    if (maximized)
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    else if (minimized)
        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
    if (!viewport.decorated)
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    // Create window with graphics context
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    viewportData->handle = glfwCreateWindow(viewport.actualWidth, viewport.actualHeight, viewport.title.c_str(), nullptr, nullptr);
    glfwSetWindowPos(viewportData->handle, viewport.xpos, viewport.ypos);
    glfwSetWindowSizeLimits(viewportData->handle, (int)viewport.minwidth, (int)viewport.minheight, (int)viewport.maxwidth, (int)viewport.maxheight);

    viewport.clientHeight = viewport.actualHeight;
    viewport.clientWidth = viewport.actualWidth;

    std::vector<GLFWimage> images;

    if (!viewport.small_icon.empty())
    {
        int image_width, image_height;
        unsigned char* image_data = stbi_load(viewport.small_icon.c_str(), &image_width, &image_height, nullptr, 4);
        if (image_data)
        {
            images.push_back({ image_width, image_height, image_data });
        }
    }

    if (!viewport.large_icon.empty())
    {
        int image_width, image_height;
        unsigned char* image_data = stbi_load(viewport.large_icon.c_str(), &image_width, &image_height, nullptr, 4);
        if (image_data)
        {
            images.push_back({ image_width, image_height, image_data });
        }
    }

    if (!images.empty())
        glfwSetWindowIcon(viewportData->handle, images.size(), images.data());

    glfwMakeContextCurrent(viewportData->handle);

    gl3wInit();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    if (GContext->IO.loadIniFile)
    {
        ImGui::LoadIniSettingsFromDisk(GContext->IO.iniFile.c_str());
        io.IniFilename = nullptr;
        if (GContext->IO.autoSaveIniFile)
            io.IniFilename = GContext->IO.iniFile.c_str();
    }
    else
    {
        if (GContext->IO.iniFile.empty())
            io.IniFilename = nullptr;
        else
            io.IniFilename = GContext->IO.iniFile.c_str();
    }

    if(GContext->IO.kbdNavigation)
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    if (GContext->IO.docking)
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup style
    ImGui::StyleColorsDark();
    SetDefaultTheme();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(viewportData->handle, true);
        

    // Setup callbacks
    glfwSetWindowSizeCallback(viewportData->handle, window_size_callback);
    glfwSetWindowCloseCallback(viewportData->handle, window_close_callback);
    glfwSetDropCallback(viewportData->handle, window_drop_callback);
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
    mvPrerender();

    if (GImGui->CurrentWindow == nullptr)
        return;

    Render();

    present(GContext->graphics, GContext->viewport->clearColor, GContext->viewport->vsync);
}

 void
mvToggleFullScreen(mvViewport& viewport)
{
    static size_t storedWidth = 0;
    static size_t storedHeight = 0;
    static int    storedXPos = 0;
    static int    storedYPos = 0;

    auto viewportData = (mvViewportData*)viewport.platformSpecifics;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    int framerate = -1;
    if (viewport.vsync)
    {
        framerate = mode->refreshRate;
    }

    if (viewport.fullScreen)
    {
        glfwSetWindowMonitor(viewportData->handle, nullptr, storedXPos, storedYPos, storedWidth, storedHeight, framerate);
        viewport.fullScreen = false;
    }
    else
    {
        storedWidth = (size_t)viewport.actualWidth;
        storedHeight = (size_t)viewport.actualHeight;
        storedXPos = viewport.xpos;
        storedYPos = viewport.ypos;
        glfwSetWindowMonitor(viewportData->handle, monitor, 0, 0, mode->width, mode->height, framerate);
        viewport.fullScreen = true;
    }
}