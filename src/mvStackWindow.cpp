#include "mvStackWindow.h"

#include "mvContext.h"
#include "mvProfiler.h"

#include "imgui_internal.h"

void mvStackWindow::drawWidgets()
{
    ImGui::SetWindowPos({1000000, 1000000});
    ImGui::ShowIDStackToolWindow();
}