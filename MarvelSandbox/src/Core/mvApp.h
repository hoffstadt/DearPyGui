#pragma once

#include <vector>
#include <map>
#include <stack>
#include <string>
#include "Core/mvAppItem.h"
#include "mvTheme.h"
#include "mvMouse.h"
#include "mvAppStyle.h"

namespace Marvel {

	class mvApp final
	{

	public:

		static mvApp* GetApp();

		void render();

		//-----------------------------------------------------------------------------
		// App Settings
		//-----------------------------------------------------------------------------
		void updateTheme    ();
		void setAppTheme    (const std::string& theme);
		void changeThemeItem(const char* name, mvColor color);

		//-----------------------------------------------------------------------------
		// Basic AppItems
		//-----------------------------------------------------------------------------
		mvAppItem* addInputText   (const std::string& name, const std::string& hint = "", bool multiline = false);
		mvAppItem* addInputInt    (const std::string& name, int default_value);
		mvAppItem* addInputFloat  (const std::string& name, float default_value);
		mvAppItem* addCheckbox    (const std::string& name, bool default_value);
		mvAppItem* addRadioButtons(const std::string& name, const std::vector<std::string>& itemnames, int default_value);
		mvAppItem* addListbox     (const std::string& name, const std::vector<std::string>& itemnames, int default_value = 0, int height = -1);
		mvAppItem* addCombo       (const std::string& name, const std::vector<std::string>& itemnames, const std::string& default_value = "");

		//-----------------------------------------------------------------------------
		// Buttons
		//-----------------------------------------------------------------------------
		mvAppItem* addButton(const std::string& name);

		//-----------------------------------------------------------------------------
		// Color Items
		//-----------------------------------------------------------------------------
		mvAppItem* addColorEdit4(const std::string& name, mvColor color);

		//-----------------------------------------------------------------------------
		// Text
		//-----------------------------------------------------------------------------
		mvAppItem* addText     (const std::string& name, int wrap = 0, mvColor color = MV_DEFAULT_COLOR, bool bullet = false);
		mvAppItem* addLabelText(const std::string& name, const std::string& value, int wrap = 0, mvColor color = MV_DEFAULT_COLOR, bool bullet = false);

		//-----------------------------------------------------------------------------
		// Tabs
		//-----------------------------------------------------------------------------
		mvAppItem* addTabBar(const std::string& name);
		mvAppItem* addTab   (const std::string& name);
		mvAppItem* endTab   ();
		mvAppItem* endTabBar();

		//-----------------------------------------------------------------------------
		// Menus
		//-----------------------------------------------------------------------------
		mvAppItem* addMenuBar (const std::string& name);
		mvAppItem* addMenu    (const std::string& name);
		mvAppItem* addMenuItem(const std::string& name);
		mvAppItem* endMenu    ();
		mvAppItem* endMenuBar ();

		//-----------------------------------------------------------------------------
		// Grouping
		//-----------------------------------------------------------------------------
		mvAppItem* addGroup           (const std::string& name);
		mvAppItem* endGroup           ();
		mvAppItem* addChild           (const std::string& name, int width, int height);
		mvAppItem* endChild           ();
		mvAppItem* addCollapsingHeader(const std::string& name);
		mvAppItem* endCollapsingHeader();
		mvAppItem* addWindow          (const std::string& name, int width, int height);
		mvAppItem* endWindow          ();

		//-----------------------------------------------------------------------------
		// Drawing
		//-----------------------------------------------------------------------------
		mvAppItem* addDrawing     (const std::string& name, int width, int height);
		void       clearDrawing   (const std::string& drawing);
		void       drawLine       (const std::string& drawing, const mvVec2& p1, const mvVec2& p2, const mvColor& color, int thickness = 1);
		void       drawTriangle   (const std::string& drawing, const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvColor& color, const mvColor& fill = MV_DEFAULT_COLOR, float thickness = 1.0f);
		void       drawRectangle  (const std::string& drawing, const mvVec2& pmin, const mvVec2& pmax, const mvColor& color, const mvColor& fill = MV_DEFAULT_COLOR, float rounding = 0.0f, float thickness = 1.0f);
		void       drawQuad       (const std::string& drawing, const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvVec2& p4, const mvColor& color, const mvColor& fill = MV_DEFAULT_COLOR, float thickness = 1.0f);
		void       drawText       (const std::string& drawing, const mvVec2& pos, const std::string& text, const mvColor& color = MV_DEFAULT_COLOR, int size = 10);
		void       drawCircle     (const std::string& drawing, const mvVec2& center, float radius, const mvColor& color, int segments = 12, float thickness = 1.0f, const mvColor& fill = MV_DEFAULT_COLOR);
		void       drawPolyline   (const std::string& drawing, const std::vector<mvVec2>& points, const mvColor& color, bool closed = false, float thickness = 1.0f);
		void       drawPolygon    (const std::string& drawing, const std::vector<mvVec2>& points, const mvColor& color, const mvColor& fill = MV_DEFAULT_COLOR, float thickness = 1.0f);
		void       drawBezierCurve(const std::string& drawing, const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvVec2& p4, const mvColor& color, float thickness = 1.0f, int segments = 0);

		//-----------------------------------------------------------------------------
		// Misc Items
		//-----------------------------------------------------------------------------
		mvAppItem* addSpacing         (int count = 1);
		mvAppItem* addSameLine        (float offsetx = 0.0f, float spacing = -1.0f);
		mvAppItem* addTooltip         (const std::string& parent, const std::string& name);
		mvAppItem* endTooltip         ();
		mvAppItem* addSeperator       ();
		mvAppItem* indent             (float offset = 0.0f);
		mvAppItem* unindent           (float offset = 0.0f);

		//-----------------------------------------------------------------------------
		// Item modifications
		//-----------------------------------------------------------------------------
		void setItemCallback(const std::string& name, const std::string& callback);
		void setItemWidth   (const std::string& name, int width);
		void setItemTip     (const std::string& name, const std::string& tip);

		//-----------------------------------------------------------------------------
		// Parent stack operations
		//-----------------------------------------------------------------------------
		void       pushParent(mvAppItem* item);
		mvAppItem* popParent();
		mvAppItem* topParent();

		//-----------------------------------------------------------------------------
		// Callbacks
		//-----------------------------------------------------------------------------
		void setMainCallback(const std::string& callback) { m_callback = callback; }
		void triggerCallback(const std::string& name, const std::string& sender);
		void triggerCallback(const std::string& name, const std::string& sender, const std::string& data);
		void setMouseClickCallback(const std::string& callback) { m_mouseClickCallback = callback; }
		void setMouseDownCallback(const std::string& callback) { m_mouseDownCallback = callback; }
		void setMouseDoubleClickCallback(const std::string& callback) { m_mouseDoubleClickCallback = callback; }
		void setKeyDownCallback(const std::string& callback) { m_keyDownCallback = callback; }
		void setKeyPressCallback(const std::string& callback) { m_keyPressCallback = callback; }
		void setKeyReleaseCallback(const std::string& callback) { m_keyReleaseCallback = callback; }

		//-----------------------------------------------------------------------------
		// Logging
		//-----------------------------------------------------------------------------
		void setLogLevel  (unsigned level) { m_loglevel = level; }
		void Log          (const std::string& text, const std::string& level = "TRACE");
		void LogDebug     (const std::string& text);
		void LogInfo      (const std::string& text);
		void LogWarning   (const std::string& text);
		void LogError     (const std::string& text);
		void ClearLog     ();
		void turnOnLogger () { m_showLog = true; }
		void turnOffLogger() { m_showLog = false; }
		bool showLog      () const { return m_showLog; }

		//-----------------------------------------------------------------------------
		// Inputs
		//-----------------------------------------------------------------------------
		mvMousePos getMousePosition() const { return m_mousePos; }
		bool       isMouseButtonPressed(int button) const;
		bool       isKeyPressed(int keycode) const;
		bool       isItemHovered(const std::string& item);
		bool       isItemActive(const std::string& item);
		bool       isItemFocused(const std::string& item);
		bool       isItemClicked(const std::string& item);
		bool       isItemVisible(const std::string& item);
		bool       isItemEdited(const std::string& item);
		bool       isItemActivated(const std::string& item);
		bool       isItemDeactivated(const std::string& item);
		bool       isItemDeactivatedAfterEdit(const std::string& item);
		bool       isItemToogledOpen(const std::string& item);
		mvVec2     getItemRectMin(const std::string& item);
		mvVec2     getItemRectMax(const std::string& item);
		mvVec2     getItemRectSize(const std::string& item);

		//-----------------------------------------------------------------------------
		// Styles
		//-----------------------------------------------------------------------------
		void updateStyle();
		bool setStyleItem(const std::string& item, float x, float y = 0.0f);


		//-----------------------------------------------------------------------------
		// Plotting
		//-----------------------------------------------------------------------------
		mvAppItem* addSimplePlot(const std::string& name, const std::vector<float> value, 
			const std::string& overlay, float scale_min, float scale_max, float height, bool histogram);
		mvAppItem* addPlot(const std::string& name, int width, int height);

		//-----------------------------------------------------------------------------
		// Utilities
		//-----------------------------------------------------------------------------
		mvAppItem* getItem(const std::string& name);
		void       setModuleDict  (PyObject* dict) { m_pDict = dict; }
		void       setSize(unsigned width, unsigned height) { m_width = width; m_height = height; }
		bool       isOk() const { return m_ok; }
		void       setOk(bool ok) { m_ok = ok; }
		
	private:

		mvApp()
		{
			m_style = getAppDefaultStyle();
		}

		mvApp(const mvApp& other) = delete;
		mvApp(mvApp&& other) = delete;
		mvApp operator=(const mvApp& other) = delete;
		mvApp operator=(mvApp&& other) = delete;

		bool doesParentAllowRender(mvAppItem* item);
		void prepareStandardCallbacks();

	private:

		static mvApp*           s_instance;
		std::vector<mvAppItem*> m_items;
		std::stack<mvAppItem*>  m_parents;
		PyObject*               m_pDict;
		unsigned                m_width;
		unsigned                m_height;
		std::string             m_callback;
		bool                    m_ok = true;
		bool                    m_showLog = true;
		mvTheme                 m_theme;
		unsigned                m_loglevel = 0;
		ImGuiWindowFlags        m_windowflags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings 
			| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

		// inputs
		mvMousePos m_mousePos;

		// styles
		mvStyle m_style;

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