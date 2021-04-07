#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvProgressBar, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float, 1);
	class mvProgressBar : public mvFloatPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvProgressBar, add_progress_bar)

		MV_CREATE_CONSTANT(mvThemeCol_ProgressBar_Text			, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ProgressBar_Bar			, ImGuiCol_PlotHistogram, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ProgressBar_Bg			, ImGuiCol_FrameBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ProgressBar_Border		, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ProgressBar_BorderShadow	, ImGuiCol_BorderShadow, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_ProgressBar_Rounding			, ImGuiStyleVar_FrameRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ProgressBar_BorderSize			, ImGuiStyleVar_FrameBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ProgressBar_PaddingX			, ImGuiStyleVar_FramePadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ProgressBar_PaddingY			, ImGuiStyleVar_FramePadding	, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ProgressBar_InnerItemSpacingX	, ImGuiStyleVar_ItemInnerSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ProgressBar_InnerItemSpacingY	, ImGuiStyleVar_ItemInnerSpacing, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ProgressBar_Text,			mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ProgressBar_Bar,				mvColor(230, 179,   0, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ProgressBar_Bg,				mvColor( 41,  74, 122, 138)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ProgressBar_Border,			mvColor(110, 110, 128, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ProgressBar_BorderShadow,	mvColor(  0,   0,   0,   0)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_ProgressBar_BorderSize,        0, 1),
			MV_ADD_CONSTANT(mvThemeStyle_ProgressBar_Rounding,          0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ProgressBar_PaddingX,          8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ProgressBar_PaddingY,          4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ProgressBar_InnerItemSpacingX, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ProgressBar_InnerItemSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvProgressBar(const std::string& name, float default_value, const std::string& dataSource);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		std::string m_overlay;

	};

}
