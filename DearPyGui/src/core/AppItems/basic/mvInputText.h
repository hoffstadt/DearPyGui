#pragma once

#include "mvApp.h"
#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_input_text(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvInputText : public mvStringPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::InputText, mvInputText, "add_input_text")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeCol_InputText_Text			, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeCol_InputText_TextHighlight	, 44L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeCol_InputText_Bg				, 7L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeCol_InputText_Hint			, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeCol_InputText_Border			, 5L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::InputText, mvThemeCol_InputText_BorderShadow	, 6L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_TextHighlight),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_Hint),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_InputText_BorderShadow),
		MV_END_COLOR_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvInputText(const std::string& name, const std::string& default_value, const std::string& dataSource);

		void setEnabled        (bool value)     override;
		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		std::string         m_hint;
		bool                m_multiline = false;
		ImGuiInputTextFlags m_flags = 0;
		ImGuiInputTextFlags m_stor_flags = 0;

	};

}