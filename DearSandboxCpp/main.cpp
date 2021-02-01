#include <iostream>
#include <Windows.h>
#include "mvCppPyObject.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include "mvAppItems.h"

using namespace Marvel;

int main(int argc, char* argv[])
{

	{
		auto man = addc_window("Test Window", std::unordered_map<std::string, PyObject>{
			{"no_close", PyObject(true)},
			{ "x_pos", PyObject(0) }});
	}
	mvApp::GetApp()->start("");

	mvApp::DeleteApp();
	mvEventBus::Reset();
	mvAppLog::Clear();
		
}