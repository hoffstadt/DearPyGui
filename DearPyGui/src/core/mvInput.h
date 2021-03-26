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
#include "mvPythonParser.h"

namespace Marvel {

#ifdef MV_CPP
#else
	
	void AddInputCommands                    (std::map<std::string, mvPythonParser>* parsers);

	// mouse polling
	PyObject* is_mouse_button_dragging       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_mouse_button_down           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_mouse_button_clicked        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_mouse_button_double_clicked (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_mouse_button_released       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_mouse_pos                  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_plot_mouse_pos             (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_drawing_mouse_pos          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_mouse_drag_delta           (PyObject* self, PyObject* args);

	// mouse callbacks
	PyObject* set_mouse_down_callback        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_mouse_drag_callback        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_mouse_double_click_callback(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_mouse_click_callback       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_mouse_release_callback     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_mouse_wheel_callback       (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_mouse_move_callback        (PyObject* self, PyObject* args, PyObject* kwargs);

	// key polling
	PyObject* is_key_pressed                 (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_key_released                (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_key_down                    (PyObject* self, PyObject* args, PyObject* kwargs);

	// key callbacks
	PyObject* set_key_down_callback          (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_key_press_callback         (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_key_release_callback       (PyObject* self, PyObject* args, PyObject* kwargs);

	// app callbacks
	PyObject* set_resize_callback            (PyObject* self, PyObject* args, PyObject* kwargs);
#endif

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
