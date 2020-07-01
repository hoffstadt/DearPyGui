#pragma once

#define MV_SANDBOX_VERSION "0.1(WIP)"

#include <vector>
#include <map>
#include <stack>
#include <string>
#include <atomic>
#include <queue>
#include "Core/AppItems/mvAppItem.h"
#include "mvMouse.h"
#include "mvAppStyle.h"
#include "mvTextEditor.h"
#include "mvModuleInitializer.h"
#include "Core/StandardWindows/mvStandardWindow.h"

namespace Marvel {

	class mvApp : public mvStandardWindow
	{

		struct NewRuntimeItem
		{
			mvAppItem* item;
			std::string before;
			std::string parent;

			NewRuntimeItem(const std::string& parent, const std::string& before, mvAppItem* item)
				:item(item), before(before), parent(parent)
			{}
		};

	public:

		static mvApp*            GetApp();
		static mvStandardWindow* GetAppStandardWindow();
		static const char*       GetVersion() { return MV_SANDBOX_VERSION; }

		~mvApp();

		void         precheck  ();                           // precheck before the main render loop has started
		virtual void prerender () override;                  // actual render loop		
		virtual void render    (bool& show) override;        // actual render loop		
		virtual void postrender() override;                  // post rendering (every frame)
		bool         isStarted() const { return m_started; } // has the application started running

		//-----------------------------------------------------------------------------
		// App Settings
		//-----------------------------------------------------------------------------
		void                    setFile        (const std::string& file);
		void                    setWindowSize  (unsigned width, unsigned height);
		void                    setModuleDict  (PyObject* dict) { m_pDict = dict; }
		void                    setStarted     () { m_started = true; }
		void                    setActiveWindow(const std::string& window) { m_activeWindow = window; }

		const std::string&       getFile        () const { return m_file; }
		const std::string&       getActiveWindow() const { return m_activeWindow; }
		std::vector<mvAppItem*>& getWindows     () { return m_windows; }

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
		void     setThreadPoolThreshold      (double time) { m_threadPoolThreshold = time; }
		void     setThreadCount              (unsigned count) { m_threads = count; }
		void     setThreadPoolAuto           () { m_threadPoolAuto = true; }
		void     activateThreadPool          () { m_threadPool = true; }
		void     setThreadPoolHighPerformance() { m_threadPoolHighPerformance = true; }

		double   getThreadPoolThreshold        () const { return m_threadPoolThreshold; }
		unsigned getThreadCount                () const { return m_threads; }
		bool     usingThreadPool               () const { return m_threadPool; }
		bool     usingThreadPoolHighPerformance() const { return m_threadPoolHighPerformance; }
		bool     isThreadPoolAuto              () const { return m_threadPoolAuto; }

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
		// Direct DearImGui Calls
		//     - should only be used in callbacks
		//-----------------------------------------------------------------------------
		void closePopup();

		//-----------------------------------------------------------------------------
		// Parent stack operations
		//-----------------------------------------------------------------------------
		void       pushParent(mvAppItem* item); // pushes parent onto stack
		mvAppItem* popParent();                 // pop parent off stack and return it
		mvAppItem* topParent();                 // returns top parent without popping

		//-----------------------------------------------------------------------------
		// Callbacks
		//     - triggerCallback methods performs checks to determine if callback
		//     - actually exists
		//-----------------------------------------------------------------------------
		void runMainCallback            (const std::string& name, const std::string& sender); // single threaded callback
		void runCallback                (const std::string& name, const std::string& sender);
		void triggerCallback            (std::atomic<bool>* p, const std::string* name, const std::string* sender);
		void runCallbackD               (const std::string& name, int sender, float data = 0.0f);                // data sending version
		void triggerCallbackD           (std::atomic<bool>* p, const std::string* name, int sender, float data); // data sending version

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
		// Input Polling
		//-----------------------------------------------------------------------------
		void          setMousePosition     (float x, float y) { m_mousePos.x = x; m_mousePos.y = y; }
		void          setMouseDragThreshold(float threshold) { m_mouseDragThreshold = threshold; }
		void          setMouseDragging     (bool drag) { m_mouseDragging = drag; }
		void          setMouseDragDelta    (const mvVec2& delta) { m_mouseDragDelta = delta; }

		float         getMouseDragThreshold() const { return m_mouseDragThreshold; }
		const mvVec2& getMouseDragDelta    () const { return m_mouseDragDelta; }
		mvMousePos    getMousePosition     () const { return m_mousePos; }
		bool          isMouseDragging      () const { return m_mouseDragging; }
		bool          isMouseButtonPressed (int button)  const;
		bool          isKeyPressed         (int keycode) const;
			
	private:

		mvApp();

		mvApp          (const mvApp& other) = delete;
		mvApp          (mvApp&& other)      = delete;
		mvApp operator=(const mvApp& other) = delete;
		mvApp operator=(mvApp&& other)      = delete;

	private:

		static mvApp* s_instance;
		mvStyle       m_style;
		std::string   m_theme = "dark";
		std::string   m_file;
		PyObject*     m_pDict;

		std::stack<mvAppItem*>      m_parents; // parent stack for adding items
		std::vector<mvAppItem*>     m_windows;

		// runtime widget modifications
		std::queue<std::string>     m_deleteChildrenQueue;
		std::queue<std::string>     m_deleteQueue;
		std::queue<std::string>     m_upQueue;
		std::queue<std::string>     m_downQueue;
		std::vector<NewRuntimeItem> m_newItemVec;
		
		// concurrency
		bool     m_threadPoolAuto = true;             // will the threadpool automatically activate based on threshold
		bool     m_threadPool = false;                // is threadpool activated
		double   m_threadPoolThreshold = 1.0;         // callback delay (seconds) before threadpool activation
		unsigned m_threads = 2;                       // how many threads to use
		bool     m_threadPoolHighPerformance = false; // when true, use max number of threads
		
		// input state
		bool        m_started = false; // to change to runtime behavior
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