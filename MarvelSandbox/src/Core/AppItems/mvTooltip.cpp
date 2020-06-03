#include "mvTooltip.h"
#include <imgui.h>
#include <string>
#include "Core/mvApp.h"

namespace Marvel {

	void mvTooltip::draw()
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

	PyObject* mvTooltip::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("i", m_value);

		return pvalue;

	}


	void mvEndTooltip::draw()
	{
		mvApp::GetApp()->popParent();
		ImGui::EndTooltip();
	}

}