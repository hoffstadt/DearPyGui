#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvChild
	//-----------------------------------------------------------------------------
	class mvChild : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Child)

		mvChild(const std::string& parent, const std::string& name, int width, int height)
			: mvAppItem(parent, name), m_value(false)
		{
			m_width = width;
			m_height = height;
		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("i", m_value);

			return pvalue;
		}

		virtual void draw() override
		{
			ImGui::BeginChild(m_label.c_str(), ImVec2(float(m_width), float(m_height)), true);

			mvApp::GetApp()->pushParent(this);

			// set current child value true
			m_value = true;

			if (m_tip != "" && ImGui::IsItemHovered())
				ImGui::SetTooltip(m_tip.c_str());
		}

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;
		int m_height;

	};

	//-----------------------------------------------------------------------------
	// mvEndChild
	//-----------------------------------------------------------------------------
	class mvEndChild : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndChild)
		MV_NORETURN_VALUE()

		mvEndChild(const std::string& parent)
			: mvAppItem(parent, "Endgroup")
		{
		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
			ImGui::EndChild();
		}

	};

}