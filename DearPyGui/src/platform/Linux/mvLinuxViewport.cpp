#include "mvLinuxViewport.h"
#include <iostream>
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
        mvApp::GetApp()->getViewport()->setActualHeight(height);
        mvApp::GetApp()->getViewport()->setClientHeight(height);
        mvApp::GetApp()->getViewport()->setActualWidth(width);
        mvApp::GetApp()->getViewport()->setClientWidth(width);
        mvApp::GetApp()->getViewport()->onResizeEvent();
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

        if (!_resizable)
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        if(_alwaysOnTop)
            glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        if(maximized)
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        else if(minimized)
            glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
        if(!_decorated)
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        // Create window with graphics context
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        _window = glfwCreateWindow(_actualWidth, _actualHeight, _title.c_str(), nullptr, nullptr);
        glfwSetWindowPos(_window, _xpos, _ypos);
        glfwSetWindowSizeLimits(_window, (int)_minwidth, (int)_minheight, (int)_maxwidth, (int)_maxheight);

        std::vector<GLFWimage> images;

        if(!_small_icon.empty())
        {
            int image_width, image_height;
            unsigned char* image_data = stbi_load(_small_icon.c_str(), &image_width, &image_height, nullptr, 4);
            if(image_data)
            {
                images.push_back({image_width, image_height, image_data});
            }
        }

        if(!_large_icon.empty())
        {
            int image_width, image_height;
            unsigned char* image_data = stbi_load(_large_icon.c_str(), &image_width, &image_height, nullptr, 4);
            if(image_data)
            {
                images.push_back({image_width, image_height, image_data});
            }
        }

        if(!images.empty())
            glfwSetWindowIcon(_window, images.size(), images.data());

        mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
                CreateEventArgument("actual_width", (int)_actualWidth),
                CreateEventArgument("actual_height", (int)_actualHeight),
                CreateEventArgument("client_width", (int)_actualWidth),
                CreateEventArgument("client_height", (int)_actualHeight)
        });

        glfwMakeContextCurrent(_window);

        gl3wInit();

        // Setup Dear ImGui binding
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        imnodes::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigWindowsMoveFromTitleBarOnly = true;

        if (mvApp::GetApp()->_loadIniFile)
        {
            ImGui::LoadIniSettingsFromDisk(mvApp::GetApp()->_iniFile.c_str());
            io.IniFilename = nullptr;
        }
        else
        {
            if (mvApp::GetApp()->_iniFile.empty())
                io.IniFilename = nullptr;
            else
                io.IniFilename = mvApp::GetApp()->_iniFile.c_str();
        }

        if (mvApp::GetApp()->_docking)
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Setup style
        ImGui::StyleColorsDark();
        mvApp::SetDefaultTheme();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(_window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Setup callbacks
        glfwSetWindowSizeCallback(_window, window_size_callback);
        glfwSetWindowCloseCallback(_window, window_close_callback);
    }

    mvLinuxViewport::~mvLinuxViewport()
	{
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        imnodes::DestroyContext();
        ImGui::DestroyContext();

        glfwDestroyWindow(_window);
        glfwTerminate();
        mvApp::s_started = false;
	}

	void mvLinuxViewport::maximize()
	{
        glfwMaximizeWindow(_window);
	}

    void mvLinuxViewport::fullscreen()
	{
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        int framerate = -1;
        if (mvViewport::_vsync){
            framerate = mode->refreshRate;
        }
        if (mvViewport::is_fullscreened){
            glfwSetWindowMonitor(_window, nullptr, mvViewport::_storedX, mvViewport::_storedY, mvViewport::_storedWidth, mvViewport::_storedHeight, framerate);
        } else {
            mvViewport::_storedWidth = mvViewport::_actualWidth;
            mvViewport::_storedHeight = mvViewport::_actualHeight;
            mvViewport::_storedX = mvViewport::_xpos;
            mvViewport::_storedY = mvViewport::_ypos;
            glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, framerate);
        }
        mvViewport::is_fullscreened = !mvViewport::is_fullscreened;
	}

	void mvLinuxViewport::minimize()
	{
        glfwIconifyWindow(_window);
	}

    void mvLinuxViewport::restore()
    {
        glfwRestoreWindow(_window);
    }

    void mvLinuxViewport::renderFrame()
    {

        prerender();

        if(GImGui->CurrentWindow == nullptr)
            return;

        _app->render();

        postrender();
    }

    void mvLinuxViewport::run()
    {

        setup();
        while (_running)
            renderFrame();

    }

    void mvLinuxViewport::prerender()
    {
        _running = !glfwWindowShouldClose(_window);

        if(_posDirty)
        {
            glfwSetWindowPos(_window, _xpos, _ypos);
            _posDirty = false;
        }

        if(_sizeDirty)
        {
            glfwSetWindowSizeLimits(_window, (int)_minwidth, (int)_minheight, (int)_maxwidth, (int)_maxheight);
            glfwSetWindowSize(_window, _actualWidth, _actualHeight);
            _sizeDirty = false;
        }

        if(_modesDirty)
        {
            std::cout << "Dirty modes" << std::endl;
            glfwSetWindowAttrib(_window, GLFW_RESIZABLE, _resizable ? GLFW_TRUE : GLFW_FALSE);
            glfwSetWindowAttrib(_window, GLFW_DECORATED, _decorated ? GLFW_TRUE : GLFW_FALSE);
            glfwSetWindowAttrib(_window, GLFW_FLOATING, _alwaysOnTop ? GLFW_TRUE : GLFW_FALSE);
            _modesDirty = false;
        }

        if (_titleDirty)
        {
            glfwSetWindowTitle(_window, _title.c_str());
            _titleDirty = false;
        }

        if(glfwGetWindowAttrib(_window, GLFW_ICONIFIED))
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

    void mvLinuxViewport::postrender()
    {
        glfwGetWindowPos(_window, &_xpos, &_ypos);

        glfwSwapInterval(_vsync ? 1 : 0); // Enable vsync

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(_window, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);
        glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(_window);
    }

}
