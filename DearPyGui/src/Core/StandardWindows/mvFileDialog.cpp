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
				m_filePathName = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
				auto selections = igfd::ImGuiFileDialog::Instance()->GetSelection();

				// messy. Fix this
				for (auto& key : selections)
					m_filePath = key.first;

				mvApp::GetApp()->runCallback(m_callback, "File Dialog", ToPyPair(m_filePathName, m_filePath));
				m_filePath = "";
				m_filePathName = "";
				m_callback = nullptr;
				// action
			}
			// close
			igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
			show = false;
		}

	}

}