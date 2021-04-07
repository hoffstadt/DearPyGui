#pragma once
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <imnodes.h>
#include <stdint.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvNode, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvNode : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		friend class mvNodeEditor;

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvNode, add_node)

		MV_CREATE_CONSTANT(mvThemeCol_Node_Background			, imnodes::ColorStyle::ColorStyle_NodeBackground		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Node_BackgroundHovered	, imnodes::ColorStyle::ColorStyle_NodeBackgroundHovered	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Node_BackgroundSelected	, imnodes::ColorStyle::ColorStyle_NodeBackgroundSelected, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Node_Outline				, imnodes::ColorStyle::ColorStyle_NodeOutline			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Node_TitleBar				, imnodes::ColorStyle::ColorStyle_TitleBar				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Node_TitleBarHovered		, imnodes::ColorStyle::ColorStyle_TitleBarHovered		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Node_TitleBarSelected		, imnodes::ColorStyle::ColorStyle_TitleBarSelected		, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Node_CornerRounding		, imnodes::StyleVar::StyleVar_NodeCornerRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Node_PaddingHorizontal	, imnodes::StyleVar::StyleVar_NodePaddingHorizontal	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Node_PaddingVertical	, imnodes::StyleVar::StyleVar_NodePaddingVertical	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Node_BorderThickness	, imnodes::StyleVar::StyleVar_NodeBorderThickness	, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_Background,         mvColor( 50,  50,  50, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_BackgroundHovered,  mvColor( 75,  75,  75, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_BackgroundSelected, mvColor( 75,  75,  75, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_Outline,            mvColor(100, 100, 100, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_TitleBar,           mvColor( 41,  74, 122, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_TitleBarHovered,    mvColor( 66, 150, 250, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Node_TitleBarSelected,   mvColor( 66, 150, 250, 255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_Node_CornerRounding,		5, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Node_PaddingHorizontal,	8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Node_PaddingVertical,		8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Node_BorderThickness,		1, 20),

		MV_END_STYLE_CONSTANTS

	public:

		mvNode(const std::string& name);

		void setNodePos(float x, float y);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		bool canChildBeAdded(mvAppItemType type) override;

		int getId() const {return m_id;}

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

	    int m_id = 0;
		bool m_dirty_pos = true;
		int m_xpos = 100;
		int m_ypos = 100;
		bool m_draggable = true;
	};

}