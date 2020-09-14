#include "mvAppleWindow.h"
#include <implot.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "implot.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"
#include <stdio.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

mvAppleWindow::mvAppleWindow()
{
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup style
    ImGui::StyleColorsDark();

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    // Create window with graphics context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+Metal example", NULL, NULL);


    id <MTLDevice> device = MTLCreateSystemDefaultDevice();;
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
}

void mvAppleWindow::setup()
{

}

void mvAppleWindow::prerender()
{
    m_running = !glfwWindowShouldClose(m_window);

    @autoreleasepool {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        m_layer.drawableSize = CGSizeMake(width, height);
        id <CAMetalDrawable> drawable = [m_layer nextDrawable];

        id <MTLCommandBuffer> commandBuffer = [m_commandQueue commandBuffer];
        m_renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(m_clear_color[0], m_clear_color[1],
                                                                                  m_clear_color[2], m_clear_color[3]);
        m_renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
        m_renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
        m_renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        id <MTLRenderCommandEncoder> m_renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:m_renderPassDescriptor];
        [m_renderEncoder pushDebugGroup:@"ImGui demo"];

        // Start the Dear ImGui frame
        ImGui_ImplMetal_NewFrame(m_renderPassDescriptor);
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        render();

        // Rendering
        ImGui::Render();
        ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, m_renderEncoder);

        [m_renderEncoder popDebugGroup];
        [m_renderEncoder endEncoding];

        [commandBuffer presentDrawable:drawable];
        [commandBuffer commit];
    }

}

void mvAppleWindow::render()
{
    ImGui::ShowDemoWindow();
    ImPlot::ShowDemoWindow();
}

void mvAppleWindow::cleanup()
{
    // Cleanup
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

