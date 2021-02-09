#include <iostream>
#include <Windows.h>
#include "mvCppInterface.h"

using namespace Marvel;


int main(int argc, char* argv[])
{

	add_window("Test Window");

		add_button("Press me", []() {std::cout << "Button Pressed" << std::endl; });

		add_checkbox("Click me");
	
	end();

	start_dearpygui();
		
}