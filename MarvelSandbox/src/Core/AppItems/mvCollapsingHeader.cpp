#include "mvCollapsingHeader.h"
#include <imgui.h>
#include <string>
#include "Core/mvApp.h"

namespace Marvel {

	void mvCollapsingHeader::draw()
	{

		// create menu and see if its selected
		if (ImGui::CollapsingHeader(m_label.c_str(),0))
		{
			// set current menu value true
			m_value = true;
			showAll();
		}
		else
			m_value = false;

	}

	PyObject* mvCollapsingHeader::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("i", m_value);

		return pvalue;

	}

}