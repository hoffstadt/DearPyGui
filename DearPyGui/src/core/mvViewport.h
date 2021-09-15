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

#include "mvContext.h"
#include "mvEvents.h"
#include "cpp.hint"

namespace Marvel {

	class mvViewport
	{

	public:


        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_CREATE_COMMAND(show_viewport);
        MV_CREATE_COMMAND(create_viewport);
        MV_CREATE_COMMAND(configure_viewport);
        MV_CREATE_COMMAND(get_viewport_configuration);
        MV_CREATE_COMMAND(is_viewport_ok);
        
		// viewport operations
        MV_CREATE_COMMAND(maximize_viewport);
        MV_CREATE_COMMAND(minimize_viewport);

		MV_START_COMMANDS
			MV_ADD_COMMAND(show_viewport)
			MV_ADD_COMMAND(create_viewport)
			MV_ADD_COMMAND(configure_viewport)
			MV_ADD_COMMAND(maximize_viewport)
			MV_ADD_COMMAND(minimize_viewport)
			MV_ADD_COMMAND(get_viewport_configuration)
			MV_ADD_COMMAND(is_viewport_ok)
		MV_END_COMMANDS

		static mvViewport* CreateViewport(unsigned width, unsigned height);

		mvViewport(unsigned width, unsigned height);
		virtual ~mvViewport() = default;

		void getConfigDict(PyObject* dict);
		void setConfigDict(PyObject* dict);

		virtual void invalidateObjects() {}

		virtual void show       (bool minimized, bool maximized) {}
		virtual void run        () {}
		virtual void setup      () {}
		virtual void renderFrame() {}

		virtual void maximize() {}
		virtual void minimize() {}
		virtual void restore() {}
		
		void stop() { _running = false; }
		bool running() const { return _running; }

		// for use by primary window
		void setActualWidth(int width) { _actualWidth = width; }
		void setActualHeight(int height) { _actualHeight = height; }
		void setClientWidth(int width) { _clientWidth = width; }
		void setClientHeight(int height) { _clientHeight = height; }
		int getClientWidth() const { return (int)_clientWidth; }
		int getClientHeight() const { return (int)_clientHeight; }
		void onResizeEvent();

	protected:

		bool        _running = true;
		std::string _title = "DearPyGui";
		std::string _small_icon;
		std::string _large_icon;
		mvColor     _clearColor = mvColor(0, 0, 0, 255);
		bool        _shown = false;


		// window modes
		bool _titleDirty = false;
		bool _modesDirty = false;
		bool _vsync = true;
		bool _resizable = true;
		bool _alwaysOnTop = false;
		bool _decorated = true;

		// position/size
		bool     _sizeDirty = false;
		bool     _posDirty = false;
		unsigned _width;
		unsigned _minwidth = 250;
		unsigned _maxwidth = 10000;
		unsigned _height;
		unsigned _minheight = 250;
		unsigned _maxheight = 10000;
		int      _actualWidth = 1280;
		int      _actualHeight = 800;
		int      _clientWidth = 1280;
		int      _clientHeight = 800;
		int      _xpos = 100;
		int      _ypos = 100;
		

	};

}
