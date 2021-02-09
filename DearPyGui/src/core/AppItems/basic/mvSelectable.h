#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_selectable(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvSelectable : public mvBoolPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::Selectable, mvSelectable, "add_selectable")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Selectable, mvThemeCol_Selectable_Text		,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Selectable, mvThemeCol_Selectable_Bg		, 24L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Selectable, mvThemeCol_Selectable_BgHovered	, 25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Selectable, mvThemeCol_Selectable_BgActive	, 26L, 0L);


		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Selectable_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Selectable_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Selectable_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Selectable_BgActive),
		MV_END_COLOR_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvSelectable(const std::string& name, bool default_value, const std::string& dataSource);

		void setEnabled(bool value)     override;
		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:
		ImGuiSelectableFlags m_flags = ImGuiSelectableFlags_None;
	};

}
