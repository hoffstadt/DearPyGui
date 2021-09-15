/***************************************************************************//*/
Copyright (c) 2021 Dear PyGui, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

/******************************************************************************/

#pragma once

#include <vector>
#include <map>
#include <stack>
#include <string>
#include <queue>
#include <thread>
#include <future>
#include <atomic>
#include <memory>
#include "mvCore.h"
#include "cpp.hint"
#include "mvPythonParser.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

    //-----------------------------------------------------------------------------
    // forward declarations
    //-----------------------------------------------------------------------------
    class  mvViewport;
    class  mvCallbackRegistry;
    struct mvItemRegistry;
    struct mvIO;
    struct mvContext;
    struct mvInput;

    //-----------------------------------------------------------------------------
    // public API
    //-----------------------------------------------------------------------------

    extern mvContext* GContext;

    const char*                            GetVersion();
    mvUUID                                 GenerateUUID();
    void                                   SetDefaultTheme();
    void                                   Render();
    std::map<std::string, mvPythonParser>& GetParsers();

    struct mvInput
    {
        struct AtomicVec2
        {
            std::atomic_int x;
            std::atomic_int y;
        };

        struct AtomicFloatVec2
        {
            std::atomic<float> x;
            std::atomic<float> y;
        };

        AtomicVec2       mousePos = { 0, 0 };
        AtomicVec2       mouseGlobalPos = { 0, 0 };
        AtomicFloatVec2  mousePlotPos = { 0, 0 };
        AtomicVec2       mouseDrawingPos = { 0, 0 };
        std::atomic_int  mouseDragThreshold = 20;
        AtomicVec2       mouseDragDelta = { 0, 0 };

        // keys
        std::atomic_bool keysdown[512];
        std::atomic_int  keysdownduration[512]; // 1/100 seconds
        std::atomic_bool keyspressed[512];
        std::atomic_bool keysreleased[512];

        // mouse
        std::atomic_int  mousewheel;
        std::atomic_bool mousedown[5];
        std::atomic_bool mouseDragging[5];
        std::atomic_int  mousedownduration[5]; // 1/100 seconds
        std::atomic_bool mouseclick[5];
        std::atomic_bool mousedoubleclick[5];
        std::atomic_bool mousereleased[5];
    };

    struct mvIO
    {

        bool        docking = false;
        bool        dockingViewport = false;

        std::string iniFile;
        bool        loadIniFile = false;

        bool        info_auto_device = false;
        int         info_device = -1;
        std::string info_device_name;
    };

    struct mvContext
    {
        std::atomic_bool    waitOneFrame       = false;
        std::atomic_bool    manualMutexControl = false;
        std::atomic_bool    started            = false;
        std::mutex          mutex;
        std::future<bool>   future;
        float               deltaTime = 0.0f;   // time since last frame
        double              time      = 0.0;    // total time since starting
        int                 frame     = 0;      // frame count
        int                 framerate = 0;      // frame rate
        mvUUID              id = MV_START_UUID; // current ID
        mvViewport*         viewport = nullptr;
        bool                resetTheme = false;
        mvIO                IO;
        mvItemRegistry*     itemRegistry = nullptr;
        mvCallbackRegistry* callbackRegistry = nullptr;
        mvInput             input;

    };
    
    //-----------------------------------------------------------------------------
    // Python Parsing
    //-----------------------------------------------------------------------------

    void InsertParser_mvContext   (std::map<std::string, mvPythonParser>* parsers);
    void InsertConstants_mvContext(std::vector<std::pair<std::string, long>>& constants);

    MV_CREATE_FREE_COMMAND(lock_mutex);
    MV_CREATE_FREE_COMMAND(unlock_mutex);
    MV_CREATE_FREE_COMMAND(setup_dearpygui);
    MV_CREATE_FREE_COMMAND(render_dearpygui_frame);
    MV_CREATE_FREE_COMMAND(create_context);
    MV_CREATE_FREE_COMMAND(destroy_context);
    MV_CREATE_FREE_COMMAND(get_delta_time);
    MV_CREATE_FREE_COMMAND(get_total_time);
    MV_CREATE_FREE_COMMAND(stop_dearpygui);
    MV_CREATE_FREE_COMMAND(is_dearpygui_running);
    MV_CREATE_FREE_COMMAND(generate_uuid);
    MV_CREATE_FREE_COMMAND(save_init_file);
    MV_CREATE_FREE_COMMAND(load_image);
    MV_CREATE_FREE_COMMAND(split_frame);
    MV_CREATE_FREE_COMMAND(get_frame_count);
    MV_CREATE_FREE_COMMAND(get_frame_rate);
    MV_CREATE_FREE_COMMAND(get_app_configuration);
    MV_CREATE_FREE_COMMAND(configure_app);
    MV_CREATE_FREE_COMMAND(get_drawing_mouse_pos);
    MV_CREATE_FREE_COMMAND(is_mouse_button_dragging);
    MV_CREATE_FREE_COMMAND(is_mouse_button_down);
    MV_CREATE_FREE_COMMAND(is_mouse_button_clicked);
    MV_CREATE_FREE_COMMAND(is_mouse_button_double_clicked);
    MV_CREATE_FREE_COMMAND(is_mouse_button_released);
    MV_CREATE_FREE_COMMAND(get_mouse_drag_delta);
    MV_CREATE_FREE_COMMAND(get_mouse_pos);
    MV_CREATE_FREE_COMMAND(get_plot_mouse_pos);
    MV_CREATE_FREE_COMMAND(is_key_pressed);
    MV_CREATE_FREE_COMMAND(is_key_released);
    MV_CREATE_FREE_COMMAND(is_key_down);

    MV_START_FREE_COMMANDS(mvContextCommands)
        MV_ADD_COMMAND(create_context);
        MV_ADD_COMMAND(destroy_context);
        MV_ADD_COMMAND(lock_mutex);
        MV_ADD_COMMAND(unlock_mutex);
        MV_ADD_COMMAND(setup_dearpygui);
        MV_ADD_COMMAND(render_dearpygui_frame);
        MV_ADD_COMMAND(get_delta_time);
        MV_ADD_COMMAND(get_total_time);
        MV_ADD_COMMAND(stop_dearpygui);
        MV_ADD_COMMAND(is_dearpygui_running);
        MV_ADD_COMMAND(generate_uuid);
        MV_ADD_COMMAND(save_init_file);
        MV_ADD_COMMAND(load_image);
        MV_ADD_COMMAND(split_frame);
        MV_ADD_COMMAND(get_frame_count);
        MV_ADD_COMMAND(get_frame_rate);
        MV_ADD_COMMAND(get_app_configuration);
        MV_ADD_COMMAND(configure_app);
        MV_ADD_COMMAND(get_drawing_mouse_pos);
        MV_ADD_COMMAND(is_mouse_button_dragging);
        MV_ADD_COMMAND(is_mouse_button_down);
        MV_ADD_COMMAND(is_mouse_button_clicked);
        MV_ADD_COMMAND(is_mouse_button_double_clicked);
        MV_ADD_COMMAND(is_mouse_button_released);
        MV_ADD_COMMAND(get_mouse_drag_delta);
        MV_ADD_COMMAND(get_mouse_pos);
        MV_ADD_COMMAND(get_plot_mouse_pos);
        MV_ADD_COMMAND(is_key_pressed);
        MV_ADD_COMMAND(is_key_released);
        MV_ADD_COMMAND(is_key_down);
    MV_END_COMMANDS
}
