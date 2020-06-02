#include "mvChild.h"
#include <imgui.h>
#include <string>
#include "Core/mvApp.h"

namespace Marvel {

	void mvChild::draw()
	{

		ImGui::BeginChild(m_label.c_str(), ImVec2(250.0f, 250.0f), true);

		mvApp::GetApp()->pushParent(this);

		// set current child value true
		m_value = true;

		if (m_tip != "" && ImGui::IsItemHovered())
			ImGui::SetTooltip(m_tip.c_str());

	}


	PyObject* mvChild::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("i", m_value);

		return pvalue;

	}


	void mvEndChild::draw()
	{
		mvApp::GetApp()->popParent();
		ImGui::EndChild();
	}

}