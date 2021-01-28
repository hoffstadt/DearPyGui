#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_tab_button(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTabButton : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::TabButton, "add_tab_button")

		mvTabButton(const std::string& name);

		void draw()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiTabItemFlags m_flags = ImGuiTabItemFlags_None;
	};
}