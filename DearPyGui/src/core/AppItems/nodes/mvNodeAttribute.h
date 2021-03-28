#pragma once
#include "mvAppItem.h"
#include <stdint.h>

namespace Marvel {

	PyObject* add_node_attribute(PyObject* self, PyObject* args, PyObject* kwargs);

	MV_REGISTER_WIDGET(mvNodeAttribute);
	class mvNodeAttribute : public mvAppItem
	{

		MV_APPITEM_TYPE(mvAppItemType::mvNodeAttribute, "add_node_attribute")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_NodeAttr_Pin,						10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_NodeAttr_PinHovered,				11L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeAttr_Pin,        mvColor(53, 150, 250, 180)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_NodeAttr_PinHovered, mvColor(53, 150, 250, 255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvNodeAttribute(const std::string& name);
		~mvNodeAttribute();

		void draw() override;
		bool isParentCompatible(mvAppItemType type) override;

		int getId() const {return m_id;}

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:
        int m_id = 0;
		bool m_output = false;
		bool m_static = false;

	};

}