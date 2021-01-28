#include <iostream>
#include <Windows.h>

using namespace Marvel;

int main(int argc, char* argv[])
{

#ifdef MV_RELEASE
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
#else
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOW);	
#endif

	std::cout << "DearCppSandbox";
		
}