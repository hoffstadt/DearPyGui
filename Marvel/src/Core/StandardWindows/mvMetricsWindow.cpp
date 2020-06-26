#include "mvMetricsWindow.h"
#include <imgui.h>
#include "Core/mvApp.h"


namespace Marvel {

	mvMetricsWindow* mvMetricsWindow::s_instance = nullptr;

	mvMetricsWindow* mvMetricsWindow::GetWindow()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvMetricsWindow();
		return s_instance;
	}

	void mvMetricsWindow::render(bool& show)
	{
		if (!ImGui::Begin("MarvelSandbox Metrics", &show, ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
			return;
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("MarvelSandbox %s", mvApp::getVersion());
		ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
		ImGui::Text("%d active windows (%d visible)", io.MetricsActiveWindows, io.MetricsRenderWindows);
		ImGui::Text("%d active allocations", io.MetricsActiveAllocations);
		ImGui::End();
	}

}