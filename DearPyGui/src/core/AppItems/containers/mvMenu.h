#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_menu(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvMenu : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::Menu, "add_menu")

		mvMenu(const std::string& name);

		void draw() override;

		void setExtraConfigDict(PyObject* dict) override;

		void getExtraConfigDict(PyObject* dict) override;

	};

}