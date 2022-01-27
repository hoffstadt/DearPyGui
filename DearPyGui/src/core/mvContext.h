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
#include "mvTypes.h"
#include "mvGraphics.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

    //-----------------------------------------------------------------------------
    // forward declarations
    //-----------------------------------------------------------------------------
    struct mvViewport;
    struct mvCallbackRegistry;
    struct mvItemRegistry;
    struct mvIO;
    struct mvContext;
    struct mvInput;

    //-----------------------------------------------------------------------------
    // public API
    //-----------------------------------------------------------------------------

    extern mvContext* GContext;

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

        b8 docking = false;
        b8 dockingViewport = false;

        std::string iniFile;
        b8          loadIniFile = false;
        b8          autoSaveIniFile = false;
        b8          waitForInput = false;

        // GPU selection
        b8          info_auto_device = false;
        i32         info_device = -1;
        std::string info_device_name;

        // item registry
        b8 allowAliasOverwrites = false;
        b8 manualAliasManagement = false;
        b8 skipRequiredArgs = false;
        b8 skipPositionalArgs = false;
        b8 skipKeywordArgs = false;

        // callback registry
        b8 manualCallbacks = false;
    };

    struct mvContext
    {
        std::atomic_bool    waitOneFrame       = false;
        std::atomic_bool    manualMutexControl = false;
        std::atomic_bool    started            = false;
        std::mutex          mutex;
        std::future<b8>     future;
        f32                 deltaTime = 0.0f;   // time since last frame
        f64                 time      = 0.0;    // total time since starting
        i32                 frame     = 0;      // frame count
        i32                 framerate = 0;      // frame rate
        mvUUID              id = MV_START_UUID; // current ID
        mvViewport*         viewport = nullptr;
        mvGraphics          graphics;
        b8                  resetTheme = false;
        mvIO                IO;
        mvItemRegistry*     itemRegistry = nullptr;
        mvCallbackRegistry* callbackRegistry = nullptr;
        mvInput             input;

    };
    
    //-----------------------------------------------------------------------------
    // Python Parsing
    //-----------------------------------------------------------------------------

    void InsertConstants_mvContext(std::vector<std::pair<std::string, long>>& constants);
}
