#pragma once

#include "mvTypeBases.h"

namespace Marvel {

#ifdef MV_CPP
#else
	PyObject* add_same_line(PyObject* self, PyObject* args, PyObject* kwargs);
#endif

	class mvSameLine : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::SameLine, mvSameLine, "add_same_line")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvSameLine(const std::string& name);

		void draw() override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif

	private:

		float m_xoffset = 0.0f;
		float m_spacing = -1.0f;

	};

}