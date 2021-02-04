#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_radio_button(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvRadioButton : public mvIntPtrBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::RadioButtons, "add_radio_button")

		mvRadioButton(const std::string& name, int default_value, const std::string& dataSource);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		std::vector<std::string> m_itemnames;
		bool                     m_horizontal = false;

	};

}
