#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvNodeAttribute, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvNodeAttribute : public mvAppItem
	{

		enum class AttributeType { 
			mvAttr_Input = 0L, 
			mvAttr_Output, 
			mvAttr_Static
		};

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvNodeAttribute, add_node_attribute)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN

		MV_CREATE_CONSTANT(mvNode_PinShape_Circle, imnodes::PinShape::PinShape_Circle);
		MV_CREATE_CONSTANT(mvNode_PinShape_CircleFilled, imnodes::PinShape::PinShape_CircleFilled);
		MV_CREATE_CONSTANT(mvNode_PinShape_Triangle, imnodes::PinShape::PinShape_Triangle);
		MV_CREATE_CONSTANT(mvNode_PinShape_TriangleFilled, imnodes::PinShape::PinShape_TriangleFilled);
		MV_CREATE_CONSTANT(mvNode_PinShape_Quad, imnodes::PinShape::PinShape_Quad);
		MV_CREATE_CONSTANT(mvNode_PinShape_QuadFilled, imnodes::PinShape::PinShape_QuadFilled);

		MV_CREATE_CONSTANT(mvNode_Attr_Input, 0L);
		MV_CREATE_CONSTANT(mvNode_Attr_Output, 1L);
		MV_CREATE_CONSTANT(mvNode_Attr_Static, 2L);

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvStagingContainer),
			MV_ADD_PARENT(mvAppItemType::mvNode)
		MV_END_PARENTS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvNode_PinShape_Circle),
			MV_ADD_CONSTANT(mvNode_PinShape_CircleFilled),
			MV_ADD_CONSTANT(mvNode_PinShape_Triangle),
			MV_ADD_CONSTANT(mvNode_PinShape_TriangleFilled),
			MV_ADD_CONSTANT(mvNode_PinShape_Quad),
			MV_ADD_CONSTANT(mvNode_PinShape_QuadFilled),
			MV_ADD_CONSTANT(mvNode_Attr_Input),
			MV_ADD_CONSTANT(mvNode_Attr_Output),
			MV_ADD_CONSTANT(mvNode_Attr_Static)
		MV_END_CONSTANTS

	public:

		mvNodeAttribute(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		int getId() const {return _id;}
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:
        int _id = 1;
		AttributeType _attrType = AttributeType::mvAttr_Input;
		imnodes::PinShape _shape = imnodes::PinShape_CircleFilled;
		std::string _category = "general";

	};

}