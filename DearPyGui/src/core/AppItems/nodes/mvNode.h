#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvNode, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvNode : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		friend class mvNodeEditor;

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvNode, add_node)
		MV_NO_COMMANDS
		MV_NO_CONSTANTS

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
			MV_ADD_PARENT(mvAppItemType::mvStagingContainer),
			MV_ADD_PARENT(mvAppItemType::mvNodeEditor)
		MV_END_PARENTS
		
		MV_START_CHILDREN
			MV_ADD_CHILD(mvAppItemType::mvNodeAttribute),
			MV_ADD_CHILD(mvAppItemType::mvActiveHandler),
			MV_ADD_CHILD(mvAppItemType::mvClickedHandler),
			MV_ADD_CHILD(mvAppItemType::mvHoverHandler),
			MV_ADD_CHILD(mvAppItemType::mvVisibleHandler),
		MV_END_CHILDREN

	public:

		mvNode(mvUUID uuid);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void draw(ImDrawList* drawlist, float x, float y) override;

		int getId() const {return _id;}

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void applySpecificTemplate(mvAppItem* item) override;

	private:

	    int _id = 0;
		bool _draggable = true;
	};

}