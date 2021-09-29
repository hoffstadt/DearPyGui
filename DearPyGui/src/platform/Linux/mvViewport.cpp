#include "mvViewport.h"
#include "mvFontManager.h"
#include <GLFW/glfw3.h>
#include "implot.h"
#include "imgui.h"
#include "imnodes.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/gl3w.h>
#include <stdio.h>
#include <stb_image.h>
#include "mvToolManager.h"

static GLFWwindow* ghandle = nullptr;

namespace Marvel {

    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    static void window_close_callback(GLFWwindow* window)
    {
        GContext->started = false;
    }

    static void window_size_callback(GLFWwindow* window, int width, int height)
    {
        GContext->viewport->actualHeight = height;
        GContext->viewport->clientHeight = height;
        GContext->viewport->actualWidth = width;
        GContext->viewport->clientWidth = width;
        mvOnResize();
    }

    static void mvPrerender()
    {
        mvViewport* viewport = GContext->viewport;

        viewport->running = !glfwWindowShouldClose(ghandle);

        if (viewport->posDirty)
        {
            glfwSetWindowPos(ghandle, viewport->xpos, viewport->ypos);
            viewport->posDirty = false;
        }

        if (viewport->sizeDirty)
        {
            glfwSetWindowSizeLimits(ghandle, (int)viewport->minwidth, (int)viewport->minheight, (int)viewport->maxwidth, (int)viewport->maxheight);
            glfwSetWindowSize(ghandle, viewport->actualWidth, viewport->actualHeight);
            viewport->sizeDirty = false;
        }

        if (viewport->modesDirty)
        {
            glfwSetWindowAttrib(ghandle, GLFW_RESIZABLE, viewport->resizable ? GLFW_TRUE : GLFW_FALSE);
            glfwSetWindowAttrib(ghandle, GLFW_DECORATED, viewport->decorated ? GLFW_TRUE : GLFW_FALSE);
            glfwSetWindowAttrib(ghandle, GLFW_FLOATING, viewport->alwaysOnTop ? GLFW_TRUE : GLFW_FALSE);
            viewport->modesDirty = false;
        }

        if (viewport->titleDirty)
        {
            glfwSetWindowTitle(ghandle, viewport->title.c_str());
            viewport->titleDirty = false;
        }

        if (glfwGetWindowAttrib(ghandle, GLFW_ICONIFIED))
        {
            glfwWaitEvents();
            return;
        }

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
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

    static void mvPostrender()
    {
        mvViewport* viewport = GContext->viewport;

        glfwGetWindowPos(ghandle, &viewport->xpos, &viewport->ypos);

        glfwSwapInterval(viewport->vsync ? 1 : 0); // Enable vsync

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(ghandle, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);
        glClearColor(viewport->clearColor.r, viewport->clearColor.g, viewport->clearColor.b, viewport->clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(ghandle);
    }

    mvViewport* mvCreateViewport(unsigned width, unsigned height)
    {
        mvViewport* viewport = new mvViewport();
        viewport->width = width;
        viewport->height = height;
        return viewport;
    }

    void mvCleanupViewport()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        imnodes::DestroyContext();
        ImGui::DestroyContext();

        glfwDestroyWindow(ghandle);
        glfwTerminate();
        GContext->started = false;
    }

    void mvShowViewport(bool minimized, bool maximized)
    {
        mvViewport* viewport = GContext->viewport;

        // Setup window
        glfwSetErrorCallback(glfw_error_callback);
        glfwInit();

        if (!viewport->resizable)
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        if (viewport->alwaysOnTop)
            glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        if (maximized)
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        else if (minimized)
            glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
        if (!viewport->decorated)
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        // Create window with graphics context
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        ghandle = glfwCreateWindow(viewport->actualWidth, viewport->actualHeight, viewport->title.c_str(), nullptr, nullptr);
        glfwSetWindowPos(ghandle, viewport->xpos, viewport->ypos);
        glfwSetWindowSizeLimits(ghandle, (int)viewport->minwidth, (int)viewport->minheight, (int)viewport->maxwidth, (int)viewport->maxheight);

        std::vector<GLFWimage> images;

        if (!viewport->small_icon.empty())
        {
            int image_width, image_height;
            unsigned char* image_data = stbi_load(viewport->small_icon.c_str(), &image_width, &image_height, nullptr, 4);
            if (image_data)
            {
                images.push_back({ image_width, image_height, image_data });
            }
        }

        if (!viewport->large_icon.empty())
        {
            int image_width, image_height;
            unsigned char* image_data = stbi_load(viewport->large_icon.c_str(), &image_width, &image_height, nullptr, 4);
            if (image_data)
            {
                images.push_back({ image_width, image_height, image_data });
            }
        }

        if (!images.empty())
            glfwSetWindowIcon(ghandle, images.size(), images.data());

        mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
                CreateEventArgument("actual_width", (int)viewport->actualWidth),
                CreateEventArgument("actual_height", (int)viewport->actualHeight),
                CreateEventArgument("client_width", (int)viewport->actualWidth),
                CreateEventArgument("client_height", (int)viewport->actualHeight)
            });

        glfwMakeContextCurrent(ghandle);

        gl3wInit();

        // Setup Dear ImGui binding
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        imnodes::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigWindowsMoveFromTitleBarOnly = true;

        if (GContext->IO.loadIniFile)
        {
            ImGui::LoadIniSettingsFromDisk(GContext->IO.iniFile.c_str());
            io.IniFilename = nullptr;
        }
        else
        {
            if (GContext->IO.iniFile.empty())
                io.IniFilename = nullptr;
            else
                io.IniFilename = GContext->IO.iniFile.c_str();
        }

        if (GContext->IO.docking)
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Setup style
        ImGui::StyleColorsDark();
        SetDefaultTheme();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(ghandle, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Setup callbacks
        glfwSetWindowSizeCallback(ghandle, window_size_callback);
        glfwSetWindowCloseCallback(ghandle, window_close_callback);
    }
    
    void mvMaximizeViewport()
    {
        glfwMaximizeWindow(ghandle);
    }

    void mvMinimizeViewport()
    {
        glfwIconifyWindow(ghandle);
    }

    void mvRestoreViewport()
    {
        glfwRestoreWindow(ghandle);
    }

    void mvRenderFrame()
    {
        mvPrerender();

        if (GImGui->CurrentWindow == nullptr)
            return;

        Render();

        mvPostrender();
    }

    void mvToggleFullScreen()
    {
        static size_t storedWidth = 0;
        static size_t storedHeight = 0;
        static int    storedXPos = 0;
        static int    storedYPos = 0;

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        int framerate = -1;
        if (GContext->viewport->vsync)
        {
            framerate = mode->refreshRate;
        }

        if (GContext->viewport->fullScreen)
        {
            glfwSetWindowMonitor(ghandle, nullptr, storedXPos, storedYPos, storedWidth, storedHeight, framerate);
            GContext->viewport->fullScreen = false;
        }
        else
        {
            storedWidth = (size_t)GContext->viewport->actualWidth;
            storedHeight = (size_t)GContext->viewport->actualHeight;
            storedXPos = GContext->viewport->xpos;
            storedYPos = GContext->viewport->ypos;
            glfwSetWindowMonitor(ghandle, monitor, 0, 0, mode->width, mode->height, framerate);
            GContext->viewport->fullScreen = true;
        }
    }

}
