#if defined (_WIN32)
#include "mvWindowsWindow.h"
#define mvWindowClass mvWindowsWindow
#elif defined(__APPLE__)
#include "mvAppleWindow.h"
#define mvWindowClass mvAppleWindow
#else
#include "mvLinuxWindow.h"
#define mvWindowClass mvLinuxWindow
#endif

#include <implot.h>

int main()
{
    auto window = new mvWindowClass();

    window->show();

    window->setup();
    while (window->m_running)
    {
        window->prerender();

#if defined (_WIN32)
        ImGui::ShowDemoWindow();
        ImPlot::ShowDemoWindow();
        window->postrender();
#elif defined(__APPLE__)
#else
        ImGui::ShowDemoWindow();
        ImPlot::ShowDemoWindow();
        window->postrender();
#endif


    }

}