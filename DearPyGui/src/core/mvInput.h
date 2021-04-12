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
#include "cpp.hint"
#include "mvApp.h"

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

		struct AtomicFloatVec2
		{
			std::atomic<float> x;
			std::atomic<float> y;
		};

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_CREATE_EXTRA_COMMAND(get_drawing_mouse_pos);
		MV_CREATE_EXTRA_COMMAND(is_mouse_button_dragging);
		MV_CREATE_EXTRA_COMMAND(is_mouse_button_down);
		MV_CREATE_EXTRA_COMMAND(is_mouse_button_clicked);
		MV_CREATE_EXTRA_COMMAND(is_mouse_button_double_clicked);
		MV_CREATE_EXTRA_COMMAND(is_mouse_button_released);
		MV_CREATE_EXTRA_COMMAND(get_mouse_drag_delta);
		MV_CREATE_EXTRA_COMMAND(get_mouse_pos);
		MV_CREATE_EXTRA_COMMAND(get_plot_mouse_pos);
		MV_CREATE_EXTRA_COMMAND(is_key_pressed);
		MV_CREATE_EXTRA_COMMAND(is_key_released);
		MV_CREATE_EXTRA_COMMAND(is_key_down);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(get_drawing_mouse_pos);
			MV_ADD_EXTRA_COMMAND(is_mouse_button_dragging);
			MV_ADD_EXTRA_COMMAND(is_mouse_button_down);
			MV_ADD_EXTRA_COMMAND(is_mouse_button_clicked);
			MV_ADD_EXTRA_COMMAND(is_mouse_button_double_clicked);
			MV_ADD_EXTRA_COMMAND(is_mouse_button_released);
			MV_ADD_EXTRA_COMMAND(get_mouse_drag_delta);
			MV_ADD_EXTRA_COMMAND(get_mouse_pos);
			MV_ADD_EXTRA_COMMAND(get_plot_mouse_pos);
			MV_ADD_EXTRA_COMMAND(is_key_pressed);
			MV_ADD_EXTRA_COMMAND(is_key_released);
			MV_ADD_EXTRA_COMMAND(is_key_down);
		MV_END_EXTRA_COMMANDS


	public:

		static void          setMousePosition       (float x, float y);
		static void          setGlobalMousePosition (float x, float y);
		static void          setPlotMousePosition   (float x, float y);
		static void          setDrawingMousePosition(float x, float y);
		static void          setMouseDragThreshold  (float threshold);
		static void          setMouseDragDelta      (const mvVec2& delta);

		static int           getMouseDragThreshold  ();
		static mvVec2        getMouseDragDelta      ();
		static mvVec2        getMousePosition       ();
		static mvVec2        getGlobalMousePosition ();
		static mvVec2        getPlotMousePosition   ();
		static mvVec2        getDrawingMousePosition();

		static bool          isMouseDragging           (int button, float threshold);
		static bool          isMouseButtonDown         (int button);
		static bool          isMouseButtonClicked      (int button);
		static bool          isMouseButtonDoubleClicked(int button);
		static bool          isMouseButtonReleased     (int button);
		static bool          isKeyDown                 (int keycode);
		static bool          isKeyPressed              (int keycode);
		static bool          isKeyReleased             (int keycode);
		
		static void          InsertConstants(std::vector<std::pair<std::string, long>>& constants);

		static void          CheckInputs();

	private:

		static AtomicVec2       s_mousePos;
		static AtomicVec2       s_mouseGlobalPos;
		static AtomicFloatVec2  s_mousePlotPos;
		static AtomicVec2       s_mouseDrawingPos;
		static std::atomic_int  s_mouseDragThreshold;
		static AtomicVec2       s_mouseDragDelta;

		// keys
		static std::atomic_bool s_keysdown[512];
		static std::atomic_int  s_keysdownduration[512]; // 1/100 seconds
		static std::atomic_bool s_keyspressed[512];
		static std::atomic_bool s_keysreleased[512];

		// mouse
		static std::atomic_int  s_mousewheel;
		static std::atomic_bool s_mousedown[5];
		static std::atomic_bool s_mouseDragging[5];
		static std::atomic_int  s_mousedownduration[5]; // 1/100 seconds
		static std::atomic_bool s_mouseclick[5];
		static std::atomic_bool s_mousedoubleclick[5];
		static std::atomic_bool s_mousereleased[5];

	};

}
