#include <iostream>
#include <Windows.h>
#include "mvCppInterface.h"

using namespace Marvel;

int main(int argc, char* argv[])
{

	mv_add_window("Test Window");

	mv_end();

	mvWindowAppItemConfig config;
	config.label = "Pizza";
	mv_configure_item("Test Window", &config);

	mv_start_dearpygui();
		
}