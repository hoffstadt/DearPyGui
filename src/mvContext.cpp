#include "mvContext.h"
#include "dearpygui.h"
#include "mvViewport.h"
#include "mvCallbackRegistry.h"
#include <thread>
#include <future>
#include <chrono>
#include "mvProfiler.h"
#include <implot.h>
#include "mvFontManager.h"
#include "mvCallbackRegistry.h"
#include "mvPyUtils.h"
#include <frameobject.h>
#include "mvToolManager.h"
#include <imnodes.h>
#include <thread>
#include <stb_image.h>
#include "mvCustomTypes.h"
#include "mvAppItemCommons.h"
#include "mvItemRegistry.h"

mvContext* GContext = nullptr;

static void
UpdateInputs(mvInput& input)
{

    MV_PROFILE_SCOPE("Input Routing")

    // update mouse
    // mouse move event
    ImVec2 mousepos = ImGui::GetMousePos();
    if (ImGui::IsMousePosValid(&mousepos))
    {
        if (input.mouseGlobalPos.x != mousepos.x || input.mouseGlobalPos.y != mousepos.y)
        {
            input.mouseGlobalPos.x = (i32)mousepos.x;
            input.mouseGlobalPos.y = (i32)mousepos.y;
        }
    }


    // route key events
    for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysData); i++)
    {
        input.keysdown[i] = ImGui::IsKeyDown((ImGuiKey)i);
        input.keyspressed[i] = ImGui::IsKeyPressed((ImGuiKey)i);
        input.keysreleased[i] = ImGui::IsKeyReleased((ImGuiKey)i);

        ImGuiKeyData& key = ImGui::GetIO().KeysData[i];
        // route key down event
        if (key.DownDuration >= 0.0f)
            input.keysdownduration[i] = (i32)(key.DownDuration * 100.0);
    }

    // route mouse wheel event
    if (ImGui::GetIO().MouseWheel != 0.0f)
        input.mousewheel = (i32)ImGui::GetIO().MouseWheel;

    // route mouse dragging event
    for (i32 i = 0; i < 3; i++)
    {
        input.mouseDragging[i] = ImGui::IsMouseDragging(i, (f32)input.mouseDragThreshold);

        if (ImGui::IsMouseDragging(i, (f32)input.mouseDragThreshold))
        {
            input.mouseDragDelta.x = (i32)ImGui::GetMouseDragDelta().x;
            input.mouseDragDelta.y = (i32)ImGui::GetMouseDragDelta().y;
            break;
        }

    }

    // route other mouse events (note mouse move callbacks are handled in mvWindowAppItem)
    for (i32 i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
    {
        input.mousedown[i] = ImGui::GetIO().MouseDown[i];

        // route mouse down event
        if (ImGui::GetIO().MouseDownDuration[i] >= 0.0f)
            input.mousedownduration[i] = (i32)(ImGui::GetIO().MouseDownDuration[i] * 100.0);
        else
            input.mousedownduration[i] = 0;
    }
}

mvUUID 
GenerateUUID() 
{ 
    return ++GContext->id; 
}

void 
SetDefaultTheme()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text] = MV_BASE_COL_textColor;
    colors[ImGuiCol_TextDisabled] = MV_BASE_COL_textDisabledColor;
    colors[ImGuiCol_WindowBg] = mvImGuiCol_WindowBg;
    colors[ImGuiCol_ChildBg] = mvImGuiCol_ChildBg;
    colors[ImGuiCol_PopupBg] = mvImGuiCol_PopupBg;
    colors[ImGuiCol_Border] = mvImGuiCol_Border;
    colors[ImGuiCol_BorderShadow] = mvImGuiCol_BorderShadow;
    colors[ImGuiCol_FrameBg] = mvImGuiCol_FrameBg;
    colors[ImGuiCol_FrameBgHovered] = mvImGuiCol_FrameBgHovered;
    colors[ImGuiCol_FrameBgActive] = mvImGuiCol_FrameBgActive;
    colors[ImGuiCol_TitleBg] = mvImGuiCol_TitleBg;
    colors[ImGuiCol_TitleBgActive] = mvImGuiCol_TitleBgActive;
    colors[ImGuiCol_TitleBgCollapsed] = mvImGuiCol_TitleBgCollapsed;
    colors[ImGuiCol_MenuBarBg] = mvImGuiCol_MenuBarBg;
    colors[ImGuiCol_ScrollbarBg] = mvImGuiCol_ScrollbarBg;
    colors[ImGuiCol_ScrollbarGrab] = mvImGuiCol_ScrollbarGrab;
    colors[ImGuiCol_ScrollbarGrabHovered] = mvImGuiCol_ScrollbarGrabHovered;
    colors[ImGuiCol_ScrollbarGrabActive] = mvImGuiCol_ScrollbarGrabActive;
    colors[ImGuiCol_CheckMark] = mvImGuiCol_CheckMark;
    colors[ImGuiCol_SliderGrab] = mvImGuiCol_SliderGrab;
    colors[ImGuiCol_SliderGrabActive] = mvImGuiCol_SliderGrabActive;
    colors[ImGuiCol_Button] = mvImGuiCol_Button;
    colors[ImGuiCol_ButtonHovered] = mvImGuiCol_ButtonHovered;
    colors[ImGuiCol_ButtonActive] = mvImGuiCol_ButtonActive;
    colors[ImGuiCol_Header] = mvImGuiCol_Header;
    colors[ImGuiCol_HeaderHovered] = mvImGuiCol_HeaderHovered;
    colors[ImGuiCol_HeaderActive] = mvImGuiCol_HeaderActive;
    colors[ImGuiCol_Separator] = mvImGuiCol_Separator;
    colors[ImGuiCol_SeparatorHovered] = mvImGuiCol_SeparatorHovered;
    colors[ImGuiCol_SeparatorActive] = mvImGuiCol_SeparatorActive;
    colors[ImGuiCol_ResizeGrip] = mvImGuiCol_ResizeGrip;
    colors[ImGuiCol_ResizeGripHovered] = mvImGuiCol_ResizeGripHovered;
    colors[ImGuiCol_ResizeGripActive] = mvImGuiCol_ResizeGripHovered;
    colors[ImGuiCol_Tab] = mvImGuiCol_Tab;
    colors[ImGuiCol_TabHovered] = mvImGuiCol_TabHovered;
    colors[ImGuiCol_TabActive] = mvImGuiCol_TabActive;
    colors[ImGuiCol_TabUnfocused] = mvImGuiCol_TabUnfocused;
    colors[ImGuiCol_TabUnfocusedActive] = mvImGuiCol_TabUnfocusedActive;
    colors[ImGuiCol_PlotLines] = mvImGuiCol_PlotLines;
    colors[ImGuiCol_PlotLinesHovered] = mvImGuiCol_PlotLinesHovered;
    colors[ImGuiCol_PlotHistogram] = mvImGuiCol_PlotHistogram;
    colors[ImGuiCol_PlotHistogramHovered] = mvImGuiCol_PlotHistogramHovered;
    colors[ImGuiCol_TableHeaderBg] = mvImGuiCol_TableHeaderBg;
    colors[ImGuiCol_TableBorderStrong] = mvImGuiCol_TableBorderStrong;   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableBorderLight] = mvImGuiCol_TableBorderLight;   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableRowBg] = mvImGuiCol_TableRowBg;
    colors[ImGuiCol_TableRowBgAlt] = mvImGuiCol_TableRowBgAlt;
    colors[ImGuiCol_TextSelectedBg] = mvImGuiCol_TextSelectedBg;
    colors[ImGuiCol_DragDropTarget] = mvImGuiCol_DragDropTarget;
    colors[ImGuiCol_NavHighlight] = mvImGuiCol_NavHighlight;
    colors[ImGuiCol_NavWindowingHighlight] = mvImGuiCol_NavWindowingHighlight;
    colors[ImGuiCol_NavWindowingDimBg] = mvImGuiCol_NavWindowingDimBg;
    colors[ImGuiCol_ModalWindowDimBg] = mvImGuiCol_ModalWindowDimBg;

    ImNodes::GetStyle().Colors[ImNodesCol_NodeBackground] = mvColor::ConvertToUnsignedInt(mvColor(62, 62, 62, 255));
    ImNodes::GetStyle().Colors[ImNodesCol_NodeBackgroundHovered] = mvColor::ConvertToUnsignedInt(mvColor(75, 75, 75, 255));
    ImNodes::GetStyle().Colors[ImNodesCol_NodeBackgroundSelected] = mvColor::ConvertToUnsignedInt(mvColor(75, 75, 75, 255));
    ImNodes::GetStyle().Colors[ImNodesCol_NodeOutline] = mvColor::ConvertToUnsignedInt(mvColor(100, 100, 100, 255));
    ImNodes::GetStyle().Colors[ImNodesCol_TitleBar] = mvColor::ConvertToUnsignedInt(mvImGuiCol_TitleBg);
    ImNodes::GetStyle().Colors[ImNodesCol_TitleBarHovered] = mvColor::ConvertToUnsignedInt(mvImGuiCol_TitleBgActive);
    ImNodes::GetStyle().Colors[ImNodesCol_TitleBarSelected] = mvColor::ConvertToUnsignedInt(mvImGuiCol_FrameBgActive);
    ImNodes::GetStyle().Colors[ImNodesCol_Link] = mvColor::ConvertToUnsignedInt(mvColor(255, 255, 255, 200));
    ImNodes::GetStyle().Colors[ImNodesCol_LinkHovered] = mvColor::ConvertToUnsignedInt(mvColor(66, 150, 250, 255));
    ImNodes::GetStyle().Colors[ImNodesCol_LinkSelected] = mvColor::ConvertToUnsignedInt(mvColor(66, 150, 250, 255));
    ImNodes::GetStyle().Colors[ImNodesCol_Pin] = mvColor::ConvertToUnsignedInt(mvColor(199, 199, 41, 255));
    ImNodes::GetStyle().Colors[ImNodesCol_PinHovered] = mvColor::ConvertToUnsignedInt(mvColor(255, 255, 50, 255));
    ImNodes::GetStyle().Colors[ImNodesCol_BoxSelector] = mvColor::ConvertToUnsignedInt(mvColor(61, 133, 224, 30));
    ImNodes::GetStyle().Colors[ImNodesCol_BoxSelectorOutline] = mvColor::ConvertToUnsignedInt(mvColor(61, 133, 224, 150));
    ImNodes::GetStyle().Colors[ImNodesCol_GridBackground] = mvColor::ConvertToUnsignedInt(mvColor(35, 35, 35, 255));
    ImNodes::GetStyle().Colors[ImNodesCol_GridLine] = mvColor::ConvertToUnsignedInt(mvColor(0, 0, 0, 255));

}

void 
Render()
{

    // update timing
    GContext->deltaTime = ImGui::GetIO().DeltaTime;
    GContext->time = ImGui::GetTime();
    GContext->frame = ImGui::GetFrameCount();
    GContext->framerate = (i32)ImGui::GetIO().Framerate;

    ImGui::GetIO().FontGlobalScale = mvToolManager::GetFontManager().getGlobalFontScale();

    mvFrameCallback(ImGui::GetFrameCount());

    // route input callbacks
    UpdateInputs(GContext->input);

    mvToolManager::Draw();

    {
        std::lock_guard<std::recursive_mutex> lk(GContext->mutex);
        if (GContext->resetTheme)
        {
            SetDefaultTheme();
            GContext->resetTheme = false;
        }

        mvRunTasks();
        RenderItemRegistry(*GContext->itemRegistry);
        mvRunTasks();
    }

    if (GContext->waitOneFrame == true)
        GContext->waitOneFrame = false;
}

std::map<std::string, mvPythonParser>& 
GetParsers()
{ 
    return const_cast<std::map<std::string, mvPythonParser>&>(GetModuleParsers());
}

void 
InsertConstants_mvContext(std::vector<std::pair<std::string, long>>& constants)
{

    //-----------------------------------------------------------------------------
    // Graphics Backends
    //-----------------------------------------------------------------------------
    constants.emplace_back("mvGraphicsBackend_D3D11",  0);
    constants.emplace_back("mvGraphicsBackend_D3D12",  1);
    constants.emplace_back("mvGraphicsBackend_VULKAN", 2);
    constants.emplace_back("mvGraphicsBackend_METAL",  3);
    constants.emplace_back("mvGraphicsBackend_OPENGL", 4);

    //-----------------------------------------------------------------------------
    // Mouse Codes
    //-----------------------------------------------------------------------------
    constants.emplace_back("mvMouseButton_Left", ImGuiMouseButton_Left);
    constants.emplace_back("mvMouseButton_Right", ImGuiMouseButton_Right);
    constants.emplace_back("mvMouseButton_Middle", ImGuiMouseButton_Middle);
    constants.emplace_back("mvMouseButton_X1", ImGuiKey_MouseX1); // TODO: Probably not okay like this
    constants.emplace_back("mvMouseButton_X2", ImGuiKey_MouseX2);

    //-----------------------------------------------------------------------------
    // Key Codes
    //-----------------------------------------------------------------------------
    constants.emplace_back("mvKey_0", ImGuiKey_0);
    constants.emplace_back("mvKey_1", ImGuiKey_1);
    constants.emplace_back("mvKey_2", ImGuiKey_2);
    constants.emplace_back("mvKey_3", ImGuiKey_3);
    constants.emplace_back("mvKey_4", ImGuiKey_4);
    constants.emplace_back("mvKey_5", ImGuiKey_5);
    constants.emplace_back("mvKey_6", ImGuiKey_6);
    constants.emplace_back("mvKey_7", ImGuiKey_7);
    constants.emplace_back("mvKey_8", ImGuiKey_8);
    constants.emplace_back("mvKey_9", ImGuiKey_9);
    constants.emplace_back("mvKey_A", ImGuiKey_A);
    constants.emplace_back("mvKey_B", ImGuiKey_B);
    constants.emplace_back("mvKey_C", ImGuiKey_C);
    constants.emplace_back("mvKey_D", ImGuiKey_D);
    constants.emplace_back("mvKey_E", ImGuiKey_E);
    constants.emplace_back("mvKey_F", ImGuiKey_F);
    constants.emplace_back("mvKey_G", ImGuiKey_G);
    constants.emplace_back("mvKey_H", ImGuiKey_H);
    constants.emplace_back("mvKey_I", ImGuiKey_I);
    constants.emplace_back("mvKey_J", ImGuiKey_J);
    constants.emplace_back("mvKey_K", ImGuiKey_K);
    constants.emplace_back("mvKey_L", ImGuiKey_L);
    constants.emplace_back("mvKey_M", ImGuiKey_M);
    constants.emplace_back("mvKey_N", ImGuiKey_N);
    constants.emplace_back("mvKey_O", ImGuiKey_O);
    constants.emplace_back("mvKey_P", ImGuiKey_P);
    constants.emplace_back("mvKey_Q", ImGuiKey_Q);
    constants.emplace_back("mvKey_R", ImGuiKey_R);
    constants.emplace_back("mvKey_S", ImGuiKey_S);
    constants.emplace_back("mvKey_T", ImGuiKey_T);
    constants.emplace_back("mvKey_U", ImGuiKey_U);
    constants.emplace_back("mvKey_V", ImGuiKey_V);
    constants.emplace_back("mvKey_W", ImGuiKey_W);
    constants.emplace_back("mvKey_X", ImGuiKey_X);
    constants.emplace_back("mvKey_Y", ImGuiKey_Y);
    constants.emplace_back("mvKey_Z", ImGuiKey_Z);

    constants.emplace_back("mvKey_Back", ImGuiKey_Backspace);
    constants.emplace_back("mvKey_Tab", ImGuiKey_Tab);
    // constants.emplace_back("mvKey_Clear", 0x0C);  // FIXME: No Clear in ImGuiKey_
    constants.emplace_back("mvKey_Return", ImGuiKey_Enter);
    constants.emplace_back("mvKey_LShift", ImGuiKey_LeftShift);
    constants.emplace_back("mvKey_RShift", ImGuiKey_RightShift);
    constants.emplace_back("mvKey_LControl", ImGuiKey_LeftCtrl);
    constants.emplace_back("mvKey_RControl", ImGuiKey_RightCtrl);
    constants.emplace_back("mvKey_LAlt", ImGuiKey_LeftAlt);
    constants.emplace_back("mvKey_RAlt", ImGuiKey_RightAlt); // Alt gr ??
    constants.emplace_back("mvKey_Pause", ImGuiKey_Pause);
    constants.emplace_back("mvKey_CapsLock", ImGuiKey_CapsLock);
    constants.emplace_back("mvKey_Escape", ImGuiKey_Escape);
    constants.emplace_back("mvKey_Spacebar", ImGuiKey_Space);
    constants.emplace_back("mvKey_End", ImGuiKey_End);
    constants.emplace_back("mvKey_Home", ImGuiKey_Home);
    constants.emplace_back("mvKey_Left", ImGuiKey_LeftArrow);
    constants.emplace_back("mvKey_Up", ImGuiKey_UpArrow);
    constants.emplace_back("mvKey_Right", ImGuiKey_RightArrow);
    constants.emplace_back("mvKey_Down", ImGuiKey_DownArrow);
    constants.emplace_back("mvKey_PrintScreen", ImGuiKey_PrintScreen);
    constants.emplace_back("mvKey_Insert", ImGuiKey_Insert);
    constants.emplace_back("mvKey_Delete", ImGuiKey_Delete);
    constants.emplace_back("mvKey_NumPad0", ImGuiKey_Keypad0);
    constants.emplace_back("mvKey_NumPad1", ImGuiKey_Keypad1);
    constants.emplace_back("mvKey_NumPad2", ImGuiKey_Keypad2);
    constants.emplace_back("mvKey_NumPad3", ImGuiKey_Keypad3);
    constants.emplace_back("mvKey_NumPad4", ImGuiKey_Keypad4);
    constants.emplace_back("mvKey_NumPad5", ImGuiKey_Keypad5);
    constants.emplace_back("mvKey_NumPad6", ImGuiKey_Keypad6);
    constants.emplace_back("mvKey_NumPad7", ImGuiKey_Keypad7);
    constants.emplace_back("mvKey_NumPad8", ImGuiKey_Keypad8);
    constants.emplace_back("mvKey_NumPad9", ImGuiKey_Keypad9);
    constants.emplace_back("mvKey_Subtract", ImGuiKey_KeypadSubtract);
    constants.emplace_back("mvKey_Decimal", ImGuiKey_KeypadDecimal);
    constants.emplace_back("mvKey_Divide", ImGuiKey_KeypadDivide);
    constants.emplace_back("mvKey_F1", ImGuiKey_F1);
    constants.emplace_back("mvKey_F2", ImGuiKey_F2);
    constants.emplace_back("mvKey_F3", ImGuiKey_F3);
    constants.emplace_back("mvKey_F4", ImGuiKey_F4);
    constants.emplace_back("mvKey_F5", ImGuiKey_F5);
    constants.emplace_back("mvKey_F6", ImGuiKey_F6);
    constants.emplace_back("mvKey_F7", ImGuiKey_F7);
    constants.emplace_back("mvKey_F8", ImGuiKey_F8);
    constants.emplace_back("mvKey_F9", ImGuiKey_F9);
    constants.emplace_back("mvKey_F10", ImGuiKey_F10);
    constants.emplace_back("mvKey_F11", ImGuiKey_F11);
    constants.emplace_back("mvKey_F12", ImGuiKey_F12);
    constants.emplace_back("mvKey_F13", ImGuiKey_F13);
    constants.emplace_back("mvKey_F14", ImGuiKey_F14);
    constants.emplace_back("mvKey_F15", ImGuiKey_F15);
    constants.emplace_back("mvKey_F16", ImGuiKey_F16);
    constants.emplace_back("mvKey_F17", ImGuiKey_F17);
    constants.emplace_back("mvKey_F18", ImGuiKey_F18);
    constants.emplace_back("mvKey_F19", ImGuiKey_F19);
    constants.emplace_back("mvKey_F20", ImGuiKey_F20);
    constants.emplace_back("mvKey_F21", ImGuiKey_F21);
    constants.emplace_back("mvKey_F22", ImGuiKey_F22);
    constants.emplace_back("mvKey_F23", ImGuiKey_F23);
    constants.emplace_back("mvKey_F24", ImGuiKey_F24);
    constants.emplace_back("mvKey_NumLock", ImGuiKey_NumLock);
    constants.emplace_back("mvKey_ScrollLock", ImGuiKey_ScrollLock);
    constants.emplace_back("mvKey_LShift", ImGuiKey_LeftShift);
    constants.emplace_back("mvKey_RShift", ImGuiKey_RightShift);
    constants.emplace_back("mvKey_LControl", ImGuiKey_LeftCtrl);
    constants.emplace_back("mvKey_RControl", ImGuiKey_RightCtrl);
    constants.emplace_back("mvKey_Period", ImGuiKey_Period);
    constants.emplace_back("mvKey_Slash", ImGuiKey_Slash);
    constants.emplace_back("mvKey_Backslash", ImGuiKey_Backslash);
    constants.emplace_back("mvKey_Open_Brace", ImGuiKey_LeftBracket);
    constants.emplace_back("mvKey_Close_Brace", ImGuiKey_RightBracket);
    constants.emplace_back("mvKey_Browser_Back", ImGuiKey_AppBack);
    constants.emplace_back("mvKey_Browser_Forward", ImGuiKey_AppForward);
    constants.emplace_back("mvKey_Multiply", 0x6A);
    constants.emplace_back("mvKey_Add", 0x6B);
    constants.emplace_back("mvKey_Separator", 0x6C);
    // constants.emplace_back("mvKey_Help", 0x2F);
    constants.emplace_back("mvKey_Prior", 0x21);
    constants.emplace_back("mvKey_Next", 0x22);
    constants.emplace_back("mvKey_Select", 0x29);
    constants.emplace_back("mvKey_Print", 0x2A);
    constants.emplace_back("mvKey_Execute", 0x2B);
    constants.emplace_back("mvKey_LWin", 0x5B);
    constants.emplace_back("mvKey_RWin", 0x5C);
    constants.emplace_back("mvKey_Apps", 0x5D);
    constants.emplace_back("mvKey_Sleep", 0x5F);
#if defined (_WIN32)
    constants.emplace_back("mvKey_LMenu", ImGuiKey_Menu);ImGuiKey_Alt
    constants.emplace_back("mvKey_Volume_Down", 0xAE);
    constants.emplace_back("mvKey_Volume_Up", 0xAF);
    constants.emplace_back("mvKey_Media_Next_Track", 0xB0);
    constants.emplace_back("mvKey_Media_Prev_Track", 0xB1);
    constants.emplace_back("mvKey_Media_Stop", 0xB2);
    constants.emplace_back("mvKey_Media_Play_Pause", 0xB3);
    constants.emplace_back("mvKey_Launch_Mail", 0xB4);
    constants.emplace_back("mvKey_Launch_Media_Select", 0xB5);
    constants.emplace_back("mvKey_Launch_App1", 0xB6);
    constants.emplace_back("mvKey_Launch_App2", 0xB7);
    constants.emplace_back("mvKey_Colon", 0xBA);
    constants.emplace_back("mvKey_Plus", 0xBB);
    constants.emplace_back("mvKey_Comma", 0xBC);
    constants.emplace_back("mvKey_Minus", 0xBD);
    constants.emplace_back("mvKey_Slash", 0xBF);
    constants.emplace_back("mvKey_Tilde", 0xC0);
    constants.emplace_back("mvKey_Quote", 0xDE);
#else
    constants.emplace_back("mvKey_Clear", 259);
    constants.emplace_back("mvKey_Prior", 266);
    constants.emplace_back("mvKey_Next", 267);
    constants.emplace_back("mvKey_Select", -1);
    constants.emplace_back("mvKey_Print", -1);
    constants.emplace_back("mvKey_Execute", -1);
    constants.emplace_back("mvKey_Help", -1);
    constants.emplace_back("mvKey_LWin", 343);
    constants.emplace_back("mvKey_RWin", 347);
    constants.emplace_back("mvKey_Apps", -1);
    constants.emplace_back("mvKey_Sleep", -1);
    constants.emplace_back("mvKey_Multiply", 332);
    constants.emplace_back("mvKey_Add", 334);
    constants.emplace_back("mvKey_Separator", -1);
    constants.emplace_back("mvKey_F25", 314);
    constants.emplace_back("mvKey_LMenu", -1);
    constants.emplace_back("mvKey_RMenu", -1);
    constants.emplace_back("mvKey_Browser_Refresh", -1);
    constants.emplace_back("mvKey_Browser_Stop", -1);
    constants.emplace_back("mvKey_Browser_Search", -1);
    constants.emplace_back("mvKey_Browser_Favorites", -1);
    constants.emplace_back("mvKey_Browser_Home", -1);
    constants.emplace_back("mvKey_Volume_Mute", -1);
    constants.emplace_back("mvKey_Volume_Down", -1);
    constants.emplace_back("mvKey_Volume_Up", -1);
    constants.emplace_back("mvKey_Media_Next_Track", -1);
    constants.emplace_back("mvKey_Media_Prev_Track", -1);
    constants.emplace_back("mvKey_Media_Stop", -1);
    constants.emplace_back("mvKey_Media_Play_Pause", -1);
    constants.emplace_back("mvKey_Launch_Mail", -1);
    constants.emplace_back("mvKey_Launch_Media_Select", -1);
    constants.emplace_back("mvKey_Launch_App1", -1);
    constants.emplace_back("mvKey_Launch_App2", -1);
    constants.emplace_back("mvKey_Colon", 59);
    constants.emplace_back("mvKey_Plus", 61);
    constants.emplace_back("mvKey_Comma", 44);
    constants.emplace_back("mvKey_Minus", 45);
    constants.emplace_back("mvKey_Tilde", 96);
    constants.emplace_back("mvKey_Quote", 39);
#endif
}
