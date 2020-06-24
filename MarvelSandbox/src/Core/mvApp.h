#pragma once

#define MV_SANDBOX_VERSION "0.1(WIP)"

#include <vector>
#include <map>
#include <stack>
#include <string>
#include <atomic>
#include "Core/AppItems/mvAppItem.h"
#include "mvMouse.h"
#include "mvAppStyle.h"
#include "mvTextEditor.h"

namespace Marvel {

	class mvApp final
	{

	public:

		static mvApp* GetApp();
		static const char* getVersion() { return MV_SANDBOX_VERSION; }

		// actual render loop
		void preRender();
		void render();

		//-----------------------------------------------------------------------------
		// App Settings
		//-----------------------------------------------------------------------------
		void          setAppTheme      (const std::string& theme);
		void          changeThemeItem  (long item, mvColor color);
		void          changeStyleItem  (long item, float x, float y);
		void          addFlag          (ImGuiWindowFlags flag) { m_windowflags |= flag; }
		void          addItemColorStyle(const std::string& name, ImGuiCol item, mvColor color);
		void          setWindowSize    (unsigned width, unsigned height) { m_width = width; m_height = height; }
		unsigned      getWindowWidth   () const { return m_width; }
		unsigned      getWindowHeight  () const { return m_height; }
		mvTextEditor& getEditor        () { return m_editor; }
		std::string&  getFile          () { return m_file; }

		//-----------------------------------------------------------------------------
		// Adding Items
		//-----------------------------------------------------------------------------
		void       addItemManual   (mvAppItem* item); // only adds item
		void       addItem         (mvAppItem* item); // auto sets parent
		mvAppItem* getItem         (const std::string& name);

		//-----------------------------------------------------------------------------
		// Direct DearImGui Calls
		//     - should only be used in callbacks
		//-----------------------------------------------------------------------------
		void closePopup();

		//-----------------------------------------------------------------------------
		// Standard Windows
		//-----------------------------------------------------------------------------
		inline void showMetrics() { m_showMetrics = true; }
		inline void showAbout  () { m_showAbout = true; }
		inline void showSource () { m_showSource = true; }
		inline void showLogger () { m_showLog = true; }
		inline void showDoc    () { m_showDoc = true; }

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
		bool runCallback                (const std::string& name, const std::string& sender);
		bool runCallbackD               (const std::string& name, const std::string& sender, const std::string& data);
		void triggerCallback            (std::atomic<bool>& p, const std::string& name, const std::string& sender);
		void triggerCallbackD           (std::atomic<bool>& p, const std::string& name, const std::string& sender, const std::string& data); // sends data with the callback
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
		mvMousePos getMousePosition() const { return m_mousePos; }
		bool       isMouseButtonPressed(int button) const;
		bool       isKeyPressed(int keycode) const;

		//-----------------------------------------------------------------------------
		// Internal Utilities
		//-----------------------------------------------------------------------------
		void  setModuleDict (PyObject* dict) { m_pDict = dict; }
		bool& isLoggerShown () { return m_showLog; }
		void  setSize       (unsigned width, unsigned height) { m_width = width; m_height = height; }
		bool  isOk          () const { return m_ok; }
		void  setOk         (bool ok) { m_ok = ok; }
		void  setStarted    () { m_started = true; }
		void  setFile       (const std::string& file);
		void  addKeyword    (const std::string& keyword, const std::string& description) { m_keywords.emplace_back(keyword, description); }
		std::vector<std::pair<std::string, std::string>> getKeywords() { return m_keywords; }
				
	private:

		mvApp();

		mvApp(const mvApp& other) = delete;
		mvApp(mvApp&& other) = delete;
		mvApp operator=(const mvApp& other) = delete;
		mvApp operator=(mvApp&& other) = delete;

	private:

		static mvApp*           s_instance;
		mvMousePos              m_mousePos;
		mvStyle                 m_style;
		std::vector<mvAppItem*> m_items;
		std::stack<mvAppItem*>  m_parents;
		PyObject*               m_pDict;
		unsigned                m_width = 1280;
		unsigned                m_height = 800;
		std::string             m_callback;
		bool                    m_ok = true;
		bool                    m_showLog = false;
		bool                    m_showMetrics = false;
		bool                    m_showAbout = false;
		bool                    m_showSource = false;
		bool                    m_showDoc = false;
		unsigned                m_loglevel = 0;
		ImGuiWindowFlags        m_windowflags = 0;
		bool                    m_started = false; // to prevent widgets from being added
		bool                    m_multithread = false;

		// standard callbacks
		std::string m_mouseDownCallback = "";
		std::string m_mouseClickCallback = "";
		std::string m_mouseReleaseCallback = "";
		std::string m_mouseDoubleClickCallback = "";
		std::string m_keyDownCallback = "";
		std::string m_keyPressCallback = "";
		std::string m_keyReleaseCallback = "";

		mvTextEditor               m_editor;
		std::string                m_file;
		std::vector<std::pair<std::string, std::string>> m_keywords;

	};

}