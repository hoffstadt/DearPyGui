#pragma once
#include "mvAppItem.h"
#include <stdint.h>

namespace Marvel {

	PyObject* add_node(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvNode : public mvAppItem
	{

		friend class mvNodeEditor;

		MV_APPITEM_TYPE(mvAppItemType::Node, "add_node")

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvNode(const std::string& name);

		void setNodePos(float x, float y);

		void draw() override;

		int getId() const {return m_id;}

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

	    int m_id = 0;
		bool m_dirty_pos = true;
		int m_xpos = 100;
		int m_ypos = 100;
		bool m_draggable = true;
	};

}