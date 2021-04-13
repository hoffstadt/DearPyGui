#include "mvAboutWindow.h"
#include "mvTypeBases.h"
#include "mvAppItem.h"
#include "mvInput.h"
#include "mvModule_Core.h"
#include "mvApp.h"
#include <string>
#include "mvItemRegistry.h"
#include "mvFontScope.h"

namespace Marvel {

    void mvAboutWindow::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {

        mvPythonParser parser(mvPyDataType::String);
        mvAppItem::AddCommonArgs(parser);
        parser.removeArg("parent");
        parser.removeArg("before");
        parser.removeArg("source");
        parser.removeArg("callback");
        parser.removeArg("callback_data");
        parser.removeArg("enabled");

        parser.addArg<mvPyDataType::Integer>("x_pos", mvArgType::KEYWORD_ARG, "200");
        parser.addArg<mvPyDataType::Integer>("y_pos", mvArgType::KEYWORD_ARG, "200");

        parser.addArg<mvPyDataType::Bool>("autosize", mvArgType::KEYWORD_ARG, "False", "Autosized the window to fit it's items.");
        parser.addArg<mvPyDataType::Bool>("no_resize", mvArgType::KEYWORD_ARG, "False", "Allows for the window size to be changed or fixed");
        parser.addArg<mvPyDataType::Bool>("no_title_bar", mvArgType::KEYWORD_ARG, "False", "Title name for the title bar of the window");
        parser.addArg<mvPyDataType::Bool>("no_move", mvArgType::KEYWORD_ARG, "False", "Allows for the window's position to be changed or fixed");
        parser.addArg<mvPyDataType::Bool>("no_scrollbar", mvArgType::KEYWORD_ARG, "False", " Disable scrollbars (window can still scroll with mouse or programmatically)");
        parser.addArg<mvPyDataType::Bool>("no_collapse", mvArgType::KEYWORD_ARG, "False", "Disable user collapsing window by double-clicking on it");
        parser.addArg<mvPyDataType::Bool>("horizontal_scrollbar", mvArgType::KEYWORD_ARG, "False", "Allow horizontal scrollbar to appear (off by default).");
        parser.addArg<mvPyDataType::Bool>("no_focus_on_appearing", mvArgType::KEYWORD_ARG, "False", "Disable taking focus when transitioning from hidden to visible state");
        parser.addArg<mvPyDataType::Bool>("no_bring_to_front_on_focus", mvArgType::KEYWORD_ARG, "False", "Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)");
        parser.addArg<mvPyDataType::Bool>("no_close", mvArgType::KEYWORD_ARG, "False");
        parser.addArg<mvPyDataType::Bool>("no_background", mvArgType::KEYWORD_ARG, "False");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvAboutWindow::mvAboutWindow(const std::string& name)
        : mvBaseWindowAppitem(name)
    {
        m_windowflags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
    }

    void mvAboutWindow::draw(ImDrawList* drawlist, float x, float y)
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
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogramHovered), "Corporate");
        ImGui::BulletText("Anthony Tanbakuchi");
        ImGui::Separator();
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogramHovered), "Partners");
        ImGui::BulletText("Dean Keinan");
        ImGui::BulletText("He Lingfeng");
        ImGui::BulletText("Anthony Doupe");
        ImGui::BulletText("Ray Heasman");
        ImGui::BulletText("Marco Studer");
        ImGui::Separator();
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogramHovered), "ImPlot");
        ImGui::Text("ImPlot Author Evan Pezent.");
        ImGui::Separator();
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogramHovered), "imnodes");
        ImGui::Text("imnodes Author Johann Muszynski.");
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
            ImVec2 mousePos = ImGui::GetMousePos();
            float x = mousePos.x - ImGui::GetWindowPos().x;
            float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
            mvInput::setMousePosition(x, y);

            if (mvApp::GetApp()->getItemRegistry().getActiveWindow() != m_name)
                mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", m_name) });
        }

        ImGui::End();
        
    }

}
