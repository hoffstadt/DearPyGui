#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"

namespace Marvel {

	PyObject* add_menu_item(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvMenuItem : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::MenuItem, "add_menu_item")

		explicit mvMenuItem(const std::string& name);

		void draw() override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		std::string m_shortcut;
		bool        m_check = false;

	};

}