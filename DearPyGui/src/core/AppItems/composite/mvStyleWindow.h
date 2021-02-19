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

	class mvStyleWindow : public mvBaseWindowAppitem
	{
		std::vector<std::pair<std::string, long>> colorConstants;
		std::vector<std::pair<std::string, long>> styleConstants;
		std::vector<std::pair<long, ImVec4>> themeColor;
		std::vector<std::pair<long, float>> themeStyle;

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::StyleWindow, "add_style_window")

	public:

		mvStyleWindow(const std::string& name);

		void draw() override;

	};

}