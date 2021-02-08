#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_collapsing_header(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvCollapsingHeader : public mvBoolPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::CollapsingHeader, mvCollapsingHeader, "add_collapsing_header")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeCol_CollapsingHeader_Text			,  0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeCol_CollapsingHeader_Bg			, 24L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeCol_CollapsingHeader_BgHovered		, 25L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeCol_CollapsingHeader_BgActive		, 26L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeCol_CollapsingHeader_Border		,  5L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::CollapsingHeader, mvThemeCol_CollapsingHeader_BorderShadow	,  6L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_CollapsingHeader_BorderShadow),
		MV_END_COLOR_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvCollapsingHeader(const std::string& name);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiTreeNodeFlags m_flags = ImGuiTreeNodeFlags_None;
		bool               m_closable = false;

	};

}