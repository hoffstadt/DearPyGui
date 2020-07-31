#if defined (_WIN32)
#include "mvWindowsWindow.h"
#define mvWindowClass mvWindowsWindow
#else
#define mvWindowClass mvLinuxWindow
#include "mvLinuxWindow.h"
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

        ImGui::ShowDemoWindow();

        ImPlot::ShowDemoWindow();

        window->postrender();
    }

}