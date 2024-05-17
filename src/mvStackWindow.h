#pragma once

#include "mvToolWindow.h"

class mvStackWindow final : public mvToolWindow
{

public:
    mvStackWindow()
    {
        m_windowflags =
              ImGuiWindowFlags_NoSavedSettings
            | ImGuiWindowFlags_NoInputs
            | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoScrollbar
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBackground
            | ImGuiWindowFlags_NoFocusOnAppearing
            | ImGuiWindowFlags_AlwaysAutoResize;
    }

    [[nodiscard]] mvUUID getUUID() const override { return MV_TOOL_STACK_UUID; }
    [[nodiscard]] const char* getTitle() const override { return "Stack tool"; }

protected:
    void drawWidgets() override;

};
