#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_menu(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvMenu : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::Menu, "add_menu")

		mvMenu(const std::string& name);

		void draw() override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	};

}