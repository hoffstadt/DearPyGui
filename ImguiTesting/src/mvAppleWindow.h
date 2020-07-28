#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"
#include <stdio.h>
#include <implot.h>

class mvAppleWindow
{

public:

    bool m_running = true;

    mvAppleWindow();

    virtual void show();

    virtual void setup();
    virtual void prerender();
    virtual void postrender();
    virtual void cleanup();

};
