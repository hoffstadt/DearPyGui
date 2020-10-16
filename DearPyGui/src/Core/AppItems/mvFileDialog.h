#pragma once
#include "Core/AppItems/mvTypeBases.h"
#include "mvAppItem.h"
#include "mvApp.h"
#include <ImGuiFileDialog.h>
#include "mvPythonTranslator.h"

namespace Marvel {

	class mvFileDialog : public mvBaseWindowAppitem
	{
		MV_APPITEM_TYPE(mvAppItemType::FileDialog, "no_command_set")

	public:

		mvFileDialog() : mvBaseWindowAppitem("filedialog") {}

		bool canBeDeleted() const override { return false; }
		bool prerender2() { return true; }
		void setCallback(PyObject* callback) { m_callback = callback; }

		void draw() override
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

	private:

		PyObject*   m_callback;
	};

}