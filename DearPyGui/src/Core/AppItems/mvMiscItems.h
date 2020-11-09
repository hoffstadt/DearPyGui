#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSameLine
//     * mvSeparator
//     * mvSpacing
//     * mvIndent
//     * mvUnindent
//     * mvDummy
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvSameLine
	//-----------------------------------------------------------------------------
	class mvSameLine : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SameLine, "add_same_line")

		mvSameLine(const std::string& name)
			: mvAppItem(name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			ImGui::SameLine(m_xoffset, m_spacing);
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "xoffset")) m_xoffset = ToFloat(item);
			if (PyObject* item = PyDict_GetItemString(dict, "spacing")) m_spacing = ToFloat(item);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "xoffset", ToPyFloat(m_xoffset));
			PyDict_SetItemString(dict, "spacing", ToPyFloat(m_spacing));
		}

	private:

		float m_xoffset = 0.0f;
		float m_spacing = -1.0f;

	};

	//-----------------------------------------------------------------------------
	// mvSeparator
	//-----------------------------------------------------------------------------
	class mvSeparator : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Separator, "add_separator")

		mvSeparator(const std::string& name)
			: mvAppItem(name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			auto styleManager = m_styleManager.getScopedStyleManager();

			ImGui::Separator();

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

	};

	//-----------------------------------------------------------------------------
	// mvSpacing
	//-----------------------------------------------------------------------------
	class mvSpacing : public mvIntPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Spacing, "add_spacing")

		mvSpacing(const std::string& name, int count)
			: mvIntPtrBase(name, count, name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			for (int i = 0; i < m_value[0]; i++)
				ImGui::Spacing();
		}

	};

	//-----------------------------------------------------------------------------
	// mvIndent
	//-----------------------------------------------------------------------------
	class mvIndent : public mvFloatPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Indent, "add_indent")

		mvIndent(const std::string& name, float default_value)
			: mvFloatPtrBase(name, default_value, name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			ImGui::Indent(m_value[0]);
		}

	};

	//-----------------------------------------------------------------------------
	// mvUnindent
	//-----------------------------------------------------------------------------
	class mvUnindent : public mvFloatPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Unindent, "unindent")

		mvUnindent(const std::string& name, float default_value)
			: mvFloatPtrBase(name, default_value, name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			ImGui::Unindent(m_value[0]);
		}

	};

	//-----------------------------------------------------------------------------
	// mvDummy
	//-----------------------------------------------------------------------------
	class mvDummy : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Dummy, "add_dummy")

		mvDummy(const std::string& name)
			: mvAppItem(name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			ImGui::Dummy({ (float)m_width, (float)m_height });
		}

	};

}