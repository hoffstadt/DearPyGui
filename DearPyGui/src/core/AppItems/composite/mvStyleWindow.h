#pragma once
#include "mvTypeBases.h"

namespace Marvel {

	struct mvStyleWindowConfig : public mvAppItemConfig
	{
		int x_pos = 200;
		int y_pos = 200;
		bool autosize = false;
		bool no_resize = false;
		bool no_title_bar = false;
		bool no_move = false;
		bool no_scrollbar = false;
		bool no_collapse = false;
		bool horizontal_scrollbar = false;
		bool no_focus_on_appearing = false;
		bool no_bring_to_front_on_focus = false;
		bool no_close = false;
		bool no_background = false;

		mvStyleWindowConfig()
		{
			width = 700;
			height = 500;
		}
	};

	PyObject* add_style_window(PyObject* self, PyObject* args, PyObject* kwargs);

	MV_REGISTER_WIDGET(mvStyleWindow);
	class mvStyleWindow : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE(mvAppItemType::mvStyleWindow, "add_style_window")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvStyleWindow(const std::string& name) 
			: mvBaseWindowAppitem(name) 
		{
			m_description.deleteAllowed = false;
			init();
		}

		void draw() override;

		void init();

	private:

		std::vector<std::tuple<std::string, long, mvColor>> m_colors;

	};

}