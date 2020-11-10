#pragma once

#include "Core/mvWindow.h"
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

struct GLFWwindow;

namespace Marvel {

    class mvAppleWindow : public mvWindow
    {

    public:

        mvAppleWindow(unsigned width, unsigned height, bool error = false);
        ~mvAppleWindow();

        void run        () override {render();}
        void render     () override;
        void renderFrame() override;
        void cleanup    () override;
        void setWindowText(const std::string& name) override;

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
