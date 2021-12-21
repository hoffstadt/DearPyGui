#pragma once

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvGlobalIntepreterLock
	//     - A convience class to automatically handle aqcuiring and releasing
	//       python's global intpreter lock between python c api calls.
	//-----------------------------------------------------------------------------
	class mvGlobalIntepreterLock
	{

	public:

		mvGlobalIntepreterLock();
		~mvGlobalIntepreterLock();

	private:

		int m_gstate;

	};

}
