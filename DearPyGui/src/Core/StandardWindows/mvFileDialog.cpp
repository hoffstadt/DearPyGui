#include "mvFileDialog.h"
#include <imgui.h>
#include "mvApp.h"
#include <ImGuiFileDialog.h>
#include "mvPythonTranslator.h"

namespace Marvel {



	void mvFileDialog::render(bool& show)
	{
		// display
		if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, ImVec2(300, 300)))
		{

			// action if OK
			if (igfd::ImGuiFileDialog::Instance()->IsOk)
			{
				m_filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
				m_filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();

				mvApp::GetApp()->runCallback(m_callback, "File Dialog", mvPythonTranslator::ToPyPair(m_filePath, m_filePathName));
				m_filePath = "";
				m_filePathName = "";
				m_callback = "";
				// action
			}
			// close
			igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
			show = false;
		}

	}

}