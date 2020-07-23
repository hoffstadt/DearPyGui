#pragma once

//-----------------------------------------------------------------------------
// mvEventHandler
//
//     - This is a temporary class for v0.1 until a more complete event 
//       handling system is put in place. Which will be required for the
//       upcoming v0.2 custom widget API.
//     
//     - Currently this is used by mvWindowAppItem and mvApp
//-----------------------------------------------------------------------------

#include <string>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvEventHandler
	//-----------------------------------------------------------------------------
	class mvEventHandler
	{

	public:

		mvEventHandler() = default;

		bool isMouseHandled   () const { return m_handleMouse; }
		bool isKeyboardHandled() const { return m_handleKeyboard; }

		//-----------------------------------------------------------------------------
		// Callbacks
		//-----------------------------------------------------------------------------
		void setRenderCallback          (const std::string& callback) { m_renderCallback = callback; }
		void setResizeCallback          (const std::string& callback) { m_resizeCallback = callback; }
		void setMouseClickCallback      (const std::string& callback) { m_handleMouse = true; m_mouseClickCallback = callback; }
		void setMouseDownCallback       (const std::string& callback) { m_handleMouse = true; m_mouseDownCallback = callback; }
		void setMouseDoubleClickCallback(const std::string& callback) { m_handleMouse = true; m_mouseDoubleClickCallback = callback; }
		void setMouseReleaseCallback    (const std::string& callback) { m_handleMouse = true; m_mouseReleaseCallback = callback; }
		void setMouseWheelCallback      (const std::string& callback) { m_handleMouse = true; m_mouseWheelCallback = callback; }
		void setMouseDragCallback       (const std::string& callback) { m_handleMouse = true; m_mouseDragCallback = callback; }
		void setKeyDownCallback         (const std::string& callback) { m_handleKeyboard = true; m_keyDownCallback = callback; }
		void setKeyPressCallback        (const std::string& callback) { m_handleKeyboard = true; m_keyPressCallback = callback; }
		void setKeyReleaseCallback      (const std::string& callback) { m_handleKeyboard = true; m_keyReleaseCallback = callback; }

		const std::string& getRenderCallback          () const { return m_renderCallback; }
		const std::string& getResizeCallback          () const { return m_resizeCallback; }
		const std::string& getMouseReleaseCallback    () const { return m_mouseReleaseCallback; }
		const std::string& getMouseClickCallback      () const { return m_mouseClickCallback; }
		const std::string& getMouseDownCallback       () const { return m_mouseDownCallback; }
		const std::string& getMouseDoubleClickCallback() const { return m_mouseDoubleClickCallback; }
		const std::string& getMouseDragCallback       () const { return m_mouseDragCallback; }
		const std::string& getKeyDownCallback         () const { return m_keyDownCallback; }
		const std::string& getKeyPressCallback        () const { return m_keyPressCallback; }
		const std::string& getKeyReleaseCallback      () const { return m_keyReleaseCallback; }
		const std::string& getMouseWheelCallback      () const { return m_mouseWheelCallback; }

	private:

		// standard callbacks
		std::string m_renderCallback = "";
		std::string m_mouseDownCallback = "";
		std::string m_mouseClickCallback = "";
		std::string m_mouseReleaseCallback = "";
		std::string m_mouseDoubleClickCallback = "";
		std::string m_mouseWheelCallback = "";
		std::string m_mouseDragCallback = "";
		std::string m_keyDownCallback = "";
		std::string m_keyPressCallback = "";
		std::string m_keyReleaseCallback = "";
		std::string m_resizeCallback = "";

		bool m_handleMouse    = false;
		bool m_handleKeyboard = false;

	};

}