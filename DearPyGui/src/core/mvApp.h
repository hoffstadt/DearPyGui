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

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <queue>
#include <thread>
#include "mvEvents.h"
#include "core/AppItems/mvAppItem.h"
#include "PythonUtilities/mvPythonParser.h"
#include "Registries/mvItemRegistry.h"
#include "Registries/mvDrawList.h"
#include "Registries/mvTextureStorage.h"

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef std::map<ImGuiStyleVar, ImVec2> mvStyle;

namespace Marvel {

    //-----------------------------------------------------------------------------
    // Forward Declarations
    //-----------------------------------------------------------------------------
    class mvWindowAppitem;
    class mvTextEditor;
    class mvWindow;
    struct mvColor;
    
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

        void start(const std::string& primaryWindow);

        ~mvApp();

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
        // App Settings
        //-----------------------------------------------------------------------------
        void                     turnOnDocking     (bool shiftOnly, bool dockSpace);
        void                     addRemapChar      (int dst, int src) { m_charRemaps.push_back({ dst, src }); }
        void                     setVSync          (bool value) { m_vsync = value; }
        void                     setResizable      (bool value) { m_resizable = value; }			
        void                     setMainPos        (int x, int y);			
        void                     setActiveWindow   (const std::string& window) { m_activeWindow = window; }
        void                     setGlobalFontScale(float scale);
        void                     setViewport       (mvWindow* viewport) { m_viewport = viewport; }
        void                     setTitle          (const std::string& title) { m_title = title; }
        void                     setFont(const std::string& file, float size = 13.0f, const std::string& glyphRange = "",
                                    std::vector<std::array<ImWchar, 3>> customRanges = {},
                                    std::vector<ImWchar> chars= {});
        
        const std::string&       getActiveWindow   () const { return m_activeWindow; }
        float&                   getGlobalFontScale()       { return m_globalFontScale; }
        int                      getActualWidth    () const { return m_actualWidth; }
        int                      getActualHeight   () const { return m_actualHeight; }
        int                      getClientWidth    () const { return m_clientWidth; }
        int                      getClientHeight   () const { return m_clientHeight; }
        ImGuiStyle&              getStyle          ()       { return m_newstyle; }
        mvWindow*                getViewport       ()       { return m_viewport; }
        bool                     getVSync          () const { return m_vsync; }
        bool                     getResizable      () const { return m_resizable; }
        
        //-----------------------------------------------------------------------------
        // Styles/Themes
        //-----------------------------------------------------------------------------
        void                     setAppTheme      (const std::string& theme);
        void                     setThemeItem     (long item, mvColor color);
        void                     setStyleChanged  () { m_styleChange = true; }
                                 
        const std::string&       getAppTheme () const { return m_theme; }
        mvColor                  getThemeItem(long item);

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
        std::map<std::string, mvPythonParser>* getParsers      () { return m_parsers; }
            
    private:

        //-----------------------------------------------------------------------------
        // Post Rendering Methods
        //     - actually performs queued operations
        //-----------------------------------------------------------------------------
        void postProfile    ();

        mvApp();

        void updateStyle();
        
    private:

        static mvApp* s_instance;
        static bool   s_started;

        // docking
        bool                                   m_docking          = false;
        bool                                   m_dockingShiftOnly = true;
        bool                                   m_dockingViewport  = false;

        mvWindow*                              m_viewport = nullptr;
        std::string                            m_activeWindow;
        int                                    m_actualWidth = 1280;
        int                                    m_actualHeight = 800;
        int                                    m_clientWidth = 1280;
        int                                    m_clientHeight = 800;
        int                                    m_mainXPos = 100;
        int                                    m_mainYPos = 100;
        std::string                            m_title = "DearPyGui";
        std::map<std::string, mvPythonParser>* m_parsers;
        
        // appearance
        std::string m_theme = "Dark";
        float       m_globalFontScale = 1.0f;
        ImGuiStyle  m_newstyle;
        bool        m_styleChange = true;
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
        float                        m_deltaTime; // time since last frame
        double                       m_time;      // total time since starting
        
        std::thread::id                  m_mainThreadID;

    };

}