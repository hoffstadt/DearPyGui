#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_combo(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvCombo : public mvStringPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::Combo, "add_combo")

		mvCombo(const std::string& name, const std::string& default_value, const std::string& dataSource);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:
		ImGuiComboFlags m_flags = ImGuiComboFlags_None;
		std::vector<std::string> m_items;
	};

}
