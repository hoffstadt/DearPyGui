#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_tab(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTab : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::TabItem, "add_tab")

		mvTab(const std::string& name);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		bool              m_closable = false;
		ImGuiTabItemFlags m_flags = ImGuiTabItemFlags_None;

	};

}