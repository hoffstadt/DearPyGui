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
	class mvSameLine : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SameLine)

		mvSameLine(const std::string& parent, float xoffset, float spacing)
			: mvNoneItemBase(parent, "SameLine"), m_xoffset(xoffset), m_spacing(spacing)
		{
		}

		virtual void draw() override
		{
			ImGui::SameLine(m_xoffset, m_spacing);
		}

	private:

		float m_xoffset;
		float m_spacing;

	};

	//-----------------------------------------------------------------------------
	// mvSeparator
	//-----------------------------------------------------------------------------
	class mvSeparator : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Separator)

		mvSeparator(const std::string& parent)
			: mvNoneItemBase(parent, "separator")
		{
		}

		virtual void draw() override
		{
			ImGui::Separator();
		}

	};

	//-----------------------------------------------------------------------------
	// mvSpacing
	//-----------------------------------------------------------------------------
	class mvSpacing : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Spacing)

		mvSpacing(const std::string& parent, int count)
			: mvNoneItemBase(parent, "Spacing")
		{
			m_value = count;
		}

		virtual void draw() override
		{
			for (int i = 0; i < m_value; i++)
				ImGui::Spacing();
		}

		inline int getValue() const { return m_value; }

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

		mvIndent(const std::string& parent, float default_value)
			: mvFloatItemBase(parent, "indent", 1, default_value)
		{
		}

		virtual void draw() override
		{
			ImGui::Indent(m_value[0]);
		}

	};

	//-----------------------------------------------------------------------------
	// mvUnindent
	//-----------------------------------------------------------------------------
	class mvUnindent : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputFloat)

		mvUnindent(const std::string& parent, float default_value)
			: mvFloatItemBase(parent, "indent", 1, default_value)
		{
		}

		virtual void draw() override
		{
			ImGui::Unindent(m_value[0]);
		}

	};

}