#include "mvFileDialog.h"

namespace Marvel {

	mvFileDialog::mvFileDialog() : mvBaseWindowAppitem("filedialog")
	{
		m_description.deleteAllowed = false;
	}

	bool mvFileDialog::prerender2() 
	{ 
		return true; 
	}

	void mvFileDialog::setCallback(PyObject* callback)
	{ 
		m_callback = callback; 
	}

	void mvFileDialog::draw()
	{
		if (!prerender2())
			return;

		// display
		if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, ImVec2(500, 600)))
		{

			// action if OK
			if (igfd::ImGuiFileDialog::Instance()->IsOk)
			{

				mvApp::GetApp()->runCallback(m_callback, "File Dialog", ToPyList({ igfd::ImGuiFileDialog::Instance()->GetCurrentPath(), igfd::ImGuiFileDialog::Instance()->FileNameBuffer }));

				// action
				if (m_callback)
					Py_XDECREF(m_callback);
				m_callback = nullptr;
			}
			// close
			igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
			m_show = false;
		}
	}

}