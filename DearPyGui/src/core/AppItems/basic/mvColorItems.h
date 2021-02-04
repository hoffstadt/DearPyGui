#pragma once

#include "mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvColorButton
//     * mvColorEdit3
//     * mvColorEdit4
//     * mvColorPicker3
//     * mvColorPicker4
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvColorButton
	//-----------------------------------------------------------------------------

	PyObject* add_color_button(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvColorButton : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::ColorButton, "add_color_button")

		mvColorButton(const std::string& name, const mvColor& color);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;
		ImVec4 m_color;

	};

	//-----------------------------------------------------------------------------
	// mvColorEdit3
	//-----------------------------------------------------------------------------

	PyObject* add_color_edit3(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvColorEdit3 : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::ColorEdit3, "add_color_edit3")

			mvColorEdit3(const std::string& name, float* color, const std::string& dataSource);

		void draw              ()               override;


#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
	
	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvColorEdit4
	//-----------------------------------------------------------------------------

	PyObject* add_color_edit4(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvColorEdit4 : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::ColorEdit4, "add_color_edit4")

		mvColorEdit4(const std::string& name, float* color, const std::string& dataSource);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker3
	//-----------------------------------------------------------------------------

	PyObject* add_color_picker3(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvColorPicker3 : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::ColorPicker3, "add_color_picker3")

		mvColorPicker3(const std::string& name, float* color, const std::string& dataSource);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker4
	//-----------------------------------------------------------------------------

	PyObject* add_color_picker4(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvColorPicker4 : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::ColorPicker4, "add_color_picker4")

		mvColorPicker4(const std::string& name, float* color, const std::string& dataSource);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

}
