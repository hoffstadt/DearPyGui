#pragma once

#include "mvApp.h"
#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_input_text(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvInputText : public mvStringPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::InputText, "add_input_text")

		mvInputText(const std::string& name, const std::string& default_value, const std::string& dataSource);

		void setEnabled        (bool value)     override;
		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		std::string         m_hint;
		bool                m_multiline = false;
		ImGuiInputTextFlags m_flags = 0;
		ImGuiInputTextFlags m_stor_flags = 0;

	};

}