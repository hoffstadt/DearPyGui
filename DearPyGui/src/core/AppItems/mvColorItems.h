#pragma once

#include "core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
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
	class mvColorButton : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorButton, "add_color_button")

		mvColorButton(const std::string& name, const mvColor& color);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;
		ImVec4 m_color;

	};

	//-----------------------------------------------------------------------------
	// mvColorEdit3
	//-----------------------------------------------------------------------------
	class mvColorEdit3 : public mvPtrBase<float*, 4>
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit3, "add_color_edit3")

			mvColorEdit3(const std::string& name, float* color, const std::string& dataSource);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
	
	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvColorEdit4
	//-----------------------------------------------------------------------------
	class mvColorEdit4 : public mvPtrBase<float*, 4>
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit4, "add_color_edit4")

		mvColorEdit4(const std::string& name, float* color, const std::string& dataSource);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker3
	//-----------------------------------------------------------------------------
	class mvColorPicker3 : public mvPtrBase<float*, 4>
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker3, "add_color_picker3")

		mvColorPicker3(const std::string& name, float* color, const std::string& dataSource);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker4
	//-----------------------------------------------------------------------------
	class mvColorPicker4 : public mvPtrBase<float*, 4>
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker4, "add_color_picker4")

		mvColorPicker4(const std::string& name, float* color, const std::string& dataSource);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

	};

}
