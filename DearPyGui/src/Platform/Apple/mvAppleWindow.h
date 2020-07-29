#pragma once

#include "Core/mvWindow.h"
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

struct GLFWwindow;

namespace Marvel {

    class mvAppleWindow
    {

    public:

        mvAppleWindow(unsigned width, unsigned height, bool editor = false, bool error = false, bool doc = false);
        ~mvAppleWindow();

        void run() {render();}
        void render();
        void cleanup();

    private:

        float m_clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};

        bool              m_running       = true;
        bool              m_editor        = false;
        bool              m_error         = false;
        bool              m_doc           = false;
        mvStandardWindow* m_app           = nullptr;
        mvStandardWindow* m_appEditor     = nullptr;
        mvStandardWindow* m_documentation = nullptr;
        unsigned          m_width;
        unsigned          m_height;

        GLFWwindow *m_window;
        CAMetalLayer *m_layer;
        MTLRenderPassDescriptor *m_renderPassDescriptor;
        id <MTLCommandQueue> m_commandQueue;

    };
}
