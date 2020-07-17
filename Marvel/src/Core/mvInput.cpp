#pragma once

#include "mvInput.h"
#include "mvApp.h"

namespace Marvel {

	mvVec2  mvInput::s_mousePos = {0.0f, 0.0f};
	float   mvInput::s_mouseWheel = 0.0f;
	float   mvInput::s_mouseDragThreshold = 20.0f;
	bool    mvInput::s_mouseDragging = false;
	mvVec2  mvInput::s_mouseDragDelta = { 0.0f, 0.0f };

	void mvInput::setMousePosition(float x, float y)
	{
		s_mousePos.x = x;
		s_mousePos.y = y;
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
