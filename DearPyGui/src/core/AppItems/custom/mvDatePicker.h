#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_date_picker(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvDatePicker : public mvTimePtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::DatePicker, "add_date_picker")

		mvDatePicker(const std::string& name, tm default_value);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		int m_level = 0;

	};

}