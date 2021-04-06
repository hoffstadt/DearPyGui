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
#include "mvEvents.h"
#include "mvPython.h"
#include "cpp.hint"

namespace Marvel {

	class mvApp;

	//-----------------------------------------------------------------------------
	// mvViewport
	//-----------------------------------------------------------------------------
	class mvViewport
	{

	public:


        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_CREATE_EXTRA_COMMAND(show_viewport);
        MV_CREATE_EXTRA_COMMAND(create_viewport);
        MV_CREATE_EXTRA_COMMAND(configure_viewport);
        
		// viewport operations
        MV_CREATE_EXTRA_COMMAND(maximize_viewport);
        MV_CREATE_EXTRA_COMMAND(minimize_viewport);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(show_viewport)
			MV_ADD_EXTRA_COMMAND(create_viewport)
			MV_ADD_EXTRA_COMMAND(configure_viewport)
			MV_ADD_EXTRA_COMMAND(maximize_viewport)
			MV_ADD_EXTRA_COMMAND(minimize_viewport)
		MV_END_EXTRA_COMMANDS

		static mvViewport* CreateViewport(unsigned width, unsigned height, bool error = false);

		mvViewport(unsigned width, unsigned height, bool error = false);
		virtual ~mvViewport() = default;

		void setConfigDict(PyObject* dict);

		virtual void invalidateObjects() {}

		virtual void show       (bool minimized, bool maximized) {}
		virtual void run        () {}
		virtual void setup      () {}
		virtual void prerender  () {}
		virtual void render     () {}
		virtual void renderFrame() {}
		virtual void postrender () {}
		virtual void cleanup    () {}
		virtual void setWindowText(const std::string& name) {}

		virtual void maximize() {}
		virtual void minimize() {}
		virtual void restore() {}
		virtual void handleModes() {}
		
		void stop() { m_running = false; }

		// for use by primary window
		int getClientWidth() const { return (int)m_clientWidth; }
		int getClientHeight() const { return (int)m_clientHeight; }
		void onResizeEvent();

	protected:

		bool              m_running = true;
		mvApp*            m_app     = nullptr;
		bool              m_error   = false;
		std::string       m_title = "DearPyGui";


		// window modes
		bool              m_modesDirty = false;
		bool              m_vsync = true;
		bool              m_resizable = true;
		bool              m_alwaysOnTop = false;
		bool              m_maximizeBox = true;
		bool              m_minimizeBox = true;
		bool              m_border = true;
		bool              m_caption = true;
		bool              m_overlapped = true;

		// position/size
		bool              m_sizeDirty = false;
		bool              m_posDirty = false;
		unsigned          m_width;
		unsigned          m_minwidth = 250;
		unsigned          m_maxwidth = 10000;
		unsigned          m_height;
		unsigned          m_minheight = 250;
		unsigned          m_maxheight = 10000;
		int               m_actualWidth = 1280;
		int               m_actualHeight = 800;
		int               m_clientWidth = 1280;
		int               m_clientHeight = 800;
		int               m_xpos = 100;
		int               m_ypos = 100;
		

	};

}
