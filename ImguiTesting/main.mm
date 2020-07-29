#include "mvAppleWindow.h"
#include <implot.h>

int main()
{
    auto window = new mvAppleWindow();

    window->show();

    window->setup();
    while (window->m_running)
        window->prerender();

    window->cleanup();

}