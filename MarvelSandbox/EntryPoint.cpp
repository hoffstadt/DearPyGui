#include "Core/mvWindow.h"
#include "Platform/Windows/mvWindowsWindow.h"

int main()
{
	mvWindow* window = new mvWindowsWindow();

	window->show();

	window->run();
}