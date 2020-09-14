#include "mvAppleWindow.h"
#include "Core/mvDataStorage.h"
#include <implot.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "imgui.h"
#include "implot.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"
#include <stdio.h>

namespace Marvel {

    id <MTLDevice> mvAppleWindow::device;

    mvWindow* mvWindow::CreatemvWindow(unsigned width, unsigned height, bool error)
	{
		return new mvAppleWindow(width, height, error);
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

    static void glfw_error_callback(int error, const char *description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    mvAppleWindow::mvAppleWindow(unsigned width, unsigned height, bool error)
        : mvWindow(width, height, error)
    {
        // Setup Dear ImGui binding
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.IniFilename = nullptr;
        (void) io;
        setupFonts();
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

        // Setup style
        ImGui::StyleColorsDark();

        // Setup window
        glfwSetErrorCallback(glfw_error_callback);
        glfwInit();

        if (!mvApp::GetApp()->getResizable())
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Create window with graphics context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(width, height, mvApp::GetApp()->m_title.c_str(), nullptr, nullptr);
	    mvApp::GetApp()->setWindowSize(width, height);

        device = MTLCreateSystemDefaultDevice();;
        m_commandQueue = [device newCommandQueue];

        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplMetal_Init(device);

        NSWindow *nswin = glfwGetCocoaWindow(m_window);
        m_layer = [CAMetalLayer layer];
        m_layer.device = device;
        m_layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        nswin.contentView.layer = m_layer;
        nswin.contentView.wantsLayer = YES;

        m_renderPassDescriptor = [MTLRenderPassDescriptor new];

        // Setup callbacks
        glfwSetWindowSizeCallback(m_window, window_size_callback);
        glfwSetWindowCloseCallback(m_window, window_close_callback);
    }

    mvAppleWindow::~mvAppleWindow()
    {
        // Cleanup
        ImGui_ImplMetal_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_window);
        glfwTerminate();

    }

    void mvAppleWindow::setWindowText(const std::string& name)
    {
        glfwSetWindowTitle(m_window, name.c_str());
    }

    void mvAppleWindow::renderFrame()
    {
        m_running = !glfwWindowShouldClose(m_window);

        if(glfwGetWindowAttrib(m_window, GLFW_ICONIFIED))
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
            glfwPollEvents();

            //glfwSwapInterval(mvApp::GetApp()->getVSync() ? 1 : 0); // Enable vsync

            m_layer.displaySyncEnabled = mvApp::GetApp()->getVSync();

            int width, height;
            glfwGetFramebufferSize(m_window, &width, &height);
            m_layer.drawableSize = CGSizeMake(width, height);
            id <CAMetalDrawable> drawable = [m_layer nextDrawable];

            id <MTLCommandBuffer> commandBuffer = [m_commandQueue commandBuffer];
            m_renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(m_clear_color[0],
                                                                                      m_clear_color[1],
                                                                                      m_clear_color[2],
                                                                                      m_clear_color[3]);
            m_renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
            m_renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
            m_renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
            id <MTLRenderCommandEncoder> m_renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:m_renderPassDescriptor];
            [m_renderEncoder pushDebugGroup:@"ImGui demo"];

            // Start the Dear ImGui frame
            ImGui_ImplMetal_NewFrame(m_renderPassDescriptor);
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            mvDataStorage::UpdateData();

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

            // Rendering
            ImGui::Render();
            ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, m_renderEncoder);

            [m_renderEncoder popDebugGroup];
            [m_renderEncoder endEncoding];

            [commandBuffer presentDrawable:drawable];
            [commandBuffer commit];
        }
	}

    void mvAppleWindow::render() {

        while(m_running)
            renderFrame();
    }

    void mvAppleWindow::cleanup() {
        // Cleanup
        ImGui_ImplMetal_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

}
