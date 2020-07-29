#pragma once

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

class GLFWwindow;


class mvAppleWindow
{

public:

    bool m_running = true;

    mvAppleWindow();

    virtual void show() {}

    virtual void setup();
    virtual void prerender();
    virtual void render();
    virtual void cleanup();

private:

    float m_clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};

    GLFWwindow*              m_window;
    CAMetalLayer*            m_layer;
    MTLRenderPassDescriptor* m_renderPassDescriptor;
    id <MTLCommandQueue>     m_commandQueue;

};
