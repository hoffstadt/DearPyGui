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
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvSameLine
	//-----------------------------------------------------------------------------
	class mvSameLine : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SameLine)

		mvSameLine(const std::string& parent, const std::string& name, float xoffset, float spacing)
			: mvAppItem(parent, name), m_xoffset(xoffset), m_spacing(spacing)
		{
		}

		void draw() override
		{
			ImGui::SameLine(m_xoffset, m_spacing);
		}

		[[nodiscard]] bool areDuplicatesAllowed() const override { return true; }

	private:

		float m_xoffset;
		float m_spacing;

	};

	//-----------------------------------------------------------------------------
	// mvSeparator
	//-----------------------------------------------------------------------------
	class mvSeparator : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Separator)

		mvSeparator(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name)
		{
		}

		void draw() override
		{
			ImGui::Separator();
		}

		[[nodiscard]] bool areDuplicatesAllowed() const override { return true; }

	};

	//-----------------------------------------------------------------------------
	// mvSpacing
	//-----------------------------------------------------------------------------
	class mvSpacing : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Spacing)

		mvSpacing(const std::string& parent, const std::string& name, int count)
			: mvAppItem(parent, name)
		{
			m_value = count;
		}

		void draw() override
		{
			for (int i = 0; i < m_value; i++)
				ImGui::Spacing();
		}

		[[nodiscard]] bool areDuplicatesAllowed() const override { return true; }

		[[nodiscard]] int getValue() const { return m_value; }

	private:

		int m_value;

	};

	//-----------------------------------------------------------------------------
	// mvIndent
	//-----------------------------------------------------------------------------
	class mvIndent : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Indent)

		mvIndent(const std::string& parent, const std::string& name, float default_value)
			: mvFloatItemBase(parent, name, 1, default_value)
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

		MV_APPITEM_TYPE(mvAppItemType::Unindent)

		mvUnindent(const std::string& parent, const std::string& name, float default_value)
			: mvFloatItemBase(parent, name, 1, default_value)
		{
		}

		void draw() override
		{
			ImGui::Unindent(m_value[0]);
		}

		[[nodiscard]] bool areDuplicatesAllowed() const override { return true; }

	};

}