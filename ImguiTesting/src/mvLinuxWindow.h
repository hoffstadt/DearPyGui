#pragma once

class GLFWwindow;

class mvLinuxWindow
{

public:

    bool m_running = true;

    mvLinuxWindow();

    virtual void show() {}

    virtual void setup();
    virtual void prerender();
    virtual void render();
    virtual void postrender();
    virtual void cleanup();

private:

    float m_clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};

    GLFWwindow*              m_window;

};
