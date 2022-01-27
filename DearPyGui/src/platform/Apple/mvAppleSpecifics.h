#pragma once

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

struct mvViewportData
{
    GLFWwindow*   handle = nullptr;
    CAMetalLayer* layer = nullptr;
};

struct mvGraphics_Metal
{
    MTLRenderPassDescriptor* renderPassDescriptor;
    id <MTLCommandQueue>     commandQueue;
    id <MTLDevice>           device;
};