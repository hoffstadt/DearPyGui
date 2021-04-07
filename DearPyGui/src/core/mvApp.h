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

#include "mvPython.h"
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <queue>
#include <thread>
#include <future>
#include <atomic>
#include "mvEvents.h"
#include <memory>
#include "mvModule_Core.h"
#include "cpp.hint"

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
    class mvTextureStorage;
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

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_CREATE_EXTRA_COMMAND(enable_docking);
        MV_CREATE_EXTRA_COMMAND(get_dearpygui_version);
        MV_CREATE_EXTRA_COMMAND(setup_dearpygui);
        MV_CREATE_EXTRA_COMMAND(render_dearpygui_frame);
        MV_CREATE_EXTRA_COMMAND(cleanup_dearpygui);
        MV_CREATE_EXTRA_COMMAND(get_delta_time);
        MV_CREATE_EXTRA_COMMAND(get_total_time);
        MV_CREATE_EXTRA_COMMAND(stop_dearpygui);
        MV_CREATE_EXTRA_COMMAND(is_dearpygui_running);
        MV_CREATE_EXTRA_COMMAND(end);

        MV_START_EXTRA_COMMANDS
            MV_ADD_EXTRA_COMMAND(enable_docking);
            MV_ADD_EXTRA_COMMAND(get_dearpygui_version);
            MV_ADD_EXTRA_COMMAND(setup_dearpygui);
            MV_ADD_EXTRA_COMMAND(render_dearpygui_frame);
            MV_ADD_EXTRA_COMMAND(cleanup_dearpygui);
            MV_ADD_EXTRA_COMMAND(get_delta_time);
            MV_ADD_EXTRA_COMMAND(get_total_time);
            MV_ADD_EXTRA_COMMAND(stop_dearpygui);
            MV_ADD_EXTRA_COMMAND(is_dearpygui_running);
            MV_ADD_EXTRA_COMMAND(end);
        MV_END_EXTRA_COMMANDS

    public:

        mvApp          (const mvApp& other) = delete;
        mvApp          (mvApp&& other)      = delete;
        mvApp operator=(const mvApp& other) = delete;
        mvApp operator=(mvApp&& other)      = delete;

        static mvApp*            GetApp              ();
        static void              DeleteApp           ();
        static const char*       GetVersion          () { return MV_SANDBOX_VERSION; }
        static bool              IsAppStarted        () { return s_started; }
        static void              SetAppStopped       ();
        static void              StopApp             () { s_started = false; } // ugly

        void cleanup();

        ~mvApp();

        //-----------------------------------------------------------------------------
        // Rendering
        //-----------------------------------------------------------------------------
        void                     render          (); // actual render loop          

        //-----------------------------------------------------------------------------
        // Managers
        //-----------------------------------------------------------------------------
        mvItemRegistry&          getItemRegistry    ();
        mvTextureStorage&        getTextureStorage  ();
        mvCallbackRegistry&      getCallbackRegistry();
        mvThemeManager&          getThemeManager    ();
        mvFontManager&           getFontManager     ();
        
        //-----------------------------------------------------------------------------
        // App Settings
        //-----------------------------------------------------------------------------
        void                     turnOnDocking     (bool shiftOnly, bool dockSpace);	
        void                     setViewport       (mvViewport* viewport) { m_viewport = viewport; }
        
        mvViewport*              getViewport       ()       { return m_viewport; }

        //-----------------------------------------------------------------------------
        // Concurrency
        //-----------------------------------------------------------------------------      
        bool                     checkIfMainThread             () const;

        //-----------------------------------------------------------------------------
        // Timing
        //-----------------------------------------------------------------------------
        float                    getDeltaTime() const { return m_deltaTime; }
        double                   getTotalTime() const { return m_time; }

        //-----------------------------------------------------------------------------
        // Other
        //-----------------------------------------------------------------------------
        std::map<std::string, mvPythonParser>& getParsers() { return const_cast<std::map<std::string, mvPythonParser>&>(mvModule_Core::GetModuleParsers()); }
        std::mutex& getMutex() const { return m_mutex; }
            
    private:

        //-----------------------------------------------------------------------------
        // Post Rendering Methods
        //     - actually performs queued operations
        //-----------------------------------------------------------------------------
        void postProfile    ();

        mvApp();
        
    private:

        static mvApp* s_instance;
        static std::atomic_bool s_started;

        // managers
        mvOwnedPtr<mvItemRegistry>                     m_itemRegistry;
        mvOwnedPtr<mvTextureStorage>                   m_textureStorage;
        mvOwnedPtr<mvThemeManager>                     m_themeManager;
        mvOwnedPtr<mvFontManager>                      m_fontManager;
        mvOwnedPtr<mvCallbackRegistry>                 m_callbackRegistry;

                                                     
        // docking                                   
        bool                                         m_docking          = false;
        bool                                         m_dockingShiftOnly = true;
        bool                                         m_dockingViewport  = false;
                                                     
        mvViewport*                                  m_viewport = nullptr;

        // timing
        float                        m_deltaTime = 0.0f; // time since last frame
        double                       m_time      = 0.0;  // total time since starting
        
        std::thread::id                  m_mainThreadID;
        mutable std::mutex               m_mutex;
        std::future<bool>                m_future;

    };

}