#include "mvInput.h"
#include "mvApp.h"
#include "mvProfiler.h"

namespace Marvel {

	mvInput::AtomicVec2  mvInput::s_mousePos = {0, 0};
	mvInput::AtomicVec2  mvInput::s_mouseGlobalPos = {0, 0};
	mvInput::AtomicVec2  mvInput::s_mousePlotPos = {0, 0};
	mvInput::AtomicVec2  mvInput::s_mouseDrawingPos = { 0, 0 };
	std::atomic_int      mvInput::s_mouseDragThreshold = 20;
	mvInput::AtomicVec2  mvInput::s_mouseDragDelta = { 0, 0 };
	std::atomic_bool     mvInput::s_keysdown[512];
	std::atomic_int      mvInput::s_keysdownduration[512]; // 1/100 seconds
	std::atomic_bool     mvInput::s_keyspressed[512];
	std::atomic_bool     mvInput::s_keysreleased[512];
	std::atomic_int      mvInput::s_mousewheel;
	std::atomic_bool     mvInput::s_mousedown[5];
	std::atomic_bool     mvInput::s_mouseDragging[5];
	std::atomic_int      mvInput::s_mousedownduration[5]; // 1/100 seconds
	std::atomic_bool     mvInput::s_mouseclick[5];
	std::atomic_bool     mvInput::s_mousedoubleclick[5];
	std::atomic_bool     mvInput::s_mousereleased[5];

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
		s_mousePlotPos.x = (int)x;
		s_mousePlotPos.y = (int)y;
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

}
