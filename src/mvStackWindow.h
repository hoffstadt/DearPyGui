#pragma once

#include "mvToolWindow.h"

class mvStackWindow final : public mvToolWindow
{

public:

    mvStackWindow();

    [[nodiscard]] mvUUID getUUID() const override { return MV_TOOL_STACK_UUID; }
    [[nodiscard]] const char* getTitle() const override { return "Stack tool"; }

protected:

    void drawWidgets() override;

};
