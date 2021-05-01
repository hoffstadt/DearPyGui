#include "mvLinuxViewport.h"
#include "mvApp.h"
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

namespace Marvel {

    mvViewport* mvViewport::CreateViewport(unsigned int width, unsigned int height)
    {
        return new mvLinuxViewport(width, height);
    }

    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    static void window_close_callback(GLFWwindow* window)
    {
        mvApp::StopApp();
    }

    static void window_size_callback(GLFWwindow* window, int width, int height)
    {
        mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
            CreateEventArgument("actual_width", width),
            CreateEventArgument("actual_height", height),
            CreateEventArgument("client_width", width),
            CreateEventArgument("client_height", height)
                    });
    }

    mvLinuxViewport::mvLinuxViewport(unsigned width, unsigned height)
		: mvViewport(width, height)
	{
	}

    void mvLinuxViewport::show(bool minimized, bool maximized)
    {
        // Setup window
        glfwSetErrorCallback(glfw_error_callback);
        glfwInit();

        if (!m_resizable)
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        if(m_alwaysOnTop)
            glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        if(maximized)
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        else if(minimized)
            glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
        if(!m_caption)
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        // Create window with graphics context
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        m_window = glfwCreateWindow(m_actualWidth, m_actualHeight, m_title.c_str(), nullptr, nullptr);
        glfwSetWindowPos(m_window, m_xpos, m_ypos);
        glfwSetWindowSizeLimits(m_window, (int)m_minwidth, (int)m_minheight, (int)m_maxwidth, (int)m_maxheight);

        std::vector<GLFWimage> images;

        if(!m_small_icon.empty())
        {
            int image_width, image_height;
            unsigned char* image_data = stbi_load(m_small_icon.c_str(), &image_width, &image_height, nullptr, 4);
            if(image_data)
            {
                images.push_back({image_width, image_height, image_data});
            }
        }

        if(!m_large_icon.empty())
        {
            int image_width, image_height;
            unsigned char* image_data = stbi_load(m_large_icon.c_str(), &image_width, &image_height, nullptr, 4);
            if(image_data)
            {
                images.push_back({image_width, image_height, image_data});
            }
        }

        if(!images.empty())
            glfwSetWindowIcon(m_window, images.size(), images.data());

        mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
                CreateEventArgument("actual_width", (int)m_actualWidth),
                CreateEventArgument("actual_height", (int)m_actualHeight),
                CreateEventArgument("client_width", (int)m_actualWidth),
                CreateEventArgument("client_height", (int)m_actualHeight)
        });

        glfwMakeContextCurrent(m_window);

        gl3wInit();

        // Setup Dear ImGui binding
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        imnodes::Initialize();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.IniFilename = nullptr;

        if (mvApp::GetApp()->m_docking)
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        if (mvApp::GetApp()->m_dockingShiftOnly)
            io.ConfigDockingWithShift = true;

        // Setup style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Setup callbacks
        glfwSetWindowSizeCallback(m_window, window_size_callback);
        glfwSetWindowCloseCallback(m_window, window_close_callback);
    }

    mvLinuxViewport::~mvLinuxViewport()
	{
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        imnodes::Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_window);
        glfwTerminate();
        mvApp::s_started = false;
	}

	void mvLinuxViewport::maximize()
	{
        glfwMaximizeWindow(m_window);
	}

	void mvLinuxViewport::minimize()
	{
        glfwIconifyWindow(m_window);
	}

    void mvLinuxViewport::restore()
    {
        glfwRestoreWindow(m_window);
    }

    void mvLinuxViewport::renderFrame()
    {

        prerender();

        if(GImGui->CurrentWindow == nullptr)
            return;

        m_app->render();

        postrender();
    }

    void mvLinuxViewport::run()
    {

        setup();
        while (m_running)
            renderFrame();

    }

    void mvLinuxViewport::prerender()
    {
        m_running = !glfwWindowShouldClose(m_window);

        if(m_posDirty)
        {
            glfwSetWindowPos(m_window, m_xpos, m_ypos);
            m_posDirty = false;
        }

        if(m_sizeDirty)
        {
            glfwSetWindowSizeLimits(m_window, (int)m_minwidth, (int)m_minheight, (int)m_maxwidth, (int)m_maxheight);
            glfwSetWindowSize(m_window, m_actualWidth, m_actualHeight);
            m_sizeDirty = false;
        }

        if(m_modesDirty)
        {
            glfwSetWindowTitle(m_window, m_title.c_str());
            glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, m_resizable ? GLFW_TRUE : GLFW_FALSE);
            glfwSetWindowAttrib(m_window, GLFW_DECORATED, m_caption ? GLFW_TRUE : GLFW_FALSE);
            glfwSetWindowAttrib(m_window, GLFW_FLOATING, m_alwaysOnTop ? GLFW_TRUE : GLFW_FALSE);
            m_modesDirty = false;
        }

        if(glfwGetWindowAttrib(m_window, GLFW_ICONIFIED))
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
            mvToolManager::GetFontManager().updateDefaultFont();
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

    }

    void mvLinuxViewport::postrender()
    {
        glfwSwapInterval(m_vsync ? 1 : 0); // Enable vsync

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);
        glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }

}
