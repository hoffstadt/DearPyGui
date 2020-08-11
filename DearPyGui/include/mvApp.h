#pragma once

//-----------------------------------------------------------------------------
// mvApp
//
//     - This class acts as the primary manager for a DearPyGui app,
//       with the following responsibilities:
//
//         * Adding/Removing/Modifying AppItems
//         * Callback routing
//         * AppItem parent deduction
//         * Data storage
//         * Texture storage
//     
//-----------------------------------------------------------------------------

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <atomic>
#include <queue>
#include <chrono>
#include <mutex>
#include <thread>
#include "mvStandardWindow.h"
#include "mvAppItem.h"
#include "mvPythonParser.h"


//-----------------------------------------------------------------------------
// Typedefs for chrono's ridiculously long names
//-----------------------------------------------------------------------------
typedef std::chrono::high_resolution_clock clock_;
typedef std::chrono::duration<double, std::ratio<1> > second_;
typedef std::map<ImGuiStyleVar, ImVec2> mvStyle;

#if defined (_WIN32)
typedef std::chrono::steady_clock::time_point time_point_;
#elif defined(__APPLE__)
typedef std::chrono::steady_clock::time_point time_point_;
#else
typedef std::chrono::system_clock::time_point time_point_;
#endif

namespace Marvel {

    //-----------------------------------------------------------------------------
    // Forward Declarations
    //-----------------------------------------------------------------------------
    class mvWindowAppitem;
    class mvThreadPool;
    class mvTextEditor;
    class mvWindow;
    struct mvColor;
    

    //-----------------------------------------------------------------------------
    // mvApp
    //-----------------------------------------------------------------------------
    class mvApp : public mvStandardWindow
    {

        struct NewRuntimeItem
        {
            mvAppItem*  item;   // new item to add
            std::string before; // what item to add new item before
            std::string parent; // what parent to add item to (if not using before)
        };

        struct AsyncronousCallback
        {
            std::string name;       // name of function to run
            PyObject*   data;       // any data need by the function
            std::string returnname; // optional return function
        };

    public:

        mvApp          (const mvApp& other) = delete;
        mvApp          (mvApp&& other)      = delete;
        mvApp operator=(const mvApp& other) = delete;
        mvApp operator=(mvApp&& other)      = delete;

        static mvApp*            GetApp              ();
        static void              DeleteApp           ();
        static mvStandardWindow* GetAppStandardWindow();
        static const char*       GetVersion          () { return MV_SANDBOX_VERSION; }
        static bool              IsAppStarted        () { return s_started; }
        static void              SetAppStarted       () { s_started = true; }

        ~mvApp() override;

        void      precheck  ();                    // precheck before the main render loop has started
        void      prerender ()           override; // pre rendering (every frame)
        void      render    (bool& show) override; // actual render loop
        void      postrender()           override; // post rendering (every frame)
        void      setViewport(mvWindow* viewport) { m_viewport = viewport; }
        mvWindow* getViewport() { return m_viewport; }

        //-----------------------------------------------------------------------------
        // App Settings
        //-----------------------------------------------------------------------------
        void                     setFile           (const std::string& file);
        void                     setArgv0          (const std::string& argv0) { m_argv0 = argv0; }
        void                     setWindowSize     (unsigned width, unsigned height);
        void                     setActualSize     (unsigned width, unsigned height);			
        void                     setActiveWindow   (const std::string& window) { m_activeWindow = window; }
        void                     setGlobalFontScale(float scale);


        const std::string&       getFile           () const { return m_file; }
        const std::string&       getActiveWindow   () const { return m_activeWindow; }
        std::vector<mvAppItem*>& getWindows        ()       { return m_windows; }
        float                    getGlobalFontScale();
        int                      getActualWidth() const { return m_actualWidth; }
        int                      getActualHeight() const { return m_actualHeight; }
        const std::string&       getArgv0() const { return m_argv0; }
        

        //-----------------------------------------------------------------------------
        // Styles/Themes
        //-----------------------------------------------------------------------------
        void                    setAppTheme      (const std::string& theme);
        void                    changeThemeItem  (long item, mvColor color);
        void                    changeStyleItem  (long item, float x, float y);
        void                    addItemColorStyle(const std::string& name, ImGuiCol item, mvColor color);

        const std::string&      getAppTheme () const { return m_theme; }
        std::pair<float, float> getStyleItem(long item);
        mvColor                 getThemeItem(long item);

        //-----------------------------------------------------------------------------
        // Concurrency
        //-----------------------------------------------------------------------------
        void            setMainThreadID               (std::thread::id id) { m_mainThreadID = id; }
        void            setThreadPoolTimeout          (double time) { m_threadPoolTimeout = time; }
        void            setThreadCount                (unsigned count) { m_threads = count; }
        void            activateThreadPool            () { m_threadPool = true; }
        void            setThreadPoolHighPerformance  () { m_threadPoolHighPerformance = true; }

        std::thread::id getMainThreadID               () const { return m_mainThreadID; }
        double          getThreadPoolTimeout          () const { return m_threadPoolTimeout; }
        unsigned        getThreadCount                () const { return m_threads; }
        bool            usingThreadPool               () const { return m_threadPool; }
        bool            usingThreadPoolHighPerformance() const { return m_threadPoolHighPerformance; }

        //-----------------------------------------------------------------------------
        // AppItem Operations
        //-----------------------------------------------------------------------------
        void             addItem           (mvAppItem* item);
        void             addWindow         (mvAppItem* item);
        void             addRuntimeItem    (const std::string& parent, const std::string& before, mvAppItem* item);
        void             deleteItem        (const std::string& name) { if(name!="MainWindow") m_deleteQueue.push(name); }
        void             deleteItemChildren(const std::string& name) { m_deleteChildrenQueue.push(name); }
        void             moveItemUp        (const std::string& name) { m_upQueue.push(name); }
        void             moveItemDown      (const std::string& name) { m_downQueue.push(name); }
        mvAppItem*       getItem           (const std::string& name, bool ignoreRuntime = false);
        mvAppItem*       getRuntimeItem    (const std::string& name);
        mvWindowAppitem* getWindow         (const std::string& name);
        
        //-----------------------------------------------------------------------------
        // Parent stack operations
        //     - used for automatic parent deduction
        //-----------------------------------------------------------------------------
        void       pushParent(mvAppItem* item); // pushes parent onto stack
        mvAppItem* popParent();                 // pop parent off stack and return it
        mvAppItem* topParent();                 // returns top parent without popping

        //-----------------------------------------------------------------------------
        // Callbacks
        //-----------------------------------------------------------------------------
        void runReturnCallback(const std::string& name, const std::string& sender, PyObject* data);
        void runCallback      (const std::string& name, const std::string& sender, PyObject* data = nullptr);
        void runAsyncCallback (std::string name, PyObject* data, std::string returnname);
        void addMTCallback    (const std::string& name, PyObject* data, const std::string& returnname = "");

        template<typename T> 
        void dispatchRenderCallback(mvAppItemType itemType, mvAppItem* item)
        {
            if (item->getType() != itemType) return;
            T* renderType = static_cast<T*>(item);
            auto handler = static_cast<mvEventHandler*>(renderType);
            runCallback(handler->getRenderCallback(), m_activeWindow);
        }

        //-----------------------------------------------------------------------------
        // Timing
        //-----------------------------------------------------------------------------
        float  getDeltaTime() const { return m_deltaTime; }
        double getTotalTime() const { return m_time; }

        std::map<std::string, mvPythonParser>* getParsers() { return m_parsers; }
            
    private:

        mvApp();

        void routeInputCallbacks();
        void changeTheme();
        bool checkIfMainThread();

    private:

        static mvApp* s_instance;
        static bool   s_started;

        mvWindow*               m_viewport = nullptr;
        std::string             m_activeWindow = "MainWindow";
        std::string             m_argv0;
        std::stack<mvAppItem*>  m_parents;
        std::vector<mvAppItem*> m_windows;
        std::string             m_file;
        int                     m_actualWidth = 1280;
        int                     m_actualHeight = 800;

        std::map<std::string, mvPythonParser>* m_parsers;
        
        // appearance
        mvStyle     m_style;
        std::string m_theme = "Dark";
        bool        m_compileTimeThemeSet = false; // theme changed at compile time
        float       m_globalFontScale = 1.0f;

        // runtime widget modifications
        std::queue<std::string>          m_deleteChildrenQueue;
        std::queue<std::string>          m_deleteQueue;
        std::queue<std::string>          m_upQueue;
        std::queue<std::string>          m_downQueue;
        std::queue<AsyncronousCallback>  m_asyncReturns;
        std::vector<NewRuntimeItem>      m_newItemVec;
        std::vector<AsyncronousCallback> m_asyncCallbacks;

        // timing
        float  m_deltaTime; // time since last frame
        double m_time;      // total time since starting
        
        // concurrency
        mvThreadPool*      m_tpool = nullptr;
        mutable std::mutex m_mutex;
        std::thread::id    m_mainThreadID;
        bool               m_threadPool = false;                // is threadpool activated
        double             m_threadPoolTimeout = 30.0;          // how long til trying to delete pool
        unsigned           m_threads = 2;                       // how many threads to use
        bool               m_threadPoolHighPerformance = false; // when true, use max number of threads
        double             m_threadTime = 0.0;                  // how long threadpool has been active
        time_point_        m_poolStart;                         // threadpool start time

    };

}