#if defined (_WIN32)
#include "mvWindowsWindow.h"
#define mvWindowClass mvWindowsWindow
#elif defined(__APPLE__)
#include "mvAppleWindow.h"
#define mvWindowClass mvAppleWindow
#else
#include "mvLinuxWindow.h"
#define mvWindowClass mvLinuxWindow
#endif

#include <implot.h>
#include <ImGuiFileDialog.h>

int main()
{
    auto window = new mvWindowClass();

    window->show();

    window->setup();
    while (window->m_running)
    {
        window->prerender();

        ImGui::ShowDemoWindow();
        ImPlot::ShowDemoWindow();

        igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".cpp", ImVec4(1, 1, 0, 0.9));
        igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".h", ImVec4(0, 1, 0, 0.9));
        igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".hpp", ImVec4(0, 0, 1, 0.9));
        igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".md", ImVec4(1, 0, 1, 0.9));

        // open Dialog Simple
        if (ImGui::Button("Open File Dialog"))
            igfd::ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp,.*", ".");


        // display
        if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey"))
        {
            // action if OK
            if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
            {
                std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
                // action
            }
            // close
            igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
        }

        window->postrender();


    }

}