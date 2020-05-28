#include "mvWindowsWindow.h"

int main()
{
	mvWindowsWindow* window = new mvWindowsWindow();

	window->show();



	window->setup();
	while (window->m_running)
	{
		window->prerender();

		ImGui::ShowDemoWindow();

		window->postrender();
	}

}