#include "mvDebugWindow.h"
#include <misc/cpp/imgui_stdlib.h>
#include "Core/mvApp.h"

namespace Marvel {

	static void DebugItem(const char* label, const char* item)
	{
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), item);
	}

	static void DebugItem(const char* label, float x)
	{
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), std::to_string(x).c_str());
	}

	static void DebugItem(const char* label, float x, float y)
	{
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), (std::to_string(x) + ", " + std::to_string(y)).c_str());
	}

	void mvDebugWindow::renderItem(mvAppItem* item)
	{
		if (item->isContainer())
		{
			std::string container = item->getName() + "*";

			if (ImGui::TreeNodeEx(container.c_str()))
			{
				if (ImGui::Selectable(item->getName().c_str(), item->getName() == m_selectedItem))
					m_selectedItem = item->getName();

				for (mvAppItem* child : item->getChildren())
					renderItem(child);

				ImGui::TreePop();
			}
		}
		else
		{
			if (ImGui::Selectable(item->getName().c_str(), item->getName() == m_selectedItem))
				m_selectedItem = item->getName();
		}
	}

	void mvDebugWindow::render(bool& show)
	{
		static char ts[6] = "True";
		static char fs[6] = "False";
		ImGuiIO& io = ImGui::GetIO();

		auto app = mvApp::GetApp();

		ImGui::SetNextWindowSize(ImVec2(m_width, m_width), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Marvel Debug", &show, m_flags))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginTabBar("Main Tabbar"))
		{
			if (ImGui::BeginTabItem("App State"))
			{

				ImGui::PushItemWidth(200);
				ImGui::BeginGroup();
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
				ImGui::Text("%d active allocations", io.MetricsActiveAllocations);
				DebugItem("Marvel Version: ", mvApp::GetVersion());
				DebugItem("ImGui Version: ", IMGUI_VERSION);
				DebugItem("App File: ", app->getFile().c_str());
				DebugItem("Stored Data: ", std::to_string(app->getDataCount()).c_str());
				DebugItem("Threads Active: ", std::to_string(app->getThreadCount()).c_str());
				DebugItem("Threadpool Timeout: ", std::to_string(app->getThreadPoolTimeout()).c_str());
				DebugItem("Threadpool Active: ", app->usingThreadPool() ? ts : fs);
				DebugItem("Threadpool High: ", app->usingThreadPoolHighPerformance() ? ts : fs);
				DebugItem("Main Callback: ", app->getRenderCallback().c_str());
				DebugItem("Mouse Release Callback: ", app->getMouseReleaseCallback().c_str());
				DebugItem("Mouse Click Callback: ", app->getMouseClickCallback().c_str());
				DebugItem("Mouse Double Click Callback: ", app->getMouseDoubleClickCallback().c_str());
				DebugItem("Mouse Drag Callback: ", app->getMouseDragCallback().c_str());
				DebugItem("Mouse Wheel Callback: ", app->getMouseWheelCallback().c_str());
				DebugItem("Key Down Callback: ", app->getKeyDownCallback().c_str());
				DebugItem("Key Press Callback: ", app->getKeyPressCallback().c_str());
				DebugItem("Key Release Callback: ", app->getKeyReleaseCallback().c_str());

				ImGui::EndGroup();
				ImGui::PopItemWidth();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Input Polling"))
			{

				ImGui::PushItemWidth(200);
				ImGui::BeginGroup();

				auto mousepos = app->getMousePosition();
				DebugItem("Active Window: ", app->getActiveWindow().c_str());
				DebugItem("Local Mouse Position:", mousepos.x, mousepos.y);
				DebugItem("Global Mouse Position:", io.MousePos.x, io.MousePos.y);
				DebugItem("Mouse Drag Delta:", app->getMouseDragDelta().x, app->getMouseDragDelta().y);
				DebugItem("Mouse Drag Threshold:", app->getMouseDragThreshold());

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Text("ImGui State Inputs");

				ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
				ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
				ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
				ImGui::Text("Mouse dblclick:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
				ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
				ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

				ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("%d (0x%X) (%.02f secs)", i, i, io.KeysDownDuration[i]); }
				ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
				ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
				ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
				ImGui::Text("Chars queue:");    for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); }

				ImGui::EndGroup();
				ImGui::PopItemWidth();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("App Item Layout"))
			{
				auto selectedItem = mvApp::GetApp()->getItem(m_selectedItem);
				std::string parentName = "";
				
				if (selectedItem->getParent())
					parentName = selectedItem->getParent()->getName();

				std::string width = std::to_string(selectedItem->getWidth());
				std::string height = std::to_string(selectedItem->getHeight());

				if (selectedItem)
				{	
					
					ImGui::BeginGroup();

					if (ImGui::ArrowButton("Move Up", ImGuiDir_Up))
						app->moveItemUp(m_selectedItem);
					ImGui::SameLine();
					if (ImGui::ArrowButton("Move Down", ImGuiDir_Down))
						app->moveItemDown(m_selectedItem);
					ImGui::SameLine();
					if (ImGui::Button("Delete"))
					{
						app->deleteItem(m_selectedItem);
						m_selectedItem = "MainWindow";
					}
					ImGui::SameLine();
					if (ImGui::Button("Show"))
						app->getItem(m_selectedItem)->show();
					ImGui::SameLine();
					if (ImGui::Button("Hide"))
						app->getItem(m_selectedItem)->hide();

					ImGui::PushItemWidth(200);
					DebugItem("Item Name:", m_selectedItem.c_str());
					DebugItem("Item Type:", selectedItem->getStringType().c_str());
					DebugItem("Container:", selectedItem->isContainer() ? ts : fs);
					DebugItem("Item Parent:", parentName.c_str());
					DebugItem("Item Width:", width.c_str());
					DebugItem("Item Height:", height.c_str());
					DebugItem("Item Callback:", selectedItem->getCallback().c_str());
					DebugItem("Item Tip:", selectedItem->getTip().c_str());
					DebugItem("Item Popup:",selectedItem->getPopup().c_str());
					DebugItem("Item Show:", selectedItem->isShown() ? ts : fs);
					DebugItem("Item Visible:", selectedItem->isItemVisible() ? ts : fs);
					DebugItem("Item Hovered:", selectedItem->isItemHovered() ? ts : fs);
					DebugItem("Item Active:", selectedItem->isItemActive() ? ts : fs);
					DebugItem("Item Focused:", selectedItem->isItemFocused() ? ts : fs);
					DebugItem("Item Clicked:", selectedItem->isItemClicked() ? ts : fs);
					DebugItem("Item Edited:", selectedItem->isItemEdited() ? ts : fs);
					DebugItem("Item Activated:", selectedItem->isItemActivated() ? ts : fs);
					DebugItem("Item Deactivated:", selectedItem->isItemDeactivated() ? ts : fs);
					DebugItem("Item DeavtivatedAfterEdit:", selectedItem->isItemDeactivatedAfterEdit() ? ts : fs);
					DebugItem("Item ToggledOpen:", selectedItem->isItemToogledOpen() ? ts : fs);
					ImGui::EndGroup();
					ImGui::PopItemWidth();
					ImGui::SameLine();
				}

				ImGui::BeginChild("TreeChild", ImVec2(300.0f, 500.0f), true);
				for (auto window : mvApp::GetApp()->getWindows())
					renderItem(window);
				ImGui::EndChild();
				
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		if (ImGui::IsWindowFocused())
		{

			float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
			mvApp::GetApp()->setMousePosition(x, y);

			mvApp::GetApp()->setActiveWindow("Debug");

		}
		
		ImGui::End();


	}

}