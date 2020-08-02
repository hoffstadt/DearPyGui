#pragma once

#include "Core/mvWindow.h"
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

struct GLFWwindow;

namespace Marvel {

    class mvAppleWindow : public mvWindow
    {

    public:

        mvAppleWindow(unsigned width, unsigned height, bool editor = false, bool error = false, bool doc = false);
        ~mvAppleWindow();

        virtual void run() override {render();}
        virtual void render() override;
        virtual void cleanup() override;

    private:

        float m_clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};

        GLFWwindow *m_window;
        CAMetalLayer *m_layer;
        MTLRenderPassDescriptor *m_renderPassDescriptor;
        id <MTLCommandQueue> m_commandQueue;

    };
}
