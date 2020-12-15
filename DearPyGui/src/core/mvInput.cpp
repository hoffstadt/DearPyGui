#include "mvInput.h"
#include "mvApp.h"
#include "mvProfiler.h"

namespace Marvel {

	mvVec2  mvInput::s_mousePos = {0.0f, 0.0f};
	mvVec2  mvInput::s_mouseGlobalPos = {0.0f, 0.0f};
	mvVec2  mvInput::s_mousePlotPos = {0.0f, 0.0f};
	float   mvInput::s_mouseDragThreshold = 20.0f;
	bool    mvInput::s_mouseDragging = false;
	mvVec2  mvInput::s_mouseDragDelta = { 0.0f, 0.0f };

	void mvInput::CheckInputs()
	{
		MV_PROFILE_FUNCTION();

		// route key events
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
		{
			// route key pressed event
			if (ImGui::IsKeyPressed(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_KEY_PRESS, { CreateEventArgument("KEY", i) });

			// route key down event
			if (ImGui::GetIO().KeysDownDuration[i] >= 0.0f)
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_KEY_DOWN,
					{ CreateEventArgument("KEY", i), CreateEventArgument("DURATION", ImGui::GetIO().KeysDownDuration[i]) });

			// route key released event
			if (ImGui::IsKeyReleased(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_KEY_RELEASE, { CreateEventArgument("KEY", i) });
		}

		// route mouse wheel event
		if (ImGui::GetIO().MouseWheel != 0.0f)
			mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_WHEEL, { CreateEventArgument("DELTA", ImGui::GetIO().MouseWheel) });

		// route mouse dragging event
		for (int i = 0; i < 3; i++)
		{
			if (ImGui::IsMouseDragging(i, mvInput::getMouseDragThreshold()))
			{
				// TODO: send delta
				mvInput::setMouseDragging(true);
				mvInput::setMouseDragDelta({ ImGui::GetMouseDragDelta().x, ImGui::GetMouseDragDelta().y });
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_DRAG,
					{ CreateEventArgument("BUTTON", i),
					CreateEventArgument("X", ImGui::GetMouseDragDelta().x),
					CreateEventArgument("Y", ImGui::GetMouseDragDelta().y)
					});
				ImGui::ResetMouseDragDelta(i);
				break;
			}

			// reset, since event has already been dispatched
			mvInput::setMouseDragging(false);
			mvInput::setMouseDragDelta({ 0.0f, 0.0f });
		}

		// route other mouse events (note mouse move callbacks are handled in mvWindowAppItem)
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
		{
			// route mouse click event
			if (ImGui::IsMouseClicked(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_CLICK, { CreateEventArgument("BUTTON", i) });

			// route mouse down event
			if (ImGui::GetIO().MouseDownDuration[i] >= 0.0f)
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_DOWN,
					{ CreateEventArgument("BUTTON", i), CreateEventArgument("DURATION",  ImGui::GetIO().MouseDownDuration[i]) });

			// route mouse double clicked event
			if (ImGui::IsMouseDoubleClicked(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_DBL_CLK, { CreateEventArgument("BUTTON", i) });

			// route mouse released event
			if (ImGui::IsMouseReleased(i))
				mvEventBus::Publish(mvEVT_CATEGORY_INPUT, mvEVT_MOUSE_RELEASE, { CreateEventArgument("BUTTON", i) });
		}
	}

	void mvInput::setMousePosition(float x, float y)
	{
		s_mousePos.x = x;
		s_mousePos.y = y;
	}

	void mvInput::setGlobalMousePosition(float x, float y)
	{
		s_mouseGlobalPos.x = x;
		s_mouseGlobalPos.y = y;
	}

	void mvInput::setPlotMousePosition(float x, float y)
	{
		s_mousePlotPos.x = x;
		s_mousePlotPos.y = y;
	}

	void mvInput::setMouseDragThreshold(float threshold)
	{
		s_mouseDragThreshold = threshold;
	}

	void mvInput::setMouseDragging(bool drag)
	{
		s_mouseDragging = drag;
	}

	void mvInput::setMouseDragDelta(const mvVec2& delta)
	{
		s_mouseDragDelta.x = delta.x;
		s_mouseDragDelta.y = delta.y;
	}

	float mvInput::getMouseDragThreshold()
	{
		return s_mouseDragThreshold;
	}

	const mvVec2& mvInput::getMouseDragDelta()
	{
		return s_mouseDragDelta;
	}

	const mvVec2& mvInput::getMousePosition()
	{
		return s_mousePos;
	}

	const mvVec2& mvInput::getGlobalMousePosition()
	{
		return s_mouseGlobalPos;
	}

	const mvVec2& mvInput::getPlotMousePosition()
	{
		return s_mousePlotPos;
	}

	bool mvInput::isMouseDragging(int button, float threshold)
	{
		if (!mvApp::IsAppStarted())
			return false;
		return ImGui::IsMouseDragging(button, threshold);
	}

	bool mvInput::isMouseButtonDown(int button)
	{
		if (!mvApp::IsAppStarted())
			return false;
		return ImGui::IsMouseDown(button);
	}

	bool mvInput::isMouseButtonClicked(int button)
	{
		if (!mvApp::IsAppStarted())
			return false;
		return ImGui::IsMouseClicked(button);
	}

	bool mvInput::isMouseButtonDoubleClicked(int button)
	{
		if (!mvApp::IsAppStarted())
			return false;
		return ImGui::IsMouseDoubleClicked(button);
	}

	bool mvInput::isMouseButtonReleased(int button)
	{
		if (!mvApp::IsAppStarted())
			return false;
		return ImGui::IsMouseReleased(button);
	}

	bool mvInput::isKeyPressed(int keycode)
	{
		if (!mvApp::IsAppStarted())
			return false;
		return ImGui::IsKeyPressed(keycode);
	}

	bool mvInput::isKeyReleased(int keycode)
	{
		if (!mvApp::IsAppStarted())
			return false;
		return ImGui::IsKeyReleased(keycode);
	}

	bool mvInput::isKeyDown(int keycode)
	{
		if (!mvApp::IsAppStarted())
			return false;
		return ImGui::IsKeyDown(keycode);
	}

}
