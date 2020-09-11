#include "Platform/Linux/mvLinuxWindow.h"
#include "mvApp.h"
#include "Core/mvDataStorage.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "implot.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

namespace Marvel {

    mvWindow* mvWindow::CreatemvWindow(unsigned width, unsigned height, bool error)
    {
        return new mvLinuxWindow(width, height, error);
    }

    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    static void window_close_callback(GLFWwindow* window)
    {
        mvApp::SetAppStopped();
    }

    static void window_size_callback(GLFWwindow* window, int width, int height)
    {

        mvApp::GetApp()->setActualSize(width, height);
        mvApp::GetApp()->setWindowSize(width, height);
        mvApp::GetApp()->runCallback(mvApp::GetApp()->getResizeCallback(), "Main Application");
    }

    mvLinuxWindow::mvLinuxWindow(unsigned width, unsigned height, bool error)
		: mvWindow(width, height, error)
	{

        // Setup window
        glfwSetErrorCallback(glfw_error_callback);
        glfwInit();

        if (!mvApp::GetApp()->getResizable())
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Create window with graphics context
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        m_window = glfwCreateWindow(width, height, mvApp::GetApp()->m_title.c_str(), nullptr, nullptr);
	    mvApp::GetApp()->setWindowSize(width, height);

        glfwMakeContextCurrent(m_window);

        gl3wInit();

        // Setup Dear ImGui binding
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.IniFilename = nullptr;
        setupFonts();
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

        // Setup style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Setup callbacks
        glfwSetWindowSizeCallback(m_window, window_size_callback);
        glfwSetWindowCloseCallback(m_window, window_close_callback);

	}

    mvLinuxWindow::~mvLinuxWindow()
	{
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_window);
        glfwTerminate();
        mvApp::s_started = false;
	}

    void mvLinuxWindow::setWindowText(const std::string& name)
    {
        glfwSetWindowTitle(m_window, name.c_str());
    }

    void mvLinuxWindow::renderFrame()
    {
        prerender();

        mvDataStorage::UpdateData();

        glfwSwapInterval(mvApp::GetApp()->getVSync() ? 1 : 0); // Enable vsync

        if (m_error)
        {
            mvAppLog::setSize(m_width, m_height);
            mvAppLog::render();
        }

        else
        {
            m_app->prerender(m_running);
            m_app->render(m_running);
            m_app->postrender();
        }

        postrender();
    }

    void mvLinuxWindow::run()
    {

        setup();
        while (m_running)
            renderFrame();

    }

    void mvLinuxWindow::prerender()
    {
        m_running = !glfwWindowShouldClose(m_window);

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

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


    }

    void mvLinuxWindow::postrender()
    {
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }

    void mvLinuxWindow::cleanup()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
}
