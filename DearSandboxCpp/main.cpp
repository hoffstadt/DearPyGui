#include <iostream>
#include <Windows.h>
#include "mvCppInterface.h"

using namespace Marvel;


int main(int argc, char* argv[])
{

	add_window("Test Window");

		add_button("Press me", []() {std::cout << "Button Pressed" << std::endl; });

		add_checkbox("Click me");

		add_color_button("Color button", mvColor(255, 100, 0, 255), []() {std::cout << "Color Button Pressed" << std::endl; });
		add_color_edit3("Color edit 3");
		add_color_picker4("Color Picker 4");
	
	end();

	start_dearpygui();
		
}