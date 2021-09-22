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
#include <imgui.h>
#include "mvCallbackRegistry.h"

struct GLFWwindow;

namespace Marvel {

	struct mvViewport
	{
		bool        running = true;
		bool        shown = false;

		std::string title = "Dear PyGui";
		std::string small_icon;
		std::string large_icon;
		mvColor     clearColor = mvColor(0, 0, 0, 255);
		
		// window modes
		bool titleDirty  = false;
		bool modesDirty  = false;
		bool vsync       = true;
		bool resizable   = true;
		bool alwaysOnTop = false;
		bool decorated   = true;

		// position/size
		bool     sizeDirty    = false;
		bool     posDirty     = false;
		unsigned width        = 0;
		unsigned height       = 0;
		unsigned minwidth     = 250;
		unsigned minheight    = 250;
		unsigned maxwidth     = 10000;
		unsigned maxheight    = 10000;
		int      actualWidth  = 1280;
		int      actualHeight = 800;
		int      clientWidth  = 1280;
		int      clientHeight = 800;
		int      xpos         = 100;
		int      ypos         = 100;

	};

	mvViewport* mvCreateViewport(unsigned width, unsigned height);
	void        mvCleanupViewport();
	void        mvShowViewport(bool minimized, bool maximized);
	void        mvMaximizeViewport();
	void        mvMinimizeViewport();
	void        mvRestoreViewport();
	void        mvRenderFrame();

	static void mvOnResize()
	{
		GContext->callbackRegistry->submitCallback([=]() {
			PyObject* dimensions = PyTuple_New(4);
			PyTuple_SetItem(dimensions, 0, PyLong_FromLong(GContext->viewport->actualWidth));
			PyTuple_SetItem(dimensions, 1, PyLong_FromLong(GContext->viewport->actualHeight));
			PyTuple_SetItem(dimensions, 2, PyLong_FromLong(GContext->viewport->clientWidth));
			PyTuple_SetItem(dimensions, 3, PyLong_FromLong(GContext->viewport->clientHeight));
			GContext->callbackRegistry->addCallback(
				GContext->callbackRegistry->getResizeCallback(), MV_APP_UUID, dimensions, nullptr);
			});
	}

}
