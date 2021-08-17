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

        static std::atomic_bool s_waitOneFrame;
        static std::atomic_bool s_manualMutexControl;
        static std::mutex       s_mutex;
        static float            s_deltaTime; // time since last frame
        static double           s_time;  // total time since starting
        static int              s_frame;  // frame count
        static int              s_framerate;  // frame rate

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_CREATE_COMMAND(lock_mutex);
        MV_CREATE_COMMAND(unlock_mutex);
        MV_CREATE_COMMAND(setup_dearpygui);
        MV_CREATE_COMMAND(render_dearpygui_frame);
        MV_CREATE_COMMAND(cleanup_dearpygui);
        MV_CREATE_COMMAND(get_delta_time);
        MV_CREATE_COMMAND(get_total_time);
        MV_CREATE_COMMAND(stop_dearpygui);
        MV_CREATE_COMMAND(is_dearpygui_running);
        MV_CREATE_COMMAND(generate_uuid);
        MV_CREATE_COMMAND(save_init_file);
        MV_CREATE_COMMAND(load_image);
        MV_CREATE_COMMAND(split_frame);
        MV_CREATE_COMMAND(get_frame_count);
        MV_CREATE_COMMAND(get_frame_rate);
        MV_CREATE_COMMAND(reset_default_theme);
        MV_CREATE_COMMAND(get_app_configuration);
        MV_CREATE_COMMAND(configure_app);

        MV_START_COMMANDS
            MV_ADD_COMMAND(lock_mutex);
            MV_ADD_COMMAND(unlock_mutex);
            MV_ADD_COMMAND(setup_dearpygui);
            MV_ADD_COMMAND(render_dearpygui_frame);
            MV_ADD_COMMAND(cleanup_dearpygui);
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
            MV_ADD_COMMAND(reset_default_theme);
            MV_ADD_COMMAND(get_app_configuration);
            MV_ADD_COMMAND(configure_app);
        MV_END_COMMANDS

        //-----------------------------------------------------------------------------
        // General
        //-----------------------------------------------------------------------------
        static mvApp*      GetApp       ();
        static void        DeleteApp    ();
        static const char* GetVersion   () { return MV_SANDBOX_VERSION; }
        static const char* GetPlatform  () { return MV_PLATFORM; }
        static bool        IsAppStarted () { return s_started; }
        static void        StopApp      () { s_started = false; } // ugly
        static void        SetDefaultTheme();

        // generates UUID: used by users and internally
        static mvUUID GenerateUUID();

        //-----------------------------------------------------------------------------
        // Timing
        //-----------------------------------------------------------------------------
        static float  GetDeltaTime() { return s_deltaTime; }
        static double GetTotalTime() { return s_time; }
        static double GetFrameRate() { return s_framerate; }

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

        //-----------------------------------------------------------------------------
        // Ini utilities
        //-----------------------------------------------------------------------------
        bool isUsingIniFile() const { return !_iniFile.empty(); }
        
        //-----------------------------------------------------------------------------
        // App Settings
        //-----------------------------------------------------------------------------
        void        turnOnDocking(bool dockSpace);	
        void        setViewport  (mvViewport* viewport) { _viewport = viewport; }
        mvViewport* getViewport  () { return _viewport; }

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
        static mvUUID           s_id; // current ID

        // managers
        mvOwnedPtr<mvItemRegistry>     _itemRegistry;
        mvOwnedPtr<mvCallbackRegistry> _callbackRegistry;
                                         
        // docking                                   
        bool _docking          = false;
        bool _dockingViewport  = false;

        // ini file
        std::string _iniFile;
        bool _loadIniFile = false;
                    
        mvViewport*       _viewport = nullptr;
        std::future<bool> _future;

        bool _resetTheme = false;
        
    };

}