#pragma once

#include "mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvText
//     * mvLabelText
//
//-----------------------------------------------------------------------------

namespace Marvel {

	PyObject* add_text      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_label_text(PyObject* self, PyObject* args, PyObject* kwargs);

	//-----------------------------------------------------------------------------
	// mvText
	//-----------------------------------------------------------------------------
	class mvText : public mvStringPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::Text, mvText, "add_text")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Text, mvThemeCol_Text_Text, 0L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Text_Text),
		MV_END_COLOR_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvText(const std::string& name, const std::string& default_value, const std::string& dataSource);

		void draw              () override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		mvColor m_color = {1000, 0, 0, 255, false};
		int     m_wrap = -1;
		bool    m_bullet = false;

	};

	//-----------------------------------------------------------------------------
	// mvLabelText
	//-----------------------------------------------------------------------------
	class mvLabelText : public mvStringPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::LabelText, mvLabelText, "add_label_text")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::LabelText, mvThemeCol_LabelText_Text				,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::LabelText, mvThemeStyle_LabelText_PaddingX			, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::LabelText, mvThemeStyle_LabelText_PaddingY			, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::LabelText, mvThemeStyle_LabelText_ItemInnerSpacingX	, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::LabelText, mvThemeStyle_LabelText_ItemInnerSpacingY	, 14L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_LabelText_Text),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_LabelText_PaddingX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_LabelText_PaddingY			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_LabelText_ItemInnerSpacingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_LabelText_ItemInnerSpacingY	, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvLabelText(const std::string& name, const std::string& value, const std::string& dataSource);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		mvColor m_color = { 1000, 0, 0, 255, false };

	};

}