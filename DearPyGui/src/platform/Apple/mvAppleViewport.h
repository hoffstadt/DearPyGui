#pragma once

#include "mvViewport.h"
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

struct GLFWwindow;

namespace Marvel {

    class mvAppleViewport : public mvViewport
    {

    public:

        mvAppleViewport(unsigned width, unsigned height, bool error = false);
        ~mvAppleViewport();

        void show(bool minimized, bool maximized) override;
        void run        () override {render();}
        void renderFrame() override;
        void maximize() override;
        void minimize() override;
        void restore() override;

        static id<MTLDevice> GetDevice() { return device; }

    private:

        float m_clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};

        GLFWwindow *m_window;
        CAMetalLayer *m_layer;
        MTLRenderPassDescriptor *m_renderPassDescriptor;
        id <MTLCommandQueue> m_commandQueue;
        static id <MTLDevice> device;

    };
}
