#include "mvContext.h"
#include "mvModule_DearPyGui.h"
#include "mvViewport.h"
#include "mvCallbackRegistry.h"
#include <thread>
#include <future>
#include <chrono>
#include "mvProfiler.h"
#include <implot.h>
#include "mvFontManager.h"
#include "mvCallbackRegistry.h"
#include "mvPythonTranslator.h"
#include "mvPythonExceptions.h"
#include "mvGlobalIntepreterLock.h"
#include <frameobject.h>
#include "mvModule_DearPyGui.h"
#include "mvLog.h"
#include "mvToolManager.h"
#include <imnodes.h>
#include <thread>
#include <stb_image.h>
#include "mvBuffer.h"
#include "mvAppItemCommons.h"
#include "mvItemRegistry.h"

namespace Marvel {

    extern mvContext* GContext = nullptr;

    mv_internal void 
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
        for (i32 i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
        {
            input.keysdown[i] = ImGui::GetIO().KeysDown[i];
            input.keyspressed[i] = ImGui::GetIO().KeysDownDuration[i] == 0.0f;
            input.keysreleased[i] = ImGui::GetIO().KeysDownDurationPrev[i] >= 0.0f && !ImGui::GetIO().KeysDown[i];

            // route key down event
            if (ImGui::GetIO().KeysDownDuration[i] >= 0.0f)
                input.keysdownduration[i] = (i32)(ImGui::GetIO().KeysDownDuration[i] * 100.0);

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

    mv_internal void
    CreateContext()
    {

        if (GContext)
        {
            assert(false);
            return;
        }

        GContext = new mvContext();

        GContext->itemRegistry = new mvItemRegistry();
        GContext->callbackRegistry = new mvCallbackRegistry();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        imnodes::CreateContext();
    }

    mv_internal void
    DestroyContext()
    {

        if (GContext == nullptr)
        {
            assert(false);
            return;
        }

        // hacky fix, started was set to false
        // to exit the event loop, but needs to be
        // true in order to run DPG commands for the 
        // exit callback.
        GContext->started = true;
        mvSubmitCallback([=]() {
            mvRunCallback(GContext->callbackRegistry->onCloseCallback, 0, nullptr, nullptr);
            GContext->started = false;  // return to false after
            });

        imnodes::DestroyContext();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        mvToolManager::Reset();
        ClearItemRegistry(*GContext->itemRegistry);

        #define X(el) el::s_class_theme_component = nullptr; el::s_class_theme_disabled_component = nullptr;
        MV_ITEM_TYPES
        #undef X

        mvSubmitCallback([=]() {
            GContext->callbackRegistry->running = false;
            });
        if(GContext->future.valid())
            GContext->future.get();
        if (GContext->viewport)
            delete GContext->viewport;

        delete GContext->itemRegistry;
        delete GContext->callbackRegistry;
        delete GContext;
        GContext = nullptr;
    }

    mv_internal const char* 
    GetVersion() 
    { 
        return MV_SANDBOX_VERSION; 
    }

    mv_internal const char*
    GetPlatform() 
    { 
        return MV_PLATFORM; 
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
        colors[ImGuiCol_DockingPreview] = mvImGuiCol_DockingPreview;
        colors[ImGuiCol_DockingEmptyBg] = mvImGuiCol_DockingEmptyBg;
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

        imnodes::GetStyle().colors[imnodes::ColorStyle_NodeBackground] = mvColor::ConvertToUnsignedInt(mvColor(62, 62, 62, 255));
        imnodes::GetStyle().colors[imnodes::ColorStyle_NodeBackgroundHovered] = mvColor::ConvertToUnsignedInt(mvColor(75, 75, 75, 255));
        imnodes::GetStyle().colors[imnodes::ColorStyle_NodeBackgroundSelected] = mvColor::ConvertToUnsignedInt(mvColor(75, 75, 75, 255));
        imnodes::GetStyle().colors[imnodes::ColorStyle_NodeOutline] = mvColor::ConvertToUnsignedInt(mvColor(100, 100, 100, 255));
        imnodes::GetStyle().colors[imnodes::ColorStyle_TitleBar] = mvColor::ConvertToUnsignedInt(mvImGuiCol_TitleBg);
        imnodes::GetStyle().colors[imnodes::ColorStyle_TitleBarHovered] = mvColor::ConvertToUnsignedInt(mvImGuiCol_TitleBgActive);
        imnodes::GetStyle().colors[imnodes::ColorStyle_TitleBarSelected] = mvColor::ConvertToUnsignedInt(mvImGuiCol_FrameBgActive);
        imnodes::GetStyle().colors[imnodes::ColorStyle_Link] = mvColor::ConvertToUnsignedInt(mvColor(255, 255, 255, 200));
        imnodes::GetStyle().colors[imnodes::ColorStyle_LinkHovered] = mvColor::ConvertToUnsignedInt(mvColor(66, 150, 250, 255));
        imnodes::GetStyle().colors[imnodes::ColorStyle_LinkSelected] = mvColor::ConvertToUnsignedInt(mvColor(66, 150, 250, 255));
        imnodes::GetStyle().colors[imnodes::ColorStyle_Pin] = mvColor::ConvertToUnsignedInt(mvColor(199, 199, 41, 255));
        imnodes::GetStyle().colors[imnodes::ColorStyle_PinHovered] = mvColor::ConvertToUnsignedInt(mvColor(255, 255, 50, 255));
        imnodes::GetStyle().colors[imnodes::ColorStyle_BoxSelector] = mvColor::ConvertToUnsignedInt(mvColor(61, 133, 224, 30));
        imnodes::GetStyle().colors[imnodes::ColorStyle_BoxSelectorOutline] = mvColor::ConvertToUnsignedInt(mvColor(61, 133, 224, 150));
        imnodes::GetStyle().colors[imnodes::ColorStyle_GridBackground] = mvColor::ConvertToUnsignedInt(mvColor(35, 35, 35, 255));
        imnodes::GetStyle().colors[imnodes::ColorStyle_GridLine] = mvColor::ConvertToUnsignedInt(mvColor(0, 0, 0, 255));

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

        if (GContext->IO.dockingViewport)
            ImGui::DockSpaceOverViewport();

        mvFrameCallback(ImGui::GetFrameCount());

        // route input callbacks
        UpdateInputs(GContext->input);

        mvToolManager::Draw();

        {
            std::lock_guard<std::mutex> lk(GContext->mutex);
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
    InsertParser_mvContext(std::map<std::string, mvPythonParser>* parsers)
    {

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Returns app configuration.";
            setup.category = { "General" };
            setup.returnType = mvPyDataType::Dict;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_app_configuration", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.reserve(11);
            args.push_back({ mvPyDataType::Bool, "docking", mvArgType::KEYWORD_ARG, "False", "Enables docking support." });
            args.push_back({ mvPyDataType::Bool, "docking_space", mvArgType::KEYWORD_ARG, "False", "add explicit dockspace over viewport" });
            args.push_back({ mvPyDataType::String, "load_init_file", mvArgType::KEYWORD_ARG, "''", "Load .ini file." });
            args.push_back({ mvPyDataType::String, "init_file", mvArgType::KEYWORD_ARG, "''" });
            args.push_back({ mvPyDataType::Bool, "auto_save_init_file", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Integer, "device", mvArgType::KEYWORD_ARG, "-1", "Which display adapter to use. (-1 will use default)" });
            args.push_back({ mvPyDataType::Bool, "auto_device", mvArgType::KEYWORD_ARG, "False", "Let us pick the display adapter." });
            args.push_back({ mvPyDataType::Bool, "allow_alias_overwrites", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Bool, "manual_alias_management", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Bool, "skip_required_args", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Bool, "skip_positional_args", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Bool, "skip_keyword_args", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Bool, "wait_for_input", mvArgType::KEYWORD_ARG, "False", "New in 1.1. Only update when user input occurs"});

            mvPythonParserSetup setup;
            setup.about = "Configures app.";
            setup.category = { "General" };
            setup.unspecifiedKwargs = true;
            setup.internal = true;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "configure_app", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::String, "file" });

            mvPythonParserSetup setup;
            setup.about = "Save dpg.ini file.";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "save_init_file", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::Integer, "delay", mvArgType::KEYWORD_ARG, "32", "Minimal delay in in milliseconds" });

            mvPythonParserSetup setup;
            setup.about = "Waits one frame.";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "split_frame", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Returns frame count.";
            setup.category = { "General" };
            setup.returnType = mvPyDataType::Integer;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_frame_count", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.reserve(3);
            args.push_back({ mvPyDataType::String, "file" });
            args.push_back({ mvPyDataType::Float, "gamma", mvArgType::KEYWORD_ARG, "1.0", "Gamma correction factor. (default is 1.0 to avoid automatic gamma correction on loading." });
            args.push_back({ mvPyDataType::Float, "gamma_scale_factor", mvArgType::KEYWORD_ARG, "1.0", "Gamma scale factor." });
            
            mvPythonParserSetup setup;
            setup.about = "Loads an image. Returns width, height, channels, mvBuffer";
            setup.category = { "Textures", "Utilities"};
            setup.returnType = mvPyDataType::Object;
            
            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "load_image", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Generate a new UUID.";
            setup.category = { "General" };
            setup.returnType = mvPyDataType::UUID;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "generate_uuid", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Locks render thread mutex.";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "lock_mutex", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Unlocks render thread mutex";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "unlock_mutex", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Checks if Dear PyGui is running";
            setup.category = { "General" };
            setup.returnType = mvPyDataType::Bool;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "is_dearpygui_running", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Sets up Dear PyGui";
            setup.category = { "General" };

            args.push_back({ mvPyDataType::UUID, "viewport", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });
            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "setup_dearpygui", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Render a single Dear PyGui frame.";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "render_dearpygui_frame", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
    
            mvPythonParserSetup setup;
            setup.about = "Destroys the Dear PyGui context.";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "destroy_context", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Creates the Dear PyGui context.";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "create_context", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Stops Dear PyGui";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "stop_dearpygui", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Returns total time since Dear PyGui has started.";
            setup.category = { "General" };
            setup.returnType = mvPyDataType::Float;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_total_time", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Returns time since last frame.";
            setup.category = { "General" };
            setup.returnType = mvPyDataType::Float;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_delta_time", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Returns the average frame rate across 120 frames.";
            setup.category = { "General" };
            setup.returnType = mvPyDataType::Float;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_frame_rate", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::Bool, "local", mvArgType::KEYWORD_ARG, "True" });

            mvPythonParserSetup setup;
            setup.about = "Returns mouse position.";
            setup.category = { "Input Polling" };
            setup.returnType = mvPyDataType::IntList;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_mouse_pos", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Returns mouse position in plot.";
            setup.category = { "Input Polling" };
            setup.returnType = mvPyDataType::IntList;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_plot_mouse_pos", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Returns mouse position in drawing.";
            setup.category = { "Input Polling" };
            setup.returnType = mvPyDataType::IntList;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_drawing_mouse_pos", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Returns mouse drag delta.";
            setup.category = { "Input Polling" };
            setup.returnType = mvPyDataType::Float;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_mouse_drag_delta", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::Integer, "button" });
            args.push_back({ mvPyDataType::Float, "threshold" });

            mvPythonParserSetup setup;
            setup.about = "Checks if mouse button is down and dragging.";
            setup.category = { "Input Polling" };
            setup.returnType = mvPyDataType::Bool;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "is_mouse_button_dragging", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::Integer, "button" });

            mvPythonParserSetup setup;
            setup.about = "Checks if mouse button is down.";
            setup.category = { "Input Polling" };
            setup.returnType = mvPyDataType::Bool;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "is_mouse_button_down", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::Integer, "button" });

            mvPythonParserSetup setup;
            setup.about = "Checks if mouse button is clicked.";
            setup.category = { "Input Polling" };
            setup.returnType = mvPyDataType::Bool;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "is_mouse_button_clicked", parser });
        }


        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::Integer, "button" });

            mvPythonParserSetup setup;
            setup.about = "Checks if mouse button is released.";
            setup.category = { "Input Polling" };
            setup.returnType = mvPyDataType::Bool;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "is_mouse_button_released", parser });
        }


        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::Integer, "button" });

            mvPythonParserSetup setup;
            setup.about = "Checks if mouse button is double clicked.";
            setup.category = { "Input Polling" };
            setup.returnType = mvPyDataType::Bool;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "is_mouse_button_double_clicked", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::Integer, "key" });

            mvPythonParserSetup setup;
            setup.about = "Checks if key is pressed.";
            setup.category = { "Input Polling" };
            setup.returnType = mvPyDataType::Bool;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "is_key_pressed", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::Integer, "key" });

            mvPythonParserSetup setup;
            setup.about = "Checks if key is released.";
            setup.category = { "Input Polling" };
            setup.returnType = mvPyDataType::Bool;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "is_key_released", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::Integer, "key" });

            mvPythonParserSetup setup;
            setup.about = "Checks if key is down.";
            setup.category = { "Input Polling" };
            setup.returnType = mvPyDataType::Bool;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "is_key_down", parser });
        }
    }

    void 
    InsertConstants_mvContext(std::vector<std::pair<std::string, long>>& constants)
    {

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
        constants.emplace_back("mvKey_Control", 241);
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
        constants.emplace_back("mvKey_F24", 314);
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

    mv_python_function
    save_init_file(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* file;

        if (!Parse((GetParsers())["save_init_file"], args, kwargs, __FUNCTION__, &file))
            return GetPyNone();

        if (GContext->started)
            ImGui::SaveIniSettingsToDisk(file);
        else
            mvThrowPythonError(mvErrorCode::mvNone, "Dear PyGui must be started to use \"save_init_file\".");

        return GetPyNone();
    }

    mv_python_function
    split_frame(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        i32 delay = 32;

        if (!Parse((GetParsers())["split_frame"], args, kwargs, __FUNCTION__,
            &delay))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        Py_BEGIN_ALLOW_THREADS;
        GContext->waitOneFrame = true;
        while (GContext->waitOneFrame)
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        Py_END_ALLOW_THREADS;

        return GetPyNone();
    }

    mv_python_function
    lock_mutex(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        GContext->mutex.lock();
        GContext->manualMutexControl = true;

        return GetPyNone();
    }

    mv_python_function
    unlock_mutex(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        GContext->mutex.unlock();
        GContext->manualMutexControl = false;

        return GetPyNone();
    }

    mv_python_function
    get_frame_count(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
        return ToPyInt(GContext->frame);
    }

    mv_python_function
    load_image(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* file;
        f32 gamma = 1.0f;
        f32 gamma_scale = 1.0f;

        if (!Parse((GetParsers())["load_image"], args, kwargs, __FUNCTION__,
            &file, &gamma, &gamma_scale))
            return GetPyNone();

        // Vout = (Vin / 255)^v; Where v = gamma

        if (stbi_is_hdr(file))
        {
            stbi_hdr_to_ldr_gamma(gamma);
            stbi_hdr_to_ldr_scale(gamma_scale);
        }
        else
        {
            stbi_ldr_to_hdr_gamma(gamma);
            stbi_ldr_to_hdr_scale(gamma_scale);
        }
        

        // Load from disk into a raw RGBA buffer
        i32 image_width = 0;
        i32 image_height = 0;

        // automatic gamma correction
        f32* image_data = stbi_loadf(file, &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
            return GetPyNone();

        PyObject* newbuffer = nullptr;
        PymvBuffer* newbufferview = nullptr;
        newbufferview = PyObject_New(PymvBuffer, &PymvBufferType);
        newbufferview->arr.length = image_width * image_height * 4;
        newbufferview->arr.data = (f32*)image_data;
        newbuffer = PyObject_Init((PyObject*)newbufferview, &PymvBufferType);

        PyObject* result = PyTuple_New(4);
        PyTuple_SetItem(result, 0, Py_BuildValue("i", image_width));
        PyTuple_SetItem(result, 1, Py_BuildValue("i", image_height));
        PyTuple_SetItem(result, 2, PyLong_FromLong(4));
        PyTuple_SetItem(result, 3, newbuffer);

        return result;
    }

    mv_python_function
    is_dearpygui_running(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        return ToPyBool(GContext->started);
    }

    mv_python_function
    setup_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        Py_BEGIN_ALLOW_THREADS;

        if (GContext->started)
        {
            mvThrowPythonError(mvErrorCode::mvNone, "Cannot call \"setup_dearpygui\" while a Dear PyGUI app is already running.");
            return GetPyNone();
        }

        while (!GContext->itemRegistry->containers.empty())
            GContext->itemRegistry->containers.pop();
        MV_ITEM_REGISTRY_INFO("Container stack emptied.");

        GContext->started = true;
        GContext->future = std::async(std::launch::async, []() {return mvRunCallbacks(); });

        MV_CORE_INFO("application starting");

        Py_END_ALLOW_THREADS;

        return GetPyNone();
    }

    mv_python_function
    render_dearpygui_frame(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        MV_PROFILE_SCOPE("Frame")

        Py_BEGIN_ALLOW_THREADS;
        auto window = GContext->viewport;
        mvRenderFrame();
        Py_END_ALLOW_THREADS;

        return GetPyNone();
    }

    mv_python_function
    create_context(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        Py_BEGIN_ALLOW_THREADS;
        CreateContext();
        Py_END_ALLOW_THREADS;
        return GetPyNone();
    }

    mv_python_function
    destroy_context(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        //if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        Py_BEGIN_ALLOW_THREADS;
        DestroyContext();
        Py_END_ALLOW_THREADS;

        return GetPyNone();
    }

    mv_python_function
    stop_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
        GContext->started = false;
        auto viewport = GContext->viewport;
        if (viewport)
            viewport->running = false;
        return GetPyNone();
    }

    mv_python_function
    get_total_time(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if(!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
        return ToPyFloat((f32)GContext->time);
    }

    mv_python_function
    get_delta_time(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
        return ToPyFloat(GContext->deltaTime);

    }

    mv_python_function
    get_frame_rate(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
        return ToPyFloat((f32)GContext->framerate);

    }

    mv_python_function
    generate_uuid(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        return ToPyUUID(GenerateUUID());
    }

    mv_python_function
    configure_app(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (kwargs == nullptr)
            return GetPyNone();

        if (VerifyKeywordArguments(GetParsers()["configure_app"], kwargs))
            return GetPyNone();

        if (PyArg_ValidateKeywordArguments(kwargs) == 0)
        {
            assert(false);
            mvThrowPythonError(mvErrorCode::mvNone, "Dictionary keywords must be strings");
            return GetPyNone();
        }

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        if (PyObject* item = PyDict_GetItemString(kwargs, "auto_device")) GContext->IO.info_auto_device = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "docking")) GContext->IO.docking = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "docking_space")) GContext->IO.dockingViewport = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "load_init_file"))
        {
            std::string load_init_file = ToString(item);
            GContext->IO.iniFile = load_init_file;
            GContext->IO.loadIniFile = true;
        }

        if (PyObject* item = PyDict_GetItemString(kwargs, "allow_alias_overwrites")) GContext->IO.allowAliasOverwrites = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "manual_alias_management")) GContext->IO.manualAliasManagement = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "skip_keyword_args")) GContext->IO.skipKeywordArgs = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "skip_positional_args")) GContext->IO.skipPositionalArgs = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "skip_required_args")) GContext->IO.skipRequiredArgs = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "auto_save_init_file")) GContext->IO.autoSaveIniFile = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "wait_for_input")) GContext->IO.waitForInput = ToBool(item);

        if (PyObject* item = PyDict_GetItemString(kwargs, "init_file")) GContext->IO.iniFile = ToString(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "device_name")) GContext->IO.info_device_name = ToString(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "device")) GContext->IO.info_device = ToInt(item);

        return GetPyNone();
    }

    mv_python_function
    get_app_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
        PyObject* pdict = PyDict_New();
        PyDict_SetItemString(pdict, "auto_device", mvPyObject(ToPyBool(GContext->IO.info_auto_device)));
        PyDict_SetItemString(pdict, "docking", mvPyObject(ToPyBool(GContext->IO.docking)));
        PyDict_SetItemString(pdict, "docking_space", mvPyObject(ToPyBool(GContext->IO.docking)));
        PyDict_SetItemString(pdict, "load_init_file", mvPyObject(ToPyBool(GContext->IO.loadIniFile)));
        PyDict_SetItemString(pdict, "version", mvPyObject(ToPyString(GetVersion())));
        PyDict_SetItemString(pdict, "major_version", mvPyObject(ToPyInt(MV_DPG_MAJOR_VERSION)));
        PyDict_SetItemString(pdict, "minor_version", mvPyObject(ToPyInt(MV_DPG_MINOR_VERSION)));
        PyDict_SetItemString(pdict, "init_file", mvPyObject(ToPyString(GContext->IO.iniFile)));
        PyDict_SetItemString(pdict, "platform", mvPyObject(ToPyString(GetPlatform())));
        PyDict_SetItemString(pdict, "device", mvPyObject(ToPyInt(GContext->IO.info_device)));
        PyDict_SetItemString(pdict, "device_name", mvPyObject(ToPyString(GContext->IO.info_device_name)));
        PyDict_SetItemString(pdict, "allow_alias_overwrites", mvPyObject(ToPyBool(GContext->IO.allowAliasOverwrites)));
        PyDict_SetItemString(pdict, "manual_alias_management", mvPyObject(ToPyBool(GContext->IO.manualAliasManagement)));
        PyDict_SetItemString(pdict, "skip_keyword_args", mvPyObject(ToPyBool(GContext->IO.skipKeywordArgs)));
        PyDict_SetItemString(pdict, "skip_positional_args", mvPyObject(ToPyBool(GContext->IO.skipPositionalArgs)));
        PyDict_SetItemString(pdict, "skip_required_args", mvPyObject(ToPyBool(GContext->IO.skipRequiredArgs)));
        PyDict_SetItemString(pdict, "auto_save_init_file", mvPyObject(ToPyBool(GContext->IO.autoSaveIniFile)));
        PyDict_SetItemString(pdict, "wait_for_input", mvPyObject(ToPyBool(GContext->IO.waitForInput)));
        return pdict;
    }

    mv_python_function
    get_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        b32 local = true;

        if (!Parse((GetParsers())["get_mouse_pos"], args, kwargs, __FUNCTION__, &local))
            return GetPyNone();

        auto pos = mvVec2();

        if (local)
            pos = { (f32)GContext->input.mousePos.x, (f32)GContext->input.mousePos.y };
        else
            pos = { (f32)GContext->input.mouseGlobalPos.x, (f32)GContext->input.mouseGlobalPos.y };

        return ToPyPair(pos.x, pos.y);
    }

    mv_python_function
    get_plot_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!Parse((GetParsers())["get_plot_mouse_pos"], args, kwargs, __FUNCTION__))
            return GetPyNone();

        mvVec2 pos = { (f32)GContext->input.mousePlotPos.x, (f32)GContext->input.mousePlotPos.y };

        return ToPyPair(pos.x, pos.y);
    }

    mv_python_function
    get_drawing_mouse_pos(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!Parse((GetParsers())["get_drawing_mouse_pos"], args, kwargs, __FUNCTION__))
            return GetPyNone();

        mvVec2 pos = { (f32)GContext->input.mouseDrawingPos.x, (f32)GContext->input.mouseDrawingPos.y };

        return ToPyPair(pos.x, pos.y);
    }

    mv_python_function
    get_mouse_drag_delta(PyObject* self, PyObject* arg, PyObject* kwargss)
    {

        mvVec2 pos = { (f32)GContext->input.mouseDragDelta.x, (f32)GContext->input.mouseDragDelta.y };
        return ToPyPair(pos.x, pos.y);
    }

    mv_python_function
    is_key_pressed(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        i32 key;

        if (!Parse((GetParsers())["is_key_pressed"], args, kwargs, __FUNCTION__, &key))
            return GetPyNone();

        return ToPyBool(GContext->input.keyspressed[key]);
    }

    mv_python_function
    is_key_released(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        i32 key;

        if (!Parse((GetParsers())["is_key_released"], args, kwargs, __FUNCTION__, &key))
            return GetPyNone();

        return ToPyBool(GContext->input.keysreleased[key]);
    }

    mv_python_function
    is_key_down(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        i32 key;

        if (!Parse((GetParsers())["is_key_down"], args, kwargs, __FUNCTION__, &key))
            return GetPyNone();

        return ToPyBool(GContext->input.keysdown[key]);
    }

    mv_python_function
    is_mouse_button_dragging(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        i32 button;
        f32 threshold;

        if (!Parse((GetParsers())["is_mouse_button_dragging"], args, kwargs, __FUNCTION__, &button, &threshold))
            return GetPyNone();

        return ToPyBool((f32)GContext->input.mousedownduration[button] / 100.0f >= threshold);
    }

    mv_python_function
    is_mouse_button_down(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        i32 button;

        if (!Parse((GetParsers())["is_mouse_button_down"], args, kwargs, __FUNCTION__, &button))
            return GetPyNone();

        return ToPyBool(GContext->input.mousedown[button]);
    }

    mv_python_function
    is_mouse_button_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        i32 button;

        if (!Parse((GetParsers())["is_mouse_button_clicked"], args, kwargs, __FUNCTION__, &button))
            return GetPyNone();

        return ToPyBool(GContext->input.mouseclick[button]);
    }

    mv_python_function
    is_mouse_button_double_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        i32 button;

        if (!Parse((GetParsers())["is_mouse_button_double_clicked"], args, kwargs, __FUNCTION__, &button))
            return GetPyNone();

        return ToPyBool(GContext->input.mousedoubleclick[button]);
    }

    mv_python_function
    is_mouse_button_released(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        i32 button;

        if (!Parse((GetParsers())["is_mouse_button_released"], args, kwargs, __FUNCTION__, &button))
            return GetPyNone();

        return ToPyBool(GContext->input.mousereleased[button]);
    }

}
