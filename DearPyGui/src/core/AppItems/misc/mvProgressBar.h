#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_progress_bar(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvProgressBar : public mvFloatPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ProgressBar, mvProgressBar, "add_progress_bar")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ProgressBar, mvThemeCol_ProgressBar_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ProgressBar, mvThemeCol_ProgressBar_Bar				, 42L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ProgressBar, mvThemeCol_ProgressBar_Bg				,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ProgressBar, mvThemeCol_ProgressBar_Border			,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ProgressBar, mvThemeCol_ProgressBar_BorderShadow	,  6L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ProgressBar_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ProgressBar_Bar),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ProgressBar_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ProgressBar_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ProgressBar_BorderShadow),
		MV_END_COLOR_CONSTANTS

	public:
		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvProgressBar(const std::string& name, float default_value, const std::string& dataSource);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		std::string m_overlay;

	};

}
