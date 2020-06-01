#include "Core/mvWindow.h"
#include "Platform/Windows/mvWindowsWindow.h"

int main()
{
	mvWindow* window = new mvWindowsWindow();

	window->show();

	auto app = Marvel::mvApp::GetApp();
	app->addInputText("none", "Testing");

	window->run();
}