#pragma once

#include <vector>
#include <stack>
#include <string>
#include "Core/mvAppItem.h"
#include "mvTheme.h"
#include "mvMouse.h"

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
		mvAppItem* addInputText   (const std::string& parent, const std::string& name, const std::string& hint = "", bool multiline = false);
		mvAppItem* addInputInt    (const std::string& parent, const std::string& name, int default_value);
		mvAppItem* addInputFloat  (const std::string& parent, const std::string& name, float default_value);
		mvAppItem* addCheckbox    (const std::string& parent, const std::string& name, bool default_value);
		mvAppItem* addRadioButtons(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, int default_value);
		mvAppItem* addListbox     (const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, int default_value = 0, int height = -1);
		mvAppItem* addCombo       (const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, const std::string& default_value = "");

		//-----------------------------------------------------------------------------
		// Buttons
		//-----------------------------------------------------------------------------
		mvAppItem* addButton(const std::string& parent, const std::string& name);

		//-----------------------------------------------------------------------------
		// Color Items
		//-----------------------------------------------------------------------------
		mvAppItem* addColorEdit4(const std::string& parent, const std::string& name, mvColor color);

		//-----------------------------------------------------------------------------
		// Text
		//-----------------------------------------------------------------------------
		mvAppItem* addText     (const std::string& parent, const std::string& name, int wrap = 0, mvColor color = MV_DEFAULT_COLOR, bool bullet = false);
		mvAppItem* addLabelText(const std::string& parent, const std::string& name, const std::string& value, int wrap = 0, mvColor color = MV_DEFAULT_COLOR, bool bullet = false);

		//-----------------------------------------------------------------------------
		// Tabs
		//-----------------------------------------------------------------------------
		mvAppItem* addTabBar(const std::string& parent, const std::string& name);
		mvAppItem* addTab   (const std::string& parent, const std::string& name);
		mvAppItem* endTab   (const std::string& parent);
		mvAppItem* endTabBar(const std::string& parent);

		//-----------------------------------------------------------------------------
		// Menus
		//-----------------------------------------------------------------------------
		mvAppItem* addMenuBar (const std::string& name);
		mvAppItem* addMenu    (const std::string& parent, const std::string& name);
		mvAppItem* addMenuItem(const std::string& parent, const std::string& name);
		mvAppItem* endMenu    (const std::string& parent);
		mvAppItem* endMenuBar (const std::string& parent);

		//-----------------------------------------------------------------------------
		// Grouping
		//-----------------------------------------------------------------------------
		mvAppItem* addGroup           (const std::string& parent, const std::string& name);
		mvAppItem* endGroup           (const std::string& parent);
		mvAppItem* addChild           (const std::string& parent, const std::string& name, int width, int height);
		mvAppItem* endChild           (const std::string& parent);
		mvAppItem* addCollapsingHeader(const std::string& parent, const std::string& name);

		//-----------------------------------------------------------------------------
		// Drawing
		//-----------------------------------------------------------------------------
		mvAppItem* addDrawing(const std::string& parent, const std::string& name, int width, int height);
		void       clearDrawing(const std::string& drawing);
		void       drawLine(const std::string& drawing, int x1, int y1, int x2, int y2, const mvColor& color, int thickness = 1);

		//-----------------------------------------------------------------------------
		// Misc Items
		//-----------------------------------------------------------------------------
		mvAppItem* addSpacing         (const std::string& parent, int count = 1);
		mvAppItem* addSameLine        (const std::string& parent, float offsetx = 0.0f, float spacing = -1.0f);
		mvAppItem* addTooltip         (const std::string& parent, const std::string& name);
		mvAppItem* endTooltip         (const std::string& parent);
		mvAppItem* addSeperator       (const std::string& parent);
		mvAppItem* indent             (const std::string& parent, float offset = 0.0f);
		mvAppItem* unindent           (const std::string& parent, float offset = 0.0f);

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

		//-----------------------------------------------------------------------------
		// Plotting
		//-----------------------------------------------------------------------------
		mvAppItem* addSimplePlot(const std::string& parent, const std::string& name, const std::vector<float> value, 
			const std::string& overlay, float scale_min, float scale_max, float height, bool histogram);
		mvAppItem* addPlot(const std::string& parent, const std::string& name, int width, int height);

		//-----------------------------------------------------------------------------
		// Utilities
		//-----------------------------------------------------------------------------
		mvAppItem* getItem(const std::string& name);
		void       setModuleDict  (PyObject* dict) { m_pDict = dict; }
		void       setSize(unsigned width, unsigned height) { m_width = width; m_height = height; }
		bool       isOk() const { return m_ok; }
		void       setOk(bool ok) { m_ok = ok; }
		
	private:

		mvApp() = default;

		mvApp(const mvApp& other) = delete;
		mvApp(mvApp&& other) = delete;
		mvApp operator=(const mvApp& other) = delete;
		mvApp operator=(mvApp&& other) = delete;

		bool doesParentAllowRender(mvAppItem* item);

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

	};

}