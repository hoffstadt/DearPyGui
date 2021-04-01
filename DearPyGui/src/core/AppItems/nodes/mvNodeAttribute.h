#pragma once
#include "mvAppItem.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvNodeAttribute);
	class mvNodeAttribute : public mvAppItem
	{

		MV_APPITEM_TYPE(mvAppItemType::mvNodeAttribute, add_node_attribute)

		MV_CREATE_CONSTANT(mvNode_PinShape_Circle,				0L, 0L);
		MV_CREATE_CONSTANT(mvNode_PinShape_CircleFilled,			1L, 0L);
		MV_CREATE_CONSTANT(mvNode_PinShape_Triangle,				2L, 0L);
		MV_CREATE_CONSTANT(mvNode_PinShape_TriangleFilled,		3L, 0L);
		MV_CREATE_CONSTANT(mvNode_PinShape_Quad,				    4L, 0L);
		MV_CREATE_CONSTANT(mvNode_PinShape_QuadFilled,			5L, 0L);

		MV_CREATE_CONSTANT(mvThemeCol_NodeAttr_Pin,				10L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_NodeAttr_PinHovered,		11L, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
			MV_ADD_GENERAL_CONSTANT(mvNode_PinShape_Circle),
			MV_ADD_GENERAL_CONSTANT(mvNode_PinShape_CircleFilled),
			MV_ADD_GENERAL_CONSTANT(mvNode_PinShape_Triangle),
			MV_ADD_GENERAL_CONSTANT(mvNode_PinShape_TriangleFilled),
			MV_ADD_GENERAL_CONSTANT(mvNode_PinShape_Quad),
			MV_ADD_GENERAL_CONSTANT(mvNode_PinShape_QuadFilled)
		MV_END_GENERAL_CONSTANTS

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
		void markForDeletion() { m_delete = true; }

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:
        int m_id = 0;
		bool m_output = false;
		bool m_static = false;
		imnodes::PinShape m_shape = imnodes::PinShape_CircleFilled;
		bool m_delete = false; // specific delete instructions when node editor is deleted

	};

}