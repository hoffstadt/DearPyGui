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

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvInput
	//-----------------------------------------------------------------------------
	class mvInput
	{

	public:

		static void          setMousePosition     (float x, float y);
		static void          setMouseDragThreshold(float threshold);
		static void          setMouseDragging     (bool drag);
		static void          setMouseDragDelta    (const mvVec2& delta);

		static float         getMouseDragThreshold();
		static const mvVec2& getMouseDragDelta    ();
		static const mvVec2& getMousePosition     ();

		static bool          isMouseDragging           (int button, float threshold);
		static bool          isMouseButtonDown         (int button);
		static bool          isMouseButtonClicked      (int button);
		static bool          isMouseButtonDoubleClicked(int button);
		static bool          isMouseButtonReleased     (int button);
		static bool          isKeyDown                 (int keycode);
		static bool          isKeyPressed              (int keycode);
		static bool          isKeyReleased             (int keycode);

	private:

		static mvVec2 s_mousePos;
		static float  s_mouseWheel;
		static float  s_mouseDragThreshold;
		static bool   s_mouseDragging;
		static mvVec2 s_mouseDragDelta;

	};

}
