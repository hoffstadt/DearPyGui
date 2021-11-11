#include "mvViewport.h"
#include <implot.h>
#include "imnodes.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"
#include <stdio.h>

static GLFWwindow*              ghandle = nullptr;
static CAMetalLayer*            glayer;
static MTLRenderPassDescriptor* grenderPassDescriptor;
static id <MTLCommandQueue>     gcommandQueue;
id <MTLDevice>                  gdevice;

namespace Marvel {

    mv_impl mvViewport*
    mvCreateViewport(unsigned width, unsigned height)
    {
        mvViewport* viewport = new mvViewport();
        viewport->width = width;
        viewport->height = height;
        return viewport;
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
        mvOnResize();
    }

    mv_internal void
    glfw_error_callback(int error, const char *description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

	mv_impl void
    mvCleanupViewport()
    {
        // Cleanup
        ImGui_ImplMetal_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        glfwDestroyWindow(ghandle);
        glfwTerminate();
    }

	mv_impl void
    mvShowViewport(bool minimized, bool maximized)
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
        ghandle = glfwCreateWindow((int)viewport->actualWidth, (int)viewport->actualHeight, viewport->title.c_str(), nullptr, nullptr);
        glfwSetWindowPos(ghandle, viewport->xpos, viewport->ypos);
        glfwSetWindowSizeLimits(ghandle, (int)viewport->minwidth, (int)viewport->minheight, (int)viewport->maxwidth, (int)viewport->maxheight);

        gdevice = MTLCreateSystemDefaultDevice();
        gcommandQueue = [gdevice newCommandQueue];

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
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

        if(GContext->IO.docking)
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Setup style
        ImGui::StyleColorsDark();
        SetDefaultTheme();

        ImGui_ImplGlfw_InitForOpenGL(ghandle, true);
        ImGui_ImplMetal_Init(gdevice);

        NSWindow *nswin = glfwGetCocoaWindow(ghandle);
        glayer = [CAMetalLayer layer];
        glayer.device = gdevice;
        glayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        nswin.contentView.layer = glayer;
        nswin.contentView.wantsLayer = YES;

        grenderPassDescriptor = [MTLRenderPassDescriptor new];

        // Setup callbacks
        glfwSetWindowSizeCallback(ghandle, window_size_callback);
        //glfwSetFramebufferSizeCallback(m_window, window_size_callback);
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
        mvViewport* viewport = GContext->viewport;

        viewport->running = !glfwWindowShouldClose(ghandle);

        if(viewport->posDirty)
        {
            glfwSetWindowPos(ghandle, viewport->xpos, viewport->ypos);
            viewport->posDirty = false;
        }

        if(viewport->sizeDirty)
        {
            glfwSetWindowSizeLimits(ghandle, (int)viewport->minwidth, (int)viewport->minheight, (int)viewport->maxwidth, (int)viewport->maxheight);
            glfwSetWindowSize(ghandle, viewport->actualWidth, viewport->actualHeight);
            viewport->sizeDirty = false;
        }

        if(viewport->modesDirty)
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

        if(glfwGetWindowAttrib(ghandle, GLFW_ICONIFIED))
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
                ImGui_ImplMetal_CreateFontsTexture(gdevice);
            }

            NSWindow *nswin = glfwGetCocoaWindow(ghandle);
            if(nswin.isVisible && (nswin.occlusionState & NSWindowOcclusionStateVisible) == 0)
                usleep(32000u);

            glayer.displaySyncEnabled = viewport->vsync;

            int width;
            int height;
            glfwGetFramebufferSize(ghandle, &width, &height);
            glayer.drawableSize = CGSizeMake(width, height);
            id <CAMetalDrawable> drawable = [glayer nextDrawable];

            viewport->width = (unsigned)width;
            viewport->height = (unsigned)height;

            id <MTLCommandBuffer> commandBuffer = [gcommandQueue commandBuffer];
            grenderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(viewport->clearColor.r,
                                                                                      viewport->clearColor.g,
                                                                                      viewport->clearColor.b,
                                                                                      viewport->clearColor.a);
            grenderPassDescriptor.colorAttachments[0].texture = drawable.texture;
            grenderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
            grenderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
            id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:grenderPassDescriptor];
            [renderEncoder pushDebugGroup:@"ImGui demo"];

            // Start the Dear ImGui frame
            ImGui_ImplMetal_NewFrame(grenderPassDescriptor);
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            Render();

            glfwGetWindowPos(ghandle, &viewport->xpos, &viewport->ypos);

            // Rendering
            ImGui::Render();
            ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, renderEncoder);

            [renderEncoder popDebugGroup];
            [renderEncoder endEncoding];

            [commandBuffer presentDrawable:drawable];
            [commandBuffer commit];

        }
	}

    mv_impl void
    mvToggleFullScreen()
    {
        mv_local_persist size_t storedWidth = 0;
        mv_local_persist size_t storedHeight = 0;
        mv_local_persist int    storedXPos = 0;
        mv_local_persist int    storedYPos = 0;

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        int framerate = -1;
        if (GContext->viewport->vsync)
        {
             framerate = mode->refreshRate;
        }

        if(GContext->viewport->fullScreen)
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
