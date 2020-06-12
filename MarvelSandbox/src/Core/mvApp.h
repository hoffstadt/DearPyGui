#pragma once

#define MV_SANDBOX_VERSION "0.0.1"

#include <vector>
#include <map>
#include <stack>
#include <string>
#include "Core/AppItems/mvAppItem.h"
#include "mvMouse.h"
#include "mvAppStyle.h"

namespace Marvel {

	class mvApp final
	{

	public:

		static mvApp* GetApp();
		static const char* getVersion() { return MV_SANDBOX_VERSION; }

		// actual render loop
		void render();

		bool performChecks();

		//-----------------------------------------------------------------------------
		// App Settings
		//-----------------------------------------------------------------------------
		void setAppTheme    (const std::string& theme);
		void changeThemeItem(long item, mvColor color);
		void changeStyleItem(long item, float x, float y);
		void addFlag        (ImGuiWindowFlags flag) { m_windowflags |= flag; }

		//-----------------------------------------------------------------------------
		// Adding Items
		//-----------------------------------------------------------------------------
		void       addItemManual   (mvAppItem* item); // only adds item
		void       addItem         (mvAppItem* item); // auto sets parent
		void       addParentItem   (mvAppItem* item); // auto sets parent/pushes parent stack
		void       addEndParentItem(mvAppItem* item); // auto sets parent/pops parent stack
		mvAppItem* getItem         (const std::string& name);

		//-----------------------------------------------------------------------------
		// Item modifications
		//-----------------------------------------------------------------------------
		void setItemPopup   (const std::string& name, const std::string& popup);
		void setItemCallback(const std::string& name, const std::string& callback);
		void setItemWidth   (const std::string& name, int width);
		void setItemTip     (const std::string& name, const std::string& tip);
		int  getPopupButton (const std::string& name);

		//-----------------------------------------------------------------------------
		// Direct DearImGui Calls
		//     - should only be used in callbacks
		//-----------------------------------------------------------------------------
		void        closePopup ();
		inline void showMetrics() { m_showMetrics = true; }
		inline void showAbout  () { m_showAbout = true; }

		//-----------------------------------------------------------------------------
		// Parent stack operations
		//-----------------------------------------------------------------------------
		void       pushParent(mvAppItem* item); // pushes parent onto stack
		mvAppItem* popParent();                 // pop parent off stack and return it
		mvAppItem* topParent();                 // return top parent

		//-----------------------------------------------------------------------------
		// Callbacks
		//     - triggerCallback methods performs checks
		//-----------------------------------------------------------------------------
		void triggerCallback            (const std::string& name, const std::string& sender);
		void triggerCallback            (const std::string& name, const std::string& sender, const std::string& data);
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
		// Logging
		//-----------------------------------------------------------------------------
		void  setLogLevel  (unsigned level) { m_loglevel = level; }
		void  Log          (const std::string& text, const std::string& level = "TRACE");
		void  LogDebug     (const std::string& text);
		void  LogInfo      (const std::string& text);
		void  LogWarning   (const std::string& text);
		void  LogError     (const std::string& text);
		void  ClearLog     ();
		void  showLogger   () { m_showLog = true; }
		bool& showLog      () { return m_showLog; }

		//-----------------------------------------------------------------------------
		// Item Status
		//     - Directly from DearImGui (may not all be applicable)
		//-----------------------------------------------------------------------------
		bool   isItemHovered             (const std::string& item);
		bool   isItemActive              (const std::string& item);
		bool   isItemFocused             (const std::string& item);
		bool   isItemClicked             (const std::string& item);
		bool   isItemVisible             (const std::string& item);
		bool   isItemEdited              (const std::string& item);
		bool   isItemActivated           (const std::string& item);
		bool   isItemDeactivated         (const std::string& item);
		bool   isItemDeactivatedAfterEdit(const std::string& item);
		bool   isItemToogledOpen         (const std::string& item);
		mvVec2 getItemRectMin            (const std::string& item);
		mvVec2 getItemRectMax            (const std::string& item);
		mvVec2 getItemRectSize           (const std::string& item);

		//-----------------------------------------------------------------------------
		// Input Polling
		//-----------------------------------------------------------------------------
		mvMousePos getMousePosition() const { return m_mousePos; }
		bool       isMouseButtonPressed(int button) const;
		bool       isKeyPressed(int keycode) const;

		//-----------------------------------------------------------------------------
		// Internal Utilities
		//-----------------------------------------------------------------------------
		void setModuleDict (PyObject* dict) { m_pDict = dict; }
		void setSize       (unsigned width, unsigned height) { m_width = width; m_height = height; }
		bool isOk          () const { return m_ok; }
		void setOk         (bool ok) { m_ok = ok; }
		void setStarted    () { m_started = true; }
				
	private:

		mvApp();

		mvApp(const mvApp& other) = delete;
		mvApp(mvApp&& other) = delete;
		mvApp operator=(const mvApp& other) = delete;
		mvApp operator=(mvApp&& other) = delete;

		void showMetricsWindow();
		void showAboutWindow();

	private:

		static mvApp*           s_instance;
		mvMousePos              m_mousePos;
		mvStyle                 m_style;
		std::vector<mvAppItem*> m_items;
		std::stack<mvAppItem*>  m_parents;
		PyObject*               m_pDict;
		unsigned                m_width;
		unsigned                m_height;
		std::string             m_callback;
		bool                    m_ok = true;
		bool                    m_showLog = false;
		bool                    m_showMetrics = false;
		bool                    m_showAbout = false;
		unsigned                m_loglevel = 0;
		ImGuiWindowFlags        m_windowflags = 0;
		bool                    m_started = false; // to prevent widgets from being added

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