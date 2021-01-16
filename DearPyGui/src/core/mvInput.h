#pragma once

//-----------------------------------------------------------------------------
// mvInput
//
//     - This class is used for input polling.
//
//     - When querying, it checks to make sure that the app is actually
//       running to prevent invalid ImGui calls outside New/End Frame calls
//
//     - May be able to remove storage and rely entirely on ImGui. Will test
//       for this soon.
//     
//-----------------------------------------------------------------------------

#include "mvCore.h"
#include <atomic>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvInput
	//-----------------------------------------------------------------------------
	class mvInput
	{

		struct AtomicVec2
		{
			std::atomic_int x;
			std::atomic_int y;
		};


	public:

		static void          setMousePosition      (float x, float y);
		static void          setGlobalMousePosition(float x, float y);
		static void          setPlotMousePosition  (float x, float y);
		static void          setMouseDragThreshold (float threshold);
		static void          setMouseDragDelta     (const mvVec2& delta);

		static int           getMouseDragThreshold ();
		static mvVec2        getMouseDragDelta     ();
		static mvVec2        getMousePosition      ();
		static mvVec2        getGlobalMousePosition();
		static mvVec2        getPlotMousePosition  ();

		static bool          isMouseDragging           (int button, float threshold);
		static bool          isMouseButtonDown         (int button);
		static bool          isMouseButtonClicked      (int button);
		static bool          isMouseButtonDoubleClicked(int button);
		static bool          isMouseButtonReleased     (int button);
		static bool          isKeyDown                 (int keycode);
		static bool          isKeyPressed              (int keycode);
		static bool          isKeyReleased             (int keycode);

		static void          CheckInputs();

	private:

		static AtomicVec2       s_mousePos;
		static AtomicVec2       s_mouseGlobalPos;
		static AtomicVec2       s_mousePlotPos;
		static std::atomic_int  s_mouseDragThreshold;
		static AtomicVec2       s_mouseDragDelta;

		// keys
		static std::atomic_bool s_keysdown[512];
		static std::atomic_int  s_keysdownduration[512]; // 1/100 seconds
		static std::atomic_bool s_keyspressed[512];
		static std::atomic_bool s_keysreleased[512];

		// mouse
		static std::atomic_int  s_mousewheel;
		static std::atomic_bool s_mousedown[3];
		static std::atomic_bool s_mouseDragging[3];
		static std::atomic_int  s_mousedownduration[3]; // 1/100 seconds
		static std::atomic_bool s_mouseclick[3];
		static std::atomic_bool s_mousedoubleclick[3];
		static std::atomic_bool s_mousereleased[3];

	};

}
