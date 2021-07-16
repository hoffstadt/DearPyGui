#pragma once

#include <utility>
#include "mvTypeBases.h"
#include "mvApp.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvWindowAppItem, MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_ROOT, StorageValueTypes::None, 1);
	class mvWindowAppItem : public mvAppItem
	{

		friend class mvResizeHandler;

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvWindowAppItem, add_window)

		MV_CREATE_COMMAND(set_x_scroll);
		MV_CREATE_COMMAND(set_y_scroll);
		MV_CREATE_COMMAND(get_x_scroll);
		MV_CREATE_COMMAND(get_y_scroll);
		MV_CREATE_COMMAND(get_x_scroll_max);
		MV_CREATE_COMMAND(get_y_scroll_max);

		MV_START_COMMANDS
			MV_ADD_COMMAND(set_x_scroll);
			MV_ADD_COMMAND(set_y_scroll);
			MV_ADD_COMMAND(get_x_scroll);
			MV_ADD_COMMAND(get_y_scroll);
			MV_ADD_COMMAND(get_x_scroll_max);
			MV_ADD_COMMAND(get_y_scroll_max)
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

		enum class Status{ Normal, Transition, Dirty};

	public:

		mvWindowAppItem(mvUUID uuid, bool mainWindow = false);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void addFlag              (ImGuiWindowFlags flag) { m_windowflags |= flag; }
		void removeFlag           (ImGuiWindowFlags flag) { m_windowflags &= ~flag; }
		void setWindowAsMainStatus(bool value);
		void setLabel             (const std::string& value) override;
		void setWidth             (int width) override;
		void setHeight            (int height) override;
		void draw                 (ImDrawList* drawlist, float x, float y) override;
		bool getWindowAsMainStatus() const { return m_mainWindow; }

		void show() override;
		void hide() override;
		void onChildAdd(mvRef<mvAppItem> item) override;
		void onChildRemoved(mvRef<mvAppItem> item) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

		~mvWindowAppItem();

	private:

		ImGuiWindowFlags      m_windowflags = ImGuiWindowFlags_None;
		ImGuiWindowFlags      m_oldWindowflags = ImGuiWindowFlags_None;
		float                 m_oldxpos = 200;
		float                 m_oldypos = 200;
		int                   m_oldWidth = 200;
		int                   m_oldHeight = 200;
		bool                  m_mainWindow = false;
		bool                  m_dirty_size = true;
		bool                  m_closing = true;
		bool                  m_collapsedDirty = true;
		bool                  m_resized = false;
		bool                  m_modal = false;
		bool                  m_popup = false;
		bool                  m_popupInit = true;

		bool       m_autosize = false;
		bool       m_no_resize = false;
		bool       m_no_title_bar = false;
		bool       m_no_move = false;
		bool       m_no_scrollbar = false;
		bool       m_no_collapse = false;
		bool       m_horizontal_scrollbar = false;
		bool       m_no_focus_on_appearing = false;
		bool       m_no_bring_to_front_on_focus = false;
		bool       m_menubar = false;
		bool       m_no_close = false;
		bool       m_no_background = false;
		bool       m_collapsed = false;
		PyObject*  m_on_close = nullptr;
		mvVec2     m_min_size = { 100.0f, 100.0f };
		mvVec2     m_max_size = { 30000.0f, 30000.0f };

		// scroll info
		float m_scrollX = 0.0f;
		float m_scrollY = 0.0f;
		float m_scrollMaxX = 0.0f;
		float m_scrollMaxY = 0.0f;
		bool  m_scrollXSet = false;
		bool  m_scrollYSet = false;
		
	};

}