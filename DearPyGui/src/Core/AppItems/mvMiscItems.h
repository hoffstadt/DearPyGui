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
		}

		void draw() override
		{
			ImGui::SameLine(m_xoffset, m_spacing);
		}

		[[nodiscard]] bool areDuplicatesAllowed() const override { return true; }

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
		}

		void draw() override
		{
			pushColorStyles();

			ImGui::Separator();

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

		[[nodiscard]] bool areDuplicatesAllowed() const override { return true; }

	};

	//-----------------------------------------------------------------------------
	// mvSpacing
	//-----------------------------------------------------------------------------
	class mvSpacing : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Spacing, "add_spacing")

		mvSpacing(const std::string& name, int count)
			: mvIntItemBase(name, 1, count)
		{
		}

		void draw() override
		{
			for (int i = 0; i < m_value[0]; i++)
				ImGui::Spacing();
		}

		[[nodiscard]] bool areDuplicatesAllowed() const override { return true; }
	};

	//-----------------------------------------------------------------------------
	// mvIndent
	//-----------------------------------------------------------------------------
	class mvIndent : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Indent, "add_indent")

		mvIndent(const std::string& name, float default_value)
			: mvFloatItemBase(name, 1, default_value)
		{
		}

		void draw() override
		{
			ImGui::Indent(m_value[0]);
		}

		[[nodiscard]] bool areDuplicatesAllowed() const override { return true; }

	};

	//-----------------------------------------------------------------------------
	// mvUnindent
	//-----------------------------------------------------------------------------
	class mvUnindent : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Unindent, "unindent")

		mvUnindent(const std::string& name, float default_value)
			: mvFloatItemBase(name, 1, default_value)
		{
		}

		void draw() override
		{
			ImGui::Unindent(m_value[0]);
		}

		[[nodiscard]] bool areDuplicatesAllowed() const override { return true; }

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
		}

		void draw() override
		{
			ImGui::Dummy({ (float)m_width, (float)m_height });
		}

		[[nodiscard]] bool areDuplicatesAllowed() const override { return true; }

	};

}