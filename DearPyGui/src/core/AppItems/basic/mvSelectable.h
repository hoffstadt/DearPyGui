#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_selectable(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvSelectable : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::Selectable, "add_selectable")

		mvSelectable(const std::string& name, bool default_value, const std::string& dataSource);

		void setEnabled(bool value)     override;
		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:
		ImGuiSelectableFlags m_flags = ImGuiSelectableFlags_None;
	};

}
