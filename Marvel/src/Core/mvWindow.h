#pragma once

//-----------------------------------------------------------------------------
// mvWindow
//
//     - This abstract class is just a light wrapper for a platform specific
//       window. It's quite lacking so may need to be extended later.
//
//     - This class may need to be renamed to remove confusion between this
//       window and the mvWindowAppItem class (maybe to viewport?)
//     
//-----------------------------------------------------------------------------

#include "mvApp.h"
#include "Core/StandardWindows/mvAppEditor.h"
#include "Core/StandardWindows/mvAppLog.h"
#include "StandardWindows/mvDocWindow.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvWindow
	//-----------------------------------------------------------------------------
	class mvWindow
	{

	public:

		mvWindow(unsigned width, unsigned height, bool editor = false, bool error = false, bool doc = false);

		virtual ~mvWindow() {}

		virtual void show      () = 0;
		virtual void setup     () {}
		virtual void prerender () {}
		virtual void postrender() {}
		virtual void cleanup   () {}

		void run();

	protected:

		bool              m_running       = true;
		bool              m_editor        = false;
		bool              m_error         = false;
		bool              m_doc           = false;
		mvStandardWindow* m_app           = nullptr;
		mvStandardWindow* m_appEditor     = nullptr;
		mvStandardWindow* m_documentation = nullptr;
		mvStandardWindow* m_logger        = nullptr;
		unsigned          m_width;
		unsigned          m_height;

	};

}
