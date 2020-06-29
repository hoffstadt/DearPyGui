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

namespace Marvel {

	class mvApp final
	{

	public:

		static mvApp*      GetApp();
		static const char* GetVersion() { return MV_SANDBOX_VERSION; }

		// precheck before the main render loop has started
		void preRender();

		// actual render loop
		void render();

		//-----------------------------------------------------------------------------
		// App Settings
		//-----------------------------------------------------------------------------
		void                    setAppTheme      (const std::string& theme);
		void                    setWindowSize    (unsigned width, unsigned height);
		void                    setModuleDict    (PyObject* dict) { m_pDict = dict; }
		void                    setStarted       () { m_started = true; }
		void                    changeThemeItem  (long item, mvColor color);
		void                    changeStyleItem  (long item, float x, float y);
		void                    addItemColorStyle(const std::string& name, ImGuiCol item, mvColor color);
		unsigned                getWindowWidth   () const { return m_windows[0]->getWidth(); }
		unsigned                getWindowHeight  () const { return m_windows[0]->getHeight(); }
		bool&                   isLoggerShown    () { return m_showLog; }
		const std::string&      getAppTheme      () const { return m_theme; }
		std::pair<float, float> getStyleItem     (long item);
		mvColor                 getThemeItem     (long item);
		void                    setActiveWindow  (const std::string& window) { m_activeWindow = window; }
		const std::string&      getActiveWindow  () const { return m_activeWindow; }


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
		// Adding Items
		//-----------------------------------------------------------------------------
		void       addItem         (mvAppItem* item, bool noParent = false);
		void       addWindow       (mvAppItem* item);
		mvAppItem* getItem         (const std::string& name);
		void       deleteItem      (const std::string& name) { m_deleteQueue.push(name); }

		//-----------------------------------------------------------------------------
		// Direct DearImGui Calls
		//     - should only be used in callbacks
		//-----------------------------------------------------------------------------
		void closePopup();

		//-----------------------------------------------------------------------------
		// Standard Windows
		//-----------------------------------------------------------------------------
		inline void showMetrics() { m_showMetrics = true; }
		inline void showAbout  () { m_showAbout   = true; }
		inline void showSource () { m_showSource  = true; }
		inline void showLogger () { m_showLog     = true; }
		inline void showDoc    () { m_showDoc     = true; }

		//-----------------------------------------------------------------------------
		// Parent stack operations
		//-----------------------------------------------------------------------------
		void       pushParent(mvAppItem* item); // pushes parent onto stack
		mvAppItem* popParent();                 // pop parent off stack and return it
		mvAppItem* topParent();                 // return top parent

		//-----------------------------------------------------------------------------
		// Callbacks
		//     - triggerCallback methods performs checks to determine if callback
		//     - actually exists
		//-----------------------------------------------------------------------------
		void runMainCallback            (const std::string& name, const std::string& sender);
		void runCallback                (const std::string& name, const std::string& sender);
		void triggerCallback            (std::atomic<bool>* p, const std::string* name, const std::string* sender);
		void runCallbackD               (const std::string& name, int sender, float data = 0.0f);                // data sending version
		void triggerCallbackD           (std::atomic<bool>* p, const std::string* name, int sender, float data); // data sending version

		void setMainCallback            (const std::string& callback) { m_callback = callback; }
		void setMouseClickCallback      (const std::string& callback) { m_mouseClickCallback = callback; }
		void setMouseDownCallback       (const std::string& callback) { m_mouseDownCallback = callback; }
		void setMouseDoubleClickCallback(const std::string& callback) { m_mouseDoubleClickCallback = callback; }
		void setMouseReleaseCallback    (const std::string& callback) { m_mouseReleaseCallback = callback; }
		void setKeyDownCallback         (const std::string& callback) { m_keyDownCallback = callback; }
		void setKeyPressCallback        (const std::string& callback) { m_keyPressCallback = callback; }
		void setKeyReleaseCallback      (const std::string& callback) { m_keyReleaseCallback = callback; }

		const std::string& getMainCallback            () const { return m_callback; }
		const std::string& getMouseReleaseCallback    () const { return m_mouseReleaseCallback; }
		const std::string& getMouseClickCallback      () const { return m_mouseClickCallback; }
		const std::string& getMouseDownCallback       () const { return m_mouseDownCallback; }
		const std::string& getMouseDoubleClickCallback() const { return m_mouseDoubleClickCallback; }
		const std::string& getKeyDownCallback         () const { return m_keyDownCallback; }
		const std::string& getKeyPressCallback        () const { return m_keyPressCallback; }
		const std::string& getKeyReleaseCallback      () const { return m_keyReleaseCallback; }

		//-----------------------------------------------------------------------------
		// Input Polling
		//-----------------------------------------------------------------------------
		void       setMousePosition    (float x, float y) { m_mousePos.x = x; m_mousePos.y = y; }
		mvMousePos getMousePosition    () const { return m_mousePos; }
		bool       isMouseButtonPressed(int button)  const;
		bool       isKeyPressed        (int keycode) const;
			
	private:

		mvApp();

		mvApp(const mvApp& other) = delete;
		mvApp(mvApp&& other) = delete;
		mvApp operator=(const mvApp& other) = delete;
		mvApp operator=(mvApp&& other) = delete;

	private:

		static mvApp*           s_instance;
		std::string             m_theme = "dark";
		std::string             m_file;
		mvMousePos              m_mousePos;
		mvStyle                 m_style;
		std::string             m_activeWindow = "MainWindow";
		std::vector<mvAppItem*> m_windows;
		std::stack<mvAppItem*>  m_parents;
		std::queue<std::string> m_deleteQueue;
		PyObject*               m_pDict;
		std::string             m_callback;
		bool                    m_ok = true;
		bool                    m_showLog = false;
		bool                    m_showMetrics = false;
		bool                    m_showAbout = false;
		bool                    m_showSource = false;
		bool                    m_showDoc = false;
		bool                    m_started = false; // to prevent widgets from being added
		bool                    m_threadPoolAuto = true;
		bool                    m_threadPool = false;
		double                  m_threadPoolThreshold = 1.0;
		unsigned                m_threads = 2; // how many threads to use
		bool                    m_threadPoolHighPerformance = false;

		// standard callbacks
		std::string m_mouseDownCallback = "";
		std::string m_mouseClickCallback = "";
		std::string m_mouseReleaseCallback = "";
		std::string m_mouseDoubleClickCallback = "";
		std::string m_keyDownCallback = "";
		std::string m_keyPressCallback = "";
		std::string m_keyReleaseCallback = "";

	};

}