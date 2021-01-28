#pragma once
#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_style_window(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvStyleWindow : public mvBaseWindowAppitem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::StyleWindow, "add_style_window")

	public:

		mvStyleWindow(const std::string& name) 
			: mvBaseWindowAppitem(name) 
		{
			m_description.deleteAllowed = false;
		}

		void draw() override;

	};

}