#pragma once

#include "mvTypeBases.h"
#include "mvAppItems.h"

namespace Marvel {

	PyObject* add_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvMenuBar : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuBar, "add_menu_bar")

		explicit mvMenuBar(const std::string& name);

		void draw() override;

	};

}