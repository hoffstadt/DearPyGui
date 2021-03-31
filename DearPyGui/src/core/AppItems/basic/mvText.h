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

	struct mvTextConfig : public mvAppItemConfig
	{
		int wrap = -1;
		mvColor color{ 0, 0, 0, -1 };
		bool bullet = false;
		std::string default_value = "";
	};

	PyObject* add_text      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_label_text(PyObject* self, PyObject* args, PyObject* kwargs);

	//-----------------------------------------------------------------------------
	// mvText
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvText);
	class mvText : public mvStringPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvText, "add_text")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_Text_Text, 0L, 0L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Text_Text, mvColor(255, 255, 255, 255))
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvText(const std::string& name, const std::string& default_value, const std::string& dataSource);

		void draw              () override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		mvColor m_color = {-1.0f, 0.0f, 0.0f, 1.0f};
		int     m_wrap = -1;
		bool    m_bullet = false;
		mvTextConfig m_config;

	};

	//-----------------------------------------------------------------------------
	// mvLabelText
	//-----------------------------------------------------------------------------

	struct mvLabelTextConfig : public mvAppItemConfig
	{
		std::string default_value = "";
		mvColor color{ 0, 0, 0, -1 };
	};

	MV_REGISTER_WIDGET(mvLabelText);
	class mvLabelText : public mvStringPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvLabelText, "add_label_text")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_LabelText_Text				    ,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_LabelText_PaddingX			, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_LabelText_PaddingY			, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_LabelText_ItemInnerSpacingX	, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_LabelText_ItemInnerSpacingY	, 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_LabelText_Text, mvColor(255, 255, 255, 255))
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_LabelText_PaddingX			, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_LabelText_PaddingY			, 3, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_LabelText_ItemInnerSpacingX	, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_LabelText_ItemInnerSpacingY	, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvLabelText(const std::string& name, const std::string& value, const std::string& dataSource);

		void draw              ()               override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		mvColor m_color = { -1.0f, 0.0f, 0.0f, 1.0f};
		mvLabelTextConfig m_config;

	};

}