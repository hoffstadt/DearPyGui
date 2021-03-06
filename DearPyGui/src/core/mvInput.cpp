#include "mvInput.h"
#include "mvApp.h"
#include "mvProfiler.h"
#include "mvCallbackRegistry.h"
#include "mvItemRegistry.h"
#include "containers/mvWindowAppItem.h"

namespace Marvel {

	mvInput::AtomicVec2      mvInput::s_mousePos = {0, 0};
	mvInput::AtomicVec2      mvInput::s_mouseGlobalPos = {0, 0};
	mvInput::AtomicFloatVec2 mvInput::s_mousePlotPos = {0, 0};
	mvInput::AtomicVec2      mvInput::s_mouseDrawingPos = {0, 0};
	std::atomic_int          mvInput::s_mouseDragThreshold = 20;
	mvInput::AtomicVec2      mvInput::s_mouseDragDelta = { 0, 0 };
	std::atomic_bool         mvInput::s_keysdown[512];
	std::atomic_int          mvInput::s_keysdownduration[512]; // 1/100 seconds
	std::atomic_bool         mvInput::s_keyspressed[512];
	std::atomic_bool         mvInput::s_keysreleased[512];
	std::atomic_int          mvInput::s_mousewheel;
	std::atomic_bool         mvInput::s_mousedown[5];
	std::atomic_bool         mvInput::s_mouseDragging[5];
	std::atomic_int          mvInput::s_mousedownduration[5]; // 1/100 seconds
	std::atomic_bool         mvInput::s_mouseclick[5];
	std::atomic_bool         mvInput::s_mousedoubleclick[5];
	std::atomic_bool         mvInput::s_mousereleased[5];

	void mvInput::CheckInputs()
	{
		MV_PROFILE_FUNCTION();

		// update mouse
		// mouse move event
		ImVec2 mousepos = ImGui::GetMousePos();
		if (ImGui::IsMousePosValid(&mousepos))
		{
			if (s_mouseGlobalPos.x != mousepos.x || s_mouseGlobalPos.y !=mousepos.y)
			{
				mvInput::setGlobalMousePosition(mousepos.x, mousepos.y);

				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_MOVE,
					{
					CreateEventArgument("X", mousepos.x),
					CreateEventArgument("Y", mousepos.y)
					});
			}
		}
		

		// route key events
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
		{
			s_keysdown[i] = ImGui::GetIO().KeysDown[i];

			// route key pressed event
			if (s_keyspressed[i] = ImGui::IsKeyPressed(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_KEY_PRESS, { CreateEventArgument("KEY", i) });

			// route key down event
			if (ImGui::GetIO().KeysDownDuration[i] >= 0.0f)
			{
				s_keysdownduration[i] = (int)(ImGui::GetIO().KeysDownDuration[i] * 100.0);
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_KEY_DOWN,
					{ CreateEventArgument("KEY", i), CreateEventArgument("DURATION", ImGui::GetIO().KeysDownDuration[i]) });
			}

			// route key released event
			if (s_keysreleased[i] = ImGui::IsKeyReleased(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_KEY_RELEASE, { CreateEventArgument("KEY", i) });
		}

		// route mouse wheel event
		if (ImGui::GetIO().MouseWheel != 0.0f)
		{
			s_mousewheel = (int)ImGui::GetIO().MouseWheel;
			mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_WHEEL, { CreateEventArgument("DELTA", ImGui::GetIO().MouseWheel) });
		}

		// route mouse dragging event
		for (int i = 0; i < 3; i++)
		{
			s_mouseDragging[i] = ImGui::IsMouseDragging(i, mvInput::getMouseDragThreshold());
			
			if (ImGui::IsMouseDragging(i, mvInput::getMouseDragThreshold()))
			{
				mvInput::setMouseDragDelta({ ImGui::GetMouseDragDelta().x, ImGui::GetMouseDragDelta().y });
				// TODO: send delta
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_DRAG,
					{ CreateEventArgument("BUTTON", i),
					CreateEventArgument("X", ImGui::GetMouseDragDelta().x),
					CreateEventArgument("Y", ImGui::GetMouseDragDelta().y)
					});
				ImGui::ResetMouseDragDelta(i);
				break;
			}
			
		}

		// route other mouse events (note mouse move callbacks are handled in mvWindowAppItem)
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
		{
			s_mousedown[i] = ImGui::GetIO().MouseDown[i];

			// route mouse click event
			if (s_mouseclick[i] = ImGui::IsMouseClicked(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_CLICK, { CreateEventArgument("BUTTON", i) });

			// route mouse down event
			if (ImGui::GetIO().MouseDownDuration[i] >= 0.0f)
			{
				s_mousedownduration[i] = (int)(ImGui::GetIO().MouseDownDuration[i] * 100.0);
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_DOWN,
					{ CreateEventArgument("BUTTON", i), CreateEventArgument("DURATION",  ImGui::GetIO().MouseDownDuration[i]) });
			}
			else
				s_mousedownduration[i] = 0;

			// route mouse double clicked event
			if (s_mousedoubleclick[i] = ImGui::IsMouseDoubleClicked(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_DBL_CLK, { CreateEventArgument("BUTTON", i) });

			// route mouse released event
			if (s_mousereleased[i] = ImGui::IsMouseReleased(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_RELEASE, { CreateEventArgument("BUTTON", i) });
		}
	}

	void mvInput::setMousePosition(float x, float y)
	{
		s_mousePos.x = (int)x;
		s_mousePos.y = (int)y;
	}

	void mvInput::setGlobalMousePosition(float x, float y)
	{
		s_mouseGlobalPos.x = (int)x;
		s_mouseGlobalPos.y = (int)y;
	}

	void mvInput::setPlotMousePosition(float x, float y)
	{
		s_mousePlotPos.x = x;
		s_mousePlotPos.y = y;
	}

	void mvInput::setDrawingMousePosition(float x, float y)
	{
		s_mouseDrawingPos.x = (int)x;
		s_mouseDrawingPos.y = (int)y;
	}

	void mvInput::setMouseDragThreshold(float threshold)
	{
		s_mouseDragThreshold = threshold;
	}

	void mvInput::setMouseDragDelta(const mvVec2& delta)
	{
		s_mouseDragDelta.x = delta.x;
		s_mouseDragDelta.y = delta.y;
	}

	int mvInput::getMouseDragThreshold()
	{
		return s_mouseDragThreshold;
	}

	mvVec2 mvInput::getMouseDragDelta()
	{
		return { (float)s_mouseDragDelta.x, (float)s_mouseDragDelta.y };
	}

	mvVec2 mvInput::getMousePosition()
	{
		return { (float)s_mousePos.x, (float)s_mousePos.y };
	}

	mvVec2 mvInput::getGlobalMousePosition()
	{
		return { (float)s_mouseGlobalPos.x, (float)s_mouseGlobalPos.y };
	}

	mvVec2 mvInput::getPlotMousePosition()
	{
		return { (float)s_mousePlotPos.x, (float)s_mousePlotPos.y };
	}

	mvVec2 mvInput::getDrawingMousePosition()
	{
		return { (float)s_mouseDrawingPos.x, (float)s_mouseDrawingPos.y };
	}

	bool mvInput::isMouseDragging(int button, float threshold)
	{
		return s_mousedownduration[button]/100.0f >= threshold;
	}

	bool mvInput::isMouseButtonDown(int button)
	{
		return s_mousedown[button];
	}

	bool mvInput::isMouseButtonClicked(int button)
	{
		return s_mouseclick[button];
	}

	bool mvInput::isMouseButtonDoubleClicked(int button)
	{
		return s_mousedoubleclick[button];
	}

	bool mvInput::isMouseButtonReleased(int button)
	{
		return s_mousereleased[button];
	}

	bool mvInput::isKeyPressed(int keycode)
	{
		return s_keyspressed[keycode];
	}

	bool mvInput::isKeyReleased(int keycode)
	{
		return s_keysreleased[keycode];
	}

	bool mvInput::isKeyDown(int keycode)
	{
		return s_keysdown[keycode];
	}

	void mvInput::InsertConstants(std::vector<std::pair<std::string, long>>& constants)
	{

		//-----------------------------------------------------------------------------
		// Mouse Codes
		//-----------------------------------------------------------------------------
		constants.emplace_back("mvMouseButton_Left",   0);
		constants.emplace_back("mvMouseButton_Right",  1);
		constants.emplace_back("mvMouseButton_Middle", 2);
		constants.emplace_back("mvMouseButton_X1",     3);
		constants.emplace_back("mvMouseButton_X2",     4);

		//-----------------------------------------------------------------------------
		// Key Codes
		//-----------------------------------------------------------------------------
		constants.emplace_back("mvKey_0"                  , 0x30);
		constants.emplace_back("mvKey_1"			      , 0x31);
		constants.emplace_back("mvKey_2"			      , 0x32);
		constants.emplace_back("mvKey_3"			      , 0x33);
		constants.emplace_back("mvKey_4"			      , 0x34);
		constants.emplace_back("mvKey_5"			      , 0x35);
		constants.emplace_back("mvKey_6"			      , 0x36);
		constants.emplace_back("mvKey_7"			      , 0x37);
		constants.emplace_back("mvKey_8"			      , 0x38);
		constants.emplace_back("mvKey_9"			      , 0x39);
		constants.emplace_back("mvKey_A"			      , 0x41);
		constants.emplace_back("mvKey_B"			      , 0x42);
		constants.emplace_back("mvKey_C"			      , 0x43);
		constants.emplace_back("mvKey_D"			      , 0x44);
		constants.emplace_back("mvKey_E"			      , 0x45);
		constants.emplace_back("mvKey_F"			      , 0x46);
		constants.emplace_back("mvKey_G"			      , 0x47);
		constants.emplace_back("mvKey_H"			      , 0x48);
		constants.emplace_back("mvKey_I"			      , 0x49);
		constants.emplace_back("mvKey_J"			      , 0x4A);
		constants.emplace_back("mvKey_K"			      , 0x4B);
		constants.emplace_back("mvKey_L"			      , 0x4C);
		constants.emplace_back("mvKey_M"			      , 0x4D);
		constants.emplace_back("mvKey_N"			      , 0x4E);
		constants.emplace_back("mvKey_O"			      , 0x4F);
		constants.emplace_back("mvKey_P"			      , 0x50);
		constants.emplace_back("mvKey_Q"			      , 0x51);
		constants.emplace_back("mvKey_R"			      , 0x52);
		constants.emplace_back("mvKey_S"			      , 0x53);
		constants.emplace_back("mvKey_T"			      , 0x54);
		constants.emplace_back("mvKey_U"			      , 0x55);
		constants.emplace_back("mvKey_V"			      , 0x56);
		constants.emplace_back("mvKey_W"			      , 0x57);
		constants.emplace_back("mvKey_X"			      , 0x58);
		constants.emplace_back("mvKey_Y"			      , 0x59);
		constants.emplace_back("mvKey_Z"			      , 0x5A);

#if defined (_WIN32)
		constants.emplace_back("mvKey_Back"			      , 0x08);
		constants.emplace_back("mvKey_Tab"			      , 0x09);
		constants.emplace_back("mvKey_Clear"		      , 0x0C);
		constants.emplace_back("mvKey_Return"		      , 0x0D);
		constants.emplace_back("mvKey_Shift"		      , 0x10);
		constants.emplace_back("mvKey_Control"		      , 0x11);
		constants.emplace_back("mvKey_Alt"			      , 0x12);
		constants.emplace_back("mvKey_Pause"		      , 0x13);
		constants.emplace_back("mvKey_Capital"		      , 0x14);
		constants.emplace_back("mvKey_Escape"		      , 0x1B);
		constants.emplace_back("mvKey_Spacebar"		      , 0x20);
		constants.emplace_back("mvKey_Prior"		      , 0x21);
		constants.emplace_back("mvKey_Next"			      , 0x22);
		constants.emplace_back("mvKey_End"			      , 0x23);
		constants.emplace_back("mvKey_Home"			      , 0x24);
		constants.emplace_back("mvKey_Left"			      , 0x25);
		constants.emplace_back("mvKey_Up"			      , 0x26);
		constants.emplace_back("mvKey_Right"		      , 0x27);
		constants.emplace_back("mvKey_Down"			      , 0x28);
		constants.emplace_back("mvKey_Select"		      , 0x29);
		constants.emplace_back("mvKey_Print"		      , 0x2A);
		constants.emplace_back("mvKey_Execute"		      , 0x2B);
		constants.emplace_back("mvKey_PrintScreen"	      , 0x2C);
		constants.emplace_back("mvKey_Insert"		      , 0x2D);
		constants.emplace_back("mvKey_Delete"		      , 0x2E);
		constants.emplace_back("mvKey_Help"			      , 0x2F);
		constants.emplace_back("mvKey_LWin"		          , 0x5B);
		constants.emplace_back("mvKey_RWin"		          , 0x5C);
		constants.emplace_back("mvKey_Apps"		          , 0x5D);
		constants.emplace_back("mvKey_Sleep"		      , 0x5F);
		constants.emplace_back("mvKey_NumPad0"            , 0x60);
		constants.emplace_back("mvKey_NumPad1"            , 0x61);
		constants.emplace_back("mvKey_NumPad2"            , 0x62);
		constants.emplace_back("mvKey_NumPad3"            , 0x63);
		constants.emplace_back("mvKey_NumPad4"            , 0x64);
		constants.emplace_back("mvKey_NumPad5"            , 0x65);
		constants.emplace_back("mvKey_NumPad6"	          , 0x66);
		constants.emplace_back("mvKey_NumPad7"	          , 0x67);
		constants.emplace_back("mvKey_NumPad8"	          , 0x68);
		constants.emplace_back("mvKey_NumPad9"	          , 0x69);
		constants.emplace_back("mvKey_Multiply"	          , 0x6A);
		constants.emplace_back("mvKey_Add"		          , 0x6B);
		constants.emplace_back("mvKey_Separator"	      , 0x6C);
		constants.emplace_back("mvKey_Subtract"	          , 0x6D);
		constants.emplace_back("mvKey_Decimal"	          , 0x6E);
		constants.emplace_back("mvKey_Divide"	          , 0x6F);
		constants.emplace_back("mvKey_F1"		          , 0x70);
		constants.emplace_back("mvKey_F2"		          , 0x71);
		constants.emplace_back("mvKey_F3"		          , 0x72);
		constants.emplace_back("mvKey_F4"		          , 0x73);
		constants.emplace_back("mvKey_F5"		          , 0x74);
		constants.emplace_back("mvKey_F6"		          , 0x75);
		constants.emplace_back("mvKey_F7"		          , 0x76);
		constants.emplace_back("mvKey_F8"		          , 0x77);
		constants.emplace_back("mvKey_F9"		          , 0x78);
		constants.emplace_back("mvKey_F10"		          , 0x79);
		constants.emplace_back("mvKey_F11"		          , 0x7A);
		constants.emplace_back("mvKey_F12"		          , 0x7B);
		constants.emplace_back("mvKey_F13"		          , 0x7C);
		constants.emplace_back("mvKey_F14"		          , 0x7D);
		constants.emplace_back("mvKey_F15"		          , 0x7E);
		constants.emplace_back("mvKey_F16"		          , 0x7F);
		constants.emplace_back("mvKey_F17"		          , 0x80);
		constants.emplace_back("mvKey_F18"		          , 0x81);
		constants.emplace_back("mvKey_F19"		          , 0x82);
		constants.emplace_back("mvKey_F20"		          , 0x83);
		constants.emplace_back("mvKey_F21"		          , 0x84);
		constants.emplace_back("mvKey_F22"		          , 0x85);
		constants.emplace_back("mvKey_F23"		          , 0x86);
		constants.emplace_back("mvKey_F24"		          , 0x87);
		constants.emplace_back("mvKey_NumLock"			  , 0x90);
		constants.emplace_back("mvKey_ScrollLock"		  , 0x91);
		constants.emplace_back("mvKey_LShift"			  , 0xA0);
		constants.emplace_back("mvKey_RShift"			  , 0xA1);
		constants.emplace_back("mvKey_LControl"			  , 0xA2);
		constants.emplace_back("mvKey_RControl"			  , 0xA3);
		constants.emplace_back("mvKey_LMenu"			  , 0xA4);
		constants.emplace_back("mvKey_RMenu"			  , 0xA5);
		constants.emplace_back("mvKey_Browser_Back"		  , 0xA6);
		constants.emplace_back("mvKey_Browser_Forward"	  , 0xA7);
		constants.emplace_back("mvKey_Browser_Refresh"	  , 0xA8);
		constants.emplace_back("mvKey_Browser_Stop"		  , 0xA9);
		constants.emplace_back("mvKey_Browser_Search"	  , 0xAA);
		constants.emplace_back("mvKey_Browser_Favorites"  , 0xAB);
		constants.emplace_back("mvKey_Browser_Home"		  , 0xAC);
		constants.emplace_back("mvKey_Volume_Mute"		  , 0xAD);
		constants.emplace_back("mvKey_Volume_Down"		  , 0xAE);
		constants.emplace_back("mvKey_Volume_Up"		  , 0xAF);
		constants.emplace_back("mvKey_Media_Next_Track"	  , 0xB0);
		constants.emplace_back("mvKey_Media_Prev_Track"	  , 0xB1);
		constants.emplace_back("mvKey_Media_Stop"		  , 0xB2);
		constants.emplace_back("mvKey_Media_Play_Pause"	  , 0xB3);
		constants.emplace_back("mvKey_Launch_Mail"		  , 0xB4);
		constants.emplace_back("mvKey_Launch_Media_Select", 0xB5);
		constants.emplace_back("mvKey_Launch_App1"		  , 0xB6);
		constants.emplace_back("mvKey_Launch_App2"		  , 0xB7);
		constants.emplace_back("mvKey_Colon"			  , 0xBA);
		constants.emplace_back("mvKey_Plus"				  , 0xBB);
		constants.emplace_back("mvKey_Comma"			  , 0xBC);
		constants.emplace_back("mvKey_Minus"			  , 0xBD);
		constants.emplace_back("mvKey_Period"			  , 0xBE);
		constants.emplace_back("mvKey_Slash"			  , 0xBF);
		constants.emplace_back("mvKey_Tilde"			  , 0xC0);
		constants.emplace_back("mvKey_Open_Brace"		  , 0xDB);
		constants.emplace_back("mvKey_Backslash"		  , 0xDC);
		constants.emplace_back("mvKey_Close_Brace"		  , 0xDD);
		constants.emplace_back("mvKey_Quote"			  , 0xDE);
#else
		constants.emplace_back("mvKey_Back", 259 },
		constants.emplace_back("mvKey_Tab", 258 },
		constants.emplace_back("mvKey_Clear", 259 },
		constants.emplace_back("mvKey_Return", 257 },
		constants.emplace_back("mvKey_Shift", 340 },
		constants.emplace_back("mvKey_Control", 241 },
		constants.emplace_back("mvKey_Alt", 342 },
		constants.emplace_back("mvKey_Pause", 284 },
		constants.emplace_back("mvKey_Capital", 280 },
		constants.emplace_back("mvKey_Escape", 256 },
		constants.emplace_back("mvKey_Spacebar", 32 },
		constants.emplace_back("mvKey_Prior", 266 },
		constants.emplace_back("mvKey_Next", 267 },
		constants.emplace_back("mvKey_End", 269 },
		constants.emplace_back("mvKey_Home", 268 },
		constants.emplace_back("mvKey_Left", 263 },
		constants.emplace_back("mvKey_Up", 265 },
		constants.emplace_back("mvKey_Right", 262 },
		constants.emplace_back("mvKey_Down", 264 },
		constants.emplace_back("mvKey_Select", -1 },
		constants.emplace_back("mvKey_Print", -1 },
		constants.emplace_back("mvKey_Execute", -1 },
		constants.emplace_back("mvKey_PrintScreen", 286 },
		constants.emplace_back("mvKey_Insert", 260 },
		constants.emplace_back("mvKey_Delete", 261 },
		constants.emplace_back("mvKey_Help", -1 },
		constants.emplace_back("mvKey_LWin", 343 },
		constants.emplace_back("mvKey_RWin", 347 },
		constants.emplace_back("mvKey_Apps", -1 },
		constants.emplace_back("mvKey_Sleep", -1 },
		constants.emplace_back("mvKey_NumPad0", 320 },
		constants.emplace_back("mvKey_NumPad1", 321 },
		constants.emplace_back("mvKey_NumPad2", 322 },
		constants.emplace_back("mvKey_NumPad3", 323 },
		constants.emplace_back("mvKey_NumPad4", 324 },
		constants.emplace_back("mvKey_NumPad5", 325 },
		constants.emplace_back("mvKey_NumPad6", 326 },
		constants.emplace_back("mvKey_NumPad7", 327 },
		constants.emplace_back("mvKey_NumPad8", 328 },
		constants.emplace_back("mvKey_NumPad9", 329 },
		constants.emplace_back("mvKey_Multiply", 332 },
		constants.emplace_back("mvKey_Add", 334 },
		constants.emplace_back("mvKey_Separator", -1 },
		constants.emplace_back("mvKey_Subtract", 333 },
		constants.emplace_back("mvKey_Decimal", 330 },
		constants.emplace_back("mvKey_Divide", 331 },
		constants.emplace_back("mvKey_F1", 290 },
		constants.emplace_back("mvKey_F2", 291 },
		constants.emplace_back("mvKey_F3", 292 },
		constants.emplace_back("mvKey_F4", 293 },
		constants.emplace_back("mvKey_F5", 294 },
		constants.emplace_back("mvKey_F6", 295 },
		constants.emplace_back("mvKey_F7", 296 },
		constants.emplace_back("mvKey_F8", 297 },
		constants.emplace_back("mvKey_F9", 298 },
		constants.emplace_back("mvKey_F10", 299 },
		constants.emplace_back("mvKey_F11", 300 },
		constants.emplace_back("mvKey_F12", 301 },
		constants.emplace_back("mvKey_F13", 302 },
		constants.emplace_back("mvKey_F14", 303 },
		constants.emplace_back("mvKey_F15", 304 },
		constants.emplace_back("mvKey_F16", 305 },
		constants.emplace_back("mvKey_F17", 306 },
		constants.emplace_back("mvKey_F18", 307 },
		constants.emplace_back("mvKey_F19", 308 },
		constants.emplace_back("mvKey_F20", 309 },
		constants.emplace_back("mvKey_F21", 310 },
		constants.emplace_back("mvKey_F22", 311 },
		constants.emplace_back("mvKey_F23", 312 },
		constants.emplace_back("mvKey_F24", 313 },
		constants.emplace_back("mvKey_F24", 314 },
		constants.emplace_back("mvKey_NumLock", 282 },
		constants.emplace_back("mvKey_ScrollLock", 281 },
		constants.emplace_back("mvKey_LShift", 340 },
		constants.emplace_back("mvKey_RShift", 344 },
		constants.emplace_back("mvKey_LControl", 341 },
		constants.emplace_back("mvKey_RControl", 345 },
		constants.emplace_back("mvKey_LMenu", -1 },
		constants.emplace_back("mvKey_RMenu", -1 },
		constants.emplace_back("mvKey_Browser_Back", -1 },
		constants.emplace_back("mvKey_Browser_Forward", -1 },
		constants.emplace_back("mvKey_Browser_Refresh", -1 },
		constants.emplace_back("mvKey_Browser_Stop", -1 },
		constants.emplace_back("mvKey_Browser_Search", -1 },
		constants.emplace_back("mvKey_Browser_Favorites", -1 },
		constants.emplace_back("mvKey_Browser_Home", -1 },
		constants.emplace_back("mvKey_Volume_Mute", -1 },
		constants.emplace_back("mvKey_Volume_Down", -1 },
		constants.emplace_back("mvKey_Volume_Up", -1 },
		constants.emplace_back("mvKey_Media_Next_Track", -1 },
		constants.emplace_back("mvKey_Media_Prev_Track", -1 },
		constants.emplace_back("mvKey_Media_Stop", -1 },
		constants.emplace_back("mvKey_Media_Play_Pause", -1 },
		constants.emplace_back("mvKey_Launch_Mail", -1 },
		constants.emplace_back("mvKey_Launch_Media_Select", -1 },
		constants.emplace_back("mvKey_Launch_App1", -1 },
		constants.emplace_back("mvKey_Launch_App2", -1 },
		constants.emplace_back("mvKey_Colon", 59 },
		constants.emplace_back("mvKey_Plus", 61 },
		constants.emplace_back("mvKey_Comma", 44 },
		constants.emplace_back("mvKey_Minus", 45 },
		constants.emplace_back("mvKey_Period", 46 },
		constants.emplace_back("mvKey_Slash", 47 },
		constants.emplace_back("mvKey_Tilde", 96 },
		constants.emplace_back("mvKey_Open_Brace", 91 },
		constants.emplace_back("mvKey_Backslash", 92 },
		constants.emplace_back("mvKey_Close_Brace", 93 },
		constants.emplace_back("mvKey_Quote", 39 },
#endif
	}

#ifdef MV_CPP
#else
	void AddInputCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "set_mouse_move_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a mouse move event. Data is the mouse position in local coordinates.", "None", "Input Polling") });

		parsers->insert({ "set_render_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets the callback to be ran every frame.", "None", "Input Polling") });

		parsers->insert({ "get_mouse_pos", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "local", "", "True"}
		}, "Returns the current mouse position in relation to the active window (minus titlebar) unless local flag is unset.",
		"(int, int)", "Input Polling") });

		parsers->insert({ "get_plot_mouse_pos", mvPythonParser({
		}, "Returns the current mouse position in the currently hovered plot.",
		"(int, int)", "Input Polling") });

		parsers->insert({ "get_drawing_mouse_pos", mvPythonParser({
		}, "Returns the current mouse position in the currently hovered drawing.",
		"(int, int)", "Input Polling") });

		parsers->insert({ "get_mouse_drag_delta", mvPythonParser({
		}, "Returns the current mouse drag delta in pixels", "(float, float)", "Input Polling") });

		parsers->insert({ "is_mouse_button_dragging", mvPythonParser({
			{mvPythonDataType::Integer, "button"},
			{mvPythonDataType::Float, "threshold"},
		}, "Checks if the mouse is dragging.", "bool", "Input Polling") });

		parsers->insert({ "is_mouse_button_down", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is pressed.", "bool", "Input Polling") });

		parsers->insert({ "is_mouse_button_clicked", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is clicked.", "bool", "Input Polling") });

		parsers->insert({ "is_mouse_button_released", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is released.", "bool", "Input Polling") });

		parsers->insert({ "is_mouse_button_double_clicked", mvPythonParser({
			{mvPythonDataType::Integer, "button"}
		}, "Checks if the mouse button is double clicked.", "bool", "Input Polling") });

		parsers->insert({ "is_key_pressed", mvPythonParser({
			{mvPythonDataType::Integer, "key"}
		}, "Checks if the key is pressed.", "bool", "Input Polling") });

		parsers->insert({ "is_key_released", mvPythonParser({
			{mvPythonDataType::Integer, "key"}
		}, "Checks if the key is released.", "bool", "Input Polling") });

		parsers->insert({ "is_key_down", mvPythonParser({
			{mvPythonDataType::Integer, "key"}
		}, "Checks if the key is down.", "bool", "Input Polling") });

		parsers->insert({ "set_resize_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "handler", "Callback will be run when window is resized (default is viewport)", "''"},
		}, "Sets a callback for a window resize event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_release_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a mouse release event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_down_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a mouse down event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_drag_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"},
			{mvPythonDataType::Float, "threshold"}
		}, "Sets a callback for a mouse drag event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_wheel_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a mouse wheel event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_double_click_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a mouse double click event.", "None", "Input Polling") });

		parsers->insert({ "set_mouse_click_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a mouse click event.", "None", "Input Polling") });

		parsers->insert({ "set_key_down_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a key down event.", "None", "Input Polling") }),

			parsers->insert({ "set_key_press_callback", mvPythonParser({
				{mvPythonDataType::Callable, "callback", "Registers a callback"}
				}, "Sets a callback for a key press event.", "None", "Input Polling") });

		parsers->insert({ "set_key_release_callback", mvPythonParser({
			{mvPythonDataType::Callable, "callback", "Registers a callback"}
		}, "Sets a callback for a key release event.", "None", "Input Polling") });
	}

	PyObject* get_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int local = true;

		if (!(mvApp::GetApp()->getParsers())["get_mouse_pos"].parse(args, kwargs, __FUNCTION__, &local))
			return GetPyNone();

		mvVec2 pos;

		if (local)
			pos = mvInput::getMousePosition();
		else
			pos = mvInput::getGlobalMousePosition();

		return ToPyPair(pos.x, pos.y);
	}

	PyObject* get_plot_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!(mvApp::GetApp()->getParsers())["get_plot_mouse_pos"].parse(args, kwargs, __FUNCTION__))
			return GetPyNone();

		mvVec2 pos = mvInput::getPlotMousePosition();

		return ToPyPair(pos.x, pos.y);
	}

	PyObject* get_drawing_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!(mvApp::GetApp()->getParsers())["get_drawing_mouse_pos"].parse(args, kwargs, __FUNCTION__))
			return GetPyNone();

		mvVec2 pos = mvInput::getDrawingMousePosition();

		return ToPyPair(pos.x, pos.y);
	}

	PyObject* get_mouse_drag_delta(PyObject* self, PyObject* args)
	{

		mvVec2 pos = mvInput::getMouseDragDelta();
		return ToPyPair(pos.x, pos.y);
	}

	PyObject* is_key_pressed(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(mvApp::GetApp()->getParsers())["is_key_pressed"].parse(args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		return ToPyBool(mvInput::isKeyPressed(key));
	}

	PyObject* is_key_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(mvApp::GetApp()->getParsers())["is_key_released"].parse(args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		return ToPyBool(mvInput::isKeyReleased(key));
	}

	PyObject* is_key_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int key;

		if (!(mvApp::GetApp()->getParsers())["is_key_down"].parse(args, kwargs, __FUNCTION__, &key))
			return GetPyNone();

		return ToPyBool(mvInput::isKeyDown(key));
	}

	PyObject* is_mouse_button_dragging(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;
		float threshold;

		if (!(mvApp::GetApp()->getParsers())["is_mouse_button_dragging"].parse(args, kwargs, __FUNCTION__, &button, &threshold))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseDragging(button, threshold));
	}

	PyObject* is_mouse_button_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(mvApp::GetApp()->getParsers())["is_mouse_button_down"].parse(args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseButtonDown(button));
	}

	PyObject* is_mouse_button_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(mvApp::GetApp()->getParsers())["is_mouse_button_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseButtonClicked(button));
	}

	PyObject* is_mouse_button_double_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(mvApp::GetApp()->getParsers())["is_mouse_button_double_clicked"].parse(args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseButtonDoubleClicked(button));
	}

	PyObject* is_mouse_button_released(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int button;

		if (!(mvApp::GetApp()->getParsers())["is_mouse_button_released"].parse(args, kwargs, __FUNCTION__, &button))
			return GetPyNone();

		return ToPyBool(mvInput::isMouseButtonReleased(button));
	}

	PyObject* set_mouse_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_down_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();

		if (callback)
			Py_XINCREF(callback);

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseDownCallback(callback);
			});
		return GetPyNone();
	}

	PyObject* set_mouse_drag_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		float threshold;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_drag_callback"].parse(args, kwargs, __FUNCTION__,
			&callback, &threshold))
			return GetPyNone();

		mvInput::setMouseDragThreshold(threshold);

		if (callback)
			Py_XINCREF(callback);

		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseDragCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* set_mouse_double_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_double_click_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();

		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseDoubleClickCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* set_mouse_click_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_click_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseClickCallback(callback);
			});
		return GetPyNone();
	}

	PyObject* set_mouse_release_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_release_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseReleaseCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* set_key_down_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_key_down_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setKeyDownCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* set_key_press_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_key_press_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setKeyPressCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* set_key_release_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_key_release_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setKeyReleaseCallback(callback);
			});
		return GetPyNone();
	}

	PyObject* set_mouse_wheel_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_wheel_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseWheelCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* set_mouse_move_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;

		if (!(mvApp::GetApp()->getParsers())["set_mouse_move_callback"].parse(args, kwargs, __FUNCTION__,
			&callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setMouseMoveCallback(callback);
			});

		return GetPyNone();
	}

	PyObject* set_render_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;

		if (!(mvApp::GetApp()->getParsers())["set_render_callback"].parse(args, kwargs, __FUNCTION__, &callback))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().setRenderCallback(callback);
			});
		return GetPyNone();
	}

	PyObject* set_resize_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback = nullptr;
		const char* handler = "";

		if (!(mvApp::GetApp()->getParsers())["set_resize_callback"].parse(args, kwargs, __FUNCTION__,
			&callback, &handler))
			return GetPyNone();
		if (callback)
			Py_XINCREF(callback);


		auto fut = mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{

				if (std::string(handler).empty())
				{
					mvApp::GetApp()->getCallbackRegistry().setResizeCallback(callback);
					return std::string("");
				}

				mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(handler);

				if (item)
				{
					if (item->getDescription().root)
					{
						auto windowtype = static_cast<mvWindowAppItem*>(item.get());
						windowtype->setResizeCallback(callback);
					}
					else
						return std::string("Resize callback can only be set for window items");
				}
				return std::string("");
			});

		auto message = fut.get();

		if (!message.empty())
			ThrowPythonException(message);

		return GetPyNone();
	}
#endif // MV_CPP

}
