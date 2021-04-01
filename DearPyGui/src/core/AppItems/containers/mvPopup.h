#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPopup);
	class mvPopup : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvPopup, add_popup)

		MV_CREATE_EXTRA_COMMAND(close_popup);

		MV_CREATE_CONSTANT(mvThemeCol_Popup_TitleText				  ,  0L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_TitleBg				  , 11L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_Bg					  ,  4L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_ModalDimBg			  , 49L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_Scrollbar				  , 14L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_ScrollbarGrab			  , 15L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_ScrollbarGrabHovered	  , 16L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_ScrollbarGrabActive	  , 17L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Popup_Border				  ,  5L, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Popup_BorderSize			  ,  9L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_Rounding			  ,  8L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_PaddingX			  ,  1L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_PaddingY			  ,  1L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ItemSpacingX		  , 13L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ItemSpacingY		  , 13L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalRounding		  ,  2L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalBorderSize		  ,  3L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalTitleAlignX	  ,  5L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalTitleAlignY	  ,  5L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalMinSizeX		  ,  4L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalMinSizeY		  ,  4L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalScrollbarSize	  , 17L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Popup_ModalScrollbarRounding, 18L, 0L);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(close_popup);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_TitleText,				mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_TitleBg,				mvColor( 10,  10,  10, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_Bg,					mvColor( 20,  20,  20, 240)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_ModalDimBg,			mvColor(204, 204, 204,  89)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_Scrollbar,				mvColor(  5,   5,   5, 135)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_ScrollbarGrab,			mvColor( 79,  79,  79, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_ScrollbarGrabHovered,	mvColor(105, 105, 105, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_ScrollbarGrabActive,	mvColor(130, 130, 130, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_Border,				mvColor(110, 110, 128, 128)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_Popup_BorderSize				, 1,    1),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_Rounding				, 0,   12),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_PaddingX				, 8,   20),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_PaddingY				, 8,   20),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_ItemSpacingX			, 8,   20),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_ItemSpacingY			, 4,   20),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_ModalRounding			, 0,   12),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_ModalBorderSize			, 1,    1),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_ModalTitleAlignX		, 0,    1),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_ModalTitleAlignY		, 0.5,  1),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_ModalMinSizeX			,32,   50),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_ModalMinSizeY			,32,   50),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_ModalScrollbarSize		,14,   20),
			MV_ADD_CONSTANT(mvThemeStyle_Popup_ModalScrollbarRounding	, 9,   12),
		MV_END_STYLE_CONSTANTS

	public:

		mvPopup(const std::string& name, mvAppItem* parentAddress);

		void closePopup();

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		bool       m_modal = false;
		int        m_button = 1;
		mvAppItem* m_parentAddress = nullptr;
		bool       m_close = false;

	};

}