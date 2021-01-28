#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_group(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvGroup : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::Group, "add_group")

			mvGroup(const std::string& name);

		void draw()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		bool  m_horizontal = false;
		float m_hspacing = -1.0f;

	};

}