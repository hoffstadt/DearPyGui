#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_group(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvGroup : public mvAppItem
	{
		MV_APPITEM_TYPE(mvAppItemType::Group, mvGroup, "add_group")
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Group, mvThemeStyle_Group_ItemSpacingX, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Group, mvThemeStyle_Group_ItemSpacingY, 13L, 1L);

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Group_ItemSpacingX, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Group_ItemSpacingY, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

			mvGroup(const std::string& name);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		bool  m_horizontal = false;
		float m_hspacing = -1.0f;

	};

}