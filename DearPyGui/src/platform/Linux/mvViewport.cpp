#include "mvViewport.h"
#include "mvFontManager.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "implot.h"
#include "imgui.h"
#include "imnodes.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <stb_image.h>
#include "mvToolManager.h"

static GLFWwindow* ghandle = nullptr;

namespace Marvel {

    mv_internal void
    glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    mv_internal void
    window_close_callback(GLFWwindow* window)
    {
        GContext->started = false;
    }

    mv_internal void
    window_size_callback(GLFWwindow* window, int width, int height)
    {
        GContext->viewport->actualHeight = height;
        GContext->viewport->clientHeight = height;
        GContext->viewport->actualWidth = width;
        GContext->viewport->clientWidth = width;
        GContext->viewport->resized = true;
    }

    mv_internal void
    mvPrerender()
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

    mv_internal void
    mvPostrender()
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

    mv_impl mvViewport*
    mvCreateViewport(unsigned width, unsigned height)
    {
        mvViewport* viewport = new mvViewport();
        viewport->width = width;
        viewport->height = height;
        return viewport;
    }

    mv_impl void
    mvCleanupViewport()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        glfwDestroyWindow(ghandle);
        glfwTerminate();
        GContext->started = false;
    }

    mv_impl void
    mvShowViewport(bool minimized, bool maximized)
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

        GContext->viewport->clientHeight = viewport->actualHeight;
        GContext->viewport->clientWidth = viewport->actualWidth;

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

        glfwMakeContextCurrent(ghandle);

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
    
    mv_impl void
    mvMaximizeViewport()
    {
        glfwMaximizeWindow(ghandle);
    }

    mv_impl void
    mvMinimizeViewport()
    {
        glfwIconifyWindow(ghandle);
    }

    mv_impl void
    mvRestoreViewport()
    {
        glfwRestoreWindow(ghandle);
    }

    mv_impl void
    mvRenderFrame()
    {
        mvPrerender();

        if (GImGui->CurrentWindow == nullptr)
            return;

        Render();

        mvPostrender();
    }

    mv_impl void
    mvToggleFullScreen()
    {
        mv_local_persist size_t storedWidth = 0;
        mv_local_persist size_t storedHeight = 0;
        mv_local_persist int    storedXPos = 0;
        mv_local_persist int    storedYPos = 0;

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
