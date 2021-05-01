#pragma once

#include "mvViewport.h"
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

struct GLFWwindow;

namespace Marvel {

    class mvAppleViewport : public mvViewport
    {

    public:

        mvAppleViewport(unsigned width, unsigned height);
        ~mvAppleViewport();

        void show(bool minimized, bool maximized) override;
        void renderFrame() override;
        void maximize() override;
        void minimize() override;
        void restore() override;

        static id<MTLDevice> GetDevice() { return device; }

    private:

        GLFWwindow *m_window;
        CAMetalLayer *m_layer;
        MTLRenderPassDescriptor *m_renderPassDescriptor;
        id <MTLCommandQueue> m_commandQueue;
        static id <MTLDevice> device;

    };
}
