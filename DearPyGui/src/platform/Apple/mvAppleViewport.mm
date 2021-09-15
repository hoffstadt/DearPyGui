#include "mvAppleViewport.h"
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

    mvViewport* mvViewport::CreateViewport(unsigned width, unsigned height)
	{
		return new mvAppleViewport(width, height);
	}

    static void window_close_callback(GLFWwindow* window)
    {
        GContext->started = false;
    }

    static void window_size_callback(GLFWwindow* window, int width, int height)
    {
        GContext->viewport->setActualHeight(height);
        GContext->viewport->setClientHeight(height);
        GContext->viewport->setActualWidth(width);
        GContext->viewport->setClientWidth(width);
        GContext->viewport->onResizeEvent();
    }

    static void glfw_error_callback(int error, const char *description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    mvAppleViewport::mvAppleViewport(unsigned width, unsigned height)
        : mvViewport(width, height)
    {
    }

    mvAppleViewport::~mvAppleViewport()
    {
        // Cleanup
        ImGui_ImplMetal_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        imnodes::DestroyContext();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        glfwDestroyWindow(_window);
        glfwTerminate();

    }

    void mvAppleViewport::show(bool minimized, bool maximized)
    {

        // Setup window
        glfwSetErrorCallback(glfw_error_callback);
        glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
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
        glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_FALSE);


        // Create window with graphics context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
        _window = glfwCreateWindow((int)_actualWidth, (int)_actualHeight, _title.c_str(), nullptr, nullptr);
        glfwSetWindowPos(_window, _xpos, _ypos);
        glfwSetWindowSizeLimits(_window, (int)_minwidth, (int)_minheight, (int)_maxwidth, (int)_maxheight);


        mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
            CreateEventArgument("actual_width", (int)_actualWidth),
            CreateEventArgument("actual_height", (int)_actualHeight),
            CreateEventArgument("client_width", (int)_actualWidth),
            CreateEventArgument("client_height", (int)_actualHeight)
                    });

        device = MTLCreateSystemDefaultDevice();;
        _commandQueue = [device newCommandQueue];

        // Setup Dear ImGui binding
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        imnodes::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
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

        (void) io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

        if(mvApp::GetApp()->_docking)
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Setup style
        ImGui::StyleColorsDark();
        SetDefaultTheme();

        ImGui_ImplGlfw_InitForOpenGL(_window, true);
        ImGui_ImplMetal_Init(device);

        NSWindow *nswin = glfwGetCocoaWindow(_window);
        _layer = [CAMetalLayer layer];
        _layer.device = device;
        _layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        nswin.contentView.layer = _layer;
        nswin.contentView.wantsLayer = YES;

        _renderPassDescriptor = [MTLRenderPassDescriptor new];

        // Setup callbacks
        glfwSetWindowSizeCallback(_window, window_size_callback);
        //glfwSetFramebufferSizeCallback(m_window, window_size_callback);
        glfwSetWindowCloseCallback(_window, window_close_callback);
    }

    void mvAppleViewport::maximize()
	{
        glfwMaximizeWindow(_window);
	}

	void mvAppleViewport::minimize()
	{
        glfwIconifyWindow(_window);
	}

    void mvAppleViewport::restore()
    {
        glfwRestoreWindow(_window);
    }

    void mvAppleViewport::renderFrame()
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
                ImGui_ImplMetal_CreateFontsTexture(device);
            }

            NSWindow *nswin = glfwGetCocoaWindow(_window);
            if(nswin.isVisible && (nswin.occlusionState & NSWindowOcclusionStateVisible) == 0)
                usleep(32000u);

            _layer.displaySyncEnabled = _vsync;

            int width;
            int height;
            glfwGetFramebufferSize(_window, &width, &height);
            _layer.drawableSize = CGSizeMake(width, height);
            id <CAMetalDrawable> drawable = [_layer nextDrawable];

            _width = (unsigned)width;
            _height = (unsigned)height;

            id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
            _renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(_clearColor.r,
                                                                                      _clearColor.g,
                                                                                      _clearColor.b,
                                                                                      _clearColor.a);
            _renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
            _renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
            _renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
            id <MTLRenderCommandEncoder> _renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:_renderPassDescriptor];
            [_renderEncoder pushDebugGroup:@"ImGui demo"];

            // Start the Dear ImGui frame
            ImGui_ImplMetal_NewFrame(_renderPassDescriptor);
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            Render();

            glfwGetWindowPos(_window, &_xpos, &_ypos);

            // Rendering
            ImGui::Render();
            ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, _renderEncoder);

            [_renderEncoder popDebugGroup];
            [_renderEncoder endEncoding];

            [commandBuffer presentDrawable:drawable];
            [commandBuffer commit];

        }
	}

}
