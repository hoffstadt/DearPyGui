#pragma once

#include "mvToolWindow.h"
#include "mvAppItem.h"

class mvDebugWindow final : public mvToolWindow
{

public:

    mvDebugWindow();

    [[nodiscard]] mvUUID getUUID() const override { return MV_TOOL_DEBUG_UUID; }
    [[nodiscard]] const char* getTitle() const override { return "Dear PyGui Debug"; }

protected:

    void drawWidgets() override;

private:

    std::vector<std::pair<std::string, std::string>> m_commands;

};