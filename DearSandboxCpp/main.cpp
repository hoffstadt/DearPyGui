#include <iostream>
#include <Windows.h>
#include "mvCppInterface.h"

using namespace Marvel;


int main(int argc, char* argv[])
{

	add_window("Test Window");

		mvButtonConfig config;
		config.callback = []() {std::cout << "Button Pressed" << std::endl; };
		add_button("Press me", config);
	
	end();

	start_dearpygui();
		
}