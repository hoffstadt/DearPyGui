#include <iostream>
#include <Windows.h>
#include "mvApp.h"
#include "mvAppLog.h"
#include "mvAppItems.h"

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

	auto item = CreateRef<mvWindowAppItem>("Cpp Window", false, nullptr);

	if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, "", ""))
	{
		mvApp::GetApp()->getItemRegistry().pushParent(item);

	}

	mvApp::GetApp()->start("");

	mvApp::DeleteApp();
	mvEventBus::Reset();
	mvAppLog::Clear();
		
}