#pragma once
#include "mvTypeBases.h"
#include "mvAppItem.h"
#include "mvInput.h"

namespace Marvel {

	class mvAboutWindow : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE(mvAppItemType::AboutWindow, "add_about_window")

	public:

		mvAboutWindow(const std::string& name) 
			: mvBaseWindowAppitem(name)
		{
			m_windowflags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
			m_description.deleteAllowed = false;
		}

		void draw() override
		{
			if (!prerender())
				return;

			ImGui::Text("Dear PyGui %s", mvApp::GetVersion());
			ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
			ImGui::Separator();
			ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogramHovered), "Authors");
			ImGui::BulletText("Jonathan Hoffstadt");
			ImGui::BulletText("Preston Cothren");
			ImGui::Text("Dear PyGui is licensed under the MIT License, see LICENSE for more information.");
			ImGui::Separator();
			ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogramHovered), "Partners");
			ImGui::BulletText("Dean Keinan");
			ImGui::Separator();
			ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogramHovered), "ImPlot");
			ImGui::Text("ImPlot Author Evan Pezent.");
			ImGui::Separator();
			ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogramHovered), "Dear ImGui");
			ImGui::Text("Dear ImGui Author Omar Cornut and all Dear ImGui contributors.");
			ImGui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");

			static bool show_config_info = false;
			ImGui::Checkbox("Config/Build Information", &show_config_info);
			if (show_config_info)
			{
				ImGuiIO& io = ImGui::GetIO();
				ImGuiStyle& style = ImGui::GetStyle();

				bool copy_to_clipboard = ImGui::Button("Copy to clipboard");
				ImGui::BeginChildFrame(ImGui::GetID("cfginfos"), ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 18), ImGuiWindowFlags_NoMove);
				if (copy_to_clipboard)
				{
					ImGui::LogToClipboard();
					ImGui::LogText("```\n"); // Back quotes will make the text appears without formatting when pasting to GitHub
				}

				ImGui::Text("Dear ImGui %s (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
				ImGui::Separator();
				ImGui::Text("sizeof(size_t): %d, sizeof(ImDrawIdx): %d, sizeof(ImDrawVert): %d", (int)sizeof(size_t), (int)sizeof(ImDrawIdx), (int)sizeof(ImDrawVert));
				ImGui::Text("define: __cplusplus=%d", (int)__cplusplus);
				ImGui::Text("define: _WIN32");
				ImGui::Text("define: _WIN64");
#if defined (_WIN32)
				ImGui::Text("define: _MSC_VER=%d", _MSC_VER);
#endif
				ImGui::Separator();
				ImGui::Text("io.BackendPlatformName: %s", io.BackendPlatformName ? io.BackendPlatformName : "NULL");
				ImGui::Text("io.BackendRendererName: %s", io.BackendRendererName ? io.BackendRendererName : "NULL");
				ImGui::Text("io.ConfigFlags: 0x%08X", io.ConfigFlags);
				if (io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard)        ImGui::Text(" NavEnableKeyboard");
				if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad)         ImGui::Text(" NavEnableGamepad");
				if (io.ConfigFlags & ImGuiConfigFlags_NavEnableSetMousePos)     ImGui::Text(" NavEnableSetMousePos");
				if (io.ConfigFlags & ImGuiConfigFlags_NavNoCaptureKeyboard)     ImGui::Text(" NavNoCaptureKeyboard");
				if (io.ConfigFlags & ImGuiConfigFlags_NoMouse)                  ImGui::Text(" NoMouse");
				if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)      ImGui::Text(" NoMouseCursorChange");
				if (io.MouseDrawCursor)                                         ImGui::Text("io.MouseDrawCursor");
				if (io.ConfigMacOSXBehaviors)                                   ImGui::Text("io.ConfigMacOSXBehaviors");
				if (io.ConfigInputTextCursorBlink)                              ImGui::Text("io.ConfigInputTextCursorBlink");
				if (io.ConfigWindowsResizeFromEdges)                            ImGui::Text("io.ConfigWindowsResizeFromEdges");
				if (io.ConfigWindowsMoveFromTitleBarOnly)                       ImGui::Text("io.ConfigWindowsMoveFromTitleBarOnly");
				if (io.ConfigWindowsMemoryCompactTimer >= 0.0f)                 ImGui::Text("io.ConfigWindowsMemoryCompactTimer = %.1ff", io.ConfigWindowsMemoryCompactTimer);
				ImGui::Text("io.BackendFlags: 0x%08X", io.BackendFlags);
				if (io.BackendFlags & ImGuiBackendFlags_HasGamepad)             ImGui::Text(" HasGamepad");
				if (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors)        ImGui::Text(" HasMouseCursors");
				if (io.BackendFlags & ImGuiBackendFlags_HasSetMousePos)         ImGui::Text(" HasSetMousePos");
				if (io.BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset)   ImGui::Text(" RendererHasVtxOffset");
				ImGui::Separator();
				ImGui::Text("io.Fonts: %d fonts, Flags: 0x%08X, TexSize: %d,%d", io.Fonts->Fonts.Size, io.Fonts->Flags, io.Fonts->TexWidth, io.Fonts->TexHeight);
				ImGui::Text("io.DisplaySize: %.2f,%.2f", io.DisplaySize.x, io.DisplaySize.y);
				ImGui::Text("io.DisplayFramebufferScale: %.2f,%.2f", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
				ImGui::Separator();
				ImGui::Text("style.WindowPadding: %.2f,%.2f", style.WindowPadding.x, style.WindowPadding.y);
				ImGui::Text("style.WindowBorderSize: %.2f", style.WindowBorderSize);
				ImGui::Text("style.FramePadding: %.2f,%.2f", style.FramePadding.x, style.FramePadding.y);
				ImGui::Text("style.FrameRounding: %.2f", style.FrameRounding);
				ImGui::Text("style.FrameBorderSize: %.2f", style.FrameBorderSize);
				ImGui::Text("style.ItemSpacing: %.2f,%.2f", style.ItemSpacing.x, style.ItemSpacing.y);
				ImGui::Text("style.ItemInnerSpacing: %.2f,%.2f", style.ItemInnerSpacing.x, style.ItemInnerSpacing.y);

				if (copy_to_clipboard)
				{
					ImGui::LogText("\n```\n");
					ImGui::LogFinish();
				}
				ImGui::EndChildFrame();
			}

			if (ImGui::IsWindowFocused())
			{

				float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

				// update mouse
				mvVec2 oldMousePos = mvInput::getGlobalMousePosition();
				ImVec2 mousePos = ImGui::GetMousePos();
				mvInput::setGlobalMousePosition(mousePos.x, mousePos.y);
				float x = mousePos.x - ImGui::GetWindowPos().x;
				float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
				mvInput::setMousePosition(x, y);

				mvApp::GetApp()->setActiveWindow(m_name);

				// mouse move callback
				//if (getMouseMoveCallback() != nullptr)
				//{
				//	if (oldMousePos.x != mousePos.x || oldMousePos.y != mousePos.y)
				//	{
				//		mvCallbackRegistry::GetCallbackRegistry()->runCallback(getMouseMoveCallback(), m_name,
				//			ToPyPair(x, y));
				//	}
				//}

			}

			ImGui::End();
		}

	};

}