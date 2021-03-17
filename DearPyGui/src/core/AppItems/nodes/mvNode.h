#pragma once
#include "mvAppItem.h"
#include <stdint.h>

namespace Marvel {

	PyObject* add_node(PyObject* self, PyObject* args, PyObject* kwargs);

	MV_REGISTER_WIDGET(mvNode);
	class mvNode : public mvAppItem
	{

		friend class mvNodeEditor;

		MV_APPITEM_TYPE(mvAppItemType::mvNode, "add_node")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_Node_Background,         0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Node_BackgroundHovered,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Node_BackgroundSelected, 2L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Node_Outline,            3L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Node_TitleBar,           4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Node_TitleBarHovered,    5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Node_TitleBarSelected,   6L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_Background,         mvColor( 50,  50,  50, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_BackgroundHovered,  mvColor( 75,  75,  75, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_BackgroundSelected, mvColor( 75,  75,  75, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_Outline,            mvColor(100, 100, 100, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_TitleBar,           mvColor( 41,  74, 122, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_TitleBarHovered,    mvColor( 66, 150, 250, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_TitleBarSelected,   mvColor( 66, 150, 250, 255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvNode(const std::string& name);

		void setNodePos(float x, float y);

		void draw() override;

		int getId() const {return m_id;}

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

	    int m_id = 0;
		bool m_dirty_pos = true;
		int m_xpos = 100;
		int m_ypos = 100;
		bool m_draggable = true;
	};

}