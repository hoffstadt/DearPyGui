#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvChild, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvChild : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvChild, add_child)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvChild(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y)               override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

		void addFlag           (ImGuiWindowFlags flag);
		void removeFlag        (ImGuiWindowFlags flag);

		void setScrollX(float value) { m_scrollX = value; m_scrollXSet = true; }
		void setScrollY(float value) { m_scrollY = value; m_scrollYSet = true; }
		float getScrollX() const { return m_scrollX; }
		float getScrollY() const { return m_scrollY; }
		float getScrollXMax() const { return m_scrollMaxX; }
		float getScrollYMax() const { return m_scrollMaxY; }

	private:

		bool             m_border     = true;
		bool             m_autosize_x = false;
		bool             m_autosize_y = false;
		ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoSavedSettings;

		// scroll info
		float m_scrollX = 0.0f;
		float m_scrollY = 0.0f;
		float m_scrollMaxX = 0.0f;
		float m_scrollMaxY = 0.0f;
		bool  m_scrollXSet = false;
		bool  m_scrollYSet = false;

	};

}