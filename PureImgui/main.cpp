#include "mvWindowsWindow.h"
#include <implot.h>


int main()
{

	mvWindowsWindow* window = new mvWindowsWindow();

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