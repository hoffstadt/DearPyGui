#pragma once
#include <mutex>
#include "mvThreadPool.h"
#include "mvApp.h"
#include "mvAppLog.h"

namespace Marvel {

	static mvCallable SanitizeCallback(mvCallable callback)
	{
		if (callback == Py_None)
			return nullptr;

		return callback;
	}

	class mvThreadPool;

	class mvCallbackRegistry : public mvEventHandler
	{

		static constexpr const int s_MaxNumberOfCalls = 50;

		struct NewCallback
		{
			std::string sender;
			PyObject* callback;   // name of function to run
			PyObject* data;       // any data need by the function
		};

	public:

		mvCallbackRegistry();

		bool onEvent   (mvEvent& event) override;
		bool onFrame   (mvEvent& event);
		bool onEndFrame(mvEvent& event);
		bool onInputs  (mvEvent& event);
		bool onRender  (mvEvent& event);

		void runTasks();

        void runCallback      (mvCallable callback, const std::string& sender, PyObject* data = nullptr);
        void addCallback      (mvCallable callback, const std::string& sender, mvCallableData data);
		
		bool runCallbacks();

		void stop() { m_running = false; }

		template<typename F, typename ...Args>
		std::future<typename std::invoke_result<F, Args...>::type> submit(F f)
		{

			typedef typename std::invoke_result<F, Args...>::type result_type;
			std::packaged_task<result_type()> task(std::move(f));
			std::future<result_type> res(task.get_future());

			if (mvApp::IsAppStarted())
				m_tasks.push(std::move(task));
			else
				task();

			return res;
		}

		template<typename F, typename ...Args>
		std::future<typename std::invoke_result<F, Args...>::type> submitCallback(F f)
		{

			if (m_callCount > s_MaxNumberOfCalls)
			{
				mvAppLog::LogWarning("Too many callbacks already in the queue.");
				return {};
			}

			m_callCount++;

			typedef typename std::invoke_result<F, Args...>::type result_type;
			std::packaged_task<result_type()> task(std::move(f));
			std::future<result_type> res(task.get_future());

			m_calls.push(std::move(task));

			return res;
		}

		//-----------------------------------------------------------------------------
        // Callbacks
        //----------------------------------------------------------------------------- 
        void setResizeCallback          (mvCallable callback) { m_resizeCallback = SanitizeCallback(callback); }
        void setMouseMoveCallback       (mvCallable callback) { m_mouseMoveCallback = SanitizeCallback(callback); }
        void setOnCloseCallback         (mvCallable callback) { m_onCloseCallback = SanitizeCallback(callback); }
        void setOnStartCallback         (mvCallable callback) { m_onStartCallback = SanitizeCallback(callback); }
        void setAcceleratorCallback     (mvCallable callback) { m_acceleratorCallback = SanitizeCallback(callback); }
        void setMouseClickCallback      (mvCallable callback) { m_mouseClickCallback = SanitizeCallback(callback); }
        void setMouseDownCallback       (mvCallable callback) { m_mouseDownCallback = SanitizeCallback(callback); }
        void setMouseDoubleClickCallback(mvCallable callback) { m_mouseDoubleClickCallback = SanitizeCallback(callback); }
        void setMouseReleaseCallback    (mvCallable callback) { m_mouseReleaseCallback = SanitizeCallback(callback); }
        void setMouseWheelCallback      (mvCallable callback) { m_mouseWheelCallback = SanitizeCallback(callback); }
        void setMouseDragCallback       (mvCallable callback) { m_mouseDragCallback = SanitizeCallback(callback); }
        void setKeyDownCallback         (mvCallable callback) { m_keyDownCallback = SanitizeCallback(callback); }
        void setKeyPressCallback        (mvCallable callback) { m_keyPressCallback = SanitizeCallback(callback); }
        void setKeyReleaseCallback      (mvCallable callback) { m_keyReleaseCallback = SanitizeCallback(callback); }

        [[nodiscard]] mvCallable getResizeCallback          (){ return m_resizeCallback; }
        [[nodiscard]] mvCallable getMouseReleaseCallback    (){ return m_mouseReleaseCallback; }
        [[nodiscard]] mvCallable getMouseClickCallback      (){ return m_mouseClickCallback; }
        [[nodiscard]] mvCallable getMouseDownCallback       (){ return m_mouseDownCallback; }
        [[nodiscard]] mvCallable getMouseDoubleClickCallback(){ return m_mouseDoubleClickCallback; }
        [[nodiscard]] mvCallable getMouseDragCallback       (){ return m_mouseDragCallback; }
        [[nodiscard]] mvCallable getKeyDownCallback         (){ return m_keyDownCallback; }
        [[nodiscard]] mvCallable getKeyPressCallback        (){ return m_keyPressCallback; }
        [[nodiscard]] mvCallable getKeyReleaseCallback      (){ return m_keyReleaseCallback; }
        [[nodiscard]] mvCallable getMouseWheelCallback      (){ return m_mouseWheelCallback; }
        [[nodiscard]] mvCallable getMouseMoveCallback       (){ return m_mouseMoveCallback; }
        [[nodiscard]] mvCallable getOnCloseCallback         (){ return m_onCloseCallback; }
        [[nodiscard]] mvCallable getOnStartCallback         (){ return m_onStartCallback; }
        [[nodiscard]] mvCallable getAcceleratorCallback     (){ return m_acceleratorCallback; }
	
	private:

		// new callback system
		mvQueue<mvFunctionWrapper> m_tasks;
		mvQueue<mvFunctionWrapper> m_calls;
		std::atomic<bool> m_running;
		std::atomic<int> m_callCount = 0;

		// input callbacks
		mvCallable m_mouseDownCallback = nullptr;
		mvCallable m_mouseClickCallback = nullptr;
		mvCallable m_mouseReleaseCallback = nullptr;
		mvCallable m_mouseDoubleClickCallback = nullptr;
		mvCallable m_mouseWheelCallback = nullptr;
		mvCallable m_mouseDragCallback = nullptr;
		mvCallable m_keyDownCallback = nullptr;
		mvCallable m_keyPressCallback = nullptr;
		mvCallable m_keyReleaseCallback = nullptr;
		mvCallable m_resizeCallback = nullptr;
		mvCallable m_mouseMoveCallback = nullptr;
		mvCallable m_onCloseCallback = nullptr;
		mvCallable m_onStartCallback = nullptr;
		mvCallable m_acceleratorCallback = nullptr; // basically the same as the key press callback

	};

}