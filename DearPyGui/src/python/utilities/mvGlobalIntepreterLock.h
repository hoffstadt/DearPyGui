#pragma once

//-----------------------------------------------------------------------------
// mvGlobalIntepreterLock
//     - A convience class to automatically handle aqcuiring and releasing
//       python's global intpreter lock between python c api calls.
//-----------------------------------------------------------------------------
struct mvGlobalIntepreterLock
{
	int _gstate;

	mvGlobalIntepreterLock();
	~mvGlobalIntepreterLock();
	
};
