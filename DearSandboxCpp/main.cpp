#include <iostream>
#include <Windows.h>
#include "mvCppInterface.h"

using namespace Marvel;


int main(int argc, char* argv[])
{

	mv_add_window("Test Window");

		mvButtonConfig config;
		config.callback = []() {std::cout << "Button Pressed" << std::endl; };
		mv_add_button("Press me", config);
	
	mv_end();

	mv_start_dearpygui();
		
}