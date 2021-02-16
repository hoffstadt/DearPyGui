#pragma once
#include "mvAppItem.h"

namespace Marvel {

	PyObject* add_node_attribute(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvNodeAttribute : public mvAppItem
	{

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::NodeAttribute, "add_node_attribute")

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvNodeAttribute(const std::string& name);

		void draw() override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		bool m_output = false;
		bool m_static = false;
	};

}