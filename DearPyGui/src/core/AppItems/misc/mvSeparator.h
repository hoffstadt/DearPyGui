#pragma once

#include "mvTypeBases.h"

namespace Marvel {

#ifdef MV_CPP
#else
	PyObject* add_separator(PyObject* self, PyObject* args, PyObject* kwargs);
#endif

	MV_REGISTER_WIDGET(mvSeparator);
	class mvSeparator : public mvAppItem
	{


	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvSeparator, "add_separator")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_Separator, 27L, 0L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Separator, mvColor(110, 110, 128, 128)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvSeparator(const std::string& name);

		void draw() override;

	};

}