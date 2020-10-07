#include "mvFileDialog.h"
#include <imgui.h>
#include "mvApp.h"
#include <ImGuiFileDialog.h>
#include "mvPythonTranslator.h"

namespace Marvel {

	void mvFileDialog::render(bool& show)
	{
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
			show = false;
		}

	}

}