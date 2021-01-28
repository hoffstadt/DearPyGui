#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_listbox(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvListbox : public mvIntPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::Listbox, "add_listbox")

			mvListbox(const std::string& name, int default_value, const std::string& dataSource);

		void draw()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		std::vector<std::string> m_names;
		int                      m_itemsHeight = 3; // number of items to show (default -1)
		std::vector<const char*> m_charNames;

	};

}
