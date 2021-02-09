#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_tooltip(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTooltip : public mvBoolPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::Tooltip, mvTooltip, "add_tooltip")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Tooltip, mvThemeCol_Tooltip_Bg		,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Tooltip, mvThemeCol_Tooltip_Border	,  5L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tooltip_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tooltip_Border),
		MV_END_COLOR_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTooltip(const std::string& name);

		void draw() override;

	};

}