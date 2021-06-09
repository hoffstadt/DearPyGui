#pragma once

//-----------------------------------------------------------------------------
// mvApp
//
//     - This class acts as the primary manager for a DearPyGui app,
//       with the following responsibilities:
//
//         * Callback routing
//     
//     - This class can eventually just contain all static members & methods
//     
//-----------------------------------------------------------------------------

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
    // Forward Declarations
    //-----------------------------------------------------------------------------
    class mvWindowAppItem;
    class mvTextEditor;
    class mvViewport;
    class mvThemeManager;
    class mvFontManager;
    class mvItemRegistry;
    class mvCallbackRegistry;
    struct mvColor;
    enum class mvAppItemType;
    
    //-----------------------------------------------------------------------------
    // mvApp
    //-----------------------------------------------------------------------------
    class mvApp
    {

        friend class mvViewport;
        friend class mvWindowsViewport;
        friend class mvLinuxViewport;
        friend class mvAppleViewport;

    public:

        static std::atomic_bool s_manualMutexControl;
        static std::mutex       s_mutex;
        static float            s_deltaTime; // time since last frame
        static double           s_time;  // total time since starting

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_CREATE_EXTRA_COMMAND(lock_mutex);
        MV_CREATE_EXTRA_COMMAND(unlock_mutex);
        MV_CREATE_EXTRA_COMMAND(enable_docking);
        MV_CREATE_EXTRA_COMMAND(get_dearpygui_version);
        MV_CREATE_EXTRA_COMMAND(setup_dearpygui);
        MV_CREATE_EXTRA_COMMAND(render_dearpygui_frame);
        MV_CREATE_EXTRA_COMMAND(cleanup_dearpygui);
        MV_CREATE_EXTRA_COMMAND(get_delta_time);
        MV_CREATE_EXTRA_COMMAND(get_total_time);
        MV_CREATE_EXTRA_COMMAND(stop_dearpygui);
        MV_CREATE_EXTRA_COMMAND(is_dearpygui_running);

        MV_START_EXTRA_COMMANDS
            MV_ADD_EXTRA_COMMAND(lock_mutex);
            MV_ADD_EXTRA_COMMAND(unlock_mutex);
            MV_ADD_EXTRA_COMMAND(enable_docking);
            MV_ADD_EXTRA_COMMAND(get_dearpygui_version);
            MV_ADD_EXTRA_COMMAND(setup_dearpygui);
            MV_ADD_EXTRA_COMMAND(render_dearpygui_frame);
            MV_ADD_EXTRA_COMMAND(cleanup_dearpygui);
            MV_ADD_EXTRA_COMMAND(get_delta_time);
            MV_ADD_EXTRA_COMMAND(get_total_time);
            MV_ADD_EXTRA_COMMAND(stop_dearpygui);
            MV_ADD_EXTRA_COMMAND(is_dearpygui_running);
        MV_END_EXTRA_COMMANDS

        //-----------------------------------------------------------------------------
        // General
        //-----------------------------------------------------------------------------
        static mvApp*      GetApp       ();
        static void        DeleteApp    ();
        static const char* GetVersion   () { return MV_SANDBOX_VERSION; }
        static bool        IsAppStarted () { return s_started; }
        static void        SetAppStopped();
        static void        StopApp      () { s_started = false; } // ugly

        //-----------------------------------------------------------------------------
        // Timing
        //-----------------------------------------------------------------------------
        static float  GetDeltaTime() { return s_deltaTime; }
        static double GetTotalTime() { return s_time; }

    public:

        mvApp          (const mvApp& other) = delete;
        mvApp          (mvApp&& other)      = delete;
        mvApp operator=(const mvApp& other) = delete;
        mvApp operator=(mvApp&& other)      = delete;
        ~mvApp();

        //-----------------------------------------------------------------------------
        // Rendering
        //-----------------------------------------------------------------------------
        void render(); // actual render loop          

        //-----------------------------------------------------------------------------
        // Managers
        //-----------------------------------------------------------------------------
        mvItemRegistry&     getItemRegistry    ();
        mvCallbackRegistry& getCallbackRegistry();
        mvThemeManager&     getThemeManager    ();
        
        //-----------------------------------------------------------------------------
        // App Settings
        //-----------------------------------------------------------------------------
        void        turnOnDocking(bool dockSpace);	
        void        setViewport  (mvViewport* viewport) { m_viewport = viewport; }
        mvViewport* getViewport  ()       { return m_viewport; }

        //-----------------------------------------------------------------------------
        // Other
        //-----------------------------------------------------------------------------
        std::map<std::string, mvPythonParser>& getParsers();
        void                                   cleanup();
            
    private:

        mvApp();
        
    private:

        static mvApp*           s_instance;
        static std::atomic_bool s_started;

        // managers
        mvOwnedPtr<mvItemRegistry>     m_itemRegistry;
        mvOwnedPtr<mvThemeManager>     m_themeManager;
        mvOwnedPtr<mvCallbackRegistry> m_callbackRegistry;
                                         
        // docking                                   
        bool m_docking          = false;
        bool m_dockingViewport  = false;
                    
        mvViewport*       m_viewport = nullptr;
        std::future<bool> m_future;
        
    };

}