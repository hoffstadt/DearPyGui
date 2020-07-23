#include "mvMetricsWindow.h"
#include <imgui.h>
#include "mvApp.h"
#include "Core/mvInput.h"

namespace Marvel {

	void mvMetricsWindow::render(bool& show)
	{
		if (!ImGui::Begin("MarvelSandbox Metrics", &show, ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
			return;
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("MarvelSandbox %s", mvApp::GetVersion());
		ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
		ImGui::Text("%d active windows (%d visible)", io.MetricsActiveWindows, io.MetricsRenderWindows);
		ImGui::Text("%d active allocations", io.MetricsActiveAllocations);

		if (ImGui::IsWindowFocused())
		{

			float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			mvInput::setGlobalMousePosition(mousePos.x, mousePos.y);
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
			mvInput::setMousePosition(x, y);

			mvApp::GetApp()->setActiveWindow("Metrics");

		}

		ImGui::End();
	}

}