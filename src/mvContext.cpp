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
        input.keysdown[i] = ImGui::IsKeyDown(i);
        input.keyspressed[i] = ImGui::IsKeyPressed(i);
        input.keysreleased[i] = ImGui::IsKeyReleased(i);

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
    constants.emplace_back("mvMouseButton_Left", 0);
    constants.emplace_back("mvMouseButton_Right", 1);
    constants.emplace_back("mvMouseButton_Middle", 2);
    constants.emplace_back("mvMouseButton_X1", 3);
    constants.emplace_back("mvMouseButton_X2", 4);

    //-----------------------------------------------------------------------------
    // Key Codes
    //-----------------------------------------------------------------------------
    constants.emplace_back("mvKey_0", 0x30);
    constants.emplace_back("mvKey_1", 0x31);
    constants.emplace_back("mvKey_2", 0x32);
    constants.emplace_back("mvKey_3", 0x33);
    constants.emplace_back("mvKey_4", 0x34);
    constants.emplace_back("mvKey_5", 0x35);
    constants.emplace_back("mvKey_6", 0x36);
    constants.emplace_back("mvKey_7", 0x37);
    constants.emplace_back("mvKey_8", 0x38);
    constants.emplace_back("mvKey_9", 0x39);
    constants.emplace_back("mvKey_A", 0x41);
    constants.emplace_back("mvKey_B", 0x42);
    constants.emplace_back("mvKey_C", 0x43);
    constants.emplace_back("mvKey_D", 0x44);
    constants.emplace_back("mvKey_E", 0x45);
    constants.emplace_back("mvKey_F", 0x46);
    constants.emplace_back("mvKey_G", 0x47);
    constants.emplace_back("mvKey_H", 0x48);
    constants.emplace_back("mvKey_I", 0x49);
    constants.emplace_back("mvKey_J", 0x4A);
    constants.emplace_back("mvKey_K", 0x4B);
    constants.emplace_back("mvKey_L", 0x4C);
    constants.emplace_back("mvKey_M", 0x4D);
    constants.emplace_back("mvKey_N", 0x4E);
    constants.emplace_back("mvKey_O", 0x4F);
    constants.emplace_back("mvKey_P", 0x50);
    constants.emplace_back("mvKey_Q", 0x51);
    constants.emplace_back("mvKey_R", 0x52);
    constants.emplace_back("mvKey_S", 0x53);
    constants.emplace_back("mvKey_T", 0x54);
    constants.emplace_back("mvKey_U", 0x55);
    constants.emplace_back("mvKey_V", 0x56);
    constants.emplace_back("mvKey_W", 0x57);
    constants.emplace_back("mvKey_X", 0x58);
    constants.emplace_back("mvKey_Y", 0x59);
    constants.emplace_back("mvKey_Z", 0x5A);

#if defined (_WIN32)
    constants.emplace_back("mvKey_Back", 0x08);
    constants.emplace_back("mvKey_Tab", 0x09);
    constants.emplace_back("mvKey_Clear", 0x0C);
    constants.emplace_back("mvKey_Return", 0x0D);
    constants.emplace_back("mvKey_Shift", 0x10);
    constants.emplace_back("mvKey_Control", 0x11);
    constants.emplace_back("mvKey_Alt", 0x12);
    constants.emplace_back("mvKey_Pause", 0x13);
    constants.emplace_back("mvKey_Capital", 0x14);
    constants.emplace_back("mvKey_Escape", 0x1B);
    constants.emplace_back("mvKey_Spacebar", 0x20);
    constants.emplace_back("mvKey_Prior", 0x21);
    constants.emplace_back("mvKey_Next", 0x22);
    constants.emplace_back("mvKey_End", 0x23);
    constants.emplace_back("mvKey_Home", 0x24);
    constants.emplace_back("mvKey_Left", 0x25);
    constants.emplace_back("mvKey_Up", 0x26);
    constants.emplace_back("mvKey_Right", 0x27);
    constants.emplace_back("mvKey_Down", 0x28);
    constants.emplace_back("mvKey_Select", 0x29);
    constants.emplace_back("mvKey_Print", 0x2A);
    constants.emplace_back("mvKey_Execute", 0x2B);
    constants.emplace_back("mvKey_PrintScreen", 0x2C);
    constants.emplace_back("mvKey_Insert", 0x2D);
    constants.emplace_back("mvKey_Delete", 0x2E);
    constants.emplace_back("mvKey_Help", 0x2F);
    constants.emplace_back("mvKey_LWin", 0x5B);
    constants.emplace_back("mvKey_RWin", 0x5C);
    constants.emplace_back("mvKey_Apps", 0x5D);
    constants.emplace_back("mvKey_Sleep", 0x5F);
    constants.emplace_back("mvKey_NumPad0", 0x60);
    constants.emplace_back("mvKey_NumPad1", 0x61);
    constants.emplace_back("mvKey_NumPad2", 0x62);
    constants.emplace_back("mvKey_NumPad3", 0x63);
    constants.emplace_back("mvKey_NumPad4", 0x64);
    constants.emplace_back("mvKey_NumPad5", 0x65);
    constants.emplace_back("mvKey_NumPad6", 0x66);
    constants.emplace_back("mvKey_NumPad7", 0x67);
    constants.emplace_back("mvKey_NumPad8", 0x68);
    constants.emplace_back("mvKey_NumPad9", 0x69);
    constants.emplace_back("mvKey_Multiply", 0x6A);
    constants.emplace_back("mvKey_Add", 0x6B);
    constants.emplace_back("mvKey_Separator", 0x6C);
    constants.emplace_back("mvKey_Subtract", 0x6D);
    constants.emplace_back("mvKey_Decimal", 0x6E);
    constants.emplace_back("mvKey_Divide", 0x6F);
    constants.emplace_back("mvKey_F1", 0x70);
    constants.emplace_back("mvKey_F2", 0x71);
    constants.emplace_back("mvKey_F3", 0x72);
    constants.emplace_back("mvKey_F4", 0x73);
    constants.emplace_back("mvKey_F5", 0x74);
    constants.emplace_back("mvKey_F6", 0x75);
    constants.emplace_back("mvKey_F7", 0x76);
    constants.emplace_back("mvKey_F8", 0x77);
    constants.emplace_back("mvKey_F9", 0x78);
    constants.emplace_back("mvKey_F10", 0x79);
    constants.emplace_back("mvKey_F11", 0x7A);
    constants.emplace_back("mvKey_F12", 0x7B);
    constants.emplace_back("mvKey_F13", 0x7C);
    constants.emplace_back("mvKey_F14", 0x7D);
    constants.emplace_back("mvKey_F15", 0x7E);
    constants.emplace_back("mvKey_F16", 0x7F);
    constants.emplace_back("mvKey_F17", 0x80);
    constants.emplace_back("mvKey_F18", 0x81);
    constants.emplace_back("mvKey_F19", 0x82);
    constants.emplace_back("mvKey_F20", 0x83);
    constants.emplace_back("mvKey_F21", 0x84);
    constants.emplace_back("mvKey_F22", 0x85);
    constants.emplace_back("mvKey_F23", 0x86);
    constants.emplace_back("mvKey_F24", 0x87);
    constants.emplace_back("mvKey_F25", 0x88);
    constants.emplace_back("mvKey_NumLock", 0x90);
    constants.emplace_back("mvKey_ScrollLock", 0x91);
    constants.emplace_back("mvKey_LShift", 0xA0);
    constants.emplace_back("mvKey_RShift", 0xA1);
    constants.emplace_back("mvKey_LControl", 0xA2);
    constants.emplace_back("mvKey_RControl", 0xA3);
    constants.emplace_back("mvKey_LMenu", 0xA4);
    constants.emplace_back("mvKey_RMenu", 0xA5);
    constants.emplace_back("mvKey_Browser_Back", 0xA6);
    constants.emplace_back("mvKey_Browser_Forward", 0xA7);
    constants.emplace_back("mvKey_Browser_Refresh", 0xA8);
    constants.emplace_back("mvKey_Browser_Stop", 0xA9);
    constants.emplace_back("mvKey_Browser_Search", 0xAA);
    constants.emplace_back("mvKey_Browser_Favorites", 0xAB);
    constants.emplace_back("mvKey_Browser_Home", 0xAC);
    constants.emplace_back("mvKey_Volume_Mute", 0xAD);
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
    constants.emplace_back("mvKey_Period", 0xBE);
    constants.emplace_back("mvKey_Slash", 0xBF);
    constants.emplace_back("mvKey_Tilde", 0xC0);
    constants.emplace_back("mvKey_Open_Brace", 0xDB);
    constants.emplace_back("mvKey_Backslash", 0xDC);
    constants.emplace_back("mvKey_Close_Brace", 0xDD);
    constants.emplace_back("mvKey_Quote", 0xDE);
#else
    constants.emplace_back("mvKey_Back", 259);
    constants.emplace_back("mvKey_Tab", 258);
    constants.emplace_back("mvKey_Clear", 259);
    constants.emplace_back("mvKey_Return", 257);
    constants.emplace_back("mvKey_Shift", 340);
    constants.emplace_back("mvKey_Control", 341);
    constants.emplace_back("mvKey_Alt", 342);
    constants.emplace_back("mvKey_Pause", 284);
    constants.emplace_back("mvKey_Capital", 280);
    constants.emplace_back("mvKey_Escape", 256);
    constants.emplace_back("mvKey_Spacebar", 32);
    constants.emplace_back("mvKey_Prior", 266);
    constants.emplace_back("mvKey_Next", 267);
    constants.emplace_back("mvKey_End", 269);
    constants.emplace_back("mvKey_Home", 268);
    constants.emplace_back("mvKey_Left", 263);
    constants.emplace_back("mvKey_Up", 265);
    constants.emplace_back("mvKey_Right", 262);
    constants.emplace_back("mvKey_Down", 264);
    constants.emplace_back("mvKey_Select", -1);
    constants.emplace_back("mvKey_Print", -1);
    constants.emplace_back("mvKey_Execute", -1);
    constants.emplace_back("mvKey_PrintScreen", 286);
    constants.emplace_back("mvKey_Insert", 260);
    constants.emplace_back("mvKey_Delete", 261);
    constants.emplace_back("mvKey_Help", -1);
    constants.emplace_back("mvKey_LWin", 343);
    constants.emplace_back("mvKey_RWin", 347);
    constants.emplace_back("mvKey_Apps", -1);
    constants.emplace_back("mvKey_Sleep", -1);
    constants.emplace_back("mvKey_NumPad0", 320);
    constants.emplace_back("mvKey_NumPad1", 321);
    constants.emplace_back("mvKey_NumPad2", 322);
    constants.emplace_back("mvKey_NumPad3", 323);
    constants.emplace_back("mvKey_NumPad4", 324);
    constants.emplace_back("mvKey_NumPad5", 325);
    constants.emplace_back("mvKey_NumPad6", 326);
    constants.emplace_back("mvKey_NumPad7", 327);
    constants.emplace_back("mvKey_NumPad8", 328);
    constants.emplace_back("mvKey_NumPad9", 329);
    constants.emplace_back("mvKey_Multiply", 332);
    constants.emplace_back("mvKey_Add", 334);
    constants.emplace_back("mvKey_Separator", -1);
    constants.emplace_back("mvKey_Subtract", 333);
    constants.emplace_back("mvKey_Decimal", 330);
    constants.emplace_back("mvKey_Divide", 331);
    constants.emplace_back("mvKey_F1", 290);
    constants.emplace_back("mvKey_F2", 291);
    constants.emplace_back("mvKey_F3", 292);
    constants.emplace_back("mvKey_F4", 293);
    constants.emplace_back("mvKey_F5", 294);
    constants.emplace_back("mvKey_F6", 295);
    constants.emplace_back("mvKey_F7", 296);
    constants.emplace_back("mvKey_F8", 297);
    constants.emplace_back("mvKey_F9", 298);
    constants.emplace_back("mvKey_F10", 299);
    constants.emplace_back("mvKey_F11", 300);
    constants.emplace_back("mvKey_F12", 301);
    constants.emplace_back("mvKey_F13", 302);
    constants.emplace_back("mvKey_F14", 303);
    constants.emplace_back("mvKey_F15", 304);
    constants.emplace_back("mvKey_F16", 305);
    constants.emplace_back("mvKey_F17", 306);
    constants.emplace_back("mvKey_F18", 307);
    constants.emplace_back("mvKey_F19", 308);
    constants.emplace_back("mvKey_F20", 309);
    constants.emplace_back("mvKey_F21", 310);
    constants.emplace_back("mvKey_F22", 311);
    constants.emplace_back("mvKey_F23", 312);
    constants.emplace_back("mvKey_F24", 313);
    constants.emplace_back("mvKey_F25", 314);
    constants.emplace_back("mvKey_NumLock", 282);
    constants.emplace_back("mvKey_ScrollLock", 281);
    constants.emplace_back("mvKey_LShift", 340);
    constants.emplace_back("mvKey_RShift", 344);
    constants.emplace_back("mvKey_LControl", 341);
    constants.emplace_back("mvKey_RControl", 345);
    constants.emplace_back("mvKey_LMenu", -1);
    constants.emplace_back("mvKey_RMenu", -1);
    constants.emplace_back("mvKey_Browser_Back", -1);
    constants.emplace_back("mvKey_Browser_Forward", -1);
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
    constants.emplace_back("mvKey_Period", 46);
    constants.emplace_back("mvKey_Slash", 47);
    constants.emplace_back("mvKey_Tilde", 96);
    constants.emplace_back("mvKey_Open_Brace", 91);
    constants.emplace_back("mvKey_Backslash", 92);
    constants.emplace_back("mvKey_Close_Brace", 93);
    constants.emplace_back("mvKey_Quote", 39);
#endif
}
