#pragma once

#include "mvTypeBases.h"
#include "mvDrawList.h"
#include "mvPythonParser.h"

namespace Marvel {

	PyObject* add_drawing(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvDrawing : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::Drawing, "add_drawing")

		mvDrawing(const std::string& name);

		void draw() override;

		mvDrawList& getDrawList();


	private:

		mvDrawList  m_drawList;
		float       m_startx = 0.0f;
		float       m_starty = 0.0f;

	};
}