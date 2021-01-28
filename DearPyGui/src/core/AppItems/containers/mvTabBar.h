#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTabBar : public mvStringPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::TabBar, "add_tab_bar")

		mvTabBar(const std::string& name);

		std::string& getValue();
		void         setValue(const std::string& value);
		void         draw()               override;
		void         setExtraConfigDict(PyObject* dict) override;
		void         getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiTabBarFlags m_flags = ImGuiTabBarFlags_None;

	};

}