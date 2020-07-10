#pragma once

#include <vector>
#include <map>
#include <stack>
#include <string>
#include <atomic>
#include <queue>
#include <chrono>
#include "Core/AppItems/mvAppItem.h"
#include "mvMouse.h"
#include "mvAppStyle.h"
#include "mvTextEditor.h"
#include "mvModuleInitializer.h"
#include "Core/StandardWindows/mvStandardWindow.h"
#include "Core/Concurrency/mvThreadPool.h"

typedef std::chrono::high_resolution_clock clock_;
typedef std::chrono::duration<double, std::ratio<1> > second_;

namespace Marvel {

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

		static mvApp*            GetApp();
		static mvStandardWindow* GetAppStandardWindow();
		static const char*       GetVersion() { return MV_SANDBOX_VERSION; }

		~mvApp();

		void         precheck  ();                            // precheck before the main render loop has started
		virtual void prerender () override;                   // pre rendering (every frame)	
		virtual void render    (bool& show) override;         // actual render loop		
		virtual void postrender() override;                   // post rendering (every frame)
		bool         isStarted () const { return m_started; } // has the application started running

		//-----------------------------------------------------------------------------
		// App Settings
		//-----------------------------------------------------------------------------
		void                     setFile        (const std::string& file);
		void                     setWindowSize  (unsigned width, unsigned height);
		void                     setModuleDict  (PyObject* dict) { m_pDict = dict; }
		void                     setStarted     () { m_started = true; }
		void                     setActiveWindow(const std::string& window) { m_activeWindow = window; }

		const std::string&       getFile        () const { return m_file; }
		const std::string&       getActiveWindow() const { return m_activeWindow; }
		std::vector<mvAppItem*>& getWindows     ()       { return m_windows; }

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
		// Concurrency Settings
		//-----------------------------------------------------------------------------
		void              setMainThreadID               (std::thread::id id) { m_mainThreadID = id; }
		void              setThreadPoolTimeout          (double time) { m_threadPoolTimeout = time; }
		void              setThreadCount                (unsigned count) { m_threads = count; }
		void              activateThreadPool            () { m_threadPool = true; }
		void              setThreadPoolHighPerformance  () { m_threadPoolHighPerformance = true; }

		std::thread::id   getMainThreadID               () const { return m_mainThreadID; }
		double            getThreadPoolTimeout          () const { return m_threadPoolTimeout; }
		unsigned          getThreadCount                () const { return m_threads; }
		bool              usingThreadPool               () const { return m_threadPool; }
		bool              usingThreadPoolHighPerformance() const { return m_threadPoolHighPerformance; }

		//-----------------------------------------------------------------------------
		// Data Storage Operations
		//-----------------------------------------------------------------------------
		void       addData     (const std::string& name, PyObject* data);
		PyObject*  getData     (const std::string& name);
		void       deleteData  (const std::string& name);
		unsigned   getDataCount() const { return m_dataStorage.size(); }

		//-----------------------------------------------------------------------------
		// App Item Operations
		//-----------------------------------------------------------------------------
		void       addItem           (mvAppItem* item);
		void       addWindow         (mvAppItem* item);
		void       addRuntimeItem    (const std::string& parent, const std::string& before, mvAppItem* item);
		mvAppItem* getItem           (const std::string& name);
		void       deleteItem        (const std::string& name) { if(name!="MainWindow") m_deleteQueue.push(name); }
		void       deleteItemChildren(const std::string& name) { m_deleteChildrenQueue.push(name); }
		void       moveItemUp        (const std::string& name) { m_upQueue.push(name); }
		void       moveItemDown      (const std::string& name) { m_downQueue.push(name); }
		
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
		void runCallback                (const std::string& name, const std::string& sender);
		void runCallbackP               (const std::string& name, const std::string& sender, PyObject* data);
		void runCallbackD               (const std::string& name, int sender, float data = 0.0f);
		void runAsyncCallback           (std::string name, PyObject* data, std::string returnname);
		void runAsyncCallbackReturn     (std::string name, PyObject* data);
		void addMTCallback              (const std::string& name, PyObject* data, const std::string& returnname = "");

		void setMainCallback            (const std::string& callback) { m_callback = callback; }
		void setMouseClickCallback      (const std::string& callback) { m_mouseClickCallback = callback; }
		void setMouseDownCallback       (const std::string& callback) { m_mouseDownCallback = callback; }
		void setMouseDoubleClickCallback(const std::string& callback) { m_mouseDoubleClickCallback = callback; }
		void setMouseReleaseCallback    (const std::string& callback) { m_mouseReleaseCallback = callback; }
		void setMouseWheelCallback      (const std::string& callback) { m_mouseWheelCallback = callback; }
		void setMouseDragCallback       (const std::string& callback) { m_mouseDragCallback = callback; }
		void setKeyDownCallback         (const std::string& callback) { m_keyDownCallback = callback; }
		void setKeyPressCallback        (const std::string& callback) { m_keyPressCallback = callback; }
		void setKeyReleaseCallback      (const std::string& callback) { m_keyReleaseCallback = callback; }

		const std::string& getMainCallback            () const { return m_callback; }
		const std::string& getMouseReleaseCallback    () const { return m_mouseReleaseCallback; }
		const std::string& getMouseClickCallback      () const { return m_mouseClickCallback; }
		const std::string& getMouseDownCallback       () const { return m_mouseDownCallback; }
		const std::string& getMouseDoubleClickCallback() const { return m_mouseDoubleClickCallback; }
		const std::string& getMouseDragCallback       () const { return m_mouseDragCallback; }
		const std::string& getKeyDownCallback         () const { return m_keyDownCallback; }
		const std::string& getKeyPressCallback        () const { return m_keyPressCallback; }
		const std::string& getKeyReleaseCallback      () const { return m_keyReleaseCallback; }
		const std::string& getMouseWheelCallback      () const { return m_mouseWheelCallback; }

		//-----------------------------------------------------------------------------
		// Inputs
		//-----------------------------------------------------------------------------
		void          setMousePosition     (float x, float y) { m_mousePos.x = x; m_mousePos.y = y; }
		void          setMouseDragThreshold(float threshold) { m_mouseDragThreshold = threshold; }
		void          setMouseDragging     (bool drag) { m_mouseDragging = drag; }
		void          setMouseDragDelta    (const mvVec2& delta) { m_mouseDragDelta = delta; }

		float         getDeltaTime         () const { return m_deltaTime; }
		double        getTotalTime         () const { return m_time; }
		float         getMouseDragThreshold() const { return m_mouseDragThreshold; }
		const mvVec2& getMouseDragDelta    () const { return m_mouseDragDelta; }
		mvMousePos    getMousePosition     () const { return m_mousePos; }

		// input polling
		bool          isMouseDragging             (int button, float threshold) const;
		bool          isMouseButtonDown           (int button)  const;
		bool          isMouseButtonClicked        (int button)  const;
		bool          isMouseButtonDoubleClicked  (int button)  const;
		bool          isMouseButtonReleased       (int button)  const;
		bool          isKeyDown                   (int keycode) const;
		bool          isKeyPressed                (int keycode) const;
		bool          isKeyReleased               (int keycode) const;
			
	private:

		mvApp();

		mvApp          (const mvApp& other) = delete;
		mvApp          (mvApp&& other)      = delete;
		mvApp operator=(const mvApp& other) = delete;
		mvApp operator=(mvApp&& other)      = delete;

	private:

		static mvApp* s_instance;
		mvThreadPool* m_tpool = nullptr;
		mvStyle       m_style;
		std::string   m_theme = "Dark";
		std::string   m_file;
		PyObject*     m_pDict;
		float         m_deltaTime; // time since last frame
		double        m_time;      // total time since starting

		std::stack<mvAppItem*>  m_parents;
		std::vector<mvAppItem*> m_windows;

		// runtime widget modifications
		std::queue<std::string>          m_deleteChildrenQueue;
		std::queue<std::string>          m_deleteQueue;
		std::queue<std::string>          m_upQueue;
		std::queue<std::string>          m_downQueue;
		std::queue<AsyncronousCallback>  m_asyncReturns;
		std::vector<NewRuntimeItem>      m_newItemVec;
		std::vector<AsyncronousCallback> m_asyncCallbacks;
		std::map<std::string, PyObject*> m_dataStorage;
		
		// concurrency
		std::thread::id                       m_mainThreadID;
		bool                                  m_threadPool = false;                // is threadpool activated
		double                                m_threadPoolTimeout = 30.0;
		unsigned                              m_threads = 2;                       // how many threads to use
		bool                                  m_threadPoolHighPerformance = false; // when true, use max number of threads
		double                                m_threadTime = 0.0;                  // how long threadpool has been active
		mutable std::mutex                    m_mutex;
		std::chrono::steady_clock::time_point m_poolStart;                         // threadpool start time
		
		// input state
		bool        m_started = false;              // to change to runtime behavior
		std::string m_activeWindow = "MainWindow";
		mvMousePos  m_mousePos;
		float       m_mouseWheel;
		float       m_mouseDragThreshold = 20.0f;
		bool        m_mouseDragging = false;
		mvVec2      m_mouseDragDelta = { 0.0f, 0.0f };

		// standard callbacks
		std::string m_callback = "";
		std::string m_mouseDownCallback = "";
		std::string m_mouseClickCallback = "";
		std::string m_mouseReleaseCallback = "";
		std::string m_mouseDoubleClickCallback = "";
		std::string m_mouseWheelCallback = "";
		std::string m_mouseDragCallback = "";
		std::string m_keyDownCallback = "";
		std::string m_keyPressCallback = "";
		std::string m_keyReleaseCallback = "";

	};

}