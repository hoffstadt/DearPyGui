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
        void fullscreen() override;

        static id<MTLDevice> GetDevice() { return device; }

    private:

        GLFWwindow *_window;
        CAMetalLayer *_layer;
        MTLRenderPassDescriptor *_renderPassDescriptor;
        id <MTLCommandQueue> _commandQueue;
        static id <MTLDevice> device;

    };
}
