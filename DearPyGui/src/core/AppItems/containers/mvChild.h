#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvChild, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvChild : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvChild, add_child)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_SET_STATES(
			MV_STATE_HOVER |
			MV_STATE_ACTIVE |
			MV_STATE_FOCUSED |
			MV_STATE_CLICKED |
			MV_STATE_VISIBLE |
			MV_STATE_EDITED |
			MV_STATE_ACTIVATED |
			MV_STATE_DEACTIVATED |
			MV_STATE_DEACTIVATEDAE |
			MV_STATE_TOGGLED_OPEN |
			MV_STATE_RECT_MIN |
			MV_STATE_RECT_MAX |
			MV_STATE_RECT_SIZE |
			MV_STATE_CONT_AVAIL
		);

	public:

		mvChild(mvUUID uuid);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void applySpecificTemplate(mvAppItem* item) override;

		void addFlag           (ImGuiWindowFlags flag);
		void removeFlag        (ImGuiWindowFlags flag);

		void setScrollX(float value) { _scrollX = value; _scrollXSet = true; }
		void setScrollY(float value) { _scrollY = value; _scrollYSet = true; }
		float getScrollX() const { return _scrollX; }
		float getScrollY() const { return _scrollY; }
		float getScrollXMax() const { return _scrollMaxX; }
		float getScrollYMax() const { return _scrollMaxY; }

	private:

		bool             _border     = true;
		bool             _autosize_x = false;
		bool             _autosize_y = false;
		ImGuiWindowFlags _windowflags = ImGuiWindowFlags_NoSavedSettings;

		// scroll info
		float _scrollX = 0.0f;
		float _scrollY = 0.0f;
		float _scrollMaxX = 0.0f;
		float _scrollMaxY = 0.0f;
		bool  _scrollXSet = false;
		bool  _scrollYSet = false;

	};

}