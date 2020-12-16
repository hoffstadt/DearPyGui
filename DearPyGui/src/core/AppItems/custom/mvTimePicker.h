#pragma once

#include "mvTypeBases.h"
#include "mvPythonParser.h"

namespace Marvel {

	PyObject* add_time_picker(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTimePicker : public mvTimePtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::TimePicker, "add_time_picker")

		mvTimePicker(const std::string& name, tm default_value);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		bool m_hour24 = false;

	};

}