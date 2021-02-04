#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_checkbox(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvCheckbox : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::Checkbox, "add_checkbox")

		mvCheckbox(const std::string& name, bool default_value, const std::string& dataSource);

		void draw() override;

	};

}
