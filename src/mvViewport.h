#pragma once

//-----------------------------------------------------------------------------
// mvViewport
//
//     - Light wrapper for a platform specific
//       window. It's quite lacking so may need to be extended later.
//     
//-----------------------------------------------------------------------------

#include "mvContext.h"
#include <imgui.h>
#include "mvCallbackRegistry.h"
#include "mvGraphics.h"

struct GLFWwindow;

struct mvViewport
{
	b8 running = true;
	b8 shown = false;
	b8 resized = false;

	std::string title = "Dear PyGui";
	std::string small_icon;
	std::string large_icon;
	mvColor     clearColor = mvColor(0, 0, 0, 255);
		
	// window modes
	b8 titleDirty  = false;
	b8 modesDirty  = false;
	b8 vsync       = true;
	b8 resizable   = true;
	b8 alwaysOnTop = false;
	b8 decorated   = true;
    b8 fullScreen  = false;
	b8 disableClose = false;

	// position/size
	b8  sizeDirty    = false;
	b8  posDirty     = false;
	u32 width        = 0;
	u32 height       = 0;
	u32 minwidth     = 250;
	u32 minheight    = 250;
	u32 maxwidth     = 10000;
	u32 maxheight    = 10000;
	i32 actualWidth  = 1280;
	i32 actualHeight = 800;
	i32 clientWidth  = 1280;
	i32 clientHeight = 800;
	i32 xpos         = 100;
	i32 ypos         = 100;

	void* platformSpecifics = nullptr; // platform specifics

};

mvViewport* mvCreateViewport  (u32 width, u32 height);
void        mvCleanupViewport (mvViewport& viewport);
void        mvShowViewport    (mvViewport& viewport, b8 minimized, b8 maximized);
void        mvMaximizeViewport(mvViewport& viewport);
void        mvMinimizeViewport(mvViewport& viewport);
void        mvRestoreViewport (mvViewport& viewport);
void        mvRenderFrame();
void        mvToggleFullScreen(mvViewport& viewport);

static void mvOnResize()
{
	mvSubmitCallback([=]() {
		PyObject* dimensions = PyTuple_New(4);
		PyTuple_SetItem(dimensions, 0, PyLong_FromLong(GContext->viewport->actualWidth));
		PyTuple_SetItem(dimensions, 1, PyLong_FromLong(GContext->viewport->actualHeight));
		PyTuple_SetItem(dimensions, 2, PyLong_FromLong(GContext->viewport->clientWidth));
		PyTuple_SetItem(dimensions, 3, PyLong_FromLong(GContext->viewport->clientHeight));
		GContext->callbackRegistry->viewportResizeCallbackPoint.run(MV_APP_UUID, dimensions);
		Py_XDECREF(dimensions);
		});
}
