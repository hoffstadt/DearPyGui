#pragma once

#include "mvToolWindow.h"

class mvStackWindow final : public mvToolWindow
{

public:

    [[nodiscard]] mvUUID getUUID() const override { return MV_TOOL_STACK_UUID; }
    [[nodiscard]] const char* getTitle() const override { return "Stack tool"; }

protected:
    // TODO: Find a way to remove extra "Stack tool" window
    void drawWidgets() override;

};
