#pragma once
#include <queue>
#include <string>
#include <mutex>
#include "mvEvents.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

	static PyObject* SanitizeCallback(PyObject* callback)
	{
		if (callback == Py_None)
			return nullptr;
		return callback;
	}

	class mvThreadPool;

	class mvCallbackRegistry : public mvEventHandler
	{
		struct NewCallback
		{
			std::string sender;
			PyObject* callback;   // name of function to run
			PyObject* data;       // any data need by the function
		};

		struct AsyncronousCallback
		{
			PyObject* name;       // name of function to run
			PyObject* data;       // any data need by the function
			PyObject* returnname; // optional return function
		};

	public:

		static mvCallbackRegistry* GetCallbackRegistry();

		bool onEvent   (mvEvent& event) override;
		bool onFrame   (mvEvent& event);
		bool onEndFrame(mvEvent& event);
		bool onInputs  (mvEvent& event);
		bool onRender  (mvEvent& event);

		void runReturnCallback(PyObject* callback, const std::string& sender, PyObject* data);
        void runCallback      (PyObject* callback, const std::string& sender, PyObject* data = nullptr);
        void runAsyncCallback (PyObject* callback, PyObject* data, PyObject* returnname);
        void addMTCallback    (PyObject* name, PyObject* data, PyObject* returnname = nullptr);
        void addCallback      (PyObject* callback, const std::string& sender, PyObject* data);

		void runAsyncCallbacks(mvThreadPool* threadpool);
		void runAsyncCallbackReturns();
		void runCallbacks();

		bool hasAsyncCallbacks() { return !m_asyncCallbacks.empty(); }

		//-----------------------------------------------------------------------------
        // Callbacks
        //----------------------------------------------------------------------------- 
        void setRenderCallback          (PyObject* callback) { m_renderCallback = SanitizeCallback(callback); }
        void setResizeCallback          (PyObject* callback) { m_resizeCallback = SanitizeCallback(callback); }
        void setMouseMoveCallback       (PyObject* callback) { m_mouseMoveCallback = SanitizeCallback(callback); }
        void setOnCloseCallback         (PyObject* callback) { m_onCloseCallback = SanitizeCallback(callback); }
        void setOnStartCallback         (PyObject* callback) { m_onStartCallback = SanitizeCallback(callback); }
        void setAcceleratorCallback     (PyObject* callback) { m_acceleratorCallback = SanitizeCallback(callback); }
        void setMouseClickCallback      (PyObject* callback) { m_mouseClickCallback = SanitizeCallback(callback); }
        void setMouseDownCallback       (PyObject* callback) { m_mouseDownCallback = SanitizeCallback(callback); }
        void setMouseDoubleClickCallback(PyObject* callback) { m_mouseDoubleClickCallback = SanitizeCallback(callback); }
        void setMouseReleaseCallback    (PyObject* callback) { m_mouseReleaseCallback = SanitizeCallback(callback); }
        void setMouseWheelCallback      (PyObject* callback) { m_mouseWheelCallback = SanitizeCallback(callback); }
        void setMouseDragCallback       (PyObject* callback) { m_mouseDragCallback = SanitizeCallback(callback); }
        void setKeyDownCallback         (PyObject* callback) { m_keyDownCallback = SanitizeCallback(callback); }
        void setKeyPressCallback        (PyObject* callback) { m_keyPressCallback = SanitizeCallback(callback); }
        void setKeyReleaseCallback      (PyObject* callback) { m_keyReleaseCallback = SanitizeCallback(callback); }

        [[nodiscard]] PyObject* getRenderCallback          (){ return m_renderCallback; }
        [[nodiscard]] PyObject* getResizeCallback          (){ return m_resizeCallback; }
        [[nodiscard]] PyObject* getMouseReleaseCallback    (){ return m_mouseReleaseCallback; }
        [[nodiscard]] PyObject* getMouseClickCallback      (){ return m_mouseClickCallback; }
        [[nodiscard]] PyObject* getMouseDownCallback       (){ return m_mouseDownCallback; }
        [[nodiscard]] PyObject* getMouseDoubleClickCallback(){ return m_mouseDoubleClickCallback; }
        [[nodiscard]] PyObject* getMouseDragCallback       (){ return m_mouseDragCallback; }
        [[nodiscard]] PyObject* getKeyDownCallback         (){ return m_keyDownCallback; }
        [[nodiscard]] PyObject* getKeyPressCallback        (){ return m_keyPressCallback; }
        [[nodiscard]] PyObject* getKeyReleaseCallback      (){ return m_keyReleaseCallback; }
        [[nodiscard]] PyObject* getMouseWheelCallback      (){ return m_mouseWheelCallback; }
        [[nodiscard]] PyObject* getMouseMoveCallback       (){ return m_mouseMoveCallback; }
        [[nodiscard]] PyObject* getOnCloseCallback         (){ return m_onCloseCallback; }
        [[nodiscard]] PyObject* getOnStartCallback         (){ return m_onStartCallback; }
        [[nodiscard]] PyObject* getAcceleratorCallback     (){ return m_acceleratorCallback; }
	
	private:

		mvCallbackRegistry();

		static mvCallbackRegistry*       s_instance;
		mutable std::mutex               m_mutex;

		// new callback system
		std::queue<NewCallback>          m_callbacks;

		// concurrency
		std::queue<AsyncronousCallback>  m_asyncReturns;
		std::vector<AsyncronousCallback> m_asyncCallbacks;

		// input callbacks
		PyObject* m_renderCallback = nullptr;
		PyObject* m_mouseDownCallback = nullptr;
		PyObject* m_mouseClickCallback = nullptr;
		PyObject* m_mouseReleaseCallback = nullptr;
		PyObject* m_mouseDoubleClickCallback = nullptr;
		PyObject* m_mouseWheelCallback = nullptr;
		PyObject* m_mouseDragCallback = nullptr;
		PyObject* m_keyDownCallback = nullptr;
		PyObject* m_keyPressCallback = nullptr;
		PyObject* m_keyReleaseCallback = nullptr;
		PyObject* m_resizeCallback = nullptr;
		PyObject* m_mouseMoveCallback = nullptr;
		PyObject* m_onCloseCallback = nullptr;
		PyObject* m_onStartCallback = nullptr;
		PyObject* m_acceleratorCallback = nullptr; // basically the same as the key press callback

	};

}