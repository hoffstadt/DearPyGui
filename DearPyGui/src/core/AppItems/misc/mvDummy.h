#pragma once

#include "mvTypeBases.h"

namespace Marvel {

#ifdef MV_CPP
#else
	PyObject* add_dummy(PyObject* self, PyObject* args, PyObject* kwargs);
#endif

	MV_REGISTER_WIDGET(mvDummy);
	class mvDummy : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::mvDummy, "add_dummy")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvDummy(const std::string& name);

		void draw() override;

	};

}