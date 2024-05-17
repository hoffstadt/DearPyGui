#include "mvStackWindow.h"
#include "mvProfiler.h"
#include "mvContext.h"
#include "imgui_internal.h"

void mvStackWindow::drawWidgets()
{
    ImGui::SetWindowPos({1000000, 1000000});
    ImGui::ShowIDStackToolWindow();
}