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

namespace Marvel {

    //-----------------------------------------------------------------------------
    // Forward Declarations
    //-----------------------------------------------------------------------------
    class mvWindowAppItem;
    class mvTextEditor;
    class mvWindow;
    class mvThemeManager;
    class mvItemRegistry;
    class mvTextureStorage;
    class mvCallbackRegistry;
    struct mvColor;
    enum class mvAppItemType;
    
    //-----------------------------------------------------------------------------
    // mvApp
    //-----------------------------------------------------------------------------
    class mvApp : public mvEventHandler
    {

        friend class mvWindow;
        friend class mvWindowsWindow;
        friend class mvLinuxWindow;
        friend class mvAppleWindow;

    public:

        mvApp          (const mvApp& other) = delete;
        mvApp          (mvApp&& other)      = delete;
        mvApp operator=(const mvApp& other) = delete;
        mvApp operator=(mvApp&& other)      = delete;

        static mvApp*            GetApp              ();
        static void              DeleteApp           ();
        static const char*       GetVersion          () { return MV_SANDBOX_VERSION; }
        static bool              IsAppStarted        () { return s_started; }
        static void              SetAppStarted       ();
        static void              SetAppStopped       ();
        static void              StopApp             () { s_started = false; } // ugly

        void start(const std::string& primaryWindow);

        ~mvApp() override;

        //-----------------------------------------------------------------------------
        // New event handling system
        //-----------------------------------------------------------------------------
        bool onEvent         (mvEvent& event) override;
        bool onViewPortResize(mvEvent& event);

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
        
        //-----------------------------------------------------------------------------
        // App Settings
        //-----------------------------------------------------------------------------
        void                     turnOnDocking     (bool shiftOnly, bool dockSpace);
        void                     addRemapChar      (int dst, int src) { m_charRemaps.emplace_back( dst, src ); }
        void                     setVSync          (bool value) { m_vsync = value; }
        void                     setResizable      (bool value) { m_resizable = value; }			
        void                     setMainPos        (int x, int y);			
        void                     setGlobalFontScale(float scale);
        void                     setViewport       (mvWindow* viewport) { m_viewport = viewport; }
        void                     setTitle          (const std::string& title) { m_title = title; }
        void                     setFont(const std::string& file, float size = 13.0f, const std::string& glyphRange = "",
                                    std::vector<std::array<ImWchar, 3>> customRanges = {},
                                    std::vector<ImWchar> chars= {});
        
        
        float&                   getGlobalFontScale()       { return m_globalFontScale; }
        int                      getActualWidth    () const { return m_actualWidth; }
        int                      getActualHeight   () const { return m_actualHeight; }
        int                      getClientWidth    () const { return m_clientWidth; }
        int                      getClientHeight   () const { return m_clientHeight; }
        mvWindow*                getViewport       ()       { return m_viewport; }
        bool                     getVSync          () const { return m_vsync; }
        bool                     getResizable      () const { return m_resizable; }
       

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
        std::map<std::string, mvPythonParser>* getParsers      () { return m_parsers.get(); }
        std::mutex& getMutex() const { return m_mutex; }
        std::unordered_map<mvAppItemType, mvThemeColors>& getColors() { return m_colors; }
        std::unordered_map<mvAppItemType, mvThemeStyles>& getStyles() { return m_styles; }
            
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
        mvOwnedPtr<mvCallbackRegistry>                 m_callbackRegistry;
        std::unordered_map<mvAppItemType, mvThemeColors> m_colors;
        std::unordered_map<mvAppItemType, mvThemeStyles> m_styles;
                                                     
        // docking                                   
        bool                                         m_docking          = false;
        bool                                         m_dockingShiftOnly = true;
        bool                                         m_dockingViewport  = false;
                                                     
        mvWindow*                                    m_viewport = nullptr;
        int                                          m_actualWidth = 1280;
        int                                          m_actualHeight = 800;
        int                                          m_clientWidth = 1280;
        int                                          m_clientHeight = 800;
        int                                          m_mainXPos = 100;
        int                                          m_mainYPos = 100;
        std::string                                  m_title = "DearPyGui";
        mvRef<std::map<std::string, mvPythonParser>> m_parsers;
        
        // appearance
        float       m_globalFontScale = 1.0f;
        bool        m_vsync = true;
        bool        m_resizable = true;

        // fonts
        std::string                         m_fontFile;
        std::string                         m_fontGlyphRange;
        float                               m_fontSize = 13.0f;
        std::vector<std::array<ImWchar, 3>> m_fontGlyphRangeCustom;
        std::vector<ImWchar>                m_fontGlyphChars;
        std::vector<std::pair<int, int>>    m_charRemaps;

        // timing
        float                        m_deltaTime = 0.0f; // time since last frame
        double                       m_time      = 0.0;  // total time since starting
        
        std::thread::id                  m_mainThreadID;
        mutable std::mutex               m_mutex;
        std::future<bool>                m_future;

    };

}