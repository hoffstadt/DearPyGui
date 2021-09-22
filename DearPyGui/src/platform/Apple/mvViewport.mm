#include "mvViewport.h"
#include <implot.h>
#include "imnodes.h"
#include "mvFontManager.h"
#include "mvToolManager.h"

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

    id <MTLDevice> mvAppleViewport::device;

    mvViewport* mvCreateViewport(unsigned width, unsigned height)
    {
        mvViewport* viewport = new mvViewport();
        viewport->width = width;
        viewport->height = height;
        return viewport;
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

    static void glfw_error_callback(int error, const char *description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

	void mvCleanupViewport()
    {
        // Cleanup
        ImGui_ImplMetal_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        imnodes::DestroyContext();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        glfwDestroyWindow(GContext->viewport->handle);
        glfwTerminate();
    }

	void mvShowViewport(bool minimized, bool maximized)
    {
        mvViewport* viewport = GContext->viewport;

        // Setup window
        glfwSetErrorCallback(glfw_error_callback);
        glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
        glfwInit();

        if (!viewport->resizable)
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        if(viewport->alwaysOnTop)
            glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        if(maximized)
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        else if(minimized)
            glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
        if(!viewport->decorated)
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_FALSE);


        // Create window with graphics context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
        viewport->handle = glfwCreateWindow((int)viewport->actualWidth, (int)viewport->actualHeight, viewport->title.c_str(), nullptr, nullptr);
        glfwSetWindowPos(viewport->handle, viewport->xpos, viewport->ypos);
        glfwSetWindowSizeLimits(viewport->handle, (int)viewport->minwidth, (int)viewport->minheight, (int)viewport->maxwidth, (int)viewport->maxheight);


        mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
            CreateEventArgument("actual_width", (int)viewport->actualWidth),
            CreateEventArgument("actual_height", (int)viewport->actualHeight),
            CreateEventArgument("client_width", (int)viewport->actualWidth),
            CreateEventArgument("client_height", (int)viewport->actualHeight)
                    });

        viewport->device = MTLCreateSystemDefaultDevice();
        viewport->commandQueue = [viewport->device newCommandQueue];

        // Setup Dear ImGui binding
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        imnodes::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
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

        (void) io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

        if(GContext->IO.docking)
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Setup style
        ImGui::StyleColorsDark();
        SetDefaultTheme();

        ImGui_ImplGlfw_InitForOpenGL(viewport->handle, true);
        ImGui_ImplMetal_Init(viewport->device);

        NSWindow *nswin = glfwGetCocoaWindow(viewport->handle);
        viewport->layer = [CAMetalLayer viewport->layer];
        viewport->layer.device = viewport->device;
        viewport->layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        nswin.contentView.layer = viewport->layer;
        nswin.contentView.wantsLayer = YES;

        viewport->renderPassDescriptor = [MTLRenderPassDescriptor new];

        // Setup callbacks
        glfwSetWindowSizeCallback(viewport->handle, window_size_callback);
        //glfwSetFramebufferSizeCallback(m_window, window_size_callback);
        glfwSetWindowCloseCallback(viewport->handle, window_close_callback);
    }
  
    void mvMaximizeViewport()
    {
        glfwMaximizeWindow(GContext->viewport->handle);
    }

    void mvMinimizeViewport()
    {
        glfwIconifyWindow(GContext->viewport->handle);
    }

    void mvRestoreViewport()
    {
        glfwRestoreWindow(GContext->viewport->handle);
    }

    void mvRenderFrame()
    {
        mvViewport* viewport = GContext->viewport;

        viewport->running = !glfwWindowShouldClose(viewport->handle);

        if(viewport->posDirty)
        {
            glfwSetWindowPos(viewport->handle, viewport->xpos, viewport->ypos);
            viewport->posDirty = false;
        }

        if(viewport->sizeDirty)
        {
            glfwSetWindowSizeLimits(viewport->handle, (int)viewport->minwidth, (int)viewport->minheight, (int)viewport->maxwidth, (int)viewport->maxheight);
            glfwSetWindowSize(viewport->handle, viewport->actualWidth, viewport->actualHeight);
            viewport->sizeDirty = false;
        }

        if(viewport->modesDirty)
        {
            glfwSetWindowAttrib(viewport->handle, GLFW_RESIZABLE, viewport->resizable ? GLFW_TRUE : GLFW_FALSE);
            glfwSetWindowAttrib(viewport->handle, GLFW_DECORATED, viewport->decorated ? GLFW_TRUE : GLFW_FALSE);
            glfwSetWindowAttrib(viewport->handle, GLFW_FLOATING, viewport->alwaysOnTop ? GLFW_TRUE : GLFW_FALSE);
            viewport->modesDirty = false;
        }

        if (viewport->titleDirty)
        {
            glfwSetWindowTitle(viewport->handle, viewport->title.c_str());
            viewport->titleDirty = false;
        }

        if(glfwGetWindowAttrib(viewport->handle, GLFW_ICONIFIED))
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

            if (mvToolManager::GetFontManager().isInvalid())
            {
                mvToolManager::GetFontManager().rebuildAtlas();
                ImGui_ImplMetal_DestroyFontsTexture();
                mvToolManager::GetFontManager().updateAtlas();
                ImGui_ImplMetal_CreateFontsTexture(viewport->device);
            }

            NSWindow *nswin = glfwGetCocoaWindow(viewport->handle);
            if(nswin.isVisible && (nswin.occlusionState & NSWindowOcclusionStateVisible) == 0)
                usleep(32000u);

            viewport->layer.displaySyncEnabled = viewport->vsync;

            int width;
            int height;
            glfwGetFramebufferSize(viewport->handle, &width, &height);
            viewport->layer.drawableSize = CGSizeMake(width, height);
            id <CAMetalDrawable> drawable = [viewport->layer nextDrawable];

            viewport->width = (unsigned)width;
            viewport->height = (unsigned)height;

            id <MTLCommandBuffer> commandBuffer = [viewport->commandQueue commandBuffer];
            viewport->renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(viewport->clearColor.r,
                                                                                      viewport->clearColor.g,
                                                                                      viewport->clearColor.b,
                                                                                      viewport->clearColor.a);
            viewport->renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
            viewport->renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
            viewport->renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
            id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:viewport->renderPassDescriptor];
            [renderEncoder pushDebugGroup:@"ImGui demo"];

            // Start the Dear ImGui frame
            ImGui_ImplMetal_NewFrame(viewport->renderPassDescriptor);
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            Render();

            glfwGetWindowPos(viewport->handle, &viewport->xpos, &viewport->ypos);

            // Rendering
            ImGui::Render();
            ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, renderEncoder);

            [renderEncoder popDebugGroup];
            [renderEncoder endEncoding];

            [commandBuffer presentDrawable:drawable];
            [commandBuffer commit];

        }
	}

}
