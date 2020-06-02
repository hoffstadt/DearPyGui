#include "Core/mvWindow.h"
#include "Platform/Windows/mvWindowsWindow.h"
#include <iostream>

using namespace Marvel;

void ItemCallback(const std::string& sender, void* data)
{
	std::cout << "Send by: " << sender << std::endl;
}

int main()
{
	mvWindow* window = new mvWindowsWindow();

	window->show();

	auto app = mvApp::GetApp();

	app->addInputText("", "Testing", "Type something");

	app->addTabBar("", "TabBar1");
	{
		app->addTab("TabBar1", "Tab1");
		{
			app->addInputText("Tab1", "Testing1", "Type something1");
			app->addInputText("Tab1", "Testing2", "Type something2");
		}
		app->endTab("Tab1");

		app->addTab("TabBar1", "Tab2");
		{
			app->addInputText("Tab2", "Testing3", "Type something3");
			app->addInputText("Tab2", "Testing4", "Type something4");
		}
		app->endTab("Tab2");
	}
	app->endTabBar("TabBar1");

	app->setItemCallback("Testing", ItemCallback);

	window->run();
}