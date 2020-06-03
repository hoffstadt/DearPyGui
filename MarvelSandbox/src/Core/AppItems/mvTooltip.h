#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvTooltip
	//-----------------------------------------------------------------------------
	class mvTooltip : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Tooltip)

		mvTooltip(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name), m_value(false)
		{
			// has to be showed that way it can check for hovering
			// otherwise it will never show
			m_show = true;
		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("i", m_value);

			return pvalue;
		}

		virtual void draw() override
		{
			if (ImGui::IsItemHovered() &&
				mvApp::GetApp()->topParent() == getParent()->getParent()
				|| mvApp::GetApp()->topParent() == getParent())
			{
				mvApp::GetApp()->pushParent(this);

				ImGui::BeginTooltip();

				// set current menu value true
				m_value = true;
			}
			else
				m_value = false;
		}

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvEndTooltip
	//-----------------------------------------------------------------------------
	class mvEndTooltip : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndTooltip)
		MV_NORETURN_VALUE()

		mvEndTooltip(const std::string& parent)
			: mvAppItem(parent, "EndTooltip")
		{
		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
			ImGui::EndTooltip();
		}

	};

}